#include "chathandler.h"
#include "roommanager.h"
#include "backend.h"
#include "serveruser.h"
#include <QBuffer>
#include <QDateTime>
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
    } else if (cmd == "list_p"){
        //on the work
        ChatHandler::listProductHandle(clientSocket, obj);
    }else if (cmd == "add_p") {
        ChatHandler::productAddHandle(clientSocket, obj);
    } else if (cmd == "list_c"){
        //on the work
        ChatHandler::listCustomerHandle(clientSocket, obj);
    }else if (cmd == "add_c") {
        ChatHandler::customerAddHandle(clientSocket, obj);
    }else if (cmd == "list_o"){
        //on the work
        ChatHandler::listOrderHandle(clientSocket,obj);
    }else if (cmd == "add_o") {
        ChatHandler::orderAddHandle(clientSocket, obj);
    } else if (cmd == "list_r") {
        ChatHandler::listRoomHandle(clientSocket, obj);
    } else if (cmd == "add_r") {
        // on the work - no debug
        ChatHandler::addRoomHandle(clientSocket, obj);
    } else if (cmd == "delete_r"){
        // on the work - no debug
        ChatHandler::deleteRoomHandle(clientSocket, obj);
    } else if (cmd == "join_r"){
        // on the work - no debug
        ChatHandler::joinRoomHandle(clientSocket, obj);
    } else if (cmd == "leave_r"){
        // on the work - no debug
        ChatHandler::leaveRoomHandle(clientSocket, obj);
    } else if (cmd == "add_r_image"){
        ChatHandler::addRoomImageHandle(clientSocket, obj);
    }
}
void ChatHandler::loginHandle(QTcpSocket *clientSocket, const QJsonObject &obj)
{
    //on the work
    qDebug() << "login sequence";
    QString name = obj["cName"].toString();
    QString pwd = obj["cPwd"].toString();
    auto customer = Backend::getInstance().searchCustomerLogin(name,pwd);

    //serveruser처리와 다시 보내기
    QJsonObject ret;
    ret["cmd"] = "ret_login";
    if(customer==nullptr){
        ret["text"] = "failed";
    }else{
        ServerUser::getInstance().ChangeUserId(clientSocket,customer->getId(),customer->getName());
        QJsonObject cus = customer->toJson();
        ret["text"] = "success";
        ret["cus"] = cus;
    }

    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
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
        ret["text"]="failed";
        QJsonDocument doc(ret);
        emit sendMessage(clientSocket,doc);
    }else{
        QString sName = ServerUser::getInstance().SearchNameSocket(clientSocket);
        //로그 찍는다.
        QString log = QString("%1 - %2 : %3").arg(QDateTime::currentDateTime().toString("MM/dd hh:mm"),sName,text);
        room->logMessage(log);
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
    //새로운 customer 만들고(name, pwd)
    QString name = obj["cName"].toString();
    QString pwd = obj["cPwd"].toString();
    auto customer = Backend::getInstance().searchCustomerLogin(name,pwd);
    QJsonObject ret;
    ret["cmd"] = "ret_add_c";
    if(customer==nullptr){
        auto newcustomer = Customer::fromJsonEXID(obj);
        //backend에 저장.
        Backend::getInstance().addCustomer(newcustomer);
        ServerUser::getInstance().ChangeUserId(clientSocket,newcustomer->getId(),newcustomer->getName());
        QJsonObject cus = newcustomer->toJson();
        ret["text"] = "success";
        ret["cus"] = cus;
    }else{
        ret["text"] = "failed";
    }

    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}
void ChatHandler::productAddHandle(QTcpSocket *clientSocket, const QJsonObject &obj)
{
    qDebug() << "append product sequence";
    auto newproduct = Product::fromJson(obj);
    Backend::getInstance().addProduct(newproduct);
}
void ChatHandler::orderAddHandle(QTcpSocket *clientSocket, const QJsonObject &obj){
    qDebug() << "append order sequence";
    //제품을 샀음 -> customer에 제품 넣고, 제품 갯수 낮추고, 안되면 안된다고 보내야됨.
    int cid = ServerUser::getInstance().SearchIdSocket(clientSocket);
    QString pname = obj["pname"].toString();
    int pcnt = obj["pcnt"].toInt();
    qDebug()<<pname<<pcnt;

    //제품 찾기, 찾았으면 customer의 장바구니에 들어가야하고, 갯수 낮추고 order만든다.
    //room쪽
    int flag=0;
    auto prod = Backend::getInstance().searchProductName(pname);
    auto cus = Backend::getInstance().searchCustomerId(cid);
    if(prod!=nullptr&&prod->getCnt()>=pcnt){
        qDebug()<<"prod:"<<prod;
        if(cus!=nullptr){
            qDebug()<<"cus:"<<cus;
            //살수있다.
            flag = 1;
        }
    }
    qDebug()<<"flag:"<<flag;
    QJsonObject ret;
    ret["cmd"] = "ret_add_o";
    if(flag){
        //order추가, prod갯수 줄이기, customer에 prodid추가
        auto neworder = QSharedPointer<Order>::create(cid,prod->getId(),pcnt);
        qDebug()<<neworder->getId();
        prod->setCnt(prod->getCnt()-pcnt);
        cus->addprod(prod->getName(),prod->getId());
        Backend::getInstance().addOrder(neworder);
        ret["text"]="success";
    } else {
        ret["text"]="failed";
    }

    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}
void ChatHandler::listRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj){
    qDebug()<<"list room sequence";
    auto vec = RoomManager::getInstance().getAllRoom();

    QJsonObject ret;
    ret["cmd"] = "ret_list_r";
    QJsonArray roomArray;

    for(const auto& room:vec){
        QJsonObject roomobj;
        QString name = room->getRName();
        int cnt = room->getRCnt();
        roomobj["name"] = name;
        roomobj["cnt"] = cnt;
        qDebug()<<name<<cnt;
        roomArray.append(roomobj);
    }

    ret["roomlist"] = roomArray;

    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}
void ChatHandler::addRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj)
{
    //rName 받은거 처리
    qDebug() << "add room sequence";
    QString rName = obj["rName"].toString();
    qDebug() << rName;
    //roommanager통해서 room 만들기 + 다시 보내줄 결과메시지생성
    QJsonObject ret;
    ret["cmd"] = "ret_add_r";
    //yes = return json 생성했다.
    if(RoomManager::getInstance().addRoom(rName,clientSocket)){
        ret["text"] = "success";
    } else {
        ret["text"] = "failed";
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
        ret["text"] = "success";
    } else {
        ret["text"] = "failed";
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
        ret["text"] = "success";
        ret["rName"] = rName;
    } else {
        ret["text"] = "failed";
    }
    //doc보내기
    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}
//방 나가기 - 추가
void ChatHandler::leaveRoomHandle(QTcpSocket *clientSocket, const QJsonObject &obj)
{
    qDebug() << "leave room sequence";
    QString rName = obj["rName"].toString();

    QJsonObject ret;
    ret["cmd"] = "ret_leave_r";
    if(RoomManager::getInstance().leaveRoom(rName,clientSocket)){
        ret["text"] = "success";
    } else {
        ret["text"] = "failed";
    }
    //doc보내기
    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}
//===================
//on the work
//==================
void ChatHandler::listProductHandle(QTcpSocket *clientSocket, const QJsonObject &obj){
    qDebug()<< "list product";
    auto vec = Backend::getInstance().getProductList();
    QJsonObject ret;
    ret["cmd"] = "ret_list_p";
    QJsonArray prodArray;

    for(const auto& prod:vec){
        QJsonObject prodobj;
        int id = prod->getId();
        QString name = prod->getName();
        int price = prod->getPrice();
        int cnt = prod->getCnt();
        QPixmap pix = prod->getImage();
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);

        // PNG 형식으로 QPixmap을 QByteArray로 저장
        pix.save(&buffer, "PNG");
        QString pixstr = QString::fromUtf8(byteArray.toBase64());
        prodobj["pId"] = id;
        prodobj["pName"] = name;
        prodobj["pPrice"] = price;
        prodobj["pCnt"] = cnt;
        prodobj["pImage"] = pixstr;
        qDebug()<<name<<cnt;
        prodArray.append(prodobj);
    }
    ret["productlist"]=prodArray;
    //진행중
    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}
void ChatHandler::listOrderHandle(QTcpSocket *clientSocket, const QJsonObject &obj){
    qDebug()<< "list order";
    auto vec = Backend::getInstance().getOrderList();
    QJsonObject ret;
    ret["cmd"] = "ret_list_o";
    QJsonArray ordArray;

    for(const auto& ord:vec){
        QJsonObject ordobj;
        int id = ord->getId();
        int pid = ord->getPID();
        int cid = ord->getCID();
        int cnt = ord->getCnt();
        ordobj["id"] = id;
        ordobj["cid"] = pid;
        ordobj["pid"] = cid;
        ordobj["cnt"] = cnt;
        qDebug()<<id<<pid;
        ordArray.append(ordobj);
    }
    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}
void ChatHandler::listCustomerHandle(QTcpSocket *clientSocket, const QJsonObject &obj){
    qDebug()<< "list customer";
    auto vec = Backend::getInstance().getCustomerList();
    QJsonObject ret;
    ret["cmd"] = "ret_list_c";
    QJsonArray cusArray;

    for(const auto& cus:vec){
        QJsonObject cusobj;
        QString name = cus->getName();
        int id = cus->getId();
        cusobj["name"] = name;
        cusobj["id"] = id;
        qDebug()<<name<<id;
        cusArray.append(cusobj);
    }

    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}
//===================//
//here
//===================//
void ChatHandler::addRoomImageHandle(QTcpSocket *clientSocket, const QJsonObject &obj){
    //특정 방에 뭐 추가하라고 준거잖음? 그럼 방 이름도 줘야지?


}
