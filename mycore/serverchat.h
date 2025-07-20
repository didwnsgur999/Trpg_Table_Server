#ifndef SERVERCHAT_H
#define SERVERCHAT_H

#include <QObject>
#include <QTcpServer>
#include "chathandler.h"
class ServerChat : public QObject
{
    Q_OBJECT
public:
    static ServerChat &getInstance()
    {
        static ServerChat s;
        return s;
    }

    bool OpenServer();
    int getPort() const { return m_tcpServer->serverPort(); }
    void sendData(QTcpSocket* clientSocket,const QJsonDocument &doc);

private slots:
    void clientConnect();
    void getData();

private:
    ServerChat(QObject *parent = 0);
    ServerChat(const ServerChat &ref) {}
    ServerChat &operator=(const ServerChat &ref) {}
    ~ServerChat() {}
    //QTcpserver 멤버변수
    QTcpServer *m_tcpServer;
    //받은 데이터 파싱 및 처리용 class
    ChatHandler *m_chatHander;
    //데이터 받을때
    QHash<QTcpSocket *, QByteArray> m_buffer;
};

#endif // SERVERCHAT_H
