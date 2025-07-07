#include "serverchat.h"
#include <QtGUI>
#include <QtNetwork>
#include <QtWidgets>
#include "serveruser.h"

#define BLOCK_SIZE 1024
#define TCP_PORT 30800 //Random num fix

ServerChat::ServerChat(QObject *parent)
    : QObject(parent)
{
    m_chatHander = new ChatHandler(this);
    connect(m_chatHander,&ChatHandler::sendMessage,this,&ServerChat::sendData);
}

bool ServerChat::OpenServer()
{
    const quint16 fixedPort = TCP_PORT;
    m_tcpServer = new QTcpServer(this);
    connect(m_tcpServer, SIGNAL(newConnection()), SLOT(clientConnect()));
    if (!m_tcpServer->listen(QHostAddress::Any, fixedPort)) {
        qDebug() << tr("Unable to start server :%1").arg(m_tcpServer->errorString());
        return false;
    }
    return true;
}
//접속시 socket얻는다.
void ServerChat::clientConnect()
{
    QTcpSocket *clientConnection = m_tcpServer->nextPendingConnection();
    //끊어지면 없애기
    connect(clientConnection, &QTcpSocket::disconnected, this, [=](){
        ServerUser::getInstance().RemoveUser(clientConnection);
        clientConnection->deleteLater();
    });
    //읽을게 왔다면 echodata
    connect(clientConnection, SIGNAL(readyRead()), SLOT(getData()));
    //일단 붙은 clientConnection을 user로 넣기.
    ServerUser::getInstance().AssignUser(clientConnection);
}

//json데이터 받으면 chatHandler에서 처리한다.
void ServerChat::getData()
{
    QTcpSocket *clientConnection = dynamic_cast<QTcpSocket *>(sender());
    if (!clientConnection)
        return;
    //받은 데이터를 TcpSocket* 별로
    m_buffer[clientConnection] += clientConnection->readAll();

    int newLineIndex;
    while ((newLineIndex = m_buffer[clientConnection].indexOf('\n')) != -1) {
        QByteArray oneMessage = m_buffer[clientConnection].left(newLineIndex);
        m_buffer[clientConnection].remove(0, newLineIndex + 1);

        m_chatHander->getByteData(clientConnection, oneMessage);
    }
}
//chatHandler에서 처리된 데이터 보내기
void ServerChat::sendData(QTcpSocket* clientSocket,const QJsonDocument &doc){
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    data.append('\n'); //구분자
    clientSocket->write(data);
    //clientSocket->flush();
}
