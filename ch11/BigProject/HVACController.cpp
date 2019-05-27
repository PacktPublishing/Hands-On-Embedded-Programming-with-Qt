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

/// update the state machine based on the temperatures
QString HVACController::updateSMfromTemperature(float minTemp,
                                                float maxTemp,
                                                float currentTemp,
                                                QStringList activeStates)
{
    QString transition = "none";

    if (!activeStates.contains("Wait")) {
        // update the state machine based on settings
        if (currentTemp > maxTemp) {
            if (activeStates.contains("Heating")) {
                transition = "TargetReached";
            } else {
                transition = "TooHot";
            }
        } else if (currentTemp < minTemp) {
            if (activeStates.contains("Heating")) {
                transition = "TargetReached";
            } else {
                transition = "TooCold";
            }
        } else {
            transition = "TargetReached";
        }
    }
    return transition;
}
