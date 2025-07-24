#include "chathandler.h"
#include "roommanager.h"
#include "backend.h"
#include "serveruser.h"
#include "roomitem.h"
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
    if (cmd == "login") { //로그인 처리
        ChatHandler::loginHandle(clientSocket, obj);
    } else if (cmd == "chat") { // 채팅 처리
        ChatHandler::chatHandle(clientSocket, obj);
    } else if (cmd == "list_p"){ // 제품 목록 요청
        ChatHandler::listProductHandle(clientSocket, obj);
    }else if (cmd == "add_p") { // 제품 추가 요청
        ChatHandler::productAddHandle(clientSocket, obj);
    } else if (cmd == "list_c"){ // 고객 목록 요청
        ChatHandler::listCustomerHandle(clientSocket, obj);
    }else if (cmd == "add_c") { // 고객 생성 - 회원가입
        ChatHandler::customerAddHandle(clientSocket, obj);
    }else if (cmd == "list_o"){ //주문 목록 요청
        ChatHandler::listOrderHandle(clientSocket,obj);
    }else if (cmd == "add_o") { //주문 생성
        ChatHandler::orderAddHandle(clientSocket, obj);
    } else if (cmd == "list_r") { // 채팅방 목록 전부 요청
        ChatHandler::listRoomHandle(clientSocket, obj);
    } else if (cmd == "add_r") { //채팅방 추가하기
        ChatHandler::addRoomHandle(clientSocket, obj);
    } else if (cmd == "delete_r"){ // 채팅방 지우기
        ChatHandler::deleteRoomHandle(clientSocket, obj);
    } else if (cmd == "join_r"){ // 채팅방 들어가기
        // on the work - no debug
        ChatHandler::joinRoomHandle(clientSocket, obj);
    } else if (cmd == "leave_r"){ // 채팅방 나가기
        ChatHandler::leaveRoomHandle(clientSocket, obj);
    } else if (cmd == "add_r_item"){ // 채팅방 아이템 추가
        ChatHandler::addRoomItemHandle(clientSocket, obj);
    } else if (cmd == "del_r_item"){ // 채팅방 아이템 지우기
        ChatHandler::deleteRoomItemHandle(clientSocket, obj);
    } else if (cmd == "mov_r_item"){ // 채팅방 아이템 움직임
        ChatHandler::movRoomItemHandle(clientSocket,obj);
    } else if (cmd == "list_r_users"){ //채팅방 유저 전부 요청
        ChatHandler::listRoomUserHandle(clientSocket,obj);
    } else if (cmd == "list_users"){ //현재 유저 전부 요청
        ChatHandler::listUserHandle(clientSocket,obj);
    } else if (cmd == "list_r_items"){ //채팅방 아이템 전부 요청
        ChatHandler::listRoomItemHandle(clientSocket,obj);
    } else if (cmd == "invite_r"){ //초대처리
        ChatHandler::inviteRoomUser(clientSocket,obj);
    } else if (cmd == "ban_r_user"){ //강퇴처리
        ChatHandler::banRoomUser(clientSocket,obj);
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
        if(RoomManager::getInstance().getRoom(rName)->getRCnt()==0){
            RoomManager::getInstance().deleteRoom(rName);
        }
    } else {
        ret["text"] = "failed";
    }
    //doc보내기
    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}

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
void ChatHandler::addRoomItemHandle(QTcpSocket *clientSocket, const QJsonObject &obj){
    //특정 방에 뭐 추가하라고 준거잖음? 그럼 방 이름도 줘야지?
    qDebug()<<"add room item";
    //제품 id랑 rName 받았다.
    int pid = obj["pid"].toInt();
    QString rName = obj["rName"].toString();

    auto product = Backend::getInstance().searchProductId(pid);
    auto room = RoomManager::getInstance().getRoom(rName);

    QJsonObject ret;
    ret["cmd"] = "ret_add_r_item";
    if(product!=nullptr && room!=nullptr){
        qDebug()<<"success";
        //둘다 포인터가 돌아왔으면 방에 추가하고 모든 room 사람들한테 알려야됨.
        auto item = room->addRoomItem(pid,product->getName());
        //룸 아이템 json으로 보내줘야됨.
        ret["text"]="success";
        ret["item"]=item.toJson();
        qDebug()<<"emit: "<<item.pid;
        for(auto socket:room->getRMember()){
            QJsonDocument doc(ret);
            emit sendMessage(socket,doc);
        }
    } else {
        qDebug()<<"failure";
        ret["text"]="failure";
        QJsonDocument doc(ret);
        emit sendMessage(clientSocket,doc);
    }
}
void ChatHandler::deleteRoomItemHandle(QTcpSocket *clientSocket, const QJsonObject &obj){
    qDebug()<<"del room item";
    //제품 id랑 rName + 좌표받았다.
    int iid = obj["iid"].toInt();
    QString rName = obj["rName"].toString();

    auto room = RoomManager::getInstance().getRoom(rName);
    qDebug()<<iid;
    QJsonObject ret;
    ret["cmd"]="ret_del_r_item";
    if(room!=nullptr){ //방 있음
        //지워짐.
        if(room->delRoomItem(iid)){
            qDebug()<<"delete item success";
            ret["text"]="success";
            ret["iid"]=iid;
            //방 전원한테 지우라고 broadcast 해야됨.
            for(auto socket:room->getRMember()){
                QJsonDocument doc(ret);
                emit sendMessage(socket,doc);
            }
        } else { //안 지워짐
            ret["text"]="failure no item in the room";
            QJsonDocument doc(ret);
            emit sendMessage(clientSocket,doc);
        }
    } else { //방 자체가 없음
        qDebug()<<"failure";
        //방 없음 지워짐.
        ret["text"]="failure by room erased";
        QJsonDocument doc(ret);
        emit sendMessage(clientSocket,doc);
    }
}
void ChatHandler::movRoomItemHandle(QTcpSocket *clientSocket, const QJsonObject &obj){
    qDebug()<<"server: moveRoom handler called";
    int iid = obj["iid"].toInt();
    int newx = obj["newx"].toInt();
    int newy = obj["newy"].toInt();
    int z = obj["z"].toInt();
    QString rName = obj["rName"].toString();

    auto room = RoomManager::getInstance().getRoom(rName);
    qDebug()<<iid;
    QJsonObject ret;
    ret["cmd"]="ret_mov_r_item";
    //아래 서버 mov item 코드
    if(room!=nullptr){
        int finx=0,finy=0,finz=0;
        if(room->movRoomItem(iid,newx,newy,z,&finx,&finy,&finz)){
            ret["text"]="success";
            ret["iid"]=iid;
            ret["finx"]=finx;
            ret["finy"]=finy;
            ret["finz"]=finz;
            for(auto socket:room->getRMember()){
                QJsonDocument doc(ret);
                emit sendMessage(socket,doc);
            }
        }else{
            ret["text"]="failed";
            QJsonDocument doc(ret);
            emit sendMessage(clientSocket,doc);
        }
    }else {
        ret["text"]="failed";
        QJsonDocument doc(ret);
        emit sendMessage(clientSocket,doc);
    }
}
void ChatHandler::listRoomUserHandle(QTcpSocket *clientSocket, const QJsonObject &obj){
    QString roomName = obj["rName"].toString();
    auto room = RoomManager::getInstance().getRoom(roomName);
    QJsonArray arr;
    auto members = room->getRMember();
    for(auto it = members.begin(); it!=members.end(); it++){
        QJsonObject userobj;
        userobj["id"] = it.key();
        userobj["Name"] = Backend::getInstance().searchCustomerId(it.key())->getName();
        arr.append(userobj);
    }
    QJsonObject ret;
    ret["cmd"]="ret_list_r_users";
    ret["rusers"]=arr;
    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}
void ChatHandler::listUserHandle(QTcpSocket *clientSocket, const QJsonObject &obj){
    //그냥 serveruser name 다 보내지 뭐.
    auto hashUserName = ServerUser::getInstance().getUserName();
    QJsonArray arr;
    for(auto it = hashUserName.begin(); it!=hashUserName.end(); it++){
        QJsonObject userobj;
        userobj["id"]=it.key();
        userobj["Name"]=it.value();
        arr.append(userobj);
    }
    QJsonObject ret;
    ret["cmd"]="ret_list_users";
    ret["users"]=arr;
    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}

void ChatHandler::listRoomItemHandle(QTcpSocket *clientSocket,const QJsonObject &obj){
    //room Item 보내줘야됨
    QString rName = obj["rName"].toString();
    auto room = RoomManager::getInstance().getRoom(rName);
    auto Itemlist = room->getRItem();
    QJsonArray arr;
    for(auto item:Itemlist ){
        arr.append(item.toJson());
    }
    QJsonObject ret;
    ret["cmd"]="ret_list_r_items";
    ret["rItems"]=arr;
    QJsonDocument doc(ret);
    emit sendMessage(clientSocket,doc);
}
void ChatHandler::inviteRoomUser(QTcpSocket *clientSocket,const QJsonObject &obj){
    QString rName = obj["rName"].toString();
    int cid = obj["cid"].toInt();

    auto Socket = ServerUser::getInstance().SearchSocketId(cid);

    QJsonObject ret;
    ret["cmd"]="ret_invite_r";
    //socket is not nullptr
    if(Socket!=nullptr){
        ret["text"]="success";
        ret["rName"]=rName;

        QJsonDocument doc(ret);
        //이건 다른 유저한테 보내는거
        emit sendMessage(Socket,doc);
    } else {
        ret["text"]="failure";
        QJsonDocument doc(ret);
        //이건 본인한테 보내는거
        emit sendMessage(clientSocket,doc);
    }
}
void ChatHandler::banRoomUser(QTcpSocket *clientSocket,const QJsonObject &obj){
    QString rName = obj["rName"].toString();
    int mid = obj["mid"].toInt();
    int cid = obj["cid"].toInt();
    auto room = RoomManager::getInstance().getRoom(rName);
    bool qualify=false;
    if(!(room==nullptr)){
        if(room->getRMId()==mid){
            qualify=true;
        }
    }
    //qualify = true, socket존재 -> ban가능.
    //qualify = false -> 권한없음 돌려줌
    //qualify = true, socket null -> 이미 나간사람.
    auto Socket = ServerUser::getInstance().SearchSocketId(cid);

    QJsonObject ret;
    ret["cmd"]="ret_ban_r";
    //권한있음
    if(qualify==true){
        if(Socket==nullptr){
            ret["text"]="유저 접속 종료됨";
            QJsonDocument doc(ret);
            //본인에게 결과 보냄
            emit sendMessage(clientSocket,doc);
        }else{
            //상대방 leave room시킴 이미 나갔으면 후처리 필요없음 그냥 인과가 같음.
            if(RoomManager::getInstance().leaveRoom(rName,Socket)){
                ret["cmd"]="ret_leave_r";
                ret["text"]="success";
                QJsonDocument doc(ret);
                emit sendMessage(Socket,doc);
                //그리고 밴됬다는 사실 알려줘야됨.
                QJsonObject ban;
                ban["cmd"]="ret_banned_r";
                //상대방 죽임.
                QJsonDocument docban(ban);
                emit sendMessage(Socket,docban);
            }
        }
    } else {
        ret["text"]="방장 권한 없음";
        QJsonDocument doc(ret);
        //본인에게 결과 보냄
        emit sendMessage(clientSocket,doc);
    }
}
