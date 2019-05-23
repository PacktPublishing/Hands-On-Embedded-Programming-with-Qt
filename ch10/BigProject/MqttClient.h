#ifndef MQQTCLIENT_H
#define MQQTCLIENT_H

#include <QMqttClient>
#include <QObject>


class MqttClient : public QObject
{
    Q_OBJECT

public:
    MqttClient(QObject *parent = nullptr);
    virtual ~MqttClient();

    void connectBroker();
    void disconnectBroker();

    QString hostname() const;
    void setHostname(const QString &hostname);

    quint16 port() const;
    void setPort(quint16 port);

    QString deviceName() const;
    void setDeviceName(const QString &deviceName);

    QMqttClient::ClientState state() const;

signals:
    /// sent when we are connected to the host
    void HostConnectionUpdate(QString state, const QString &hostname,
                              const quint16 port, const QString& id);

public slots:
    void SendStatus(QString status);    ///< Publish a new status

private slots:
    void messageReceived(const QByteArray &message, const QMqttTopicName &topic);
    void handleStateChange();
    void brokerDisconnected();

private:
    QString     m_hostname;     ///< the host to connect to
    quint16     m_port;         ///< the port on the host to connect to
    QMqttClient *m_client;      ///< the QMqttClient

    QString     m_deviceName;   ///< A way to uniquely identify us

};

#endif // MQQTCLIENT_H
