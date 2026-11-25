/* Kids4Edu - SyncAgent.h
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Team
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SYNCAGENT_H
#define SYNCAGENT_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRecursiveMutex>

class SyncAgent : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
    Q_PROPERTY(QString backendUrl READ backendUrl WRITE setBackendUrl NOTIFY backendUrlChanged)
    Q_PROPERTY(QString authToken READ authToken WRITE setAuthToken NOTIFY authTokenChanged)
    Q_PROPERTY(int pendingCount READ pendingCount NOTIFY countsChanged)
    Q_PROPERTY(int inFlightCount READ inFlightCount NOTIFY countsChanged)
    Q_PROPERTY(int syncedCount READ syncedCount NOTIFY countsChanged)
    Q_PROPERTY(int rejectedCount READ rejectedCount NOTIFY countsChanged)
    Q_PROPERTY(QString lastSyncTime READ lastSyncTime NOTIFY lastSyncTimeChanged)

public:
    static SyncAgent* getInstance();
    virtual ~SyncAgent();

    bool isRunning() const { return m_isRunning; }
    QString backendUrl() const { return m_backendUrl; }
    void setBackendUrl(const QString &url);

    QString authToken() const;
    void setAuthToken(const QString &token);

    int pendingCount() const { return m_pendingCount; }
    int inFlightCount() const { return m_inFlightCount; }
    int syncedCount() const { return m_syncedCount; }
    int rejectedCount() const { return m_rejectedCount; }
    QString lastSyncTime() const { return m_lastSyncTime; }

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void triggerSyncNow();
    Q_INVOKABLE void refreshDiagnostics();

Q_SIGNALS:
    void isRunningChanged();
    void backendUrlChanged();
    void authTokenChanged();
    void countsChanged();
    void lastSyncTimeChanged();
    void syncBatchCompleted(int accepted, int duplicates, int conflicts);
    void syncError(const QString &errorMsg);
    void workstationUnauthorized();

private Q_SLOTS:
    void onSyncTimer();
    void onNetworkReplyFinished(QNetworkReply *reply);

private:
    explicit SyncAgent(QObject *parent = nullptr);
    void initDatabaseConnection();
    void recoverInFlightEvents();
    void sendPendingBatch();

    static SyncAgent *m_instance;

    bool m_isRunning;
    QString m_backendUrl;
    QString m_apiAuthToken;
    int m_maxBatchSize;
    int m_syncIntervalMs;
    int m_currentBackoffMs;
    int m_maxBackoffMs;

    int m_pendingCount;
    int m_inFlightCount;
    int m_syncedCount;
    int m_rejectedCount;
    QString m_lastSyncTime;

    QTimer *m_syncTimer;
    QNetworkAccessManager *m_networkManager;
    QString m_dbConnectionName;
    mutable QRecursiveMutex m_mutex;
    bool m_batchInFlight;
    QString m_currentBatchId;
};

#endif // SYNCAGENT_H
