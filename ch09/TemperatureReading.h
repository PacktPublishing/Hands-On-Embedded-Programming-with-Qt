#ifndef TEMPERATUREDATA_H
#define TEMPERATUREDATA_H

#include <QDateTime>



class TemperatureReading
{
public:
    TemperatureReading();
    TemperatureReading(const QDateTime &timestamp, float temperature);
    TemperatureReading &operator=(const TemperatureReading &right) = default;
    ~TemperatureReading() = default;

    bool operator==(const TemperatureReading &right);
    bool operator<(const TemperatureReading &right);

    bool isValid() const;

    float temperatureC() const;
    void setTemperatureC(float temperatureC);

    QDateTime timestamp() const;
    void setTimestamp(const QDateTime &timestamp);

private:
    float       m_temperatureC; ///< the temperature in C
    QDateTime   m_timestamp;    ///< the time of the reading
};

#endif // TEMPERATUREDATA_H
