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

void CalcObject::run()
{
    qDebug() << "Do Calcs Runing on Thread: " << this->thread()->currentThreadId();

    emit CalcProgressUpdate(0.0, 0);

    m_lastCount = 0;
    for (int i = 0; i < m_loopCount; ++i)
    {
        long long sum = 0;
        for (int j = 0; j < m_loopCount; ++j)
        {
            sum += QRandomGenerator64::system()->generate() % 100000;
        }
        if (i % 100 == 0)
        {
            auto numThisUpdate = i - m_lastCount;
            m_lastCount = i;

            emit CalcProgressUpdate((double) i / (double) m_loopCount, numThisUpdate);
        }
    }

    emit CalcProgressUpdate(1.0, m_loopCount - m_lastCount);

    qDebug() << "Done Runing on Thread: " << this->thread()->currentThreadId();

    emit Done();
}

int CalcObject::loopCount() const
{
    return m_loopCount;
}

void CalcObject::setLoopCount(int loopCount)
{
    m_loopCount = loopCount;
}
