#ifndef CAR_H
#define CAR_H

#include <QObject>
class QWaitCondition;
class QMutex;

class Car : public QObject
{
    Q_OBJECT
public:
    explicit Car(int carNum, QMutex *mutex, QWaitCondition *condition, QObject *parent = nullptr);

    int carNumber() const;
    void setCarNumber(int carNumber);

    QMutex *trackMutex() const;
    void setTrackMutex(QMutex *trackMutex);

    QWaitCondition *condition() const;
    void setCondition(QWaitCondition *condition);

signals:

public slots:
    void    run();

private:
    int             m_carNumber;
    QMutex          *m_trackMutex;
    QWaitCondition  *m_condition;
};

#endif // CAR_H
