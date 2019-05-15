#include "AmbientTempSensor.h"

#include <QAmbientTemperatureSensor>
#include <QSensorManager>
#include <QDateTime>

AmbientTempSensor::AmbientTempSensor(QObject *parent) :
    TemperatureSensorIF(parent),
    m_sensor(nullptr),
    m_connected(false)
{
    // find the sensor and start it
    m_sensor = new QAmbientTemperatureSensor(this);
    m_connected = m_sensor->connectToBackend();
    m_sensor->start();

    // connect the timer to read the sensor
    connect(&m_updateTimer, &QTimer::timeout,
            this, &AmbientTempSensor::readSensor);
    m_updateTimer.setInterval(1000);
    m_updateTimer.setSingleShot(false);
    m_updateTimer.start();
}

bool AmbientTempSensor::connected() const
{
    return m_connected;
}

void AmbientTempSensor::readSensor()
{
    auto reading = m_sensor->reading();
    float temp = reading->temperature();
    auto now = QDateTime::currentDateTime();
    emit newTemperature(now, temp);
}
