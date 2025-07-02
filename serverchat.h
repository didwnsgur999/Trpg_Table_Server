#ifndef SERVERCHAT_H
#define SERVERCHAT_H

#include <QObject>
#include <QTcpServer>

class ServerChat:public QObject
{
    Q_OBJECT
public:
    static ServerChat& getInstance() {
        static ServerChat s;
        return s;
    }

    QString OpenServer();
    int getPort() const {return tcpServer->serverPort();}

private slots:
    void clientConnect();
    void echoData();

private:
    ServerChat(QObject* parent = 0);
    QTcpServer *tcpServer;
    ServerChat(const ServerChat& ref) {}
    ServerChat& operator=(const ServerChat& ref) {}
    ~ServerChat() {}
};

#endif // SERVERCHAT_H
