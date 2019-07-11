#include "MyObject.h"
#include <QDBusConnection>
#include <QDBusConnectionInterface>

#include <QTimer>

MyObject::MyObject(QObject *parent) : QObject(parent)
{
    /// [0] This is a trick to let constructor finish and then start our work.
    QTimer::singleShot(10, this, &MyObject::doIt);
}

void MyObject::doIt()
{
    qDebug() << "----- start -----";

    /// [1] Get a connection to the session bus.
    QDBusConnection             bus = QDBusConnection::sessionBus();

    /// [2] Create an interface to it
    QDBusConnectionInterface    *busIF = bus.interface();

    /// [3] Register as a service with the interface name
    QString ifName = "com.packt.bigproject";
    busIF->registerService(ifName,
                           QDBusConnectionInterface::ReplaceExistingService,
                           QDBusConnectionInterface::AllowReplacement);

    /// [4] See who is on the bus
    QDBusReply<QStringList> serviceNames = busIF->registeredServiceNames();
    qDebug() << bus.name() << "knows the following Services:" << serviceNames.value();

    /// [5] Register to receive a "ping" request. Note similar to QObject::connect(..)
    QString service = "";
    QString path = "";
    QString name = "ping";
    bus.connect(service, path, ifName, name, this, SLOT(pingReceived(QString)));

    /// [6] Send a Ping Message
    QDBusMessage    msg = QDBusMessage::createSignal("/", ifName, name);
    msg << "Hello World!";
    bus.send(msg);

    /// [7] Do it again in 7 seconds
    QTimer::singleShot(5000, this, &MyObject::doIt);
    qDebug() << "----- end -----";
}

void MyObject::pingReceived(QString msg)
{
    /// [8] Show the received ping message
    qDebug() << __FUNCTION__ << "Ping:" << msg;
}
