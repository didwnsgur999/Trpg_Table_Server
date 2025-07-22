#include "roommanager.h"
#include "serveruser.h"     //for clientId;
RoomManager::RoomManager() {}
//룸 만들기
bool RoomManager::addRoom(const QString &roomName, QTcpSocket *socket)
{
    if (m_rooms.contains(roomName))
        return false;
    //룸마 ID가 필요함. -> serveruser에서 Id들고오는게 맞겠다.
    int m_RMId = ServerUser::getInstance().SearchIdSocket(socket);
    if(m_RMId==-1){
        return false;
    }
    QSharedPointer<Room> newroom = QSharedPointer<Room>::create(roomName, m_RMId, socket);
    m_rooms[roomName] = newroom;

    joinRoom(roomName, socket);
    return true;
}
//룸 지우기
bool RoomManager::deleteRoom(const QString &roomName)
{
    //그런방 없으면 false
    if(!m_rooms.contains(roomName))
        return false;
    auto room=m_rooms.value(roomName);
    //방에 본인빼고 누가있으면 false
    if(room->getRCnt()>1){
        return false;
    }
    //방 지우기.
    room->clearall();
    m_rooms.remove(roomName);

    return true;
}
//룸에 들어가기
bool RoomManager::joinRoom(const QString &roomName,QTcpSocket *socket){
    int userId = ServerUser::getInstance().SearchIdSocket(socket);
    //사람 Id 없으면 실패
    if(userId==-1){
        return false;
    }
    //방 이름 없으면 실패
    if(!m_rooms.contains(roomName)){
        return false;
    }
    auto room = m_rooms.value(roomName);
    //방있고, 사람 있으면
    //이제 방 들어가야됨
    //룸에 들어가는 처리
    //대신 다른 룸에는 이미 없다고 가정해야됨.
    room->addMem(userId,socket);
    return true;
}
//룸 나가기
bool RoomManager::leaveRoom(const QString &roomName,QTcpSocket *socket){
    int userId = ServerUser::getInstance().SearchIdSocket(socket);
    //사람 Id 없으면 실패
    if(userId==-1){
        return false;
    }
    //방 이름 없으면 실패
    if(!m_rooms.contains(roomName)){
        return false;
    }
    qDebug()<<"leave room seq";
    auto room = m_rooms.value(roomName);
    //방있고, 사람 있으면
    //방 나가야됨
    //룸에서 나가는 처리
    room->deleteMem(userId);
    qDebug()<<room->getRCnt();
    return true;
}

QSharedPointer<Room> RoomManager::getRoom(const QString &roomName)
{
    if (m_rooms.contains(roomName))
        return m_rooms[roomName];
    return QSharedPointer<Room>(nullptr);
}
QVector<QSharedPointer<Room>> RoomManager::getAllRoom(){
    QVector<QSharedPointer<Room>> vec;
    for(auto i:m_rooms){
        vec.append(i);
    }
    return vec;
}
