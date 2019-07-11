#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include "HVACStateMachine.h"
#include "HVACController.h"
#include "WeatherFetcher.h"
#include "MqttClient.h"
#include "ThermostatInfoAdapter.h"

class TemperatureSensorIF;
class TemperatureStorage;
class WSReporter;

class QSettings;
class QPrinter;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(TemperatureSensorIF *tempSensor, QWidget *parent = nullptr);
    ~MainWindow();

    void saveThermostatSettings();

private slots:
    void    updateDisplay();
    void    updateTempDisplay(QDateTime timestamp, float temperature);
    void    updatedWeather(QString forecast, QString conditions,
                           double temperature, double hi,
                           double low);
    void    updateMqttConnection(QString state, const QString &hostname,
                                 const quint16 port, const QString& id);
    void    setMinTemp(double minTemp);
    void    setMaxTemp(double maxTemp);


    void on_maxTemperatureSpinBox_valueChanged(int arg1);

    void on_minTemperatureSpinBox_valueChanged(int arg1);

    void on_locationLineEdit_editingFinished();

    void on_unitsComboBox_currentIndexChanged(const QString &arg1);

    void on_updateWeather_clicked();

    void on_actionConnect_to_MQTT_Broker_triggered();

    void on_actionDisconnect_from_MQTT_Broker_triggered();

    void on_maxTemperatureSpinBox_editingFinished();

    void on_minTemperatureSpinBox_editingFinished();

    void on_actionExport_History_to_PDF_triggered();

    void DoPrint(QPrinter *ap_printer);
private:
    Ui::MainWindow      *ui;
    QTimer              m_updateTimer;  ///< the timer we use for udpates
    TemperatureSensorIF *m_tempSensor;  ///< the temperature sensor we use
    TemperatureStorage  *m_tempStorage; ///< persistent temperature storage

    HVACStateMachine    m_hvacSM;       ///< state machine for controlling hvac
    HVACController      m_hvacCtrl;     ///< controller or the HVAC system

    WSReporter          *m_wsReporter;  ///< A Websocket Reporter

    WeatherFetcher      *m_weatherFetcher;  ///< a weather API fetcher

    MqttClient          *m_mqttClient;      ///< the MQTT Client

    QSettings           *m_settings;        ///< access to our settings

    ThermostatAdaptor   *m_thermoAdaptor;   ///< our D-Bus adapter
};

#endif // MAINWINDOW_H
