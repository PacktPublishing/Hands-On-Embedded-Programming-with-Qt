#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "MockTempSensor.h"

#include <QDateTime>
#include <QTabBar>
#include <TemperatureSensorIF.h>
#include <TemperatureStorage.h>
#include <QMessageBox>
#include <QSettings>
#include <QPrintPreviewDialog>
#include <QPrinter>

#include <QDebug>
#include <QPainter>
#include <QTableWidget>
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

    // Create and start the weather fetcher
    m_weatherFetcher = new WeatherFetcher(this);
    connect(m_weatherFetcher, &WeatherFetcher::UpdatedWeather,
            this, &MainWindow::updatedWeather);

    // ***** Setup MQTT Connection *****
    m_mqttClient = new MqttClient(this);
    connect(m_mqttClient, &MqttClient::HostConnectionUpdate,
            this, &MainWindow::updateMqttConnection);

    // make sure our virtual keyboard can get touch events
    ui->quickWidget->setAttribute(Qt::WA_AcceptTouchEvents);


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

    // ** D-Bus Adapter **
    m_thermoAdaptor = new ThermostatAdaptor(this);
    QDBusConnection dbus = QDBusConnection::systemBus();
    dbus.registerObject("/Thermostat", this);
    dbus.registerService("com.packt.bigproject.thermostat");

    // connect hvac states
    m_hvacSM.connectToState("Heating", m_thermoAdaptor, &ThermostatAdaptor::heatingOnChanged);
    m_hvacSM.connectToState("Cooling", m_thermoAdaptor, &ThermostatAdaptor::coolingOnChanged);
    m_hvacSM.connectToState("FanOn",   m_thermoAdaptor, &ThermostatAdaptor::fanOnChanged);

    // ** load settings **
    m_settings = new QSettings();

    // - thermostat
    ui->minTemperatureSpinBox->setValue(m_settings->value("thermostat/min", 20).toInt());
    ui->maxTemperatureSpinBox->setValue(m_settings->value("thermostat/max", 15).toInt());
    m_tempSensor->setReadingSecs(m_settings->value("thermostat/readingSecs", 30).toInt());

    // - weather fetcher
    auto loc = m_settings->value("weather/location", "London, UK").toString();
    auto units = m_settings->value("weather/units", "Celsius").toString();
    ui->locationLineEdit->setText(loc);
    ui->unitsComboBox->setCurrentText(units);
    ui->locationDisplay->setText(ui->locationLineEdit->text());
    QString weatherAPI = m_settings->value("weather/apiKey", "*XYZZY*").toString();
    m_weatherFetcher->setKey(weatherAPI);
    m_weatherFetcher->setUpdateSecs(m_settings->value("weather/updateSecs", (15 * 60)).toInt());
    m_weatherFetcher->setUnits(units);
    m_weatherFetcher->setLocation(loc);
    m_weatherFetcher->ForceUpdate();

    // - mqtt broker
    QString hostname = m_settings->value("mqtt/broker", "broker.hivemq.com").toString();
    quint16 port     = m_settings->value("mqtt/port", 1883).toUInt();
    m_mqttClient->setHostname(hostname);
    m_mqttClient->setPort(port);

    // get a tempeture reading ASAP after the constructor exits
    QTimer::singleShot(10, [this] () {m_tempSensor->readNow();});


    // ** setup the display update timer **
    m_updateTimer.setSingleShot(false);
    connect(&m_updateTimer, &QTimer::timeout, this, &MainWindow::updateDisplay);
    m_updateTimer.setInterval(1000);
    m_updateTimer.start();


// uncomment these lines to automatically take a screen shot of each tab 2 seconds after the tab is changed
//    connect(ui->tabWidget, &QTabWidget::tabBarClicked,
//            [this] (int index){
//                QTimer::singleShot(2000, [this, index]() {
//                    this->grab().save(QString("screenshot_%1-%2.png")
//                                      .arg(index)
//                                      .arg(ui->tabWidget->currentWidget()->objectName()));
//                });
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
    Q_UNUSED(timestamp)     // silence the unused variable warning

    ui->tempDisplay->setText(QString("%1 C").arg(temperature));

    auto activeStates = m_hvacSM.activeStateNames(false);

    if (!activeStates.contains("Wait")) {
        // update the state machine based on settings
        QString transition;
        if (temperature > ui->maxTemperatureSpinBox->value()) {
            if (activeStates.contains("Heating")) {
                transition = "TargetReached";
            } else {
                transition = "TooHot";
            }
        } else if (temperature < ui->minTemperatureSpinBox->value()) {
            if (activeStates.contains("Heating")) {
                transition = "TargetReached";
            } else {
                transition = "TooCold";
            }
        } else {
            transition = "TargetReached";
        }
        m_hvacSM.submitEvent(transition);
    }

    // send an udpate
    if (m_mqttClient->state() == QMqttClient::Connected)
    {
        // time: temp activeControlStates
        auto stateString = QString("%1: %2 %3")
                                .arg(timestamp.toString())
                                .arg(temperature)
                                .arg(activeStates.join("|"));
        m_mqttClient->SendStatus(stateString);
    }

    // notify d-bus adapter
    m_thermoAdaptor->currentTempChanged(temperature);
}

void MainWindow::updatedWeather(QString forecast, QString conditions, double temperature, double hi, double low)
{
    Q_UNUSED(forecast);

    ui->conditions->setText(conditions);
    ui->currentTemp->display(temperature);
    ui->highTemp->display(hi);
    ui->lowTemp->display(low);
}

void MainWindow::updateMqttConnection(QString state, const QString &hostname, const quint16 port, const QString &id)
{
    auto statusMsg = QString("%1 %2:%3 (%4)")
            .arg(id)
            .arg(hostname).arg(port)
            .arg(state);

    // show a message box with the information
    QMessageBox::information(this, "MQTT Connection Information", statusMsg);

    // Update the bottom statusBar with the status information.
    statusBar()->showMessage(statusMsg);
}

void MainWindow::setMinTemp(double minTemp)
{
    ui->minTemperatureSpinBox->setValue(minTemp);
}

void MainWindow::setMaxTemp(double maxTemp)
{
    ui->maxTemperatureSpinBox->setValue(maxTemp);
}

void MainWindow::saveThermostatSettings()
{
    auto min = ui->minTemperatureSpinBox->value();
    auto max = ui->maxTemperatureSpinBox->value();

    // notify dbus changes
    m_thermoAdaptor->minTempChanged(min);
    m_thermoAdaptor->maxTempChanged(max);

    m_settings->setValue("thermostat/min", min);
    m_settings->setValue("thermostat/max", max);
    m_settings->sync();
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

void MainWindow::on_locationLineEdit_editingFinished()
{
    auto loc = ui->locationLineEdit->text();
    ui->locationDisplay->setText(loc);
    m_weatherFetcher->setLocation(loc);

    // save settings
    m_settings->setValue("weather/location", loc);
    m_settings->sync();
}

void MainWindow::on_unitsComboBox_currentIndexChanged(const QString &arg1)
{
    m_weatherFetcher->setUnits(arg1);

    // save settings
    m_settings->setValue("weather/units", arg1);
    m_settings->sync();
}

void MainWindow::on_updateWeather_clicked()
{
    if (!m_weatherFetcher->ForceUpdate())
    {
        QMessageBox::information(this, "Weather Update",
                                 QString("Please wait %1 seconds between updates.").arg(m_weatherFetcher->minUpdateSecs())
                                 );
    }
}

void MainWindow::on_actionConnect_to_MQTT_Broker_triggered()
{
    m_mqttClient->connectBroker();
}

void MainWindow::on_actionDisconnect_from_MQTT_Broker_triggered()
{
    m_mqttClient->disconnectBroker();
}

void MainWindow::on_maxTemperatureSpinBox_editingFinished()
{
    saveThermostatSettings();
}

void MainWindow::on_minTemperatureSpinBox_editingFinished()
{
    saveThermostatSettings();
}

void MainWindow::on_actionExport_History_to_PDF_triggered()
{
    QPrinter    printer;
    QPrintPreviewDialog preview(&printer);

    preview.connect(&preview, SIGNAL(paintRequested(QPrinter*)),
                    this, SLOT(DoPrint(QPrinter*)));
    preview.exec();
}

void MainWindow::DoPrint(QPrinter *ap_printer)
{
    ui->historyForm->render(ap_printer);
}
