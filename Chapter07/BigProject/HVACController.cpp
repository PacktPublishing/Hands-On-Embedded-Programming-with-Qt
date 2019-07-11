#include "HVACController.h"

#include <QDebug>

HVACController::HVACController(QObject *parent) : QObject(parent)
{
    // nothing to do here
}

void HVACController::setHeatingOn(bool on)
{
    qDebug() << "Heating set to" << on;
}

void HVACController::setCoolingOn(bool on)
{
    qDebug() << "Cooling set to" << on;
}

void HVACController::setFanOn(bool on)
{
    qDebug() << "Fan set to" << on;
}
