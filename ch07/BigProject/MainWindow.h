#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include "HVACStateMachine.h"
#include "HVACController.h"

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

    void on_maxTemperatureSpinBox_valueChanged(int arg1);

    void on_minTemperatureSpinBox_valueChanged(int arg1);

private:
    Ui::MainWindow      *ui;
    QTimer              m_updateTimer;  ///< the timer we use for udpates
    TemperatureSensorIF *m_tempSensor;  ///< the temperature sensor we use
    TemperatureStorage  *m_tempStorage; ///< persistent temperature storage

    HVACStateMachine    m_hvacSM;       ///< state machine for controlling hvac
    HVACController      m_hvacCtrl;     ///< controller or the HVAC system

    WSReporter          *m_wsReporter;  ///< A Websocket Reporter
};

#endif // MAINWINDOW_H
