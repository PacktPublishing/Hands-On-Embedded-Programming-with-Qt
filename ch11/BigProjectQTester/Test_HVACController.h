#ifndef TEST_HVACCONTROLLER_H
#define TEST_HVACCONTROLLER_H

#include "HVACController.h"

#include <QTest>

class Test_HVACController : public QObject
{
    Q_OBJECT
public:
    explicit Test_HVACController(QObject *parent = nullptr);

signals:

public slots:

private slots:
    void sampleTests()
    {
        QWARN("This is a simple warning message.");
        QVERIFY((3 - 2) == 1);
        QCOMPARE(3 - 2, 1);
    }


    /// The data for the test
    void updateSMfromTemperatureTest_data()
    {
        QTest::addColumn<QStringList>("states");
        QTest::addColumn<float>("min");
        QTest::addColumn<float>("max");
        QTest::addColumn<float>("current");
        QTest::addColumn<QString>("transition");

        QTest::newRow("Idle-Between") << QStringList({"FanOff", "Idle"}) << 10.0f << 15.0f << 12.5f << QString("none");
        QTest::newRow("Idle-High")    << QStringList({"FanOff", "Idle"}) << 10.0f << 15.0f << 20.0f << QString("TooHot");
        QTest::newRow("Idle-Low")     << QStringList({"FanOff", "Idle"}) << 10.0f << 15.0f <<  5.0f << QString("TooCold");

        QTest::newRow("Heating-Between") << QStringList({"FanOn" , "Heating"}) << 10.0f << 15.0f << 12.5f << QString("TargetReached");
        QTest::newRow("Heating-High")    << QStringList({"FanOn" , "Heating"}) << 10.0f << 15.0f << 20.0f << QString("TargetReached");
        QTest::newRow("Heating-Low")     << QStringList({"FanOn" , "Heating"}) << 10.0f << 15.0f <<  5.0f << QString("none");

        QTest::newRow("Cooling-Between") << QStringList({"FanOn" , "Cooling"}) << 10.0f << 15.0f << 12.5f << QString("TargetReached");
        QTest::newRow("Cooling-High")    << QStringList({"FanOn" , "Cooling"}) << 10.0f << 15.0f << 20.0f << QString("none");
        QTest::newRow("Cooling-Low")     << QStringList({"FanOn" , "Cooling"}) << 10.0f << 15.0f <<  5.0f << QString("TargetReached");
    }

    /// The Test
    void updateSMfromTemperatureTest()
    {
        QFETCH(QStringList, states);
        QFETCH(float, min);
        QFETCH(float, max);
        QFETCH(float, current);
        QFETCH(QString, transition);

        QCOMPARE(HVACController::updateSMfromTemperature(min, max, current, states),
                 transition);
    }
};

#endif // TEST_HVACCONTROLLER_H
