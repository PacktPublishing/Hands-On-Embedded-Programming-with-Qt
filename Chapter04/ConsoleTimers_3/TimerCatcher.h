#ifndef TIMERCATCHER_H
#define TIMERCATCHER_H

#include <QObject>
#include <QTimer>

class TimerCatcher : public QObject
{
    Q_OBJECT
public:
    explicit TimerCatcher(QObject *parent = nullptr);

signals:
    void newCount(int count);   ///< sent when we have a new count

public slots:
    void catchExpiredTimer();           ///< called when m_timer timesout
    void printCountDirect(int count);   ///< direct connection print the count value
    void printCountQueued(int count);   ///< queued print the count value

private:
    QTimer  m_timer;            ///< my timer
    int     m_count;            ///< my count
};

#endif // TIMERCATCHER_H
