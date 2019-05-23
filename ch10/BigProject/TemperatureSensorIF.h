#ifndef TEMPERATURESENSORIF_H
#define TEMPERATURESENSORIF_H

#include <QDateTime>
#include <QObject>

class TemperatureSensorIF : public QObject
{
    Q_OBJECT
public:
    explicit TemperatureSensorIF(QObject *parent = nullptr);

    virtual bool connected() const = 0;

    virtual void readNow() = 0;

    int readingSecs() const;
    void setReadingSecs(int readingSecs);

signals:
    void newTemperature(QDateTime timestamp, float degreesCelsius);

public slots:

protected:
    int m_readingSecs;  ///< the seconds between readings
};

#endif // TEMPERATURESENSORIF_H
