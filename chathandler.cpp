#include "chathandler.h"

ChatHandler::ChatHandler(QObject* parent)
    : QObject(parent)
{}
//json으로 날라온걸 받았겠지? QByteArray로 끌고온다. clientSocket도 가지고있으니까 안에서 room 찾아서 보내기도 된다.
void ChatHandler::getByteData(QTcpSocket* clientSocket, QByteArray& data){
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) return;

    QJsonObject obj=doc.object();
    QString cmd = obj.value("cmd").toString();

    //이 아래가 signal처리 connect는 기본적으로 되어있어야한다.
    //if(cmd==)
    if(cmd == "chat"){
        qDebug()<<obj.value("text").toString();
        clientSocket->write(data);
    }
}
