/* Kids4Edu - SyncAgent.cpp
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Team
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "SyncAgent.h"
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>
#include <QUuid>
#include <QNetworkRequest>
#include <QDebug>
#include <QRandomGenerator>

SyncAgent* SyncAgent::m_instance = nullptr;

SyncAgent* SyncAgent::getInstance()
{
    if (!m_instance) {
        m_instance = new SyncAgent();
    }
    return m_instance;
}

SyncAgent::SyncAgent(QObject *parent)
    : QObject(parent),
      m_isRunning(false),
      m_backendUrl(QStringLiteral("http://127.0.0.1:8000/v1/telemetry/events/batch")),
      m_apiAuthToken(QStringLiteral("k4e_install_synth_token_abc123")),
      m_maxBatchSize(50),
      m_syncIntervalMs(5000),      // sync attempt every 5 seconds
      m_currentBackoffMs(5000),
      m_maxBackoffMs(60000),       // max backoff 60 seconds
      m_pendingCount(0),
      m_inFlightCount(0),
      m_syncedCount(0),
      m_rejectedCount(0),
      m_batchInFlight(false)
{
    QByteArray envUrl = qgetenv("K4E_BACKEND_URL");
    if (!envUrl.isEmpty()) {
        m_backendUrl = QString::fromUtf8(envUrl);
    }
    QByteArray envToken = qgetenv("K4E_CLIENT_TOKEN");
    if (!envToken.isEmpty()) {
        m_apiAuthToken = QString::fromUtf8(envToken);
    }
    QByteArray envInterval = qgetenv("K4E_SYNC_INTERVAL_MS");
    if (!envInterval.isEmpty()) {
        int interval = envInterval.toInt();
        if (interval > 0) {
            m_syncIntervalMs = interval;
            m_currentBackoffMs = interval;
        }
    }

    m_dbConnectionName = QStringLiteral("Kids4EduSyncAgentConnection");
    m_syncTimer = new QTimer(this);
    m_networkManager = new QNetworkAccessManager(this);

    connect(m_syncTimer, &QTimer::timeout, this, &SyncAgent::onSyncTimer);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &SyncAgent::onNetworkReplyFinished);

    initDatabaseConnection();
    recoverInFlightEvents();
    refreshDiagnostics();
}

SyncAgent::~SyncAgent()
{
    stop();
    if (QSqlDatabase::contains(m_dbConnectionName)) {
        QSqlDatabase::removeDatabase(m_dbConnectionName);
    }
}

void SyncAgent::setBackendUrl(const QString &url)
{
    if (m_backendUrl != url) {
        m_backendUrl = url;
        Q_EMIT backendUrlChanged();
    }
}

QString SyncAgent::authToken() const
{
    QMutexLocker locker(&m_mutex);
    return m_apiAuthToken;
}

void SyncAgent::setAuthToken(const QString &token)
{
    QMutexLocker locker(&m_mutex);
    if (m_apiAuthToken != token) {
        m_apiAuthToken = token;
        Q_EMIT authTokenChanged();
    }
}

void SyncAgent::start()
{
    if (!m_isRunning) {
        m_isRunning = true;
        m_syncTimer->start(m_syncIntervalMs);
        Q_EMIT isRunningChanged();
        qDebug() << "Kids4Edu: SyncAgent started. Target backend:" << m_backendUrl;
        triggerSyncNow();
    }
}

void SyncAgent::stop()
{
    if (m_isRunning) {
        m_isRunning = false;
        m_syncTimer->stop();
        Q_EMIT isRunningChanged();
        qDebug() << "Kids4Edu: SyncAgent stopped.";
    }
}

void SyncAgent::triggerSyncNow()
{
    if (!m_batchInFlight) {
        sendPendingBatch();
    }
}

void SyncAgent::initDatabaseConnection()
{
    QMutexLocker locker(&m_mutex);
    QString dataDir;
    QByteArray envDataDir = qgetenv("KIDS4EDU_DATA_DIR");
    if (envDataDir.isEmpty()) {
        envDataDir = qgetenv("K4E_DATA_DIR");
    }
    if (!envDataDir.isEmpty()) {
        dataDir = QString::fromUtf8(envDataDir);
    } else {
        dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if (dataDir.isEmpty()) {
            dataDir = QDir::homePath() + QStringLiteral("/.kids4edu");
        }
    }
    QDir().mkpath(dataDir);
    QString dbPath = dataDir + QStringLiteral("/telemetry_offline.sqlite");

    if (!QSqlDatabase::contains(m_dbConnectionName)) {
        QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), m_dbConnectionName);
        db.setDatabaseName(dbPath);
    }
    QSqlDatabase db = QSqlDatabase::database(m_dbConnectionName);
    if (!db.isOpen()) {
        if (!db.open()) {
            qWarning() << "Kids4Edu: SyncAgent could not open database at" << dbPath << ":" << db.lastError().text();
            return;
        }
    }

    // Ensure columns for robust queue management
    QSqlQuery q(db);
    q.exec(QStringLiteral("ALTER TABLE telemetry_events ADD COLUMN last_error TEXT;"));
    q.exec(QStringLiteral("ALTER TABLE telemetry_events ADD COLUMN synced_at TEXT;"));
    q.exec(QStringLiteral("PRAGMA journal_mode = WAL;"));
}

void SyncAgent::recoverInFlightEvents()
{
    QMutexLocker locker(&m_mutex);
    QSqlDatabase db = QSqlDatabase::database(m_dbConnectionName);
    if (db.isOpen()) {
        QSqlQuery q(db);
        // Events left IN_FLIGHT from previous crash are recovered back to PENDING
        q.exec(QStringLiteral("UPDATE telemetry_events SET sync_status = 'PENDING' WHERE sync_status = 'IN_FLIGHT';"));
    }
}

void SyncAgent::refreshDiagnostics()
{
    QMutexLocker locker(&m_mutex);
    QSqlDatabase db = QSqlDatabase::database(m_dbConnectionName);
    if (!db.isOpen()) return;

    QSqlQuery q(db);
    if (q.exec(QStringLiteral("SELECT sync_status, count(*) FROM telemetry_events GROUP BY sync_status;"))) {
        m_pendingCount = 0;
        m_inFlightCount = 0;
        m_syncedCount = 0;
        m_rejectedCount = 0;
        while (q.next()) {
            QString st = q.value(0).toString();
            int cnt = q.value(1).toInt();
            if (st == QStringLiteral("PENDING") || st == QStringLiteral("RETRY")) m_pendingCount += cnt;
            else if (st == QStringLiteral("IN_FLIGHT")) m_inFlightCount = cnt;
            else if (st == QStringLiteral("SYNCED")) m_syncedCount = cnt;
            else if (st == QStringLiteral("REJECTED")) m_rejectedCount = cnt;
        }
        Q_EMIT countsChanged();
    }
}

void SyncAgent::onSyncTimer()
{
    qDebug() << "Kids4Edu: SyncAgent onSyncTimer tick. running:" << m_isRunning << "batchInFlight:" << m_batchInFlight;
    if (m_isRunning && !m_batchInFlight) {
        sendPendingBatch();
    }
}

void SyncAgent::sendPendingBatch()
{
    QJsonArray eventsArray;
    QStringList eventIds;
    QString batchId;

    {
        QMutexLocker locker(&m_mutex);
        QSqlDatabase db = QSqlDatabase::database(m_dbConnectionName);
        if (!db.isOpen()) {
            initDatabaseConnection();
            db = QSqlDatabase::database(m_dbConnectionName);
        }
        if (!db.isOpen()) {
            qWarning() << "Kids4Edu: SyncAgent sendPendingBatch: db NOT open!";
            return;
        }

        QSqlQuery q(db);
        q.prepare(QStringLiteral(
            "SELECT event_id, session_id, attempt_id, pseudonymous_user_id, organization_id, "
            "       activity_id, activity_version, dataset_id, dataset_version, native_level, "
            "       normalized_difficulty, runtime_type, delivery_mode, platform, client_version, "
            "       event_type, sequence_number, occurred_at, elapsed_ms, payload_json, problem_instance_id, teaching_session_id "
            "FROM telemetry_events "
            "WHERE sync_status IN ('PENDING', 'RETRY') "
            "ORDER BY sequence_number ASC "
            "LIMIT :limit;"
        ));
        q.bindValue(QStringLiteral(":limit"), m_maxBatchSize);

        if (!q.exec()) {
            qWarning() << "Kids4Edu: SyncAgent query pending error:" << q.lastError().text();
            return;
        }

        while (q.next()) {
            QString eventId = q.value(0).toString();
            eventIds.append(eventId);

            QJsonObject evtObj;
            evtObj[QStringLiteral("schema_version")] = QStringLiteral("1.0.0");
            evtObj[QStringLiteral("event_id")] = eventId;
            evtObj[QStringLiteral("session_id")] = q.value(1).toString();
            evtObj[QStringLiteral("attempt_id")] = q.value(2).toString();

            QString probId = q.value(20).toString();
            if (!probId.isEmpty()) {
                evtObj[QStringLiteral("problem_instance_id")] = probId;
            } else {
                evtObj[QStringLiteral("problem_instance_id")] = QJsonValue::Null;
            }

            QString teachId = q.value(21).toString();
            if (!teachId.isEmpty()) {
                evtObj[QStringLiteral("teaching_session_id")] = teachId;
            }

            evtObj[QStringLiteral("pseudonymous_user_id")] = q.value(3).toString();
            evtObj[QStringLiteral("organization_id")] = q.value(4).toString();
            evtObj[QStringLiteral("activity_id")] = q.value(5).toString();
            evtObj[QStringLiteral("activity_version")] = q.value(6).toString();
            evtObj[QStringLiteral("dataset_id")] = q.value(7).toString();
            evtObj[QStringLiteral("dataset_version")] = q.value(8).toString();
            evtObj[QStringLiteral("native_level")] = q.value(9).toInt();

            QVariant diff = q.value(10);
            if (diff.isValid() && !diff.isNull()) {
                evtObj[QStringLiteral("normalized_difficulty")] = diff.toDouble();
            } else {
                evtObj[QStringLiteral("normalized_difficulty")] = QJsonValue::Null;
            }

            evtObj[QStringLiteral("runtime_type")] = q.value(11).toString();
            evtObj[QStringLiteral("delivery_mode")] = q.value(12).toString();
            evtObj[QStringLiteral("platform")] = q.value(13).toString();
            evtObj[QStringLiteral("client_version")] = q.value(14).toString();
            evtObj[QStringLiteral("event_type")] = q.value(15).toString();
            evtObj[QStringLiteral("sequence_number")] = q.value(16).toInt();
            evtObj[QStringLiteral("occurred_at")] = q.value(17).toString();
            evtObj[QStringLiteral("elapsed_ms")] = q.value(18).toInt();

            QJsonDocument payloadDoc = QJsonDocument::fromJson(q.value(19).toString().toUtf8());
            evtObj[QStringLiteral("payload")] = payloadDoc.isObject() ? payloadDoc.object() : QJsonObject();

            eventsArray.append(evtObj);
        }
        q.finish();

        qDebug() << "Kids4Edu: SyncAgent sendPendingBatch fetched" << eventsArray.size() << "events from SQLite";
        if (eventsArray.isEmpty()) {
            return; // No pending events
        }

        // Mark events as IN_FLIGHT
        m_currentBatchId = QUuid::createUuid().toString(QUuid::WithoutBraces);
        batchId = m_currentBatchId;
        QSqlQuery updateQ(db);
        for (const QString &id : eventIds) {
            updateQ.exec(QString("UPDATE telemetry_events SET sync_status = 'IN_FLIGHT', retry_count = retry_count + 1 WHERE event_id = '%1';").arg(id));
        }

        m_batchInFlight = true;
        refreshDiagnostics();
    }

    // Prepare HTTP request outside of SQLite lock
    QJsonObject batchReq;
    batchReq[QStringLiteral("batch_id")] = batchId;
    batchReq[QStringLiteral("sent_at")] = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs);
    batchReq[QStringLiteral("events")] = eventsArray;

    QUrl targetUrl(m_backendUrl);
    QNetworkRequest request(targetUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    request.setRawHeader(QByteArray("Authorization"), QString("Bearer %1").arg(m_apiAuthToken).toUtf8());

    QJsonDocument doc(batchReq);
    qDebug() << "Kids4Edu: SyncAgent posting batch id" << batchId << "with" << eventsArray.size() << "events to" << m_backendUrl;
    m_networkManager->post(request, doc.toJson(QJsonDocument::Compact));
}

void SyncAgent::onNetworkReplyFinished(QNetworkReply *reply)
{
    m_batchInFlight = false;
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Kids4Edu: SyncAgent onNetworkReplyFinished! HTTP:" << statusCode << "Error:" << reply->error();

    if (statusCode == 401 || statusCode == 403) {
        qWarning() << "Kids4Edu: SyncAgent received HTTP" << statusCode << "- Workstation token unauthorized or revoked!";
        Q_EMIT workstationUnauthorized();
    }

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Kids4Edu: SyncAgent network transmission error:" << reply->errorString();
        // Exponential backoff with jitter
        int jitter = QRandomGenerator::global()->bounded(1000);
        m_currentBackoffMs = qMin(m_currentBackoffMs * 2, m_maxBackoffMs) + jitter;
        m_syncTimer->setInterval(m_currentBackoffMs);

        // Mark in-flight events as RETRY
        QMutexLocker locker(&m_mutex);
        QSqlDatabase db = QSqlDatabase::database(m_dbConnectionName);
        if (db.isOpen()) {
            QSqlQuery q(db);
            q.prepare(QStringLiteral("UPDATE telemetry_events SET sync_status = 'RETRY', last_error = :err WHERE sync_status = 'IN_FLIGHT';"));
            q.bindValue(QStringLiteral(":err"), reply->errorString());
            q.exec();
        }
        refreshDiagnostics();
        Q_EMIT syncError(reply->errorString());
        reply->deleteLater();
        return;
    }

    // HTTP 200 Success - Process granular event acknowledgments
    m_currentBackoffMs = m_syncIntervalMs;
    m_syncTimer->setInterval(m_syncIntervalMs);
    m_lastSyncTime = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs);
    Q_EMIT lastSyncTimeChanged();

    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    if (!doc.isObject()) {
        reply->deleteLater();
        return;
    }

    QJsonObject obj = doc.object();
    int acceptedCount = obj[QStringLiteral("accepted_count")].toInt();
    int duplicateCount = obj[QStringLiteral("duplicate_count")].toInt();
    int conflictCount = obj[QStringLiteral("conflict_count")].toInt();
    QJsonArray results = obj[QStringLiteral("results")].toArray();
    qDebug() << "Kids4Edu: SyncAgent ACK received! accepted:" << acceptedCount << "duplicates:" << duplicateCount << "conflicts:" << conflictCount;

    QMutexLocker locker(&m_mutex);
    QSqlDatabase db = QSqlDatabase::database(m_dbConnectionName);
    if (db.isOpen()) {
        QSqlQuery q(db);
        for (const QJsonValue &v : results) {
            QJsonObject res = v.toObject();
            QString eventId = res[QStringLiteral("event_id")].toString();
            QString status = res[QStringLiteral("status")].toString();
            QString msg = res[QStringLiteral("message")].toString();

            if (status == QStringLiteral("ACCEPTED") || status == QStringLiteral("DUPLICATE_IGNORED")) {
                q.prepare(QStringLiteral("UPDATE telemetry_events SET sync_status = 'SYNCED', synced_at = :ts, last_error = NULL WHERE event_id = :id;"));
                q.bindValue(QStringLiteral(":ts"), m_lastSyncTime);
                q.bindValue(QStringLiteral(":id"), eventId);
                q.exec();
            } else if (status == QStringLiteral("CONFLICT_REJECTED")) {
                q.prepare(QStringLiteral("UPDATE telemetry_events SET sync_status = 'REJECTED', last_error = :err WHERE event_id = :id;"));
                q.bindValue(QStringLiteral(":err"), msg);
                q.bindValue(QStringLiteral(":id"), eventId);
                q.exec();
            } else {
                q.prepare(QStringLiteral("UPDATE telemetry_events SET sync_status = 'RETRY', last_error = :err WHERE event_id = :id;"));
                q.bindValue(QStringLiteral(":err"), msg);
                q.bindValue(QStringLiteral(":id"), eventId);
                q.exec();
            }
        }
    }

    refreshDiagnostics();
    Q_EMIT syncBatchCompleted(acceptedCount, duplicateCount, conflictCount);
    reply->deleteLater();
}
