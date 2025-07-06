#ifndef CHATHANDLER_H
#define CHATHANDLER_H

#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QTcpSocket>

class ChatHandler : public QObject
{
    Q_OBJECT
public:
    explicit ChatHandler(QObject *parent = nullptr);
    //데이터 받기, cmd별로 정리후 아래 private함수로 보냄.
    void getByteData(QTcpSocket *clientSocket, QByteArray &data);
    void sendData(QTcpSocket *clientSocket, QByteArray &data);

private:
    //실제 데이터 처리 및 보내기
    //로그인, 채팅
    void loginHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void chatHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    //오브젝트 추가 시퀀스
    void productAddHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void customerAddHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void orderAddHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    //방 관련 시퀀스
    void showRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void addRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void deleteRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void joinRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void leaveRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    //채팅방 로그 관련 시퀀스
signals:
    void sendMessage(QTcpSocket* clientSocket,const QJsonDocument &doc);
};

#endif // CHATHANDLER_H
