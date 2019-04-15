#ifndef HVACCONTROLLER_H
#define HVACCONTROLLER_H

#include <QObject>

class HVACController : public QObject
{
    Q_OBJECT
public:
    explicit HVACController(QObject *parent = nullptr);

signals:

public slots:
    void setHeatingOn(bool on);
    void setCoolingOn(bool on);
    void setFanOn(bool on);
};

#endif // HVACCONTROLLER_H
