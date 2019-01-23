#include "TimerCatcher.h"

#include <QCoreApplication>
#include <QTextStream>
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TimerCatcher    tc;

    QTextStream a_cout(stdout);
    QTimer tocTimer;
    tocTimer.setInterval(1000);
    tocTimer.setSingleShot(false);
    QObject::connect(&tocTimer, &QTimer::timeout, [&a_cout]() {a_cout << "tic " << endl;});
    tocTimer.start();

    return a.exec();
}
