#include "multiclientserver.h"
#include <QTcpSocket>
#include <QString>
MultiClientServer::MultiClientServer()
{
    connect(this, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
}
MultiClientServer::~MultiClientServer()
{
    stop();
}
void MultiClientServer::stop()
{
    qDeleteAll(clientConnections);
}
void MultiClientServer::start()
{
    listen(QHostAddress::Any, 1234);
}

void MultiClientServer::handleNewConnection()
{
    while (hasPendingConnections()) {
            QTcpSocket *client = nextPendingConnection();
            connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
            connect(client,SIGNAL(readyRead()),this,SLOT(readyRead()));
            clientConnections.append(client);
            sendHello(client);
        }
}

void MultiClientServer::readyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
       if (!client)
           return;
       while (client->bytesAvailable()) {
           QString receive=client->readAll();
           qDebug()<<receive;
       }
}
void MultiClientServer::clientDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());

       if (!client)
           return;

       clientConnections.removeAll(client);
       client->deleteLater();
}

void MultiClientServer::sendHello(QTcpSocket *client)
{
    if (!client)
        return;

    client->write("Hello\n");
}


