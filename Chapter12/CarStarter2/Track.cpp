#include "Track.h"

#include <QMutex>
#include <QWaitCondition>
#include <QThread>
#include <QTextStream>

Track::Track(QMutex *mutex, QWaitCondition *condition, QObject *parent)
    : QObject(parent),
      m_maxCars(1),
      m_carsOnTrack(0),
      m_trackMutex(mutex),
      m_condition(condition)
{

}

int Track::maxCars() const
{
    return m_maxCars;
}

void Track::setMaxCars(int maxCars)
{
    m_maxCars = maxCars;
}

int Track::carsOnTrack() const
{
    return m_carsOnTrack;
}

void Track::setCarsOnTrack(int carsOnTrack)
{
    m_carsOnTrack = carsOnTrack;
}

QMutex *Track::trackMutex() const
{
    return m_trackMutex;
}

void Track::setTrackMutex(QMutex *trackMutex)
{
    m_trackMutex = trackMutex;
}

QWaitCondition *Track::condition() const
{
    return m_condition;
}

void Track::setCondition(QWaitCondition *condition)
{
    m_condition = condition;
}

void Track::run()
{
    QTextStream qcout(stdout);

    qcout << "Track is running." << endl;
    forever {
        qcout << "Press 1 to start 1 car, a to start all cars" << endl;
        auto c = getchar();
        if (c == '1') {
            qcout << "Starting 1 car" << endl;
            m_condition->wakeOne();
        } else if (c == 'A' || c == 'a') {
            qcout << "Starting ALL cars" << endl;
            m_condition->wakeAll();
        }
    }
}
