#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "MockTempSensor.h"

#include <QDateTime>
#include <QTabBar>
#include <TemperatureSensorIF.h>
#include <TemperatureStorage.h>

#include <QDebug>
#include <QThread>

#include "WSReporter.h"

MainWindow::MainWindow(TemperatureSensorIF *tempSensor, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_tempSensor(tempSensor),
    m_tempStorage(new TemperatureStorage)
{
    this->setAttribute(Qt::WA_QuitOnClose);     // quit app when this window closes

    ui->setupUi(this);

    // make sure indicators are off
    ui->heatingInd->setEnabled(false);
    ui->coolingInd->setEnabled(false);
    ui->fanInd->setEnabled(false);

    // setup the HVAC Control (and display)
    m_hvacSM.moveToThread(new QThread);

    m_hvacSM.connectToState("Heating", ui->heatingInd, &QLabel::setEnabled);
    m_hvacSM.connectToState("Cooling", ui->coolingInd, &QLabel::setEnabled);
    m_hvacSM.connectToState("FanOn",   ui->fanInd,     &QLabel::setEnabled);

    m_hvacSM.connectToState("Heating", &m_hvacCtrl, &HVACController::setHeatingOn);
    m_hvacSM.connectToState("Cooling", &m_hvacCtrl, &HVACController::setCoolingOn);
    m_hvacSM.connectToState("FanOn",   &m_hvacCtrl, &HVACController::setFanOn);

// uncomment the next statement to see the stable states m_hvacSM reaches
//    connect(&m_hvacSM, &QScxmlStateMachine::reachedStableState, [this]() {
//       qDebug() << "Stable States:" << m_hvacSM.activeStateNames(false);
//    });

    m_hvacSM.start();

    // mock sensor gets feedback from hvacSM
    auto mockTemp = dynamic_cast<MockTempSensor *>(m_tempSensor);
    if (mockTemp)
    {
        mockTemp->setSm(&m_hvacSM);
    }


    // Create a websockect reporter
    m_wsReporter = new WSReporter(8091, this);


    // open temperature storage with history, then tell history form about it
    m_tempStorage->OpenStorage("history");
    ui->historyForm->setTemperatureStorage(m_tempStorage);

    // Connect new temperature readings to...
    // - updating the main window
    connect(m_tempSensor, &TemperatureSensorIF::newTemperature, this, &MainWindow::updateTempDisplay);
    // - the Temperature history form
    connect(m_tempSensor, &TemperatureSensorIF::newTemperature, ui->historyForm, &TemperatureHistoryForm::temperatureUpdate);
    // - persistent storage
    connect(m_tempSensor, &TemperatureSensorIF::newTemperature,
            [this](QDateTime timestamp, float temperature) {
                m_tempStorage->AddReading(TemperatureReading(timestamp, temperature));
    });
    // - websocket reporter
    connect(m_tempSensor, &TemperatureSensorIF::newTemperature, m_wsReporter, &WSReporter::temperatureUpdate);

    m_updateTimer.setSingleShot(false);
    connect(&m_updateTimer, &QTimer::timeout, this, &MainWindow::updateDisplay);
    m_updateTimer.setInterval(1000);
    m_updateTimer.start();

// uncomment these lines to automatically take a screen shot of each tab 2 seconds after the tab is changed
//    connect(ui->tabWidget, &QTabWidget::tabBarClicked,
//            [this] (int index){
//                QTimer::singleShot(2000, [this, index]() {this->grab().save(QString("screenshot_%1.png").arg(index));});
//    });
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

    auto activeStates = m_hvacSM.activeStateNames(false);
//    qDebug() << QString("[%1, %2] : %3 - %4")
//                .arg(ui->minTemperatureSpinBox->value())
//                .arg(ui->maxTemperatureSpinBox->value())
//                .arg(temperature)
//                .arg(activeStates.join(":"));

    if (!activeStates.contains("Wait")) {
        // update the state machine based on settings
        QString transition;
        if (temperature > ui->maxTemperatureSpinBox->value()) {
            transition = "TooHot";
        } else if (temperature < ui->minTemperatureSpinBox->value()) {
            transition = "TooCold";
        } else {
            transition = "TargetReached";
        }
        m_hvacSM.submitEvent(transition);
//        qDebug() << "Sent" << transition;
    }
}

void MainWindow::on_maxTemperatureSpinBox_valueChanged(int arg1)
{
    if (ui->minTemperatureSpinBox->value() > (arg1 - 3)) {
        ui->minTemperatureSpinBox->setValue(arg1 - 3);
    }
}

void MainWindow::on_minTemperatureSpinBox_valueChanged(int arg1)
{
    if (ui->maxTemperatureSpinBox->value() < (arg1 + 3)) {
        ui->maxTemperatureSpinBox->setValue(arg1 + 3);
    }

}
