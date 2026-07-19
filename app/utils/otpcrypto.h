#pragma once

// Pure, dependency-light OTP helpers, extracted so they can be unit-tested
// without dragging in OTPPairingManager (which pulls NvHTTP/network/OpenSSL).
// The algorithm mirrors the Artemis Android implementation:
//   SHA-256( pin + salt + passphrase ) -> uppercase hex.
//
// TODO(selene): make OTPPairingManager::generateOTPHash / validatePinFormat
// delegate to these (de-duplicates the logic that today lives in the manager
// and in the legacy test_otp_hash.cpp/test_hash.py), and drop the qDebug() that
// currently logs the PIN/salt in OTPPairingManager.

#include <QString>
#include <QCryptographicHash>

namespace OtpCrypto {

  // SHA-256(pin + salt + passphrase) as an uppercase hex string.
  inline QString generateOtpHash(const QString &pin, const QString &salt, const QString &passphrase) {
    const QString plainText = pin + salt + passphrase;
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(plainText.toUtf8());
    return hash.result().toHex().toUpper();
  }

  // A valid PIN is exactly 4 characters, all decimal digits.
  inline bool validatePinFormat(const QString &pin) {
    if (pin.length() != 4) {
      return false;
    }
    for (const QChar &c : pin) {
      if (!c.isDigit()) {
        return false;
      }
    }
    return true;
  }

}  // namespace OtpCrypto
