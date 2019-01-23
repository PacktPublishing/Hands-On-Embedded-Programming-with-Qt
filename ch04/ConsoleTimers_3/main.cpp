#include "TimerCatcher.h"

#include <QCoreApplication>
#include <QTextStream>
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TimerCatcher    tc;

    QTextStream a_cout(stdout);         // create a stream to stdout
    QTimer tocTimer;                    // our timer
    tocTimer.setInterval(1000);
    tocTimer.setSingleShot(false);

    // connect to the lambda function
    QObject::connect(&tocTimer, &QTimer::timeout, [&a_cout]() {a_cout << "tic " << endl;});
    tocTimer.start();                   // start the timer

    return a.exec();
}
