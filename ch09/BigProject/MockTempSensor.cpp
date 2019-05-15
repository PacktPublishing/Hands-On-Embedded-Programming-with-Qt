#include "MockTempSensor.h"
#include <QScxmlStateMachine>

MockTempSensor::MockTempSensor(QObject *parent) :
    TemperatureSensorIF (parent),
    m_sm(nullptr)
{
    m_lastTemp = 17.5;
    m_sendTempTimer.setInterval(1000);
    m_sendTempTimer.setSingleShot(false);

    connect(&m_sendTempTimer, &QTimer::timeout, this, [this]() {
        QDateTime now = QDateTime::currentDateTime();

        // create a temp change of -4 to 5 based on the time
        float change;

        auto states = (m_sm != nullptr) ? m_sm->activeStateNames(false) : QStringList();

        if (states.contains("Heating"))
        {
            change = 0.4;
        }
        else if (states.contains("Cooling"))
        {
            change = -0.4;
        }
        else
        {
            float delta = states.contains("Fan") ? (float) 0.01 : (float) 0.04;
            change = static_cast<float>(((qrand() % 100) - 51) * delta);
        }
        m_lastTemp += change;

        emit newTemperature(now, m_lastTemp);
    });

    m_sendTempTimer.start();
}

const QScxmlStateMachine *MockTempSensor::sm() const
{
    return m_sm;
}

void MockTempSensor::setSm(const QScxmlStateMachine *sm)
{
    m_sm = sm;
}
