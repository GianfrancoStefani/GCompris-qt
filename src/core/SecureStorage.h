/* Kids4Edu - SecureStorage.h
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Core Platform Team
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef SECURESTORAGE_H
#define SECURESTORAGE_H

#include <QString>
#include <QByteArray>

class SecureStorage
{
public:
    static bool storeSecret(const QString &service, const QString &account, const QString &secret);
    static QString getSecret(const QString &service, const QString &account);
    static bool deleteSecret(const QString &service, const QString &account);

    static bool isNativeKeyringAvailable();

private:
    static QString getVaultPath();
    static QByteArray getDerivationKey();
    static QByteArray encryptData(const QByteArray &plain, const QByteArray &key);
    static QByteArray decryptData(const QByteArray &cipher, const QByteArray &key);

    static bool storeVaultSecret(const QString &service, const QString &account, const QString &secret);
    static QString getVaultSecret(const QString &service, const QString &account);
    static bool deleteVaultSecret(const QString &service, const QString &account);
};

#endif // SECURESTORAGE_H
