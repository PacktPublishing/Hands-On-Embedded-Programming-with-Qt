#include "MqttClient.h"
#include <QCoreApplication>
#include <QRandomGenerator>

MqttClient::MqttClient(QObject *parent) :
    QObject (parent)
{
    m_client = new QMqttClient(this);

    // Create a Unique Device Name
    m_deviceName = QStringLiteral("%1-%2").arg(qAppName())
                   .arg(QRandomGenerator::system()->generate() % 100000);

    // Connect the signals we need to process
    connect(m_client, &QMqttClient::stateChanged, this, &MqttClient::handleStateChange);
    connect(m_client, &QMqttClient::disconnected, this, &MqttClient::brokerDisconnected);
    connect(m_client, &QMqttClient::messageReceived, this, &MqttClient::messageReceived);

    // We need to call these methods so the settings are passed to m_client
    setHostname("broker.hivemq.com");
    setPort(1883);
}

MqttClient::~MqttClient()
{

}

void MqttClient::connectBroker()
{
    m_client->connectToHost();
}

void MqttClient::disconnectBroker()
{
    m_client->disconnectFromHost();
}

QString MqttClient::hostname() const
{
    return m_hostname;
}

void MqttClient::setHostname(const QString &hostname)
{
    m_hostname = hostname;
    m_client->setHostname(m_hostname);
}

quint16 MqttClient::port() const
{
    return m_port;
}

void MqttClient::setPort(quint16 port)
{
    m_port = port;
    m_client->setPort(m_port);
}

void MqttClient::SendStatus(QString status)
{
    if (m_client->state() == QMqttClient::Connected)
    {
        m_client->publish(m_deviceName + "/Status", status.toUtf8());
    }
}

void MqttClient::messageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    qDebug() << __FUNCTION__ << topic << ":" << message;
}

void MqttClient::handleStateChange()
{
    auto state = m_client->state();

    // convert enum to text by brute force
    QString stateName;
    if (state == QMqttClient::Connected)
    {
        stateName = "Connected";
    }
    else if (state == QMqttClient::Connecting)
    {
        stateName = "Connecting";
    }
    else if (state == QMqttClient::Disconnected)
    {
        stateName = "Disconnected";
    }

    // emit the updated state
    emit HostConnectionUpdate(stateName, m_hostname, m_port, m_deviceName);
}

void MqttClient::brokerDisconnected()
{
    handleStateChange();
}

QString MqttClient::deviceName() const
{
    return m_deviceName;
}

void MqttClient::setDeviceName(const QString &deviceName)
{
    m_deviceName = deviceName;
}

QMqttClient::ClientState MqttClient::state() const
{
    return m_client->state();
}
