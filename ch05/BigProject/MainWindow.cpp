#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDateTime>

MainWindow::MainWindow(TemperatureSensorIF *tempSensor, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_tempSensor(tempSensor)
{
    ui->setupUi(this);

    connect(m_tempSensor, &TemperatureSensorIF::newTemperature, this, &MainWindow::updateTempDisplay);

    m_updateTimer.setSingleShot(false);
    connect(&m_updateTimer, &QTimer::timeout, this, &MainWindow::updateDisplay);
    m_updateTimer.setInterval(1000);
    m_updateTimer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateDisplay()
{
    QDateTime   now = QDateTime::currentDateTime();
    ui->currentDateTime->setText(now.toString());
}

void MainWindow::updateTempDisplay(QDateTime timestamp, float temperature)
{
    ui->tempDisplay->setText(QString("%1 C").arg(temperature));
}
