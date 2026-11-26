/* Kids4Edu - WorkstationManager.cpp
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Core Platform Team
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "WorkstationManager.h"
#include "TelemetryManager.h"
#include "SyncAgent.h"
#include "SecureStorage.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QPasswordDigestor>
#include <QFileDevice>
#include <QSet>
#include <QRandomGenerator>
#include <QStandardPaths>
#include <QHostInfo>
#include <QSysInfo>
#include <QDebug>
#include <QMutexLocker>
#include <QQuickWindow>
#include <QImage>
#include <QBuffer>
#include <QTimer>

static QString resolveBatchEndpoint(const QString &baseUrl)
{
    if (baseUrl.isEmpty()) {
        return QStringLiteral("http://localhost:8000/v1/telemetry/events/batch");
    }
    if (baseUrl.endsWith(QStringLiteral("/events/batch"))) {
        return baseUrl;
    }
    QString clean = baseUrl;
    while (clean.endsWith(QLatin1Char('/'))) {
        clean.chop(1);
    }
    if (clean.endsWith(QStringLiteral("/v1/telemetry"))) {
        return clean + QStringLiteral("/events/batch");
    }
    if (clean.endsWith(QStringLiteral("/telemetry"))) {
        return clean + QStringLiteral("/events/batch");
    }
    return clean + QStringLiteral("/v1/telemetry/events/batch");
}

WorkstationManager* WorkstationManager::m_instance = nullptr;

WorkstationManager* WorkstationManager::getInstance()
{
    if (!m_instance) {
        m_instance = new WorkstationManager();
    }
    return m_instance;
}

WorkstationManager::WorkstationManager(QObject *parent)
    : QObject(parent),
      m_isEnrolled(false),
      m_workstationId(QStringLiteral("")),
      m_workstationName(QStringLiteral("")),
      m_organizationId(QStringLiteral("")),
      m_organizationName(QStringLiteral("")),
      m_apiToken(QStringLiteral("")),
      m_backendUrl(QStringLiteral("http://localhost:8000")),
      m_currentLearnerId(QStringLiteral("anonymous")),
      m_currentLearnerName(QStringLiteral("Ospite")),
      m_isOnline(true),
      m_offlineAllowanceHours(72),
      m_lastSyncTime(QStringLiteral("Mai")),
      m_statusMessage(QStringLiteral("")),
      m_lastOnlineTimestamp(QDateTime::currentDateTimeUtc()),
      m_activeSessionId(QStringLiteral("")),
      m_activeSessionTitle(QStringLiteral("")),
      m_assistanceStatus(QStringLiteral("NONE")),
      m_assistanceMessage(QStringLiteral("")),
      m_isSessionPaused(false),
      m_isObserved(false),
      m_isClassroomMode(true),
      m_isLockedNavigation(true),
      m_observerTeacherName(QStringLiteral("")),
      m_currentObservationId(QStringLiteral("")),
      m_observationStreamToken(QStringLiteral("")),
      m_appliedCommandId(QStringLiteral("")),
      m_appliedCommand(QStringLiteral("")),
      m_window(nullptr),
      m_observationTimer(new QTimer(this)),
      m_studentDeskData(QJsonObject()),
      m_networkManager(new QNetworkAccessManager(this))
{
    m_observationTimer->setInterval(1000); // 1 fps observation rate
    connect(m_observationTimer, &QTimer::timeout, this, &WorkstationManager::onObservationTimerTick);
}

WorkstationManager::~WorkstationManager()
{
}

QString WorkstationManager::getConfigFilePath() const
{
    QString dataDir;
    QByteArray envDataDir = qgetenv("KIDS4EDU_DATA_DIR");
    if (!envDataDir.isEmpty()) {
        dataDir = QString::fromUtf8(envDataDir);
    } else {
        dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if (dataDir.isEmpty()) {
            dataDir = QDir::homePath() + QStringLiteral("/.kids4edu");
        }
    }
    QDir().mkpath(dataDir);
    return dataDir + QStringLiteral("/workstation_config.json");
}

QString WorkstationManager::getCacheFilePath() const
{
    QString dataDir;
    QByteArray envDataDir = qgetenv("KIDS4EDU_DATA_DIR");
    if (!envDataDir.isEmpty()) {
        dataDir = QString::fromUtf8(envDataDir);
    } else {
        dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if (dataDir.isEmpty()) {
            dataDir = QDir::homePath() + QStringLiteral("/.kids4edu");
        }
    }
    QDir().mkpath(dataDir);
    return dataDir + QStringLiteral("/learner_cache.json");
}

QString WorkstationManager::getDeskCacheFilePath(const QString &learnerId) const
{
    QString dataDir;
    QByteArray envDataDir = qgetenv("KIDS4EDU_DATA_DIR");
    if (!envDataDir.isEmpty()) {
        dataDir = QString::fromUtf8(envDataDir);
    } else {
        dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if (dataDir.isEmpty()) {
            dataDir = QDir::homePath() + QStringLiteral("/.kids4edu");
        }
    }
    QDir().mkpath(dataDir);
    return dataDir + QStringLiteral("/student_desk_") + learnerId + QStringLiteral(".json");
}

void WorkstationManager::setClassroomMode(bool mode)
{
    QMutexLocker locker(&m_mutex);
    if (m_isClassroomMode != mode) {
        m_isClassroomMode = mode;
        Q_EMIT classroomModeChanged();
    }
}

void WorkstationManager::setLockedNavigation(bool locked)
{
    QMutexLocker locker(&m_mutex);
    if (m_isLockedNavigation != locked) {
        m_isLockedNavigation = locked;
        Q_EMIT lockedNavigationChanged();
    }
}

void WorkstationManager::initialize()
{
    QMutexLocker locker(&m_mutex);

    // Listen for unauthorized/revoked signals from SyncAgent
    connect(SyncAgent::getInstance(), &SyncAgent::workstationUnauthorized, this, &WorkstationManager::onWorkstationUnauthorized, Qt::UniqueConnection);

    loadConfiguration();
    loadCachedProfiles();

    QByteArray envLearner = qgetenv("KIDS4EDU_LEARNER_ID");

    if (m_isEnrolled) {
        qDebug() << "Kids4Edu: Workstation enrolled:" << m_workstationName << "Org:" << m_organizationId;
        TelemetryManager::getInstance()->setOrganizationId(m_organizationId);
        SyncAgent::getInstance()->setAuthToken(m_apiToken);
        SyncAgent::getInstance()->setBackendUrl(resolveBatchEndpoint(m_backendUrl));
        if (!envLearner.isEmpty()) {
            m_currentLearnerId = QString::fromUtf8(envLearner);
            m_currentLearnerName = m_currentLearnerId;
            TelemetryManager::getInstance()->switchLearner(m_currentLearnerId, m_organizationId);
        }
        fetchProfiles();
    } else {
        qDebug() << "Kids4Edu: Workstation is not enrolled yet.";
        // Check environment variable fallback for automated tests
        QByteArray envOrg = qgetenv("KIDS4EDU_ORGANIZATION_ID");
        QByteArray envToken = qgetenv("KIDS4EDU_AUTH_TOKEN");
        QByteArray envBackend = qgetenv("KIDS4EDU_BACKEND_URL");
        if (!envBackend.isEmpty()) {
            m_backendUrl = QString::fromUtf8(envBackend);
        }
        if (!envOrg.isEmpty()) {
            m_organizationId = QString::fromUtf8(envOrg);
            m_organizationName = QStringLiteral("Test Organization");
            m_workstationName = QStringLiteral("Dev-Station");
            m_isEnrolled = true;
            if (!envToken.isEmpty()) {
                m_apiToken = QString::fromUtf8(envToken);
            }
            TelemetryManager::getInstance()->setOrganizationId(m_organizationId);
            SyncAgent::getInstance()->setAuthToken(m_apiToken);
            SyncAgent::getInstance()->setBackendUrl(resolveBatchEndpoint(m_backendUrl));
            if (!envLearner.isEmpty()) {
                m_currentLearnerId = QString::fromUtf8(envLearner);
                m_currentLearnerName = m_currentLearnerId;
                TelemetryManager::getInstance()->switchLearner(m_currentLearnerId, m_organizationId);
            }
            Q_EMIT enrollmentChanged();
        }
    }
}

void WorkstationManager::onWorkstationUnauthorized()
{
    QMutexLocker locker(&m_mutex);
    if (m_isEnrolled) {
        qWarning() << "Kids4Edu: Workstation authorization revoked by server!";
        m_isEnrolled = false;
        m_apiToken = QStringLiteral("");
        m_statusMessage = QStringLiteral("Postazione revocata dal server.");
        saveConfiguration();
        TelemetryManager::getInstance()->switchLearner(QStringLiteral("anonymous"), m_organizationId);
        m_currentLearnerId = QStringLiteral("anonymous");
        m_currentLearnerName = QStringLiteral("Ospite");
        Q_EMIT statusMessageChanged();
        Q_EMIT enrollmentChanged();
        Q_EMIT learnerChanged();
    }
}

void WorkstationManager::setBackendUrl(const QString &url)
{
    QMutexLocker locker(&m_mutex);
    if (m_backendUrl != url) {
        m_backendUrl = url;
        SyncAgent::getInstance()->setBackendUrl(resolveBatchEndpoint(m_backendUrl));
        Q_EMIT backendUrlChanged();
    }
}

void WorkstationManager::loadConfiguration()
{
    QString path = getConfigFilePath();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return;

    QJsonObject obj = doc.object();
    m_isEnrolled = obj.value(QStringLiteral("is_enrolled")).toBool(false);
    m_workstationId = obj.value(QStringLiteral("workstation_id")).toString();
    m_workstationName = obj.value(QStringLiteral("workstation_name")).toString();
    m_organizationId = obj.value(QStringLiteral("organization_id")).toString();
    m_organizationName = obj.value(QStringLiteral("organization_name")).toString();

    // Secure Storage Token Migration & Retrieval
    bool migratedToken = false;
    if (obj.contains(QStringLiteral("api_token")) && !obj.value(QStringLiteral("api_token")).toString().isEmpty()) {
        m_apiToken = obj.value(QStringLiteral("api_token")).toString();
        SecureStorage::storeSecret(QStringLiteral("kids4edu"), QStringLiteral("api_token"), m_apiToken);
        migratedToken = true;
    } else {
        m_apiToken = SecureStorage::getSecret(QStringLiteral("kids4edu"), QStringLiteral("api_token"));
    }

    if (obj.contains(QStringLiteral("backend_url"))) {
        m_backendUrl = obj.value(QStringLiteral("backend_url")).toString();
    }
    if (obj.contains(QStringLiteral("offline_allowance_hours"))) {
        m_offlineAllowanceHours = obj.value(QStringLiteral("offline_allowance_hours")).toInt(72);
    }
    if (obj.contains(QStringLiteral("last_online_timestamp"))) {
        m_lastOnlineTimestamp = QDateTime::fromString(obj.value(QStringLiteral("last_online_timestamp")).toString(), Qt::ISODate);
    }
    Q_EMIT enrollmentChanged();

    if (migratedToken) {
        // Scrub plaintext credentials from disk immediately
        saveConfiguration();
    }
}

void WorkstationManager::saveConfiguration()
{
    if (!m_apiToken.isEmpty()) {
        SecureStorage::storeSecret(QStringLiteral("kids4edu"), QStringLiteral("api_token"), m_apiToken);
    }

    QString path = getConfigFilePath();
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Kids4Edu: Failed to save workstation config at" << path;
        return;
    }

    QJsonObject obj;
    obj[QStringLiteral("is_enrolled")] = m_isEnrolled;
    obj[QStringLiteral("workstation_id")] = m_workstationId;
    obj[QStringLiteral("workstation_name")] = m_workstationName;
    obj[QStringLiteral("organization_id")] = m_organizationId;
    obj[QStringLiteral("organization_name")] = m_organizationName;
    // Do NOT write plaintext api_token to JSON file (persisted in SecureStorage)
    obj[QStringLiteral("backend_url")] = m_backendUrl;
    obj[QStringLiteral("offline_allowance_hours")] = m_offlineAllowanceHours;
    obj[QStringLiteral("last_online_timestamp")] = m_lastOnlineTimestamp.toString(Qt::ISODate);

    QJsonDocument doc(obj);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    QFile::setPermissions(path, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
}

void WorkstationManager::loadCachedProfiles()
{
    QString path = getCacheFilePath();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject()) return;

    QJsonObject root = doc.object();
    QJsonArray arr = root.value(QStringLiteral("profiles")).toArray();
    m_cachedProfiles.clear();
    for (const QJsonValue &val : arr) {
        m_cachedProfiles.append(val.toObject().toVariantMap());
    }
    m_offlinePinHashes = root.value(QStringLiteral("pin_hashes")).toObject();
    Q_EMIT profilesChanged();
}

void WorkstationManager::saveCachedProfiles()
{
    QString path = getCacheFilePath();
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return;
    }

    QJsonObject root;
    QJsonArray arr;
    for (const QVariant &var : m_cachedProfiles) {
        arr.append(QJsonObject::fromVariantMap(var.toMap()));
    }
    root[QStringLiteral("profiles")] = arr;
    root[QStringLiteral("pin_hashes")] = m_offlinePinHashes;
    root[QStringLiteral("saved_at")] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);

    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Compact));
    file.close();
    QFile::setPermissions(path, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
}

QString WorkstationManager::computeSaltedHash(const QString &pin, const QString &salt) const
{
    // PBKDF2-HMAC-SHA256 with 100,000 iterations and 32 bytes (256-bit) key
    QByteArray derived = QPasswordDigestor::deriveKeyPbkdf2(
        QCryptographicHash::Sha256,
        pin.toUtf8(),
        salt.toUtf8(),
        100000,
        32
    );
    return QString::fromLatin1(derived.toHex());
}

bool WorkstationManager::verifyOfflinePin(const QString &learnerId, const QString &pin)
{
    if (!m_offlinePinHashes.contains(learnerId)) {
        return false;
    }

    QJsonObject credObj = m_offlinePinHashes.value(learnerId).toObject();
    QString expectedHash = credObj.value(QStringLiteral("hash")).toString();
    QString salt = credObj.value(QStringLiteral("salt")).toString();
    QString cachedAtStr = credObj.value(QStringLiteral("cached_at")).toString();
    int failedAttempts = credObj.value(QStringLiteral("failed_attempts")).toInt(0);
    QString lockedUntilStr = credObj.value(QStringLiteral("locked_until")).toString();

    QDateTime currentUtc = QDateTime::currentDateTimeUtc();

    // 1. Account lockout policy check (5 failed attempts -> 5 minutes lockout)
    if (!lockedUntilStr.isEmpty()) {
        QDateTime lockedUntil = QDateTime::fromString(lockedUntilStr, Qt::ISODate);
        if (lockedUntil.isValid() && currentUtc < lockedUntil) {
            qint64 secsLeft = currentUtc.secsTo(lockedUntil);
            qWarning() << "Kids4Edu: Learner" << learnerId << "locked out for" << secsLeft << "seconds.";
            return false;
        }
    }

    // 2. Clock rollback / anomaly detection
    if (m_lastOnlineTimestamp.isValid() && currentUtc < m_lastOnlineTimestamp.addSecs(-300)) {
        qWarning() << "Kids4Edu: Clock rollback anomaly detected! Current:" << currentUtc << "Last online:" << m_lastOnlineTimestamp;
        return false;
    }

    // 3. Offline window expiration check (72 hours)
    QDateTime cachedAt = QDateTime::fromString(cachedAtStr, Qt::ISODate);
    if (cachedAt.isValid()) {
        qint64 hoursElapsed = cachedAt.secsTo(currentUtc) / 3600;
        if (hoursElapsed > m_offlineAllowanceHours) {
            qWarning() << "Kids4Edu: Offline cache expired for learner" << learnerId << "elapsed hours:" << hoursElapsed;
            return false;
        }
    }

    // 4. Verify PBKDF2 hash
    QString computed = computeSaltedHash(pin, salt);
    if (computed == expectedHash) {
        // Reset failed attempts on successful verification
        credObj[QStringLiteral("failed_attempts")] = 0;
        credObj[QStringLiteral("locked_until")] = QStringLiteral("");
        m_offlinePinHashes[learnerId] = credObj;
        saveCachedProfiles();
        return true;
    } else {
        // Increment failed attempts on wrong PIN
        failedAttempts++;
        credObj[QStringLiteral("failed_attempts")] = failedAttempts;
        if (failedAttempts >= 5) {
            QDateTime lockedUntil = currentUtc.addSecs(300); // 5 minutes lockout
            credObj[QStringLiteral("locked_until")] = lockedUntil.toString(Qt::ISODate);
            qWarning() << "Kids4Edu: 5 failed PIN attempts for" << learnerId << "- Account locked until" << lockedUntil;
        }
        m_offlinePinHashes[learnerId] = credObj;
        saveCachedProfiles();
        return false;
    }
}

void WorkstationManager::enroll(const QString &backendUrl, const QString &enrollmentToken, const QString &workstationName)
{
    QMutexLocker locker(&m_mutex);
    if (backendUrl.trimmed().isEmpty() || enrollmentToken.trimmed().isEmpty()) {
        m_statusMessage = QStringLiteral("URL backend e Token di attivazione sono obbligatori.");
        Q_EMIT statusMessageChanged();
        Q_EMIT enrollmentFailed(m_statusMessage);
        return;
    }

    m_backendUrl = backendUrl.trimmed();
    if (m_backendUrl.endsWith(QLatin1Char('/'))) {
        m_backendUrl.chop(1);
    }

    QString finalWsName = workstationName.trimmed();
    if (finalWsName.isEmpty()) {
        finalWsName = QHostInfo::localHostName();
    }

    QUrl requestUrl(m_backendUrl + QStringLiteral("/management/workstations/enroll"));
    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    QJsonObject payload;
    payload[QStringLiteral("enrollment_token")] = enrollmentToken.trimmed();
    payload[QStringLiteral("workstation_name")] = finalWsName;
    payload[QStringLiteral("platform")] = QSysInfo::prettyProductName();
    payload[QStringLiteral("client_version")] = QStringLiteral("2.1.0");

    m_statusMessage = QStringLiteral("Registrazione postazione in corso...");
    Q_EMIT statusMessageChanged();

    QNetworkReply *reply = m_networkManager->post(request, QJsonDocument(payload).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onEnrollReplyFinished(reply);
    });
}

void WorkstationManager::onEnrollReplyFinished(QNetworkReply *reply)
{
    QMutexLocker locker(&m_mutex);
    reply->deleteLater();

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (reply->error() != QNetworkReply::NoError || statusCode != 200) {
        QString err = reply->errorString();
        QByteArray resp = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(resp);
        if (doc.isObject() && doc.object().contains(QStringLiteral("detail"))) {
            err = doc.object().value(QStringLiteral("detail")).toString();
        }
        m_statusMessage = QStringLiteral("Errore di attivazione (%1): %2").arg(statusCode).arg(err);
        Q_EMIT statusMessageChanged();
        Q_EMIT enrollmentFailed(m_statusMessage);
        return;
    }

    QByteArray resp = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(resp);
    if (!doc.isObject()) {
        m_statusMessage = QStringLiteral("Risposta del server non valida.");
        Q_EMIT statusMessageChanged();
        Q_EMIT enrollmentFailed(m_statusMessage);
        return;
    }

    QJsonObject obj = doc.object();
    m_isEnrolled = true;
    m_workstationId = obj.value(QStringLiteral("installation_id")).toString();
    m_organizationId = obj.value(QStringLiteral("organization_id")).toString();
    m_workstationName = obj.value(QStringLiteral("workstation_name")).toString();
    m_apiToken = obj.value(QStringLiteral("api_token")).toString();
    m_organizationName = m_organizationId;
    m_lastOnlineTimestamp = QDateTime::currentDateTimeUtc();
    m_isOnline = true;

    saveConfiguration();

    TelemetryManager::getInstance()->setOrganizationId(m_organizationId);
    SyncAgent::getInstance()->setAuthToken(m_apiToken);
    SyncAgent::getInstance()->setBackendUrl(resolveBatchEndpoint(m_backendUrl));

    m_statusMessage = QStringLiteral("Postazione registrata con successo: %1").arg(m_workstationName);
    Q_EMIT statusMessageChanged();
    Q_EMIT enrollmentChanged();
    Q_EMIT enrollmentSucceeded(m_workstationName);

    fetchProfiles();
}

void WorkstationManager::fetchProfiles()
{
    QMutexLocker locker(&m_mutex);
    if (!m_isEnrolled || m_apiToken.isEmpty()) {
        return;
    }

    QUrl requestUrl(m_backendUrl + QStringLiteral("/management/workstations/profiles"));
    QNetworkRequest request(requestUrl);
    request.setRawHeader(QByteArray("Authorization"), QString("Bearer %1").arg(m_apiToken).toUtf8());

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onProfilesReplyFinished(reply);
    });
}

void WorkstationManager::onProfilesReplyFinished(QNetworkReply *reply)
{
    QMutexLocker locker(&m_mutex);
    reply->deleteLater();

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (reply->error() != QNetworkReply::NoError || statusCode != 200) {
        if (statusCode == 401 || statusCode == 403) {
            qWarning() << "Kids4Edu: Workstation token rejected or revoked by server (HTTP" << statusCode << ")!";
            m_isEnrolled = false;
            m_apiToken = QStringLiteral("");
            m_statusMessage = QStringLiteral("Postazione revocata dal server.");
            saveConfiguration();
            loginAsGuest();
            Q_EMIT statusMessageChanged();
            Q_EMIT enrollmentChanged();
            return;
        }

        m_isOnline = false;
        Q_EMIT onlineStatusChanged();
        qWarning() << "Kids4Edu: Could not refresh learner profiles from backend, using offline cache. Error:" << reply->errorString();
        return;
    }

    m_isOnline = true;
    m_lastOnlineTimestamp = QDateTime::currentDateTimeUtc();
    m_lastSyncTime = m_lastOnlineTimestamp.toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    Q_EMIT onlineStatusChanged();
    Q_EMIT lastSyncTimeChanged();

    QByteArray resp = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(resp);
    if (!doc.isObject()) return;

    QJsonObject obj = doc.object();
    m_organizationName = obj.value(QStringLiteral("organization_name")).toString(m_organizationId);
    m_workstationName = obj.value(QStringLiteral("workstation_name")).toString(m_workstationName);

    QJsonArray learners = obj.value(QStringLiteral("learners")).toArray();
    m_cachedProfiles.clear();
    QSet<QString> activeLearnerIds;
    for (const QJsonValue &v : learners) {
        m_cachedProfiles.append(v.toObject().toVariantMap());
        activeLearnerIds.insert(v.toObject().value(QStringLiteral("id")).toString());
    }

    // Prune revoked/deleted learners from offline credentials cache
    QStringList cachedIds = m_offlinePinHashes.keys();
    for (const QString &cid : cachedIds) {
        if (!activeLearnerIds.contains(cid)) {
            qDebug() << "Kids4Edu: Pruning revoked learner from offline cache:" << cid;
            m_offlinePinHashes.remove(cid);
        }
    }

    // If currently logged-in learner was revoked, force switch to guest
    if (m_currentLearnerId != QStringLiteral("anonymous") && !activeLearnerIds.contains(m_currentLearnerId)) {
        qWarning() << "Kids4Edu: Currently active learner" << m_currentLearnerId << "has been revoked! Forcing logout.";
        loginAsGuest();
    }

    saveCachedProfiles();
    saveConfiguration();
    Q_EMIT profilesChanged();
    Q_EMIT enrollmentChanged();
}

void WorkstationManager::verifyPinAndLogin(const QString &learnerId, const QString &pin)
{
    QMutexLocker locker(&m_mutex);
    if (learnerId.isEmpty()) {
        Q_EMIT loginFailed(QStringLiteral("Selezionare uno studente."));
        return;
    }

    // Find learner display name from cached profiles
    QString displayName = learnerId;
    for (const QVariant &p : m_cachedProfiles) {
        QVariantMap map = p.toMap();
        if (map.value(QStringLiteral("id")).toString() == learnerId) {
            displayName = map.value(QStringLiteral("display_name")).toString();
            break;
        }
    }

    // Try online verification first if token and connectivity available
    if (m_isOnline && !m_apiToken.isEmpty()) {
        QUrl requestUrl(m_backendUrl + QStringLiteral("/management/workstations/verify-pin"));
        QNetworkRequest request(requestUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
        request.setRawHeader(QByteArray("Authorization"), QString("Bearer %1").arg(m_apiToken).toUtf8());

        QJsonObject payload;
        payload[QStringLiteral("learner_id")] = learnerId;
        payload[QStringLiteral("pin")] = pin;

        QNetworkReply *reply = m_networkManager->post(request, QJsonDocument(payload).toJson());
        connect(reply, &QNetworkReply::finished, this, [this, reply, learnerId, pin]() {
            onVerifyPinReplyFinished(reply, learnerId, pin);
        });
        return;
    }

    // Offline verification
    if (verifyOfflinePin(learnerId, pin)) {
        m_currentLearnerId = learnerId;
        m_currentLearnerName = displayName;
        m_studentDeskData = QJsonObject();
        TelemetryManager::getInstance()->switchLearner(learnerId, m_organizationId);
        Q_EMIT learnerChanged();
        Q_EMIT studentDeskChanged();
        Q_EMIT loginSucceeded(learnerId, displayName);
        fetchStudentDesk();
    } else {
        Q_EMIT loginFailed(QStringLiteral("PIN non valido o credenziali offline non disponibili."));
    }
}

void WorkstationManager::onVerifyPinReplyFinished(QNetworkReply *reply, const QString &learnerId, const QString &pin)
{
    QMutexLocker locker(&m_mutex);
    reply->deleteLater();

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (reply->error() != QNetworkReply::NoError || statusCode != 200) {
        // If network failed, attempt offline verification fallback
        if (reply->error() == QNetworkReply::ConnectionRefusedError ||
            reply->error() == QNetworkReply::TimeoutError ||
            statusCode >= 500) {
            m_isOnline = false;
            Q_EMIT onlineStatusChanged();
            if (verifyOfflinePin(learnerId, pin)) {
                QString displayName = learnerId;
                for (const QVariant &p : m_cachedProfiles) {
                    QVariantMap map = p.toMap();
                    if (map.value(QStringLiteral("id")).toString() == learnerId) {
                        displayName = map.value(QStringLiteral("display_name")).toString();
                        break;
                    }
                }
                m_currentLearnerId = learnerId;
                m_currentLearnerName = displayName;
                m_studentDeskData = QJsonObject();
                TelemetryManager::getInstance()->switchLearner(learnerId, m_organizationId);
                Q_EMIT learnerChanged();
                Q_EMIT studentDeskChanged();
                Q_EMIT loginSucceeded(learnerId, displayName);
                fetchStudentDesk();
                return;
            }
        }

        QString err = reply->errorString();
        QByteArray resp = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(resp);
        if (doc.isObject() && doc.object().contains(QStringLiteral("detail"))) {
            err = doc.object().value(QStringLiteral("detail")).toString();
        }
        Q_EMIT loginFailed(QStringLiteral("Autenticazione fallita: %1").arg(err));
        return;
    }

    // Online verification succeeded
    m_isOnline = true;
    m_lastOnlineTimestamp = QDateTime::currentDateTimeUtc();
    Q_EMIT onlineStatusChanged();

    // Cache salted PIN hash for offline resilience
    QString salt = QCryptographicHash::hash((learnerId + QStringLiteral("_salt")).toUtf8(), QCryptographicHash::Sha256).toHex().left(16);
    QString hash = computeSaltedHash(pin, salt);

    QJsonObject credObj;
    credObj[QStringLiteral("hash")] = hash;
    credObj[QStringLiteral("salt")] = salt;
    credObj[QStringLiteral("cached_at")] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    m_offlinePinHashes[learnerId] = credObj;
    saveCachedProfiles();

    QString displayName = learnerId;
    for (const QVariant &p : m_cachedProfiles) {
        QVariantMap map = p.toMap();
        if (map.value(QStringLiteral("id")).toString() == learnerId) {
            displayName = map.value(QStringLiteral("display_name")).toString();
            break;
        }
    }

    m_currentLearnerId = learnerId;
    m_currentLearnerName = displayName;
    m_studentDeskData = QJsonObject();
    TelemetryManager::getInstance()->switchLearner(learnerId, m_organizationId);

    Q_EMIT learnerChanged();
    Q_EMIT studentDeskChanged();
    Q_EMIT loginSucceeded(learnerId, displayName);
    fetchStudentDesk();
}

void WorkstationManager::loginAsGuest()
{
    QMutexLocker locker(&m_mutex);
    m_currentLearnerId = QStringLiteral("anonymous");
    m_currentLearnerName = QStringLiteral("Ospite");
    m_studentDeskData = QJsonObject();
    TelemetryManager::getInstance()->switchLearner(m_currentLearnerId, m_organizationId);
    Q_EMIT learnerChanged();
    Q_EMIT studentDeskChanged();
    Q_EMIT loginSucceeded(m_currentLearnerId, m_currentLearnerName);
}

void WorkstationManager::logoutLearner()
{
    QMutexLocker locker(&m_mutex);
    m_currentLearnerId = QStringLiteral("anonymous");
    m_currentLearnerName = QStringLiteral("Ospite");
    m_studentDeskData = QJsonObject();
    TelemetryManager::getInstance()->switchLearner(m_currentLearnerId, m_organizationId);
    Q_EMIT learnerChanged();
    Q_EMIT studentDeskChanged();
}

void WorkstationManager::unenroll()
{
    QMutexLocker locker(&m_mutex);
    m_isEnrolled = false;
    m_workstationId.clear();
    m_workstationName.clear();
    m_organizationId.clear();
    m_organizationName.clear();
    m_apiToken.clear();
    m_cachedProfiles.clear();
    m_offlinePinHashes = QJsonObject();
    m_studentDeskData = QJsonObject();

    SecureStorage::deleteSecret(QStringLiteral("kids4edu"), QStringLiteral("api_token"));

    QFile::remove(getConfigFilePath());
    QFile::remove(getCacheFilePath());

    SyncAgent::getInstance()->setAuthToken(QStringLiteral(""));
    TelemetryManager::getInstance()->switchLearner(QStringLiteral("anonymous"), QStringLiteral(""));

    m_statusMessage = QStringLiteral("Postazione deregistrata.");
    Q_EMIT statusMessageChanged();
    Q_EMIT enrollmentChanged();
    Q_EMIT profilesChanged();
    Q_EMIT learnerChanged();
    Q_EMIT studentDeskChanged();
}

void WorkstationManager::checkConnectivity()
{
    QMutexLocker locker(&m_mutex);
    QString healthPath = m_backendUrl;
    while (healthPath.endsWith(QLatin1Char('/'))) {
        healthPath.chop(1);
    }
    QUrl healthUrl(healthPath + QStringLiteral("/health"));
    QNetworkRequest request(healthUrl);

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        bool online = (reply->error() == QNetworkReply::NoError);
        if (m_isOnline != online) {
            m_isOnline = online;
            Q_EMIT onlineStatusChanged();
        }
        if (online) {
            m_lastOnlineTimestamp = QDateTime::currentDateTimeUtc();
        }
    });
}

QVariantMap WorkstationManager::getDiagnosticsInfo() const
{
    QMutexLocker locker(&m_mutex);
    QVariantMap diag;
    diag[QStringLiteral("is_enrolled")] = m_isEnrolled;
    diag[QStringLiteral("workstation_id")] = m_workstationId;
    diag[QStringLiteral("workstation_name")] = m_workstationName;
    diag[QStringLiteral("organization_id")] = m_organizationId;
    diag[QStringLiteral("organization_name")] = m_organizationName;
    diag[QStringLiteral("current_learner_id")] = m_currentLearnerId;
    diag[QStringLiteral("current_learner_name")] = m_currentLearnerName;
    diag[QStringLiteral("backend_url")] = m_backendUrl;
    diag[QStringLiteral("is_online")] = m_isOnline;
    diag[QStringLiteral("offline_allowance_hours")] = m_offlineAllowanceHours;
    diag[QStringLiteral("last_sync_time")] = m_lastSyncTime;
    diag[QStringLiteral("pending_events")] = SyncAgent::getInstance()->pendingCount();
    diag[QStringLiteral("synced_events")] = SyncAgent::getInstance()->syncedCount();
    diag[QStringLiteral("in_flight_events")] = SyncAgent::getInstance()->inFlightCount();
    diag[QStringLiteral("sync_agent_running")] = SyncAgent::getInstance()->isRunning();
    diag[QStringLiteral("client_version")] = QStringLiteral("2.1.0");
    diag[QStringLiteral("operating_system")] = QSysInfo::prettyProductName();
    diag[QStringLiteral("active_session_id")] = m_activeSessionId;
    diag[QStringLiteral("active_session_title")] = m_activeSessionTitle;
    diag[QStringLiteral("assistance_status")] = m_assistanceStatus;
    return diag;
}

void WorkstationManager::queryActiveSession()
{
    QMutexLocker locker(&m_mutex);
    if (!m_isEnrolled || m_apiToken.isEmpty()) {
        return;
    }

    QString basePath = m_backendUrl;
    while (basePath.endsWith(QLatin1Char('/'))) {
        basePath.chop(1);
    }
    QUrl url(basePath + QStringLiteral("/v1/workstations/active-session"));
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QStringLiteral("Bearer %1").arg(m_apiToken).toUtf8());

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onActiveSessionReplyFinished(reply);
    });
}

void WorkstationManager::onActiveSessionReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    QMutexLocker locker(&m_mutex);

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Kids4Edu: Failed to query active session:" << reply->errorString();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return;

    QJsonObject obj = doc.object();
    bool hasActive = obj.value(QStringLiteral("has_active_session")).toBool();

    if (hasActive) {
        QJsonObject sess = obj.value(QStringLiteral("session")).toObject();
        QString sessId = sess.value(QStringLiteral("id")).toString();
        QString sessTitle = sess.value(QStringLiteral("title")).toString();

        if (m_activeSessionId != sessId) {
            m_activeSessionId = sessId;
            m_activeSessionTitle = sessTitle;
            TelemetryManager::getInstance()->setTeachingSessionId(m_activeSessionId);
            Q_EMIT activeSessionChanged();
            qDebug() << "Kids4Edu: Joined active teaching session:" << m_activeSessionTitle << "(" << m_activeSessionId << ")";
        }
    } else {
        if (!m_activeSessionId.isEmpty()) {
            m_activeSessionId.clear();
            m_activeSessionTitle.clear();
            TelemetryManager::getInstance()->setTeachingSessionId(QStringLiteral(""));
            Q_EMIT activeSessionChanged();
            qDebug() << "Kids4Edu: No active teaching session for workstation.";
        }
    }
}

void WorkstationManager::sendHeartbeat(const QString &activityId, int level, const QString &activityStatus, int attempts, int errors, int durationSeconds)
{
    QMutexLocker locker(&m_mutex);
    if (!m_isEnrolled || m_apiToken.isEmpty()) {
        return;
    }

    QString basePath = m_backendUrl;
    while (basePath.endsWith(QLatin1Char('/'))) {
        basePath.chop(1);
    }
    QUrl url(basePath + QStringLiteral("/v1/workstations/heartbeat"));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    request.setRawHeader("Authorization", QStringLiteral("Bearer %1").arg(m_apiToken).toUtf8());

    QJsonObject payload;
    if (!m_activeSessionId.isEmpty()) {
        payload[QStringLiteral("teaching_session_id")] = m_activeSessionId;
    }
    payload[QStringLiteral("learner_id")] = m_currentLearnerId;
    payload[QStringLiteral("current_activity_id")] = activityId;
    payload[QStringLiteral("current_level")] = level;
    payload[QStringLiteral("activity_status")] = activityStatus.isEmpty() ? QStringLiteral("WAITING") : activityStatus;
    payload[QStringLiteral("attempts_count")] = attempts;
    payload[QStringLiteral("errors_count")] = errors;
    payload[QStringLiteral("duration_seconds")] = durationSeconds;
    payload[QStringLiteral("assistance_status")] = m_assistanceStatus;
    payload[QStringLiteral("assistance_message")] = m_assistanceMessage;
    payload[QStringLiteral("client_timestamp")] = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs);

    // Attach applied command acknowledgment if present
    if (!m_appliedCommandId.isEmpty()) {
        payload[QStringLiteral("applied_command_id")] = m_appliedCommandId;
        payload[QStringLiteral("applied_command")] = m_appliedCommand;
        payload[QStringLiteral("applied_command_status")] = QStringLiteral("APPLIED");
        m_appliedCommandId.clear();
        m_appliedCommand.clear();
    }

    QNetworkReply *reply = m_networkManager->post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onHeartbeatReplyFinished(reply);
    });
}

void WorkstationManager::onHeartbeatReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    QMutexLocker locker(&m_mutex);

    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return;

    QJsonObject obj = doc.object();
    QString serverAssistanceStatus = obj.value(QStringLiteral("assistance_status")).toString();
    if (!serverAssistanceStatus.isEmpty() && m_assistanceStatus != serverAssistanceStatus) {
        m_assistanceStatus = serverAssistanceStatus;
        Q_EMIT assistanceStatusChanged();
    }

    // Process Live Activity View observation status
    bool serverObserved = obj.value(QStringLiteral("is_observed")).toBool(false);
    QString obsId = obj.value(QStringLiteral("active_observation_id")).toString();
    QString streamToken = obj.value(QStringLiteral("observation_stream_token")).toString();
    QString teacherName = obj.value(QStringLiteral("observation_teacher_name")).toString();

    if (m_isObserved != serverObserved || m_currentObservationId != obsId) {
        m_isObserved = serverObserved;
        m_currentObservationId = obsId;
        m_observationStreamToken = streamToken;
        m_observerTeacherName = teacherName.isEmpty() ? QStringLiteral("Docente") : teacherName;
        if (m_isObserved && !m_currentObservationId.isEmpty()) {
            if (!m_observationTimer->isActive()) {
                m_observationTimer->start();
            }
            // Push first frame immediately
            QByteArray frame = captureCurrentFrame();
            if (!frame.isEmpty()) {
                uploadObservationFrame(frame);
            }
        } else {
            if (m_observationTimer->isActive()) {
                m_observationTimer->stop();
            }
        }
        Q_EMIT observationChanged();
    }

    // Check for pending command
    QJsonValue cmdVal = obj.value(QStringLiteral("pending_command"));
    if (!cmdVal.isNull() && !cmdVal.toString().isEmpty()) {
        QString cmd = cmdVal.toString();
        QString cmdId = obj.value(QStringLiteral("pending_command_id")).toString();
        QJsonObject cmdPayload = obj.value(QStringLiteral("pending_command_payload")).toObject();
        qDebug() << "Kids4Edu: Workstation received command:" << cmd << "id:" << cmdId;

        if (cmd == QStringLiteral("PAUSE") || cmd == QStringLiteral("pause")) {
            setSessionPaused(true);
        } else if (cmd == QStringLiteral("RESUME") || cmd == QStringLiteral("resume")) {
            setSessionPaused(false);
        } else if (cmd == QStringLiteral("START_OBSERVATION")) {
            m_isObserved = true;
            m_currentObservationId = cmdId;
            if (!m_observationTimer->isActive()) {
                m_observationTimer->start();
            }
            Q_EMIT observationChanged();
        } else if (cmd == QStringLiteral("STOP_OBSERVATION")) {
            m_isObserved = false;
            m_currentObservationId.clear();
            if (m_observationTimer->isActive()) {
                m_observationTimer->stop();
            }
            Q_EMIT observationChanged();
        }

        m_appliedCommandId = cmdId;
        m_appliedCommand = cmd;
        Q_EMIT commandReceived(cmd, cmdPayload);
    }
}

void WorkstationManager::requestAssistance(const QString &message)
{
    QMutexLocker locker(&m_mutex);
    if (!m_isEnrolled || m_apiToken.isEmpty()) {
        return;
    }

    m_assistanceStatus = QStringLiteral("REQUESTED");
    m_assistanceMessage = message.isEmpty() ? QStringLiteral("Richiesta di assistenza didattica") : message;
    Q_EMIT assistanceStatusChanged();

    QString basePath = m_backendUrl;
    while (basePath.endsWith(QLatin1Char('/'))) {
        basePath.chop(1);
    }
    QUrl url(basePath + QStringLiteral("/v1/workstations/assistance-request"));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    request.setRawHeader("Authorization", QStringLiteral("Bearer %1").arg(m_apiToken).toUtf8());

    QJsonObject payload;
    if (!m_activeSessionId.isEmpty()) {
        payload[QStringLiteral("teaching_session_id")] = m_activeSessionId;
    }
    payload[QStringLiteral("learner_id")] = m_currentLearnerId;
    payload[QStringLiteral("message")] = m_assistanceMessage;

    QNetworkReply *reply = m_networkManager->post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));
    connect(reply, &QNetworkReply::finished, this, [reply]() {
        reply->deleteLater();
    });
}

void WorkstationManager::cancelAssistance()
{
    QMutexLocker locker(&m_mutex);
    if (!m_isEnrolled || m_apiToken.isEmpty()) {
        return;
    }

    m_assistanceStatus = QStringLiteral("NONE");
    m_assistanceMessage.clear();
    Q_EMIT assistanceStatusChanged();

    QString basePath = m_backendUrl;
    while (basePath.endsWith(QLatin1Char('/'))) {
        basePath.chop(1);
    }
    QUrl url(basePath + QStringLiteral("/v1/workstations/assistance-cancel"));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    request.setRawHeader("Authorization", QStringLiteral("Bearer %1").arg(m_apiToken).toUtf8());

    QJsonObject payload;
    if (!m_activeSessionId.isEmpty()) {
        payload[QStringLiteral("teaching_session_id")] = m_activeSessionId;
    }
    payload[QStringLiteral("learner_id")] = m_currentLearnerId;

    QNetworkReply *reply = m_networkManager->post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));
    connect(reply, &QNetworkReply::finished, this, [reply]() {
        reply->deleteLater();
    });
}

void WorkstationManager::setSessionPaused(bool paused)
{
    QMutexLocker locker(&m_mutex);
    if (m_isSessionPaused != paused) {
        m_isSessionPaused = paused;
        Q_EMIT sessionPauseChanged();
        qDebug() << "Kids4Edu: Session pause state changed to:" << m_isSessionPaused;
    }
}

QByteArray WorkstationManager::captureCurrentFrame()
{
    QMutexLocker locker(&m_mutex);
    if (!m_window) {
        return QByteArray();
    }

    // Only grab the GCompris window content (NO desktop, NO webcam, NO audio)
    QImage img = m_window->grabWindow();
    if (img.isNull()) {
        return QByteArray();
    }

    // Scaled down to max 640x360 with aspect ratio preservation
    QImage scaled = img.scaled(640, 360, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    scaled.save(&buffer, "JPEG", 60); // 60% quality JPEG
    return ba;
}

void WorkstationManager::onObservationTimerTick()
{
    QMutexLocker locker(&m_mutex);
    if (!m_isObserved || m_currentObservationId.isEmpty() || m_apiToken.isEmpty()) {
        return;
    }

    QByteArray frameData = captureCurrentFrame();
    if (!frameData.isEmpty()) {
        uploadObservationFrame(frameData);
    }
}

void WorkstationManager::uploadObservationFrame(const QByteArray &jpegData)
{
    if (m_currentObservationId.isEmpty() || m_apiToken.isEmpty() || jpegData.isEmpty()) {
        return;
    }

    QString basePath = m_backendUrl;
    while (basePath.endsWith(QLatin1Char('/'))) {
        basePath.chop(1);
    }
    QUrl url(basePath + QStringLiteral("/v1/workstations/observations/%1/frame").arg(m_currentObservationId));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("image/jpeg"));
    request.setRawHeader("Authorization", QStringLiteral("Bearer %1").arg(m_apiToken).toUtf8());
    if (!m_observationStreamToken.isEmpty()) {
        request.setRawHeader("X-Observation-Token", m_observationStreamToken.toUtf8());
    }

    QNetworkReply *reply = m_networkManager->post(request, jpegData);
    connect(reply, &QNetworkReply::finished, this, [reply]() {
        reply->deleteLater();
    });
}

void WorkstationManager::fetchStudentDesk()
{
    QMutexLocker locker(&m_mutex);
    if (!m_isEnrolled || m_apiToken.isEmpty() || m_currentLearnerId.isEmpty() || m_currentLearnerId == QStringLiteral("anonymous")) {
        return;
    }

    if (!m_isOnline) {
        // Read from offline cache
        QString cachePath = getDeskCacheFilePath(m_currentLearnerId);
        QFile file(cachePath);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            file.close();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (doc.isObject()) {
                m_studentDeskData = doc.object();
                Q_EMIT studentDeskDataReady(m_studentDeskData);
                Q_EMIT studentDeskChanged();
                return;
            }
        }
        Q_EMIT studentDeskFetchFailed(QStringLiteral("Offline e nessuna scrivania salvata in cache"));
        return;
    }

    QString basePath = m_backendUrl;
    while (basePath.endsWith(QLatin1Char('/'))) {
        basePath.chop(1);
    }
    QUrl url(basePath + QStringLiteral("/v1/organizations/%1/learning-desks/my-desk?learner_id=%2")
             .arg(m_organizationId, m_currentLearnerId));
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QStringLiteral("Bearer %1").arg(m_apiToken).toUtf8());

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onStudentDeskReplyFinished(reply);
    });
}

void WorkstationManager::onStudentDeskReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    QMutexLocker locker(&m_mutex);

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (reply->error() != QNetworkReply::NoError || statusCode != 200) {
        // Attempt offline cache fallback
        QString cachePath = getDeskCacheFilePath(m_currentLearnerId);
        QFile file(cachePath);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            file.close();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (doc.isObject()) {
                m_studentDeskData = doc.object();
                Q_EMIT studentDeskDataReady(m_studentDeskData);
                Q_EMIT studentDeskChanged();
                return;
            }
        }
        QString err = reply->errorString();
        QByteArray resp = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(resp);
        if (doc.isObject() && doc.object().contains(QStringLiteral("detail"))) {
            err = doc.object().value(QStringLiteral("detail")).toString();
        }
        qWarning() << "Kids4Edu: Failed to fetch student desk:" << err;
        Q_EMIT studentDeskFetchFailed(err);
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        Q_EMIT studentDeskFetchFailed(QStringLiteral("Risposta scrivania non valida"));
        return;
    }

    m_studentDeskData = doc.object();

    // Cache desk locally for offline resilience (0600 permissions)
    QString cachePath = getDeskCacheFilePath(m_currentLearnerId);
    QFile file(cachePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(doc.toJson(QJsonDocument::Compact));
        file.close();
        QFile::setPermissions(cachePath, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }

    Q_EMIT studentDeskDataReady(m_studentDeskData);
    Q_EMIT studentDeskChanged();
}

void WorkstationManager::recordStepProgress(const QString &assignmentId, const QString &stepId, const QString &status, int score, int durationSeconds)
{
    QMutexLocker locker(&m_mutex);
    if (!m_isEnrolled || m_apiToken.isEmpty() || m_currentLearnerId.isEmpty() || m_currentLearnerId == QStringLiteral("anonymous")) {
        return;
    }

    QString basePath = m_backendUrl;
    while (basePath.endsWith(QLatin1Char('/'))) {
        basePath.chop(1);
    }
    QUrl url(basePath + QStringLiteral("/v1/organizations/%1/learning-desks/progress?learner_id=%2")
             .arg(m_organizationId, m_currentLearnerId));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    request.setRawHeader("Authorization", QStringLiteral("Bearer %1").arg(m_apiToken).toUtf8());

    QJsonObject payload;
    payload[QStringLiteral("assignment_id")] = assignmentId;
    payload[QStringLiteral("step_id")] = stepId;
    payload[QStringLiteral("status")] = status;
    payload[QStringLiteral("score")] = score;
    payload[QStringLiteral("duration_seconds")] = durationSeconds;

    QNetworkReply *reply = m_networkManager->post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));
    connect(reply, &QNetworkReply::finished, this, [this, reply, stepId, status]() {
        onRecordProgressReplyFinished(reply, stepId, status);
    });
}

void WorkstationManager::onRecordProgressReplyFinished(QNetworkReply *reply, const QString &stepId, const QString &status)
{
    reply->deleteLater();
    QMutexLocker locker(&m_mutex);

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (reply->error() != QNetworkReply::NoError || statusCode != 200) {
        qWarning() << "Kids4Edu: Failed to record step progress for" << stepId << ":" << reply->errorString();
        return;
    }

    Q_EMIT stepProgressRecorded(stepId, status);
    // Refresh student desk state
    fetchStudentDesk();
}
