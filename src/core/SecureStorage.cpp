/* Kids4Edu - SecureStorage.cpp
 *
 * SPDX-FileCopyrightText: 2026 Kids4Edu Core Platform Team
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Native OS Keyring / Credential Manager implementation.
 * Eliminates custom file vaults, proprietary encryption, and process execution CLI token leaks.
 */

#include "SecureStorage.h"

#include <QDebug>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>

#if defined(Q_OS_MACOS)
#include <Security/Security.h>
#include <CoreFoundation/CoreFoundation.h>
#elif defined(Q_OS_WIN)
#include <windows.h>
#include <wincred.h>
#endif

static QMap<QString, QString> s_sessionSecrets;
static QMutex s_sessionMutex;

bool SecureStorage::isSessionOnly()
{
    return qEnvironmentVariableIsSet("KIDS4EDU_SESSION_ONLY") || !isNativeKeyringAvailable();
}

void SecureStorage::setSessionSecret(const QString &service, const QString &account, const QString &secret)
{
    QMutexLocker locker(&s_sessionMutex);
    QString entryKey = service + QStringLiteral(":") + account;
    if (secret.isEmpty()) {
        s_sessionSecrets.remove(entryKey);
    } else {
        s_sessionSecrets[entryKey] = secret;
    }
}

bool SecureStorage::isNativeKeyringAvailable()
{
#if defined(Q_OS_MACOS)
    return true;
#elif defined(Q_OS_WIN)
    return true;
#else
    // On Linux without DBus Secret Service or in containerized headless testing,
    // fallback gracefully to fail-closed session-only storage.
    return false;
#endif
}

bool SecureStorage::storeSecret(const QString &service, const QString &account, const QString &secret)
{
    if (service.isEmpty() || account.isEmpty()) {
        return false;
    }

    if (qEnvironmentVariableIsSet("KIDS4EDU_SESSION_ONLY")) {
        setSessionSecret(service, account, secret);
        return true;
    }

#if defined(Q_OS_MACOS)
    return storeKeychainSecret(service, account, secret);
#elif defined(Q_OS_WIN)
    return storeWindowsCredSecret(service, account, secret);
#else
    // Fail-closed to in-memory session secret (no plaintext on disk)
    setSessionSecret(service, account, secret);
    return true;
#endif
}

QString SecureStorage::getSecret(const QString &service, const QString &account)
{
    if (service.isEmpty() || account.isEmpty()) {
        return QString();
    }

    if (qEnvironmentVariableIsSet("KIDS4EDU_SESSION_ONLY")) {
        QMutexLocker locker(&s_sessionMutex);
        QString entryKey = service + QStringLiteral(":") + account;
        return s_sessionSecrets.value(entryKey);
    }

#if defined(Q_OS_MACOS)
    return getKeychainSecret(service, account);
#elif defined(Q_OS_WIN)
    return getWindowsCredSecret(service, account);
#else
    QMutexLocker locker(&s_sessionMutex);
    QString entryKey = service + QStringLiteral(":") + account;
    return s_sessionSecrets.value(entryKey);
#endif
}

bool SecureStorage::deleteSecret(const QString &service, const QString &account)
{
    if (service.isEmpty() || account.isEmpty()) {
        return false;
    }

    {
        QMutexLocker locker(&s_sessionMutex);
        QString entryKey = service + QStringLiteral(":") + account;
        s_sessionSecrets.remove(entryKey);
    }

#if defined(Q_OS_MACOS)
    return deleteKeychainSecret(service, account);
#elif defined(Q_OS_WIN)
    return deleteWindowsCredSecret(service, account);
#else
    return true;
#endif
}

#if defined(Q_OS_MACOS)
bool SecureStorage::storeKeychainSecret(const QString &service, const QString &account, const QString &secret)
{
    CFStringRef sService = service.toCFString();
    CFStringRef sAccount = account.toCFString();
    QByteArray secretData = secret.toUtf8();
    CFDataRef sData = CFDataCreate(kCFAllocatorDefault, reinterpret_cast<const UInt8 *>(secretData.constData()), secretData.size());

    CFMutableDictionaryRef query = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    CFDictionarySetValue(query, kSecClass, kSecClassGenericPassword);
    CFDictionarySetValue(query, kSecAttrService, sService);
    CFDictionarySetValue(query, kSecAttrAccount, sAccount);
    CFDictionarySetValue(query, kSecValueData, sData);
    CFDictionarySetValue(query, kSecAttrAccessible, kSecAttrAccessibleAfterFirstUnlock);

    OSStatus status = SecItemAdd(query, nullptr);
    if (status == errSecDuplicateItem) {
        // Item already exists, perform in-place update
        CFMutableDictionaryRef updateQuery = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        CFDictionarySetValue(updateQuery, kSecClass, kSecClassGenericPassword);
        CFDictionarySetValue(updateQuery, kSecAttrService, sService);
        CFDictionarySetValue(updateQuery, kSecAttrAccount, sAccount);

        CFMutableDictionaryRef attributesToUpdate = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        CFDictionarySetValue(attributesToUpdate, kSecValueData, sData);

        status = SecItemUpdate(updateQuery, attributesToUpdate);
        CFRelease(updateQuery);
        CFRelease(attributesToUpdate);
    }

    CFRelease(query);
    CFRelease(sService);
    CFRelease(sAccount);
    CFRelease(sData);

    if (status != errSecSuccess) {
        qWarning() << "Kids4Edu SecureStorage: Apple Keychain SecItemAdd/Update returned status:" << status;
        // If Keychain access is denied or headless without unlock, fallback cleanly to session-only storage
        setSessionSecret(service, account, secret);
        return true;
    }
    return true;
}

QString SecureStorage::getKeychainSecret(const QString &service, const QString &account)
{
    CFStringRef sService = service.toCFString();
    CFStringRef sAccount = account.toCFString();

    CFMutableDictionaryRef query = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    CFDictionarySetValue(query, kSecClass, kSecClassGenericPassword);
    CFDictionarySetValue(query, kSecAttrService, sService);
    CFDictionarySetValue(query, kSecAttrAccount, sAccount);
    CFDictionarySetValue(query, kSecReturnData, kCFBooleanTrue);
    CFDictionarySetValue(query, kSecMatchLimit, kSecMatchLimitOne);

    CFTypeRef result = nullptr;
    OSStatus status = SecItemCopyMatching(query, &result);
    CFRelease(query);
    CFRelease(sService);
    CFRelease(sAccount);

    if (status == errSecSuccess && result != nullptr) {
        CFDataRef data = static_cast<CFDataRef>(result);
        QString secret = QString::fromUtf8(reinterpret_cast<const char *>(CFDataGetBytePtr(data)), static_cast<int>(CFDataGetLength(data)));
        CFRelease(result);
        return secret;
    }

    // Fallback to session secret if not found in Keychain
    QMutexLocker locker(&s_sessionMutex);
    QString entryKey = service + QStringLiteral(":") + account;
    return s_sessionSecrets.value(entryKey);
}

bool SecureStorage::deleteKeychainSecret(const QString &service, const QString &account)
{
    CFStringRef sService = service.toCFString();
    CFStringRef sAccount = account.toCFString();

    CFMutableDictionaryRef query = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    CFDictionarySetValue(query, kSecClass, kSecClassGenericPassword);
    CFDictionarySetValue(query, kSecAttrService, sService);
    CFDictionarySetValue(query, kSecAttrAccount, sAccount);

    OSStatus status = SecItemDelete(query);
    CFRelease(query);
    CFRelease(sService);
    CFRelease(sAccount);

    return (status == errSecSuccess || status == errSecItemNotFound);
}
#elif defined(Q_OS_WIN)
bool SecureStorage::storeWindowsCredSecret(const QString &service, const QString &account, const QString &secret)
{
    QString targetName = QStringLiteral("Kids4Edu:") + service + QStringLiteral(":") + account;
    std::wstring wTarget = targetName.toStdWString();
    std::wstring wUser = account.toStdWString();
    QByteArray secretData = secret.toUtf8();

    CREDENTIALW cred = {0};
    cred.Type = CRED_TYPE_GENERIC;
    cred.TargetName = const_cast<LPWSTR>(wTarget.c_str());
    cred.CredentialBlobSize = static_cast<DWORD>(secretData.size());
    cred.CredentialBlob = reinterpret_cast<LPBYTE>(const_cast<char *>(secretData.constData()));
    cred.Persist = CRED_PERSIST_LOCAL_MACHINE;
    cred.UserName = const_cast<LPWSTR>(wUser.c_str());

    BOOL res = CredWriteW(&cred, 0);
    if (!res) {
        setSessionSecret(service, account, secret);
        return true;
    }
    return true;
}

QString SecureStorage::getWindowsCredSecret(const QString &service, const QString &account)
{
    QString targetName = QStringLiteral("Kids4Edu:") + service + QStringLiteral(":") + account;
    std::wstring wTarget = targetName.toStdWString();

    PCREDENTIALW pCred = nullptr;
    if (CredReadW(wTarget.c_str(), CRED_TYPE_GENERIC, 0, &pCred)) {
        QString secret = QString::fromUtf8(reinterpret_cast<const char *>(pCred->CredentialBlob), static_cast<int>(pCred->CredentialBlobSize));
        CredFree(pCred);
        return secret;
    }

    QMutexLocker locker(&s_sessionMutex);
    QString entryKey = service + QStringLiteral(":") + account;
    return s_sessionSecrets.value(entryKey);
}

bool SecureStorage::deleteWindowsCredSecret(const QString &service, const QString &account)
{
    QString targetName = QStringLiteral("Kids4Edu:") + service + QStringLiteral(":") + account;
    std::wstring wTarget = targetName.toStdWString();
    return CredDeleteW(wTarget.c_str(), CRED_TYPE_GENERIC, 0) == TRUE;
}
#endif
