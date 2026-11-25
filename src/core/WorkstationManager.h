/* Kids4Edu - WorkstationManager.h
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Core Platform Team
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef WORKSTATIONMANAGER_H
#define WORKSTATIONMANAGER_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QRecursiveMutex>

class WorkstationManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isEnrolled READ isEnrolled NOTIFY enrollmentChanged)
    Q_PROPERTY(QString workstationId READ workstationId NOTIFY enrollmentChanged)
    Q_PROPERTY(QString workstationName READ workstationName NOTIFY enrollmentChanged)
    Q_PROPERTY(QString organizationId READ organizationId NOTIFY enrollmentChanged)
    Q_PROPERTY(QString organizationName READ organizationName NOTIFY enrollmentChanged)
    Q_PROPERTY(QString backendUrl READ backendUrl WRITE setBackendUrl NOTIFY backendUrlChanged)
    Q_PROPERTY(QString currentLearnerId READ currentLearnerId NOTIFY learnerChanged)
    Q_PROPERTY(QString currentLearnerName READ currentLearnerName NOTIFY learnerChanged)
    Q_PROPERTY(bool isOnline READ isOnline NOTIFY onlineStatusChanged)
    Q_PROPERTY(QVariantList learnerProfiles READ learnerProfiles NOTIFY profilesChanged)
    Q_PROPERTY(int offlineAllowanceHours READ offlineAllowanceHours NOTIFY offlinePolicyChanged)
    Q_PROPERTY(QString lastSyncTime READ lastSyncTime NOTIFY lastSyncTimeChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    static WorkstationManager* getInstance();
    virtual ~WorkstationManager();

    bool isEnrolled() const { return m_isEnrolled; }
    QString workstationId() const { return m_workstationId; }
    QString workstationName() const { return m_workstationName; }
    QString organizationId() const { return m_organizationId; }
    QString organizationName() const { return m_organizationName; }
    QString backendUrl() const { return m_backendUrl; }
    void setBackendUrl(const QString &url);

    QString currentLearnerId() const { return m_currentLearnerId; }
    QString currentLearnerName() const { return m_currentLearnerName; }
    bool isOnline() const { return m_isOnline; }
    QVariantList learnerProfiles() const { return m_cachedProfiles; }
    int offlineAllowanceHours() const { return m_offlineAllowanceHours; }
    QString lastSyncTime() const { return m_lastSyncTime; }
    QString statusMessage() const { return m_statusMessage; }

    void initialize();

    Q_INVOKABLE void enroll(const QString &backendUrl, const QString &enrollmentToken, const QString &workstationName);
    Q_INVOKABLE void fetchProfiles();
    Q_INVOKABLE void verifyPinAndLogin(const QString &learnerId, const QString &pin);
    Q_INVOKABLE void loginAsGuest();
    Q_INVOKABLE void logoutLearner();
    Q_INVOKABLE void unenroll();
    Q_INVOKABLE void checkConnectivity();
    Q_INVOKABLE QVariantMap getDiagnosticsInfo() const;

Q_SIGNALS:
    void enrollmentChanged();
    void backendUrlChanged();
    void learnerChanged();
    void onlineStatusChanged();
    void profilesChanged();
    void offlinePolicyChanged();
    void lastSyncTimeChanged();
    void statusMessageChanged();
    void enrollmentSucceeded(const QString &workstationName);
    void enrollmentFailed(const QString &errorMessage);
    void loginSucceeded(const QString &learnerId, const QString &displayName);
    void loginFailed(const QString &errorMessage);

private Q_SLOTS:
    void onEnrollReplyFinished(QNetworkReply *reply);
    void onProfilesReplyFinished(QNetworkReply *reply);
    void onVerifyPinReplyFinished(QNetworkReply *reply, const QString &learnerId, const QString &pin);
    void onWorkstationUnauthorized();

    friend class WorkstationManagerTest;

private:
    explicit WorkstationManager(QObject *parent = nullptr);

    QString getConfigFilePath() const;
    QString getCacheFilePath() const;
    void loadConfiguration();
    void saveConfiguration();
    void loadCachedProfiles();
    void saveCachedProfiles();

    QString computeSaltedHash(const QString &pin, const QString &salt) const;
    bool verifyOfflinePin(const QString &learnerId, const QString &pin);

    static WorkstationManager *m_instance;

    bool m_isEnrolled;
    QString m_workstationId;
    QString m_workstationName;
    QString m_organizationId;
    QString m_organizationName;
    QString m_apiToken;
    QString m_backendUrl;
    QString m_currentLearnerId;
    QString m_currentLearnerName;
    bool m_isOnline;
    int m_offlineAllowanceHours;
    QString m_lastSyncTime;
    QString m_statusMessage;
    QDateTime m_lastOnlineTimestamp;

    QVariantList m_cachedProfiles;
    QJsonObject m_offlinePinHashes; // learnerId -> { "hash": "...", "salt": "...", "cached_at": "..." }

    QNetworkAccessManager *m_networkManager;
    mutable QRecursiveMutex m_mutex;
};

#endif // WORKSTATIONMANAGER_H
