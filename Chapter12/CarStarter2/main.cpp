#include <QCoreApplication>
#include "Car.h"
#include "Track.h"

#include <QMutex>
#include <QWaitCondition>
#include <QTimer>
#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QMutex          trackMutex;
    QWaitCondition  condition;

    // setup the track
    Track   track(&trackMutex, &condition);
    auto    trackThead = new QThread;
    track.setMaxCars(10);
    track.setCarsOnTrack(0);
    track.moveToThread(trackThead);
    trackThead->start();

    QTimer::singleShot(1, &track, &Track::run);

    // setup the cars
    QList<int>  carNumbers {1, 36, 24, 902, 201};
    for (auto cn : carNumbers)
    {
        auto car = new Car(cn, &trackMutex, &condition);
        auto carThread = new QThread;
        car->moveToThread(carThread);
        carThread->start();
        QTimer::singleShot(1, car, &Car::run);
    }

    return a.exec();
}
