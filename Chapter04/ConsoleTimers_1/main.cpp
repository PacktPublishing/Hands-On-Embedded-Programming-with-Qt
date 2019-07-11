#include "TimerCatcher.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TimerCatcher    tc;

    return a.exec();
}
