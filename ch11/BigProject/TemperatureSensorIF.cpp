#include "TemperatureSensorIF.h"

TemperatureSensorIF::TemperatureSensorIF(QObject *parent)
    : QObject(parent),
      m_readingSecs(30)
{

}

int TemperatureSensorIF::readingSecs() const
{
    return m_readingSecs;
}

void TemperatureSensorIF::setReadingSecs(int readingSecs)
{
    m_readingSecs = readingSecs;
}
