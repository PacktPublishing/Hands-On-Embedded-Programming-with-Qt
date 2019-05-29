#ifndef CALCOBJECT_H
#define CALCOBJECT_H

#include <QObject>

class CalcObject : public QObject
{
    Q_OBJECT
public:
    explicit CalcObject(QObject *parent = nullptr);

signals:
    /**
     * @brief Update the progress of our calculation
     * @param part      0.0 (not started) - 1.0 (done)
     * @param lastLoop  the last loop number
     */
    void CalcProgressUpdate(double part, int lastLoop);

    void Done();

public slots:
    void DoCalcs(int loopCount);

private:
    int m_loopCount;
};

#endif // CALCOBJECT_H
