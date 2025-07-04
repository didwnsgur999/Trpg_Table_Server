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
    //데이터 받기, cmd별로 정리후 아래 private함수로 보냄.
    void getByteData(QTcpSocket* clientSocket,QByteArray& data);
    void sendData(QTcpSocket* clientSocket,QByteArray& data);

private:
    //실제 데이터 처리 및 보내기
    void loginHandle(QTcpSocket* clientSocket,const QJsonObject& obj);
    void chatHandle(QTcpSocket* clientSocket,const QJsonObject& obj);
    void productHandle(QTcpSocket* clientSocket,const QJsonObject& obj);

};

#endif // CHATHANDLER_H
