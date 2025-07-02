#ifndef SERVERCHAT_H
#define SERVERCHAT_H

#include <QObject>
#include <QTcpServer>

class ServerChat:public QObject
{
    Q_OBJECT
public:
    ServerChat(QObject* parent = 0);
    QString OpenServer();
    int getPort() const {return tcpServer->serverPort();}

private slots:
    void clientConnect();
    void echoData();

private:
    QTcpServer *tcpServer;
};

#endif // SERVERCHAT_H
