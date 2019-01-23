#include "TimerCatcher.h"

#include <QTextStream>

TimerCatcher::TimerCatcher(QObject *parent) : QObject(parent)
{
    // connect the time of m_timer to our catchExpiredTimer
    connect(&m_timer, &QTimer::timeout, this, &TimerCatcher::catchExpiredTimer);

    m_timer.setSingleShot(false);   // repeating, not single shot
    m_timer.start(5000);            // 5000 msec = 5 sec
}

void TimerCatcher::catchExpiredTimer()
{
    QTextStream(stdout) << "Got Timeout!" << endl;
}
