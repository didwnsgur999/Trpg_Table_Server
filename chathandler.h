#ifndef CHATHANDLER_H
#define CHATHANDLER_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QTcpSocket>

class ChatHandler : public QObject
{
    Q_OBJECT
public:
    explicit ChatHandler(QObject* parent = nullptr);
    void getByteData(QTcpSocket* clientSocket,QByteArray& data);
    void sendData(QTcpSocket* clientSocket,QByteArray& data);

private:

};

#endif // CHATHANDLER_H
