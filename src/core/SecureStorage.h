/* Kids4Edu - SecureStorage.h
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Core Platform Team
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Native OS Keyring / Credential Manager integration.
 * - macOS: Apple Keychain Services API (SecItemAdd, SecItemCopyMatching, SecItemDelete)
 * - Windows: Windows Credential Manager API (CredWriteW, CredReadW, CredDeleteW)
 * - Linux / Fallback: Session-only fail-closed in-memory storage (no plaintext on disk, no custom proprietary crypto).
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
    static bool isSessionOnly();
    static void setSessionSecret(const QString &service, const QString &account, const QString &secret);

private:
#if defined(Q_OS_MACOS)
    static bool storeKeychainSecret(const QString &service, const QString &account, const QString &secret);
    static QString getKeychainSecret(const QString &service, const QString &account);
    static bool deleteKeychainSecret(const QString &service, const QString &account);
#elif defined(Q_OS_WIN)
    static bool storeWindowsCredSecret(const QString &service, const QString &account, const QString &secret);
    static QString getWindowsCredSecret(const QString &service, const QString &account);
    static bool deleteWindowsCredSecret(const QString &service, const QString &account);
#endif
};

#endif // SECURESTORAGE_H
