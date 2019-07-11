#include "CalcObject.h"
#include <QRandomGenerator64>

#include <QDebug>
#include <QThread>

CalcObject::CalcObject(QObject *parent)
    : QObject(parent),
      m_loopCount(0)
{
    // nothing to see here
}

void CalcObject::DoCalcs(int loopCount)
{
    qDebug() << "Do Calcs Runing on Thread: " << this->thread()->currentThreadId();

    emit CalcProgressUpdate(0.0, loopCount);

    for (int i = 0; i < loopCount; ++i)
    {
        long long sum = 0;
        for (int j = 0; j < loopCount; ++j)
        {
            sum += QRandomGenerator64::system()->generate() % 100000;
        }
        if (i % 100 == 0)
        {
            emit CalcProgressUpdate((double) i / (double) loopCount, i);
        }
    }

    emit CalcProgressUpdate(1.0, loopCount);

    qDebug() << "Done Runing on Thread: " << this->thread()->currentThreadId();

    emit Done();
}
