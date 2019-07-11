#include "WSReporter.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocket>

#include <QDebug>

WSReporter::WSReporter(quint16 port, QObject *parent) :
    QObject(parent),
    m_WsServer(nullptr),
    m_clients(),
    m_port(port)
{
    m_WsServer = new QWebSocketServer("BigProjectReporter",
                                      QWebSocketServer::SslMode::NonSecureMode,
                                      this);

    if (m_WsServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "WSReport Listening on port" << port;
        connect(m_WsServer, &QWebSocketServer::newConnection,
                this, &WSReporter::onNewConnection);
    }
}

WSReporter::~WSReporter()
{
    m_WsServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void WSReporter::temperatureUpdate(QDateTime timestamp, float temperature)
{
    QJsonObject jObj;
    jObj.insert("timestamp",   timestamp.toString());
    jObj.insert("temperature", temperature);

    QJsonDocument   jDoc(jObj);
    QString jText = jDoc.toJson(QJsonDocument::Indented);

    for (auto client : m_clients)
    {
        client->sendTextMessage(jText);
    }
}

void WSReporter::onNewConnection()
{
    QWebSocket *pSocket = m_WsServer->nextPendingConnection();

    qDebug() << "Client connected:" << pSocket->peerName() << pSocket->origin();

    connect(pSocket, &QWebSocket::disconnected,
            this, &WSReporter::socketDisconnected);

    m_clients << pSocket;   // add a client to our list

}

void WSReporter::socketDisconnected()
{
    qDebug() << "Client disconnected";
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
