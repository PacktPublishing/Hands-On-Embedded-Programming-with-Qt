#include "MainWindow.h"
#include "MockTempSensor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // virtual keyboard
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    qputenv("QT_LOGGING_RULES", QByteArray("qt.virtualkeyboard=true"));

    // if you want the wires coming out the top of the raspi,
    // uncomment this next line.
    qputenv("QT_QPA_EGLFS_ROTATION", QByteArray("180"));

    QApplication a(argc, argv);

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    MockTempSensor  mockTemp;
    MainWindow w(&mockTemp);

    w.show();

    return a.exec();
}
