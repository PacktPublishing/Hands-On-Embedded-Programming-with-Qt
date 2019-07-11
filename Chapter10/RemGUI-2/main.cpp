#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>

#include "ThermostatInfoInterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/main.qml")));
    auto qmlObj = component.create();

    if (!qmlObj) {
        return -1;
    }


    // load the thermostat interface
    //QDBusServer dbServer("RemGUI-2");
    auto bus = QDBusConnection::systemBus();

    ComPacktBigprojectThermostatInterface   thermoIF("com.packt.bigproject.thermostat",
                                                     "/Thermostat",
                                                     bus);

    QObject::connect(&thermoIF, &ComPacktBigprojectThermostatInterface::coolingOnChanged,
                     [qmlObj] (double value) {qmlObj->setProperty("coolingOnValue", value);});
    QObject::connect(&thermoIF, &ComPacktBigprojectThermostatInterface::currentTempChanged,
                     [qmlObj] (double value) {qmlObj->setProperty("currentTempValue", value);});
    QObject::connect(&thermoIF, &ComPacktBigprojectThermostatInterface::fanOnChanged,
                                    [qmlObj] (bool value) {qmlObj->setProperty("fanOnValue", value);});
    QObject::connect(&thermoIF, &ComPacktBigprojectThermostatInterface::heatingOnChanged,
                     [qmlObj] (bool value) {qmlObj->setProperty("heatingValue", value);});
    QObject::connect(&thermoIF, &ComPacktBigprojectThermostatInterface::maxTempChanged,
                     [qmlObj] (double value) {qmlObj->setProperty("maxSettingValue", value);});
    QObject::connect(&thermoIF, &ComPacktBigprojectThermostatInterface::minTempChanged,
                     [qmlObj] (double value) {qmlObj->setProperty("minSettingValue", value);});

    QObject::connect(qmlObj, SIGNAL(minSettingSignal(double)),
                     &thermoIF, SLOT(setMinTemp(double)));
    QObject::connect(qmlObj, SIGNAL(maxSettingSignal(double)),
                     &thermoIF, SLOT(setMaxTemp(double)));

    return app.exec();
}
