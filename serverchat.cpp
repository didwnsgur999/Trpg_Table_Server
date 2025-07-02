#include "serverchat.h"
#include <QtNetwork>
#include <QtGUI>
#include <QtWidgets>

#define BLOCK_SIZE 1024

ServerChat::ServerChat(QObject* parent):QObject(parent) {}

QString ServerChat::OpenServer(){
    tcpServer = new QTcpServer(this);
    connect(tcpServer,SIGNAL(newConnection()),SLOT(clientConnect()));
    if(!tcpServer->listen()){
        qDebug()<<tr("Unable to start server :%1").arg(tcpServer->errorString());
        return tcpServer->errorString();
    }
    return tr("The server is running on port %1").arg(tcpServer->serverPort());
}
void ServerChat::clientConnect(){
    QTcpSocket* clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection,SIGNAL(disconnected()),clientConnection,SLOT(deleteLater()));
    connect(clientConnection,SIGNAL(readyRead()),SLOT(echoData()));
}
void ServerChat::echoData(){
    QTcpSocket* clientConnection=dynamic_cast<QTcpSocket*>(sender());
    if(clientConnection->bytesAvailable()>BLOCK_SIZE) return;
    QByteArray bytearray = clientConnection->read(BLOCK_SIZE);
    clientConnection->write(bytearray);
}
