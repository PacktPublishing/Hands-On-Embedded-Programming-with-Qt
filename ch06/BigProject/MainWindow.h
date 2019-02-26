#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "TemperatureSensorIF.h"

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

private:
    Ui::MainWindow      *ui;
    QTimer              m_updateTimer;  //< the timer we use for udpates
    TemperatureSensorIF *m_tempSensor;  //< the temperature sensor we use
};

#endif // MAINWINDOW_H
