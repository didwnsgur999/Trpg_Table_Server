#include "chathandler.h"
#include "backend.h"
ChatHandler::ChatHandler(QObject* parent)
    : QObject(parent)
{}
//json으로 날라온걸 받았겠지? QByteArray로 끌고온다. clientSocket도 가지고있으니까 안에서 room 찾아서 보내기도 된다.
void ChatHandler::getByteData(QTcpSocket* clientSocket, QByteArray& data){
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) return;

    const QJsonObject obj=doc.object();
    QString cmd = obj.value("cmd").toString();

    QByteArray bytearray;
    //이 아래가 signal처리 connect는 기본적으로 되어있어야한다.
    //if(cmd==)
    if(cmd == "login"){
        ChatHandler::loginHandle(clientSocket,obj);
    }
    else if(cmd == "chat"){
        qDebug()<<obj.value("text").toString();
        clientSocket->write(data);
    }
    else if(cmd == "add_p"){
        ChatHandler::productHandle(clientSocket,obj);
    }
}
void ChatHandler::loginHandle(QTcpSocket* clientSocket,const QJsonObject& obj){
    qDebug()<<"login sequence";
    QString name = obj["cName"].toString();
    QString pwd = obj["cPwd"].toString();
    qDebug()<<name<<pwd;

}
void ChatHandler::chatHandle(QTcpSocket* clientSocket,const QJsonObject& obj){
    qDebug()<<"chat sequence";
}
void ChatHandler::productHandle(QTcpSocket* clientSocket,const QJsonObject& obj){
    qDebug()<<"append product sequence";
    QString name = obj["pName"].toString();
    int id = obj["pId"].toInt();
    int price = obj["pPrice"].toInt();
    int cnt = obj["pCnt"].toInt();
    QSharedPointer<Product> newproduct = QSharedPointer<Product>::create(id,name,price,cnt);
    Backend::getInstance().addProduct(newproduct);
}
