#include "roommanager.h"

RoomManager::RoomManager() {}
//
bool RoomManager::createRoom(const QString& roomName,QString RMName,QTcpSocket* socket){
    if(m_rooms.contains(roomName))
        return false;
    QSharedPointer<Room> newroom = QSharedPointer<Room>::create(roomName,RMName,socket);
    m_rooms[roomName] = newroom;
    return true;
}
bool RoomManager::removeRoom(const QString& roomName){
    return 0;
}
QSharedPointer<Room> RoomManager::getRoom(const QString& roomName){
    if (m_rooms.contains(roomName))
        return m_rooms[roomName];
    return QSharedPointer<Room>(nullptr);
}
