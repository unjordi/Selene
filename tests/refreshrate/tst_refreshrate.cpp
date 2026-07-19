#include <QtTest>
#include <QLocale>

#include "utils/refreshrate.h"

class TstRefreshRate: public QObject {
  Q_OBJECT
private slots:
  void parseDot() {
    double v = 0;
    QVERIFY(RefreshRate::parse("59.94", &v));
    QVERIFY(qFuzzyCompare(v, 59.94));
  }

  void parseComma() {
    double v = 0;
    QVERIFY(RefreshRate::parse("59,94", &v));
    QVERIFY(qFuzzyCompare(v, 59.94));
  }

  // The whole point of #65: parsing must not depend on the process locale.
  void parseIsLocaleIndependent() {
    const QLocale prev = QLocale();
    QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));  // comma decimal
    double a = 0, b = 0;
    const bool okA = RefreshRate::parse("119.88", &a);
    const bool okB = RefreshRate::parse("119,88", &b);
    QLocale::setDefault(prev);
    QVERIFY(okA);
    QVERIFY(okB);
    QVERIFY(qFuzzyCompare(a, 119.88));
    QVERIFY(qFuzzyCompare(b, 119.88));
  }

  void rejectsGarbage_data() {
    QTest::addColumn<QString>("in");
    QTest::newRow("empty") << "";
    QTest::newRow("alpha") << "abc";
    QTest::newRow("zero")  << "0";
    QTest::newRow("neg")   << "-60";
  }
  void rejectsGarbage() {
    QFETCH(QString, in);
    double v = 0;
    QVERIFY(!RefreshRate::parse(in, &v));
  }

  void milliHz() {
    QCOMPARE(RefreshRate::toMilliHz(59.94), 59940);
    QCOMPARE(RefreshRate::toMilliHz(60.0), 60000);
  }
};

QTEST_GUILESS_MAIN(TstRefreshRate)
#include "tst_refreshrate.moc"
