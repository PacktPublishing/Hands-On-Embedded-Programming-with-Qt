#ifndef AMBIENTSENSOR_H
#define AMBIENTSENSOR_H

#include "TemperatureSensorIF.h"
#include <QTimer>

class QAmbientTemperatureSensor;

class AmbientTempSensor : public TemperatureSensorIF
{
public:
    AmbientTempSensor(QObject *parent = nullptr);

    bool connected() const;

private:
    QAmbientTemperatureSensor   *m_sensor;      ///< The QSensor we will use
    bool                        m_connected;    ///< True if we could connect to the sensor
    QTimer                      m_updateTimer;  ///< When to update things

private slots:
    void readSensor();
};

#endif // AMBIENTSENSOR_H
