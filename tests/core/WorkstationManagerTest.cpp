/* Kids4Edu - WorkstationManagerTest.cpp
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Project
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QObject>
#include <QTest>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QPasswordDigestor>
#include <QFileDevice>

#include "src/core/WorkstationManager.h"
#include "src/core/TelemetryManager.h"
#include "src/core/SyncAgent.h"

class WorkstationManagerTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testInitialState();
    void testConfigPersistence();
    void testOfflineCacheAndPinVerification();
    void testSecurityHardeningAndLockout();
    void testGuestLoginAndLearnerSwitch();
    void testDiagnosticsInfoIntegrity();
    void cleanupTestCase();
};

void WorkstationManagerTest::initTestCase()
{
    WorkstationManager *wm = WorkstationManager::getInstance();
    QVERIFY(wm != nullptr);
}

void WorkstationManagerTest::testInitialState()
{
    WorkstationManager *wm = WorkstationManager::getInstance();
    QVERIFY(!wm->workstationId().isEmpty() || !wm->isEnrolled());
    QCOMPARE(wm->offlineAllowanceHours(), 72);
}

void WorkstationManagerTest::testConfigPersistence()
{
    WorkstationManager *wm = WorkstationManager::getInstance();

    // Verify config file location
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dataDir.isEmpty()) {
        dataDir = QDir::homePath() + QStringLiteral("/.kids4edu");
    }
    QString configPath = dataDir + QStringLiteral("/workstation_config.json");

    // Manually create sample workstation config
    QJsonObject obj;
    obj[QStringLiteral("is_enrolled")] = true;
    obj[QStringLiteral("workstation_id")] = QStringLiteral("ws-test-uuid-123");
    obj[QStringLiteral("workstation_name")] = QStringLiteral("Aula-PC-01");
    obj[QStringLiteral("organization_id")] = QStringLiteral("org-test-456");
    obj[QStringLiteral("organization_name")] = QStringLiteral("Scuola Primaria Galilei");
    obj[QStringLiteral("api_token")] = QStringLiteral("k4e_inst_testtoken123");
    obj[QStringLiteral("backend_url")] = QStringLiteral("http://localhost:8000");
    obj[QStringLiteral("offline_allowance_hours")] = 72;
    obj[QStringLiteral("last_online_timestamp")] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);

    QDir().mkpath(dataDir);
    QFile f(configPath);
    QVERIFY(f.open(QIODevice::WriteOnly));
    f.write(QJsonDocument(obj).toJson());
    f.close();

    // Reload configuration
    wm->initialize();

    QCOMPARE(wm->isEnrolled(), true);
    QCOMPARE(wm->workstationId(), QStringLiteral("ws-test-uuid-123"));
    QCOMPARE(wm->workstationName(), QStringLiteral("Aula-PC-01"));
    QCOMPARE(wm->organizationId(), QStringLiteral("org-test-456"));
    QCOMPARE(wm->organizationName(), QStringLiteral("Scuola Primaria Galilei"));
    QCOMPARE(wm->backendUrl(), QStringLiteral("http://localhost:8000"));
}

void WorkstationManagerTest::testOfflineCacheAndPinVerification()
{
    WorkstationManager *wm = WorkstationManager::getInstance();

    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dataDir.isEmpty()) {
        dataDir = QDir::homePath() + QStringLiteral("/.kids4edu");
    }
    QString cachePath = dataDir + QStringLiteral("/learner_cache.json");

    // Compute expected PBKDF2 salted PIN hash for PIN "1234" (100,000 iterations)
    QString learnerId = QStringLiteral("lrn_alice_test");
    QString salt = QStringLiteral("salt_alice_123");
    QByteArray derived = QPasswordDigestor::deriveKeyPbkdf2(
        QCryptographicHash::Sha256,
        QByteArray("1234"),
        salt.toUtf8(),
        100000,
        32
    );
    QString hash = QString::fromLatin1(derived.toHex());

    QJsonObject root;
    QJsonArray arr;
    QJsonObject l1;
    l1[QStringLiteral("id")] = learnerId;
    l1[QStringLiteral("display_name")] = QStringLiteral("Alice Test");
    l1[QStringLiteral("has_pin")] = true;
    arr.append(l1);
    root[QStringLiteral("profiles")] = arr;

    QJsonObject pinHashes;
    QJsonObject credObj;
    credObj[QStringLiteral("hash")] = hash;
    credObj[QStringLiteral("salt")] = salt;
    credObj[QStringLiteral("cached_at")] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    credObj[QStringLiteral("failed_attempts")] = 0;
    credObj[QStringLiteral("locked_until")] = QStringLiteral("");
    pinHashes[learnerId] = credObj;
    root[QStringLiteral("pin_hashes")] = pinHashes;

    QFile f(cachePath);
    QVERIFY(f.open(QIODevice::WriteOnly));
    f.write(QJsonDocument(root).toJson());
    f.close();

    // Reload cache
    wm->initialize();

    QCOMPARE(wm->learnerProfiles().size(), 1);
    QCOMPARE(wm->learnerProfiles().first().toMap().value(QStringLiteral("display_name")).toString(), QStringLiteral("Alice Test"));

    // Verify correct PIN
    bool okCorrect = wm->verifyOfflinePin(learnerId, QStringLiteral("1234"));
    QVERIFY(okCorrect);

    // Verify wrong PIN fails
    bool okWrong = wm->verifyOfflinePin(learnerId, QStringLiteral("0000"));
    QVERIFY(!okWrong);
}

void WorkstationManagerTest::testSecurityHardeningAndLockout()
{
    WorkstationManager *wm = WorkstationManager::getInstance();
    QString learnerId = QStringLiteral("lrn_alice_test");

    // Perform 4 more failed attempts (already 1 from previous test, total 5)
    for (int i = 0; i < 4; ++i) {
        bool res = wm->verifyOfflinePin(learnerId, QStringLiteral("9999"));
        QVERIFY(!res);
    }

    // Now account must be locked out: even correct PIN must be rejected!
    bool resDuringLockout = wm->verifyOfflinePin(learnerId, QStringLiteral("1234"));
    QVERIFY(!resDuringLockout);
}

void WorkstationManagerTest::testGuestLoginAndLearnerSwitch()
{
    WorkstationManager *wm = WorkstationManager::getInstance();
    TelemetryManager *tm = TelemetryManager::getInstance();

    wm->loginAsGuest();
    QCOMPARE(wm->currentLearnerId(), QStringLiteral("anonymous"));
    QCOMPARE(wm->currentLearnerName(), QStringLiteral("Ospite"));
    QCOMPARE(tm->userId(), QStringLiteral("anonymous"));

    wm->logoutLearner();
    QCOMPARE(wm->currentLearnerId(), QStringLiteral("anonymous"));
}

void WorkstationManagerTest::testDiagnosticsInfoIntegrity()
{
    WorkstationManager *wm = WorkstationManager::getInstance();
    QVariantMap diag = wm->getDiagnosticsInfo();

    QVERIFY(diag.contains(QStringLiteral("is_enrolled")));
    QVERIFY(diag.contains(QStringLiteral("workstation_id")));
    QVERIFY(diag.contains(QStringLiteral("workstation_name")));
    QVERIFY(diag.contains(QStringLiteral("organization_id")));
    QVERIFY(diag.contains(QStringLiteral("organization_name")));
    QVERIFY(diag.contains(QStringLiteral("pending_events")));
    QVERIFY(diag.contains(QStringLiteral("synced_events")));
    QVERIFY(diag.contains(QStringLiteral("client_version")));
    QCOMPARE(diag.value(QStringLiteral("client_version")).toString(), QStringLiteral("2.1.0"));

    // Check security: api_token must NOT be exposed in diagnostics
    QVERIFY(!diag.contains(QStringLiteral("api_token")));
    QVERIFY(!diag.contains(QStringLiteral("token")));
    QVERIFY(!diag.contains(QStringLiteral("password")));
}

void WorkstationManagerTest::cleanupTestCase()
{
    // Clean test files
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dataDir.isEmpty()) {
        dataDir = QDir::homePath() + QStringLiteral("/.kids4edu");
    }
    QFile::remove(dataDir + QStringLiteral("/workstation_config.json"));
    QFile::remove(dataDir + QStringLiteral("/learner_cache.json"));
}

QTEST_GUILESS_MAIN(WorkstationManagerTest)
#include "WorkstationManagerTest.moc"
