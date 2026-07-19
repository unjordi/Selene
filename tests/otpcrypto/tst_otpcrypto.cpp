#include <QtTest>

#include "utils/otpcrypto.h"

class TstOtpCrypto: public QObject {
  Q_OBJECT
private slots:
  // Server-parity lock. This is byte-for-byte the algorithm the Apollo/Helios
  // host uses to validate the OTP — hex(SHA-256(pin + salt + passphrase)),
  // uppercase, salt as the raw hex string (NOT decoded). See the host's
  // nvhttp.cpp: `util::hex(crypto::hash(one_time_pin + salt + otp_passphrase), true)`.
  // Value computed independently with sha256sum. If this ever drifts, client and
  // host OTP pairing breaks.
  void matchesHostOtpAlgorithm() {
    const QString h = OtpCrypto::generateOtpHash(
      "9067", "7e4f274a9a39bd8b3f36ef811d318076", "test");
    QCOMPARE(h, QStringLiteral(
      "104B19FE9C4E5100CC4A09FDAF4550A9DA6D746CAB598FBD6A66B5640757F368"));
  }

  void hashIsUppercaseHex64() {
    const QString h = OtpCrypto::generateOtpHash("1234", "abcd", "");
    QCOMPARE(h.length(), 64);
    QCOMPARE(h, h.toUpper());
    QVERIFY(QRegularExpression("^[0-9A-F]{64}$").match(h).hasMatch());
  }

  void differentInputsDifferentHash() {
    // Different content must hash differently.
    QVERIFY(OtpCrypto::generateOtpHash("1234", "ab", "x")
            != OtpCrypto::generateOtpHash("1234", "ab", "y"));
    QVERIFY(OtpCrypto::generateOtpHash("1111", "ab", "")
            != OtpCrypto::generateOtpHash("2222", "ab", ""));
  }


  void pinValidation_data() {
    QTest::addColumn<QString>("pin");
    QTest::addColumn<bool>("valid");
    QTest::newRow("ok")        << "9067"  << true;
    QTest::newRow("zeros")     << "0000"  << true;
    QTest::newRow("too short") << "123"   << false;
    QTest::newRow("too long")  << "12345" << false;
    QTest::newRow("letters")   << "12a4"  << false;
    QTest::newRow("empty")     << ""      << false;
    QTest::newRow("space")     << "12 4"  << false;
  }
  void pinValidation() {
    QFETCH(QString, pin);
    QFETCH(bool, valid);
    QCOMPARE(OtpCrypto::validatePinFormat(pin), valid);
  }
};

QTEST_GUILESS_MAIN(TstOtpCrypto)
#include "tst_otpcrypto.moc"
