#include "TemperatureReading.h"

TemperatureReading::TemperatureReading()
    : m_temperatureC(0.0),
      m_timestamp(QDateTime())
{
    // nothing to do here
}

TemperatureReading::TemperatureReading(const QDateTime &timestamp, float temperature)
    : m_temperatureC(temperature),
      m_timestamp(timestamp)
{
    // nothing to see here
}

bool TemperatureReading::operator==(const TemperatureReading &right)
{
    return ((right.m_timestamp      == m_timestamp) &&
            (right.m_temperatureC   == m_temperatureC));
}

bool TemperatureReading::operator<(const TemperatureReading &right)
{
    return (m_timestamp.toMSecsSinceEpoch() < right.m_timestamp.toMSecsSinceEpoch());
}

bool TemperatureReading::isValid() const
{
    return m_timestamp.isValid(); // timestamp init'd to non-valid
}

float TemperatureReading::temperatureC() const
{
    return m_temperatureC;
}

void TemperatureReading::setTemperatureC(float temperatureC)
{
    m_temperatureC = temperatureC;
}

QDateTime TemperatureReading::timestamp() const
{
    return m_timestamp;
}

void TemperatureReading::setTimestamp(const QDateTime &timestamp)
{
    m_timestamp = timestamp;
}
