#include "chathandler.h"
#include "roommanager.h"
#include "backend.h"
#include "serveruser.h"
ChatHandler::ChatHandler(QObject *parent)
    : QObject(parent)
{}
//json으로 날라온걸 받았겠지? QByteArray로 끌고온다. clientSocket도 가지고있으니까 안에서 room 찾아서 보내기도 된다.
void ChatHandler::getByteData(QTcpSocket *clientSocket, QByteArray &data)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject())
        return;

    const QJsonObject obj = doc.object();
    QString cmd = obj.value("cmd").toString();

    //이 아래가 signal처리 connect는 기본적으로 되어있어야한다.
    if (cmd == "login") {
        ChatHandler::loginHandle(clientSocket, obj);
    } else if (cmd == "chat") {
        // on the work - no debug
        ChatHandler::chatHandle(clientSocket, obj);
    } else if (cmd == "add_p") {
        ChatHandler::productAddHandle(clientSocket, obj);
    } else if (cmd == "add_c") {
        //on the work
        ChatHandler::customerAddHandle(clientSocket, obj);
    } else if (cmd == "add_o") {
        //on the work
        ChatHandler::orderAddHandle(clientSocket, obj);
    } else if (cmd == "show_r") {
        //on the work
        ChatHandler::showRoomHandle(clientSocket, obj);
    } else if (cmd == "add_r") {
        // on the work - no debug
        ChatHandler::addRoomHandle(clientSocket, obj);
    } else if (cmd == "delete_r"){
        // on the work - no debug
        ChatHandler::deleteRoomHandle(clientSocket, obj);
    } else if (cmd == "join_r"){
        // on the work - no debug
        ChatHandler::joinRoomHandle(clientSocket, obj);
    } else if (cmd == "remove_r"){
        // on the work - no debug
        ChatHandler::leaveRoomHandle(clientSocket, obj);
    }
}
void ChatHandler::loginHandle(QTcpSocket *clientSocket, const QJsonObject &obj)
{
    //on the work
    //여기 만들려면 customer쪽 완성되고, id결정하는 시퀀스가 있어야 한다.
    qDebug() << "login sequence";
    QString name = obj["cName"].toString();
    QString pwd = obj["cPwd"].toString();
    qDebug() << name << pwd;
}
//커멘드, 메시지, 방이름 보내줘야 함.
void ChatHandler::chatHandle(QTcpSocket *clientSocket, const QJsonObject &obj)
{
    //본인이 어떤 방에 들어가있는지 알아야하고, 그 방 모든사람에게 send
    qDebug() << "chat sequence";
    QString text = obj["text"].toString();
    QString rName = obj["rName"].toString();
    auto room = RoomManager::getInstance().getRoom(rName);

    QJsonObject ret;
    ret["cmd"] = "ret_chat";
    if(room==nullptr){
        //본인한테 방에 없다고 알리기
        ret["text"]="you're not in the room";
        QJsonDocument doc(ret);
        emit sendMessage(clientSocket,doc);
    }else{
        QString sName = ServerUser::getInstance().SearchNameSocket(clientSocket);
        for(auto socket:room->getRMember()){
            if(socket==clientSocket){
                //본인은 그냥 보내기
                ret["text"]=text;
            } else {
                //보낸사람이름 붙여서 보내기
                ret["text"]=QString("%1 : %2")
                            .arg(sName, text);
            }
            QJsonDocument doc(ret);
            emit sendMessage(socket,doc);
        }
    }
}
//회원가입시 오는거 처리.
void ChatHandler::customerAddHandle(QTcpSocket *clientSocket, const QJsonObject &obj){
    qDebug()<<"append customer sequence";
    //새로운 customer 만들고(id, name, pwd)
    auto newcustomer = Customer::fromJson(obj);
    //backend에 저장.
    Backend::getInstance().addCustomer(newcustomer);
}
void ChatHandler::productAddHandle(QTcpSocket *clientSocket, const QJsonObject &obj)
{
    qDebug() << "append product sequence";
    auto newproduct = Product::fromJson(obj);
    Backend::getInstance().addProduct(newproduct);
}
void ChatHandler::orderAddHandle(QTcpSocket *clientSocket, const QJsonObject &obj){
    qDebug() << "append order sequence";
    auto neworder = Order::fromJson(obj);
    Backend::getInstance().addOrder(neworder);
}
void ChatHandler::showRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj){
    qDebug()<<"show room sequence";
}
void ChatHandler::addRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj)
{
    //rName 받은거 처리
    qDebug() << "add room sequence";
    QString rName = obj["rName"].toString();

    //roommanager통해서 room 만들기 + 다시 보내줄 결과메시지생성
    QJsonObject ret;
    ret["cmd"] = "ret_add_r";
    //yes = return json 생성했다.
    if(RoomManager::getInstance().addRoom(rName,clientSocket)){
        ret["text"] = "your room added";
    } else {
    //no = return 실패했다.
        ret["text"] = "room make failed";
    }
    //doc보내기
    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}
void ChatHandler::deleteRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj)
{
    qDebug() << "delete room sequence";
    QString rName = obj["rName"].toString();

    QJsonObject ret;
    ret["cmd"] = "ret_del_r";
    if(RoomManager::getInstance().deleteRoom(rName)){
        ret["text"] = "room deleted complete";
    } else {
        ret["text"] = "room delete fail";
    }
    //doc보내기
    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}
//방 입장하기
void ChatHandler::joinRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj)
{
    qDebug() << "join room sequence";
    QString rName = obj["rName"].toString();

    QJsonObject ret;
    ret["cmd"] = "ret_join_r";
    if(RoomManager::getInstance().joinRoom(rName,clientSocket)){
        ret["text"] = "room join complete";
    } else {
        ret["text"] = "room join fail";
    }
    //doc보내기
    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}
//방 나가기
void ChatHandler::leaveRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj)
{
    qDebug() << "leave room sequence";
    QString rName = obj["rName"].toString();

    QJsonObject ret;
    ret["cmd"] = "ret_leave_r";
    if(RoomManager::getInstance().leaveRoom(rName,clientSocket)){
        ret["text"] = "room leave complete";
    } else {
        ret["text"] = "room leave fail";
    }
    //doc보내기
    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}
