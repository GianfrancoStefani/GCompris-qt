/* Kids4Edu - SecureStorage.cpp
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Core Platform Team
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "SecureStorage.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QPasswordDigestor>
#include <QMessageAuthenticationCode>
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostInfo>
#include <QSysInfo>
#include <QDebug>

bool SecureStorage::isNativeKeyringAvailable()
{
#if defined(Q_OS_MACOS)
    QFileInfo secBin(QStringLiteral("/usr/bin/security"));
    return secBin.exists() && secBin.isExecutable();
#else
    return false;
#endif
}

bool SecureStorage::storeSecret(const QString &service, const QString &account, const QString &secret)
{
    if (service.isEmpty() || account.isEmpty()) {
        return false;
    }

#if defined(Q_OS_MACOS)
    if (isNativeKeyringAvailable()) {
        QProcess proc;
        QStringList args;
        args << QStringLiteral("add-generic-password")
             << QStringLiteral("-U")
             << QStringLiteral("-s") << service
             << QStringLiteral("-a") << account
             << QStringLiteral("-w") << secret;

        proc.start(QStringLiteral("/usr/bin/security"), args);
        if (proc.waitForFinished(3000) && proc.exitStatus() == QProcess::NormalExit && proc.exitCode() == 0) {
            return true;
        }
        qWarning() << "Kids4Edu SecureStorage: macOS security CLI failed (code" << proc.exitCode() << "), falling back to encrypted vault.";
    }
#endif

    return storeVaultSecret(service, account, secret);
}

QString SecureStorage::getSecret(const QString &service, const QString &account)
{
    if (service.isEmpty() || account.isEmpty()) {
        return QString();
    }

#if defined(Q_OS_MACOS)
    if (isNativeKeyringAvailable()) {
        QProcess proc;
        QStringList args;
        args << QStringLiteral("find-generic-password")
             << QStringLiteral("-s") << service
             << QStringLiteral("-a") << account
             << QStringLiteral("-w");

        proc.start(QStringLiteral("/usr/bin/security"), args);
        if (proc.waitForFinished(3000) && proc.exitStatus() == QProcess::NormalExit && proc.exitCode() == 0) {
            QString res = QString::fromUtf8(proc.readAllStandardOutput()).trimmed();
            if (!res.isEmpty()) {
                return res;
            }
        }
    }
#endif

    return getVaultSecret(service, account);
}

bool SecureStorage::deleteSecret(const QString &service, const QString &account)
{
    if (service.isEmpty() || account.isEmpty()) {
        return false;
    }

    bool success = false;

#if defined(Q_OS_MACOS)
    if (isNativeKeyringAvailable()) {
        QProcess proc;
        QStringList args;
        args << QStringLiteral("delete-generic-password")
             << QStringLiteral("-s") << service
             << QStringLiteral("-a") << account;

        proc.start(QStringLiteral("/usr/bin/security"), args);
        if (proc.waitForFinished(3000) && proc.exitStatus() == QProcess::NormalExit && proc.exitCode() == 0) {
            success = true;
        }
    }
#endif

    bool vaultDeleted = deleteVaultSecret(service, account);
    return success || vaultDeleted;
}

QString SecureStorage::getVaultPath()
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
    return dataDir + QStringLiteral("/.secure_vault");
}

QByteArray SecureStorage::getDerivationKey()
{
    // Derive machine unique identifier
    QByteArray machineIdentity;

    QFile machineIdFile(QStringLiteral("/etc/machine-id"));
    if (machineIdFile.open(QIODevice::ReadOnly)) {
        machineIdentity = machineIdFile.readAll().trimmed();
        machineIdFile.close();
    }
    if (machineIdentity.isEmpty()) {
        QFile dbusIdFile(QStringLiteral("/var/lib/dbus/machine-id"));
        if (dbusIdFile.open(QIODevice::ReadOnly)) {
            machineIdentity = dbusIdFile.readAll().trimmed();
            dbusIdFile.close();
        }
    }
    if (machineIdentity.isEmpty()) {
        machineIdentity = QSysInfo::machineUniqueId();
    }
    if (machineIdentity.isEmpty()) {
        machineIdentity = (QHostInfo::localHostName() + QStringLiteral("_k4e_salt_") + qgetenv("USER")).toUtf8();
    }

    QByteArray masterSalt = QByteArrayLiteral("kids4edu_native_vault_salt_v1");
    return QPasswordDigestor::deriveKeyPbkdf2(
        QCryptographicHash::Sha256,
        machineIdentity,
        masterSalt,
        50000,
        32
    );
}

QByteArray SecureStorage::encryptData(const QByteArray &plain, const QByteArray &key)
{
    if (plain.isEmpty() || key.isEmpty()) {
        return QByteArray();
    }

    // 16-byte random IV/nonce
    QByteArray nonce(16, 0);
    for (int i = 0; i < 16; ++i) {
        nonce[i] = static_cast<char>(QRandomGenerator::global()->bounded(256));
    }

    // Generate keystream using HMAC-SHA256 in counter mode
    QByteArray cipher;
    cipher.resize(plain.size());
    int offset = 0;
    quint32 counter = 0;

    while (offset < plain.size()) {
        QByteArray counterBlock = nonce + QByteArray::number(counter++);
        QByteArray streamBlock = QMessageAuthenticationCode::hash(counterBlock, key, QCryptographicHash::Sha256);

        int chunk = qMin(static_cast<int>(streamBlock.size()), plain.size() - offset);
        for (int i = 0; i < chunk; ++i) {
            cipher[offset + i] = plain[offset + i] ^ streamBlock[i];
        }
        offset += chunk;
    }

    // Compute HMAC authentication tag over (nonce + cipher)
    QByteArray payload = nonce + cipher;
    QByteArray tag = QMessageAuthenticationCode::hash(payload, key, QCryptographicHash::Sha256);

    // Final layout: [16-byte nonce][N-byte cipher][32-byte HMAC tag]
    return payload + tag;
}

QByteArray SecureStorage::decryptData(const QByteArray &data, const QByteArray &key)
{
    // Minimum size: 16 (nonce) + 1 (data) + 32 (HMAC tag) = 49 bytes
    if (data.size() < 49 || key.isEmpty()) {
        return QByteArray();
    }

    int payloadLen = data.size() - 32;
    QByteArray payload = data.left(payloadLen);
    QByteArray expectedTag = data.mid(payloadLen, 32);

    // Verify HMAC tag before any decryption (Encrypt-then-MAC authentication)
    QByteArray computedTag = QMessageAuthenticationCode::hash(payload, key, QCryptographicHash::Sha256);
    if (computedTag != expectedTag) {
        qWarning() << "Kids4Edu SecureStorage: HMAC integrity verification failed on vault!";
        return QByteArray();
    }

    QByteArray nonce = payload.left(16);
    QByteArray cipher = payload.mid(16);

    QByteArray plain;
    plain.resize(cipher.size());
    int offset = 0;
    quint32 counter = 0;

    while (offset < cipher.size()) {
        QByteArray counterBlock = nonce + QByteArray::number(counter++);
        QByteArray streamBlock = QMessageAuthenticationCode::hash(counterBlock, key, QCryptographicHash::Sha256);

        int chunk = qMin(static_cast<int>(streamBlock.size()), cipher.size() - offset);
        for (int i = 0; i < chunk; ++i) {
            plain[offset + i] = cipher[offset + i] ^ streamBlock[i];
        }
        offset += chunk;
    }

    return plain;
}

bool SecureStorage::storeVaultSecret(const QString &service, const QString &account, const QString &secret)
{
    QString path = getVaultPath();
    QByteArray key = getDerivationKey();

    QJsonObject root;
    QFile readFile(path);
    if (readFile.open(QIODevice::ReadOnly)) {
        QByteArray enc = readFile.readAll();
        readFile.close();
        QByteArray dec = decryptData(enc, key);
        if (!dec.isEmpty()) {
            QJsonDocument doc = QJsonDocument::fromJson(dec);
            if (doc.isObject()) {
                root = doc.object();
            }
        }
    }

    QString entryKey = service + QStringLiteral(":") + account;
    root[entryKey] = secret;

    QByteArray plainJson = QJsonDocument(root).toJson(QJsonDocument::Compact);
    QByteArray encPayload = encryptData(plainJson, key);
    if (encPayload.isEmpty()) {
        return false;
    }

    QFile writeFile(path);
    if (!writeFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Kids4Edu SecureStorage: Failed to write vault at" << path;
        return false;
    }

    writeFile.write(encPayload);
    writeFile.close();

    // Restrict permissions to owner read/write ONLY (0600)
    QFile::setPermissions(path, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    return true;
}

QString SecureStorage::getVaultSecret(const QString &service, const QString &account)
{
    QString path = getVaultPath();
    QByteArray key = getDerivationKey();

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }

    QByteArray enc = file.readAll();
    file.close();

    QByteArray dec = decryptData(enc, key);
    if (dec.isEmpty()) {
        return QString();
    }

    QJsonDocument doc = QJsonDocument::fromJson(dec);
    if (!doc.isObject()) {
        return QString();
    }

    QString entryKey = service + QStringLiteral(":") + account;
    return doc.object().value(entryKey).toString();
}

bool SecureStorage::deleteVaultSecret(const QString &service, const QString &account)
{
    QString path = getVaultPath();
    QByteArray key = getDerivationKey();

    QFile readFile(path);
    if (!readFile.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray enc = readFile.readAll();
    readFile.close();

    QByteArray dec = decryptData(enc, key);
    if (dec.isEmpty()) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(dec);
    if (!doc.isObject()) {
        return false;
    }

    QJsonObject root = doc.object();
    QString entryKey = service + QStringLiteral(":") + account;
    if (!root.contains(entryKey)) {
        return false;
    }

    root.remove(entryKey);

    QByteArray plainJson = QJsonDocument(root).toJson(QJsonDocument::Compact);
    QByteArray encPayload = encryptData(plainJson, key);

    QFile writeFile(path);
    if (!writeFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    writeFile.write(encPayload);
    writeFile.close();
    QFile::setPermissions(path, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    return true;
}
