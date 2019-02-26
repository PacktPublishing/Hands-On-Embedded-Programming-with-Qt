#ifndef MOCKTEMPSENSOR_H
#define MOCKTEMPSENSOR_H

#include "TemperatureSensorIF.h"
#include <QTimer>


class MockTempSensor : public TemperatureSensorIF
{
public:
    MockTempSensor(QObject *parent = nullptr);

private:
    QTimer  m_sendTempTimer;
    float   m_lastTemp;
};

#endif // MOCKTEMPSENSOR_H
