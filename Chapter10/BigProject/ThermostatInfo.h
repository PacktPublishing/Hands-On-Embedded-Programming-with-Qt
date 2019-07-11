#ifndef THERMOSTATINFO_H
#define THERMOSTATINFO_H

#include <QObject>

class ThermostatInfo : public QObject
{
    Q_OBJECT

    /// Provide info about our class
    Q_CLASSINFO("D-Bus Interface", "com.packt.bigproject.thermostat")

    Q_PROPERTY(bool fanOn     READ fanOn     NOTIFY fanOnChanged)
    Q_PROPERTY(bool heatingOn READ heatingOn NOTIFY heatingOnChanged)
    Q_PROPERTY(bool coolingOn READ coolingOn NOTIFY coolingOnChanged)

    Q_PROPERTY(double currentTemp READ currentTemp NOTIFY currentTempChanged)
    Q_PROPERTY(double minTemp     READ minTemp     NOTIFY minTempChanged)
    Q_PROPERTY(double maxTemp     READ maxTemp     NOTIFY maxTempChanged)

signals:
    void fanOnChanged(bool val);
    void heatingOnChanged(bool val);
    void coolingOnChanged(bool val);
    void currentTempChanged(double val);
    void minTempChanged(double val);
    void maxTempChanged(double val);

public slots:
    void setMinTemp(double minTemp);
    void setMaxTemp(double maxTemp);

public:
    explicit ThermostatInfo(QObject *parent = nullptr);
    virtual ~ThermostatInfo() = default;

    bool fanOn() const;

    bool heatingOn() const;

    bool coolingOn() const;

    double currentTemp() const;

    double minTemp() const;

    double maxTemp() const;

private:
    bool m_fanOn;
    bool m_heatingOn;
    bool m_coolingOn;

    double m_currentTemp;
    double m_minTemp;
    double m_maxTemp;
};

#endif // THERMOSTATINFO_H
