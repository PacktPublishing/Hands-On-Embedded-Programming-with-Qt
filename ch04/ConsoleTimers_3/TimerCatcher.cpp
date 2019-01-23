#include "TimerCatcher.h"

#include <QTextStream>

TimerCatcher::TimerCatcher(QObject *parent) : QObject(parent), m_count(0)
{
    // connect newCount and printCount
    connect(this, &TimerCatcher::newCount, this, &TimerCatcher::printCountDirect, Qt::DirectConnection);
    connect(this, &TimerCatcher::newCount, this, &TimerCatcher::printCountQueued, Qt::QueuedConnection);

    // connect the time of m_timer to our catchExpiredTimer
    connect(&m_timer, &QTimer::timeout, this, &TimerCatcher::catchExpiredTimer);

    m_timer.setSingleShot(false);   // repeating, not single shot
    m_timer.start(5000);            // 5000 msec = 5 sec
}

void TimerCatcher::catchExpiredTimer()
{
    emit newCount(++m_count);
    QTextStream(stdout) << "Got Timeout!" << endl;
}

void TimerCatcher::printCountDirect(int count)
{
    QTextStream(stdout) << "d count = " << count << endl;
}

void TimerCatcher::printCountQueued(int count)
{
    QTextStream(stdout) << "q count = " << count << endl;
}
