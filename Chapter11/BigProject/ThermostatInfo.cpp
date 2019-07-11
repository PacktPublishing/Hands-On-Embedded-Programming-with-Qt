#include "ThermostatInfo.h"

ThermostatInfo::ThermostatInfo(QObject *parent) : QObject(parent)
{

}

bool ThermostatInfo::fanOn() const
{
    return m_fanOn;
}

bool ThermostatInfo::heatingOn() const
{
    return m_heatingOn;
}

bool ThermostatInfo::coolingOn() const
{
    return m_coolingOn;
}

double ThermostatInfo::currentTemp() const
{
    return m_currentTemp;
}

double ThermostatInfo::minTemp() const
{
    return m_minTemp;
}

void ThermostatInfo::setMinTemp(double minTemp)
{
    emit minTempChanged(minTemp);
    m_minTemp = minTemp;
}

double ThermostatInfo::maxTemp() const
{
    return m_maxTemp;
}

void ThermostatInfo::setMaxTemp(double maxTemp)
{
    emit maxTempChanged(maxTemp);
    m_maxTemp = maxTemp;
}
