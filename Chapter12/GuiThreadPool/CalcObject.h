#ifndef CALCOBJECT_H
#define CALCOBJECT_H

#include <QObject>
#include <QRunnable>

class CalcObject : public QObject, public QRunnable
{
    Q_OBJECT

public:
    explicit CalcObject(QObject *parent = nullptr);

    int loopCount() const;
    void setLoopCount(int loopCount);

signals:
    /**
     * @brief Update the progress of our calculation
     * @param part      0.0 (not started) - 1.0 (done)
     * @param lastLoop  the last loop number
     */
    void CalcProgressUpdate(double part, int lastLoop);

    void Done();

public slots:
    void run() override;

private:
    int m_loopCount;

    int m_lastCount;
};

#endif // CALCOBJECT_H
