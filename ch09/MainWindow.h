#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include "HVACStateMachine.h"
#include "HVACController.h"
#include "WeatherFetcher.h"
#include "MqttClient.h"

class TemperatureSensorIF;
class TemperatureStorage;
class WSReporter;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(TemperatureSensorIF *tempSensor, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void    updateDisplay();
    void    updateTempDisplay(QDateTime timestamp, float temperature);
    void    updatedWeather(QString forecast, QString conditions,
                           double temperature, double hi,
                           double low);
    void    updateMqttConnection(QString state, const QString &hostname,
                                 const quint16 port, const QString& id);


    void on_maxTemperatureSpinBox_valueChanged(int arg1);

    void on_minTemperatureSpinBox_valueChanged(int arg1);

    void on_locationLineEdit_editingFinished();

    void on_unitsComboBox_currentIndexChanged(const QString &arg1);

    void on_updateWeather_clicked();

    void on_actionConnect_to_MQTT_Broker_triggered();

    void on_actionDisconnect_from_MQTT_Broker_triggered();

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
};

#endif // MAINWINDOW_H
