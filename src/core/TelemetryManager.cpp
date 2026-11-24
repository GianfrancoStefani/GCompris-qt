/* Kids4Edu - TelemetryManager.cpp
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Team
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "TelemetryManager.h"
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QSysInfo>

TelemetryManager* TelemetryManager::m_instance = nullptr;

TelemetryManager* TelemetryManager::getInstance()
{
    if (!m_instance) {
        m_instance = new TelemetryManager();
    }
    return m_instance;
}

TelemetryManager::TelemetryManager(QObject *parent)
    : QObject(parent),
      m_isEnabled(true),
      m_sequenceNumber(0),
      m_problemActionCount(0),
      m_dbInitialized(false)
{
    m_sessionId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    m_attemptId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    m_problemInstanceId.clear();
    m_userId = QStringLiteral("std_k4e_synth_default");
    m_organizationId = QStringLiteral("org_k4e_default");

    m_monotonicTimer.start();
    m_sessionStartUtc = QDateTime::currentDateTimeUtc();

    m_dbConnectionName = QStringLiteral("Kids4EduTelemetryConnection");

    initDatabase();
}

TelemetryManager::~TelemetryManager()
{
    closeDatabase();
}

void TelemetryManager::setEnabled(bool enabled)
{
    if (m_isEnabled != enabled) {
        m_isEnabled = enabled;
        Q_EMIT isEnabledChanged();
        qDebug() << "Kids4Edu: TelemetryManager isEnabled set to" << m_isEnabled;
    }
}

void TelemetryManager::setUserId(const QString &userId)
{
    if (m_userId != userId) {
        m_userId = userId;
        Q_EMIT userIdChanged();
    }
}

void TelemetryManager::setOrganizationId(const QString &orgId)
{
    if (m_organizationId != orgId) {
        m_organizationId = orgId;
        Q_EMIT organizationIdChanged();
    }
}

void TelemetryManager::startNewSession()
{
    m_sessionId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    m_attemptId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    m_sequenceNumber = 0;
    m_monotonicTimer.restart();
    m_sessionStartUtc = QDateTime::currentDateTimeUtc();

    Q_EMIT sessionIdChanged();
    Q_EMIT attemptIdChanged();

    QVariantMap payload;
    payload[QStringLiteral("action")] = QStringLiteral("session_initialized");
    recordEvent(QStringLiteral("session_started"), QStringLiteral("system"), 0, QVariant(), QStringLiteral(""), payload);
}

void TelemetryManager::endSession()
{
    QVariantMap payload;
    payload[QStringLiteral("action")] = QStringLiteral("session_finalized");
    recordEvent(QStringLiteral("session_ended"), QStringLiteral("system"), 0, QVariant(), QStringLiteral(""), payload);
    flush();
}

void TelemetryManager::startNewAttempt()
{
    m_attemptId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    m_problemInstanceId.clear();
    m_problemActionCount = 0;
    Q_EMIT attemptIdChanged();
    Q_EMIT problemInstanceIdChanged();
}

QString TelemetryManager::startNewProblem(const QString &instanceId)
{
    if (!instanceId.isEmpty()) {
        m_problemInstanceId = instanceId;
    } else {
        m_problemInstanceId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    m_problemMonotonicTimer.restart();
    m_problemActionCount = 0;
    Q_EMIT problemInstanceIdChanged();
    return m_problemInstanceId;
}

void TelemetryManager::clearProblem()
{
    m_problemInstanceId.clear();
    m_problemActionCount = 0;
    Q_EMIT problemInstanceIdChanged();
}

QString TelemetryManager::recordProblemPresented(const QString &activityId,
                                                int nativeLevel,
                                                const QVariant &normalizedDifficulty,
                                                const QString &datasetId,
                                                const QVariantMap &difficultyParams,
                                                const QVariantMap &stimulusConfig,
                                                const QString &problemId)
{
    QString pId = startNewProblem(problemId);
    QVariantMap payload;
    payload[QStringLiteral("problem_instance_id")] = pId;
    payload[QStringLiteral("difficulty_parameters")] = difficultyParams;
    payload[QStringLiteral("stimulus_configuration")] = stimulusConfig;
    recordEvent(QStringLiteral("problem_presented"), activityId, nativeLevel, normalizedDifficulty, datasetId, payload, pId);
    return pId;
}

void TelemetryManager::recordAction(const QString &activityId,
                                    int nativeLevel,
                                    const QString &actionType,
                                    const QVariantMap &actionDetails)
{
    m_problemActionCount++;
    qint64 actionElapsed = m_problemMonotonicTimer.isValid() ? m_problemMonotonicTimer.elapsed() : 0;

    if (m_problemActionCount == 1) {
        QVariantMap firstActionPayload;
        firstActionPayload[QStringLiteral("action_type")] = actionType;
        firstActionPayload[QStringLiteral("time_to_first_action_ms")] = actionElapsed;
        firstActionPayload[QStringLiteral("action_details")] = actionDetails;
        recordEvent(QStringLiteral("first_action"), activityId, nativeLevel, QVariant(), QStringLiteral(""), firstActionPayload, m_problemInstanceId);
    }

    QVariantMap payload = actionDetails;
    payload[QStringLiteral("action_type")] = actionType;
    payload[QStringLiteral("action_index")] = m_problemActionCount;
    payload[QStringLiteral("elapsed_ms_in_problem")] = actionElapsed;
    recordEvent(QStringLiteral("action_performed"), activityId, nativeLevel, QVariant(), QStringLiteral(""), payload, m_problemInstanceId);
}

void TelemetryManager::recordInvalidAction(const QString &activityId,
                                           int nativeLevel,
                                           const QString &actionType,
                                           const QString &reason,
                                           const QVariantMap &actionDetails)
{
    qint64 actionElapsed = m_problemMonotonicTimer.isValid() ? m_problemMonotonicTimer.elapsed() : 0;
    QVariantMap payload = actionDetails;
    payload[QStringLiteral("action_type")] = actionType;
    payload[QStringLiteral("reason")] = reason;
    payload[QStringLiteral("elapsed_ms_in_problem")] = actionElapsed;
    recordEvent(QStringLiteral("invalid_action"), activityId, nativeLevel, QVariant(), QStringLiteral(""), payload, m_problemInstanceId);
}

void TelemetryManager::recordAnswerEvaluated(const QString &activityId,
                                            int nativeLevel,
                                            bool isCorrect,
                                            const QVariant &submittedAnswer,
                                            const QVariant &expectedAnswer,
                                            const QVariantMap &evalDetails)
{
    qint64 actionElapsed = m_problemMonotonicTimer.isValid() ? m_problemMonotonicTimer.elapsed() : 0;
    QVariantMap payload = evalDetails;
    payload[QStringLiteral("is_correct")] = isCorrect;
    payload[QStringLiteral("submitted_answer")] = submittedAnswer;
    payload[QStringLiteral("expected_answer")] = expectedAnswer;
    payload[QStringLiteral("elapsed_ms_in_problem")] = actionElapsed;
    recordEvent(QStringLiteral("answer_evaluated"), activityId, nativeLevel, QVariant(), QStringLiteral(""), payload, m_problemInstanceId);

    if (isCorrect) {
        QVariantMap fbPayload;
        fbPayload[QStringLiteral("feedback_type")] = QStringLiteral("success_bonus");
        recordEvent(QStringLiteral("positive_feedback"), activityId, nativeLevel, QVariant(), QStringLiteral(""), fbPayload, m_problemInstanceId);
    } else {
        QVariantMap fbPayload;
        fbPayload[QStringLiteral("feedback_type")] = QStringLiteral("error_feedback");
        recordEvent(QStringLiteral("negative_feedback"), activityId, nativeLevel, QVariant(), QStringLiteral(""), fbPayload, m_problemInstanceId);
    }
}

void TelemetryManager::recordProblemCompleted(const QString &activityId,
                                             int nativeLevel,
                                             bool isSuccess,
                                             const QVariantMap &summaryStats)
{
    qint64 totalElapsed = m_problemMonotonicTimer.isValid() ? m_problemMonotonicTimer.elapsed() : 0;
    QVariantMap payload = summaryStats;
    payload[QStringLiteral("is_success")] = isSuccess;
    payload[QStringLiteral("total_actions")] = m_problemActionCount;
    payload[QStringLiteral("total_duration_ms")] = totalElapsed;

    recordEvent(QStringLiteral("problem_completed"), activityId, nativeLevel, QVariant(), QStringLiteral(""), payload, m_problemInstanceId);
    clearProblem();
}

void TelemetryManager::recordProblemAbandoned(const QString &activityId,
                                             int nativeLevel,
                                             const QString &reason,
                                             const QVariantMap &details)
{
    qint64 totalElapsed = m_problemMonotonicTimer.isValid() ? m_problemMonotonicTimer.elapsed() : 0;
    QVariantMap payload = details;
    payload[QStringLiteral("reason")] = reason;
    payload[QStringLiteral("total_actions")] = m_problemActionCount;
    payload[QStringLiteral("total_duration_ms")] = totalElapsed;
    recordEvent(QStringLiteral("problem_abandoned"), activityId, nativeLevel, QVariant(), QStringLiteral(""), payload, m_problemInstanceId);
    clearProblem();
}

void TelemetryManager::initDatabase()
{
    QMutexLocker locker(&m_dbMutex);

    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dataDir.isEmpty()) {
        dataDir = QDir::homePath() + QStringLiteral("/.kids4edu");
    }
    QDir().mkpath(dataDir);
    m_dbPath = dataDir + QStringLiteral("/telemetry_offline.sqlite");

    QSqlDatabase db;
    if (QSqlDatabase::contains(m_dbConnectionName)) {
        db = QSqlDatabase::database(m_dbConnectionName);
    } else {
        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), m_dbConnectionName);
        db.setDatabaseName(m_dbPath);
    }

    if (!db.open()) {
        qWarning() << "Kids4Edu: Failed to open SQLite telemetry database at" << m_dbPath << ":" << db.lastError().text();
        m_dbInitialized = false;
        return;
    }

    QSqlQuery q(db);
    // WAL mode for high performance concurrency and resilience
    q.exec(QStringLiteral("PRAGMA journal_mode = WAL;"));
    q.exec(QStringLiteral("PRAGMA synchronous = NORMAL;"));

    QString createTableSql = QStringLiteral(
        "CREATE TABLE IF NOT EXISTS telemetry_events ("
        "  event_id TEXT PRIMARY KEY,"
        "  session_id TEXT NOT NULL,"
        "  attempt_id TEXT NOT NULL,"
        "  pseudonymous_user_id TEXT NOT NULL,"
        "  organization_id TEXT NOT NULL,"
        "  activity_id TEXT NOT NULL,"
        "  activity_version TEXT NOT NULL,"
        "  dataset_id TEXT,"
        "  dataset_version TEXT,"
        "  native_level INTEGER NOT NULL,"
        "  normalized_difficulty REAL,"
        "  runtime_type TEXT NOT NULL,"
        "  delivery_mode TEXT NOT NULL,"
        "  platform TEXT NOT NULL,"
        "  client_version TEXT NOT NULL,"
        "  event_type TEXT NOT NULL,"
        "  sequence_number INTEGER NOT NULL,"
        "  occurred_at TEXT NOT NULL,"
        "  elapsed_ms INTEGER NOT NULL,"
        "  payload_json TEXT NOT NULL,"
        "  sync_status TEXT DEFAULT 'PENDING',"
        "  retry_count INTEGER DEFAULT 0,"
        "  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "  last_error TEXT,"
        "  synced_at TEXT,"
        "  problem_instance_id TEXT"
        ");"
    );

    if (!q.exec(createTableSql)) {
        qWarning() << "Kids4Edu: Error creating telemetry_events table:" << q.lastError().text();
        m_dbInitialized = false;
    } else {
        // Defensive column migration for existing databases
        q.exec(QStringLiteral("ALTER TABLE telemetry_events ADD COLUMN problem_instance_id TEXT;"));
        q.finish();

        // Create indexes for efficient querying and syncing
        q.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS idx_telemetry_session ON telemetry_events (session_id);"));
        q.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS idx_telemetry_sync ON telemetry_events (sync_status);"));
        q.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS idx_telemetry_activity ON telemetry_events (activity_id);"));
        q.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS idx_telemetry_problem ON telemetry_events (problem_instance_id);"));
        m_dbInitialized = true;

        // Check for interrupted previous sessions (missing session_ended)
        QSqlQuery qInterrupted(db);
        if (qInterrupted.exec(QStringLiteral(
            "SELECT DISTINCT session_id, pseudonymous_user_id, organization_id "
            "FROM telemetry_events "
            "WHERE session_id NOT IN (SELECT session_id FROM telemetry_events WHERE event_type = 'session_ended');"
        ))) {
            while (qInterrupted.next()) {
                QString prevSession = qInterrupted.value(0).toString();
                QString prevUser = qInterrupted.value(1).toString();
                QString prevOrg = qInterrupted.value(2).toString();
                qDebug() << "Kids4Edu: Recovered prior unclosed/interrupted session:" << prevSession;
            }
        }

        qDebug() << "Kids4Edu: Telemetry SQLite database initialized successfully at" << m_dbPath;
    }
}

void TelemetryManager::closeDatabase()
{
    QMutexLocker locker(&m_dbMutex);
    if (m_dbInitialized) {
        {
            QSqlDatabase db = QSqlDatabase::database(m_dbConnectionName);
            if (db.isOpen()) {
                db.close();
            }
        }
        QSqlDatabase::removeDatabase(m_dbConnectionName);
        m_dbInitialized = false;
    }
}

void TelemetryManager::recordEvent(const QString &eventType,
                                   const QString &activityId,
                                   int nativeLevel,
                                   const QVariant &normalizedDifficulty,
                                   const QString &datasetId,
                                   const QVariantMap &payload,
                                   const QString &problemInstanceId)
{
    if (!m_isEnabled) {
        return;
    }

    QString eventId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    m_sequenceNumber++;
    qint64 elapsedMs = m_monotonicTimer.elapsed();
    QString occurredAt = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs);

    QString schemaVersion = QStringLiteral("1.0.0");
    QString clientVersion = QStringLiteral("26.2.0-k4e-1.0.0");
    QString activityVersion = QStringLiteral("26.2.0");
    QString datasetVersion = QStringLiteral("1.0");
    QString runtimeType = QStringLiteral("desktop_native");
    QString deliveryMode = QStringLiteral("offline_local");
    QString platformName = QSysInfo::prettyProductName();

    QString effProblemId = problemInstanceId.isEmpty() ? m_problemInstanceId : problemInstanceId;

    QJsonObject payloadObj = QJsonObject::fromVariantMap(payload);
    QJsonDocument payloadDoc(payloadObj);
    QString payloadJson = QString::fromUtf8(payloadDoc.toJson(QJsonDocument::Compact));

    // Async thread-safe write to SQLite
    QMutexLocker locker(&m_dbMutex);
    if (!m_dbInitialized) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(m_dbConnectionName);
    if (!db.isOpen()) {
        return;
    }

    QSqlQuery q(db);
    q.prepare(QStringLiteral(
        "INSERT INTO telemetry_events ("
        "  event_id, session_id, attempt_id, pseudonymous_user_id, organization_id,"
        "  activity_id, activity_version, dataset_id, dataset_version, native_level,"
        "  normalized_difficulty, runtime_type, delivery_mode, platform, client_version,"
        "  event_type, sequence_number, occurred_at, elapsed_ms, payload_json, problem_instance_id, sync_status"
        ") VALUES ("
        "  :event_id, :session_id, :attempt_id, :user_id, :org_id,"
        "  :activity_id, :activity_version, :dataset_id, :dataset_version, :native_level,"
        "  :normalized_difficulty, :runtime_type, :delivery_mode, :platform, :client_version,"
        "  :event_type, :sequence_number, :occurred_at, :elapsed_ms, :payload_json, :problem_instance_id, 'PENDING'"
        ");"
    ));

    q.bindValue(QStringLiteral(":event_id"), eventId);
    q.bindValue(QStringLiteral(":session_id"), m_sessionId);
    q.bindValue(QStringLiteral(":attempt_id"), m_attemptId);
    q.bindValue(QStringLiteral(":user_id"), m_userId);
    q.bindValue(QStringLiteral(":org_id"), m_organizationId);
    q.bindValue(QStringLiteral(":activity_id"), activityId);
    q.bindValue(QStringLiteral(":activity_version"), activityVersion);
    q.bindValue(QStringLiteral(":dataset_id"), datasetId);
    q.bindValue(QStringLiteral(":dataset_version"), datasetVersion);
    q.bindValue(QStringLiteral(":native_level"), nativeLevel);

    if (normalizedDifficulty.isValid() && !normalizedDifficulty.isNull()) {
        q.bindValue(QStringLiteral(":normalized_difficulty"), normalizedDifficulty.toDouble());
    } else {
        q.bindValue(QStringLiteral(":normalized_difficulty"), QVariant(QMetaType(QMetaType::Double)));
    }

    q.bindValue(QStringLiteral(":runtime_type"), runtimeType);
    q.bindValue(QStringLiteral(":delivery_mode"), deliveryMode);
    q.bindValue(QStringLiteral(":platform"), platformName);
    q.bindValue(QStringLiteral(":client_version"), clientVersion);
    q.bindValue(QStringLiteral(":event_type"), eventType);
    q.bindValue(QStringLiteral(":sequence_number"), m_sequenceNumber);
    q.bindValue(QStringLiteral(":occurred_at"), occurredAt);
    q.bindValue(QStringLiteral(":elapsed_ms"), elapsedMs);
    q.bindValue(QStringLiteral(":payload_json"), payloadJson);

    if (!effProblemId.isEmpty()) {
        q.bindValue(QStringLiteral(":problem_instance_id"), effProblemId);
    } else {
        q.bindValue(QStringLiteral(":problem_instance_id"), QVariant(QMetaType(QMetaType::QString)));
    }

    if (!q.exec()) {
        qWarning() << "Kids4Edu: Failed to insert telemetry event:" << q.lastError().text();
    } else {
        Q_EMIT eventRecorded(eventId, eventType);
    }
}

int TelemetryManager::getStoredEventsCount()
{
    QMutexLocker locker(&m_dbMutex);
    if (!m_dbInitialized) {
        return 0;
    }
    QSqlDatabase db = QSqlDatabase::database(m_dbConnectionName);
    if (!db.isOpen()) {
        return 0;
    }
    QSqlQuery q(db);
    if (q.exec(QStringLiteral("SELECT COUNT(*) FROM telemetry_events;")) && q.next()) {
        return q.value(0).toInt();
    }
    return 0;
}

void TelemetryManager::flush()
{
    QMutexLocker locker(&m_dbMutex);
    if (!m_dbInitialized) return;
    QSqlDatabase db = QSqlDatabase::database(m_dbConnectionName);
    if (db.isOpen()) {
        QSqlQuery q(db);
        q.exec(QStringLiteral("PRAGMA wal_checkpoint(PASSIVE);"));
    }
}
