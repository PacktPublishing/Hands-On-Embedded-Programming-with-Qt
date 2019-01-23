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

public slots:
    void catchExpiredTimer();   ///< called when m_timer timesout

private:
    QTimer  m_timer;            ///< my timer
};

#endif // TIMERCATCHER_H
