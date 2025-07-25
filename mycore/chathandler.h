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
    void listRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void addRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void deleteRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void joinRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void leaveRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void listProductHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void listOrderHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void listCustomerHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    //방 아이템 추가 및 처리 시퀀스
    void addRoomItemHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void deleteRoomItemHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void movRoomItemHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void listRoomUserHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void listUserHandle(QTcpSocket *clientSocket, const QJsonObject &obj);
    void listRoomItemHandle(QTcpSocket *clientSocket,const QJsonObject &obj);
    //초대 / 강퇴 기능
    void inviteRoomUser(QTcpSocket *clientSocket,const QJsonObject &obj);
    void banRoomUser(QTcpSocket *clientSocket,const QJsonObject &obj);
    //채팅방 로그 관련 시퀀스
signals:
    void sendMessage(QTcpSocket* clientSocket,const QJsonDocument &doc);
};

#endif // CHATHANDLER_H
