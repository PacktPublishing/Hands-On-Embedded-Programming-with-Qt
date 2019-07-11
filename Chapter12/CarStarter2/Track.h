#ifndef TRACK_H
#define TRACK_H

#include <QObject>
class QMutex;
class QWaitCondition;

class Track : public QObject
{
    Q_OBJECT
public:
    explicit Track(QMutex *mutex, QWaitCondition *condition, QObject *parent = nullptr);

    int maxCars() const;
    void setMaxCars(int maxCars);

    int carsOnTrack() const;
    void setCarsOnTrack(int carsOnTrack);

    QMutex *trackMutex() const;
    void setTrackMutex(QMutex *trackMutex);

    QWaitCondition *condition() const;
    void setCondition(QWaitCondition *condition);

signals:

public slots:
    void run();

private:
    int             m_maxCars;
    int             m_carsOnTrack;
    QMutex          *m_trackMutex;
    QWaitCondition  *m_condition;
};

#endif // TRACK_H
