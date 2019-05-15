#include "MainWindow.h"
#include "MockTempSensor.h"
#include "AmbientTempSensor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // virtual keyboard
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    qputenv("QT_LOGGING_RULES", QByteArray("qt.virtualkeyboard=true"));

    QApplication a(argc, argv);

    // these identify the application (for QSettings)
    a.setOrganizationName("Packt");
    a.setApplicationName("BigProject");

    // create the temperature sensor
    TemperatureSensorIF *tempSensor = new AmbientTempSensor();
    // -- if we didn't connect to the SenseHat, create a mock sensor
    if (!tempSensor->connected())
    {
        delete tempSensor;
        tempSensor = new MockTempSensor;
    }
    MainWindow w(tempSensor);

    // Accept touch events
    w.setAttribute(Qt::WA_AcceptTouchEvents);

    w.show();

    auto result = a.exec();

    delete tempSensor;

    return result;
}
