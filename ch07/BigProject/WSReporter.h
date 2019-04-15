#ifndef WSREPORTER_H
#define WSREPORTER_H

#include <QObject>
#include <QWebSocketServer>

class WSReporter : public QObject
{
    Q_OBJECT
public:
    explicit WSReporter(quint16 port, QObject *parent = nullptr);
    virtual ~WSReporter();

signals:

public slots:
    void temperatureUpdate(QDateTime timestamp, float temperature);

private slots:
    void onNewConnection();
    void socketDisconnected();

private:
    QWebSocketServer    *m_WsServer;
    QList<QWebSocket *> m_clients;
    quint16             m_port;
};

#endif // WSREPORTER_H
