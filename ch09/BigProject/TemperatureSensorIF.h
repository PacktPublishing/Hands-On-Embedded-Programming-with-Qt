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

signals:
    void newTemperature(QDateTime timestamp, float degreesCelcius);

public slots:
};

#endif // TEMPERATURESENSORIF_H
