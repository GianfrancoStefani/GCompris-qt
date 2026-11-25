/* GCompris - TelemetryManagerTest.cpp
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Project
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QObject>
#include <QTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

#include "src/core/TelemetryManager.h"

class TelemetryManagerTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testInProcessSwitchLearner();
    void cleanupTestCase();
};

void TelemetryManagerTest::initTestCase()
{
    TelemetryManager *tm = TelemetryManager::getInstance();
    QVERIFY(tm != nullptr);
}

void TelemetryManagerTest::testInProcessSwitchLearner()
{
    TelemetryManager *tm = TelemetryManager::getInstance();

    // 1. Initialize for Alice A1
    const QString orgId = QStringLiteral("org_synth_school_a");
    const QString aliceId = QStringLiteral("k4e_lrn_alice_a1");
    const QString bobId = QStringLiteral("k4e_lrn_bob_a2");

    tm->switchLearner(aliceId, orgId);
    QCOMPARE(tm->userId(), aliceId);
    QCOMPARE(tm->organizationId(), orgId);

    QString aliceSessionId = tm->sessionId();
    QVERIFY(!aliceSessionId.isEmpty());

    // Record events for Alice A1
    tm->recordEvent(QStringLiteral("activity_started"), QStringLiteral("maze"), 1, QVariant(), QString(), QVariantMap());
    tm->recordEvent(QStringLiteral("level_started"), QStringLiteral("maze"), 1, QVariant(), QString(), QVariantMap());
    
    QVariantMap aliceProb;
    aliceProb[QStringLiteral("step")] = 1;
    tm->recordEvent(QStringLiteral("problem_presented"), QStringLiteral("maze"), 1, QVariant(), QString(), aliceProb, QStringLiteral("prob_a1_01"));
    tm->recordEvent(QStringLiteral("action_performed"), QStringLiteral("maze"), 1, QVariant(), QString(), QVariantMap(), QStringLiteral("prob_a1_01"));
    tm->flush();

    // 2. Perform IN-PROCESS learner switch to Bob A2
    tm->switchLearner(bobId, orgId);
    QCOMPARE(tm->userId(), bobId);
    QCOMPARE(tm->organizationId(), orgId);

    QString bobSessionId = tm->sessionId();
    QVERIFY(!bobSessionId.isEmpty());
    QVERIFY(bobSessionId != aliceSessionId); // MUST be a fresh session

    // Record events for Bob A2
    tm->recordEvent(QStringLiteral("activity_started"), QStringLiteral("maze"), 1, QVariant(), QString(), QVariantMap());
    
    QVariantMap bobProb;
    bobProb[QStringLiteral("step")] = 1;
    tm->recordEvent(QStringLiteral("problem_presented"), QStringLiteral("maze"), 1, QVariant(), QString(), bobProb, QStringLiteral("prob_b1_01"));
    tm->recordEvent(QStringLiteral("action_performed"), QStringLiteral("maze"), 1, QVariant(), QString(), QVariantMap(), QStringLiteral("prob_b1_01"));
    tm->endSession();
    tm->flush();

    // 3. Inspect SQLite directly to verify in-process isolation
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/telemetry_offline.sqlite");
    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("test_verify_conn"));
    db.setDatabaseName(dbPath);
    QVERIFY(db.open());

    QSqlQuery q(db);
    // Count events for Alice
    QVERIFY(q.exec(QStringLiteral("SELECT count(*), count(DISTINCT session_id) FROM telemetry_events WHERE pseudonymous_user_id = 'k4e_lrn_alice_a1'")));
    QVERIFY(q.next());
    int aliceEvents = q.value(0).toInt();
    int aliceSessions = q.value(1).toInt();
    QVERIFY(aliceEvents >= 4);
    QCOMPARE(aliceSessions, 1);

    // Count events for Bob
    QVERIFY(q.exec(QStringLiteral("SELECT count(*), count(DISTINCT session_id) FROM telemetry_events WHERE pseudonymous_user_id = 'k4e_lrn_bob_a2'")));
    QVERIFY(q.next());
    int bobEvents = q.value(0).toInt();
    int bobSessions = q.value(1).toInt();
    QVERIFY(bobEvents >= 3);
    QCOMPARE(bobSessions, 1);

    // Check learner switch event at the end of Alice's session
    QVERIFY(q.exec(QStringLiteral("SELECT payload_json FROM telemetry_events WHERE pseudonymous_user_id = 'k4e_lrn_alice_a1' AND event_type = 'session_ended'")));
    QVERIFY(q.next());
    QString payload = q.value(0).toString();
    QVERIFY(payload.contains(QStringLiteral("learner_switched")));
    QVERIFY(payload.contains(QStringLiteral("k4e_lrn_bob_a2")));

    // Verify monotonic sequence numbering per session
    for (const QString &sessId : {aliceSessionId, bobSessionId}) {
        q.prepare(QStringLiteral("SELECT sequence_number FROM telemetry_events WHERE session_id = :sid ORDER BY sequence_number ASC"));
        q.bindValue(QStringLiteral(":sid"), sessId);
        QVERIFY(q.exec());
        int lastSeq = 0;
        int count = 0;
        while (q.next()) {
            int seq = q.value(0).toInt();
            QCOMPARE(seq, lastSeq + 1); // Strictly sequential 1, 2, 3...
            lastSeq = seq;
            count++;
        }
        QVERIFY(count > 0);
    }

    db.close();
    QSqlDatabase::removeDatabase(QStringLiteral("test_verify_conn"));
}

void TelemetryManagerTest::cleanupTestCase()
{
}

QTEST_MAIN(TelemetryManagerTest)
#include "TelemetryManagerTest.moc"
