/* Kids4Edu - TelemetryManager.h
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Team
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef TELEMETRYMANAGER_H
#define TELEMETRYMANAGER_H

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QElapsedTimer>
#include <QDateTime>
#include <QUuid>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>
#include <QThread>

class TelemetryManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isEnabled READ isEnabled WRITE setEnabled NOTIFY isEnabledChanged)
    Q_PROPERTY(QString sessionId READ sessionId NOTIFY sessionIdChanged)
    Q_PROPERTY(QString attemptId READ attemptId NOTIFY attemptIdChanged)
    Q_PROPERTY(QString problemInstanceId READ problemInstanceId NOTIFY problemInstanceIdChanged)
    Q_PROPERTY(QString userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(QString organizationId READ organizationId WRITE setOrganizationId NOTIFY organizationIdChanged)

public:
    static TelemetryManager* getInstance();
    virtual ~TelemetryManager();

    bool isEnabled() const { return m_isEnabled; }
    void setEnabled(bool enabled);

    QString sessionId() const { return m_sessionId; }
    QString attemptId() const { return m_attemptId; }
    QString problemInstanceId() const { return m_problemInstanceId; }
    QString userId() const { return m_userId; }
    void setUserId(const QString &userId);

    QString organizationId() const { return m_organizationId; }
    void setOrganizationId(const QString &orgId);

    // Q_INVOKABLE methods callable directly from QML
    Q_INVOKABLE void startNewSession();
    Q_INVOKABLE void endSession();

    Q_INVOKABLE void startNewAttempt();
    Q_INVOKABLE QString startNewProblem(const QString &instanceId = QString());
    Q_INVOKABLE void clearProblem();

    Q_INVOKABLE void recordEvent(const QString &eventType,
                                 const QString &activityId,
                                 int nativeLevel,
                                 const QVariant &normalizedDifficulty,
                                 const QString &datasetId,
                                 const QVariantMap &payload,
                                 const QString &problemInstanceId = QString());

    // Semantic helpers
    Q_INVOKABLE QString recordProblemPresented(const QString &activityId,
                                               int nativeLevel,
                                               const QVariant &normalizedDifficulty,
                                               const QString &datasetId,
                                               const QVariantMap &difficultyParams,
                                               const QVariantMap &stimulusConfig,
                                               const QString &problemId = QString());

    Q_INVOKABLE void recordAction(const QString &activityId,
                                  int nativeLevel,
                                  const QString &actionType,
                                  const QVariantMap &actionDetails = QVariantMap());

    Q_INVOKABLE void recordInvalidAction(const QString &activityId,
                                         int nativeLevel,
                                         const QString &actionType,
                                         const QString &reason,
                                         const QVariantMap &actionDetails = QVariantMap());

    Q_INVOKABLE void recordAnswerEvaluated(const QString &activityId,
                                          int nativeLevel,
                                          bool isCorrect,
                                          const QVariant &submittedAnswer,
                                          const QVariant &expectedAnswer,
                                          const QVariantMap &evalDetails = QVariantMap());

    Q_INVOKABLE void recordProblemCompleted(const QString &activityId,
                                           int nativeLevel,
                                           bool isSuccess,
                                           const QVariantMap &summaryStats = QVariantMap());

    Q_INVOKABLE void recordProblemAbandoned(const QString &activityId,
                                           int nativeLevel,
                                           const QString &reason,
                                           const QVariantMap &details = QVariantMap());

    // Diagnostic query helpers
    Q_INVOKABLE int getStoredEventsCount();
    Q_INVOKABLE void flush();

Q_SIGNALS:
    void isEnabledChanged();
    void sessionIdChanged();
    void attemptIdChanged();
    void problemInstanceIdChanged();
    void userIdChanged();
    void organizationIdChanged();
    void eventRecorded(const QString &eventId, const QString &eventType);

private:
    explicit TelemetryManager(QObject *parent = nullptr);
    void initDatabase();
    void closeDatabase();

    static TelemetryManager *m_instance;

    bool m_isEnabled;
    QString m_sessionId;
    QString m_attemptId;
    QString m_problemInstanceId;
    QString m_userId;
    QString m_organizationId;
    qint64 m_sequenceNumber;

    QElapsedTimer m_monotonicTimer;
    QElapsedTimer m_problemMonotonicTimer;
    int m_problemActionCount;
    QDateTime m_sessionStartUtc;

    QString m_dbConnectionName;
    QString m_dbPath;
    bool m_dbInitialized;
    mutable QMutex m_dbMutex;
};

#endif // TELEMETRYMANAGER_H
