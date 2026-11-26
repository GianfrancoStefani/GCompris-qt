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

class QQuickWindow;
class QTimer;

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
    Q_PROPERTY(QString activeSessionId READ activeSessionId NOTIFY activeSessionChanged)
    Q_PROPERTY(QString activeSessionTitle READ activeSessionTitle NOTIFY activeSessionChanged)
    Q_PROPERTY(QString assistanceStatus READ assistanceStatus NOTIFY assistanceStatusChanged)
    Q_PROPERTY(QString assistanceMessage READ assistanceMessage NOTIFY assistanceStatusChanged)
    Q_PROPERTY(bool isSessionPaused READ isSessionPaused NOTIFY sessionPauseChanged)
    Q_PROPERTY(bool isObserved READ isObserved NOTIFY observationChanged)
    Q_PROPERTY(QString observerTeacherName READ observerTeacherName NOTIFY observationChanged)
    Q_PROPERTY(QString currentObservationId READ currentObservationId NOTIFY observationChanged)
    Q_PROPERTY(QJsonObject studentDeskData READ getStudentDeskData NOTIFY studentDeskChanged)
    Q_PROPERTY(bool isClassroomMode READ isClassroomMode WRITE setClassroomMode NOTIFY classroomModeChanged)
    Q_PROPERTY(bool isLockedNavigation READ isLockedNavigation WRITE setLockedNavigation NOTIFY lockedNavigationChanged)

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

    QString activeSessionId() const { return m_activeSessionId; }
    QString activeSessionTitle() const { return m_activeSessionTitle; }
    QString assistanceStatus() const { return m_assistanceStatus; }
    QString assistanceMessage() const { return m_assistanceMessage; }
    bool isSessionPaused() const { return m_isSessionPaused; }
    bool isObserved() const { return m_isObserved; }
    QString observerTeacherName() const { return m_observerTeacherName; }
    QString currentObservationId() const { return m_currentObservationId; }
    void setWindow(QQuickWindow *win) { m_window = win; }

    bool isClassroomMode() const { return m_isClassroomMode; }
    void setClassroomMode(bool mode);
    bool isLockedNavigation() const { return m_isLockedNavigation; }
    void setLockedNavigation(bool locked);
    QJsonObject getStudentDeskData() const { return m_studentDeskData; }

    void initialize();

    Q_INVOKABLE void enroll(const QString &backendUrl, const QString &enrollmentToken, const QString &workstationName);
    Q_INVOKABLE void fetchProfiles();
    Q_INVOKABLE void verifyPinAndLogin(const QString &learnerId, const QString &pin);
    Q_INVOKABLE void loginAsGuest();
    Q_INVOKABLE void logoutLearner();
    Q_INVOKABLE void unenroll();
    Q_INVOKABLE void checkConnectivity();
    Q_INVOKABLE QVariantMap getDiagnosticsInfo() const;

    // Phase 2G-B: Active Session Query, Heartbeat & Assistance Request
    Q_INVOKABLE void queryActiveSession();
    Q_INVOKABLE void sendHeartbeat(const QString &activityId, int level, const QString &activityStatus, int attempts, int errors, int durationSeconds);
    Q_INVOKABLE void requestAssistance(const QString &message = QString());
    Q_INVOKABLE void cancelAssistance();

    // Phase 2G-C: Remote Commands & Live Activity View Observation
    Q_INVOKABLE void setSessionPaused(bool paused);
    Q_INVOKABLE void uploadObservationFrame(const QByteArray &jpegData);
    Q_INVOKABLE QByteArray captureCurrentFrame();

    // Phase 3A.2: Native Learning Desk & Progress Tracking
    Q_INVOKABLE void fetchStudentDesk();
    Q_INVOKABLE void recordStepProgress(const QString &assignmentId, const QString &stepId, const QString &status, int score = 0, int durationSeconds = 0);

Q_SIGNALS:
    void enrollmentChanged();
    void backendUrlChanged();
    void learnerChanged();
    void onlineStatusChanged();
    void profilesChanged();
    void offlinePolicyChanged();
    void lastSyncTimeChanged();
    void statusMessageChanged();
    void activeSessionChanged();
    void assistanceStatusChanged();
    void sessionPauseChanged();
    void observationChanged();
    void commandReceived(const QString &command, const QJsonObject &payload);
    void enrollmentSucceeded(const QString &workstationName);
    void enrollmentFailed(const QString &errorMessage);
    void loginSucceeded(const QString &learnerId, const QString &displayName);
    void loginFailed(const QString &errorMessage);
    void studentDeskChanged();
    void studentDeskDataReady(const QJsonObject &deskData);
    void studentDeskFetchFailed(const QString &errorMessage);
    void classroomModeChanged();
    void lockedNavigationChanged();
    void stepProgressRecorded(const QString &stepId, const QString &status);

private Q_SLOTS:
    void onEnrollReplyFinished(QNetworkReply *reply);
    void onProfilesReplyFinished(QNetworkReply *reply);
    void onVerifyPinReplyFinished(QNetworkReply *reply, const QString &learnerId, const QString &pin);
    void onWorkstationUnauthorized();
    void onHeartbeatReplyFinished(QNetworkReply *reply);
    void onActiveSessionReplyFinished(QNetworkReply *reply);
    void onObservationTimerTick();
    void onStudentDeskReplyFinished(QNetworkReply *reply);
    void onRecordProgressReplyFinished(QNetworkReply *reply, const QString &stepId, const QString &status);

    friend class WorkstationManagerTest;

private:
    explicit WorkstationManager(QObject *parent = nullptr);

    QString getConfigFilePath() const;
    QString getCacheFilePath() const;
    QString getDeskCacheFilePath(const QString &learnerId) const;
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

    QString m_activeSessionId;
    QString m_activeSessionTitle;
    QString m_assistanceStatus;
    QString m_assistanceMessage;

    bool m_isSessionPaused;
    bool m_isObserved;
    bool m_isClassroomMode;
    bool m_isLockedNavigation;
    QString m_observerTeacherName;
    QString m_currentObservationId;
    QString m_observationStreamToken;
    QString m_appliedCommandId;
    QString m_appliedCommand;

    QQuickWindow *m_window;
    QTimer *m_observationTimer;

    QJsonObject m_studentDeskData;
    QVariantList m_cachedProfiles;
    QJsonObject m_offlinePinHashes; // learnerId -> { "hash": "...", "salt": "...", "cached_at": "..." }

    QNetworkAccessManager *m_networkManager;
    mutable QRecursiveMutex m_mutex;
};

#endif // WORKSTATIONMANAGER_H
