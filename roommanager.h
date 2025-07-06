#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H
#include <QHash> //for room manage
#include <QSharedPointer>
#include "room.h" //for room instance
class RoomManager
{
public:
    //singleton
    static RoomManager &getInstance()
    {
        static RoomManager instance;
        return instance;
    }

    bool addRoom(const QString &roomName, QTcpSocket *socket);
    bool deleteRoom(const QString &roomName);
    bool joinRoom(const QString &roomName, QTcpSocket *socket);
    bool leaveRoom(const QString &roomName, QTcpSocket *socket);

    //room찾아오기 함수. sharedptr는 복사해도 참조 카운트만 늘어서 &별로 필요없대요.
    QSharedPointer<Room> getRoom(const QString &roomName);

private:
    // 복사 방지, 생성방지
    RoomManager(const RoomManager &ref) {}
    RoomManager &operator=(const RoomManager &ref) {}
    RoomManager();
    QHash<QString, QSharedPointer<Room>> m_rooms;
};

#endif // ROOMMANAGER_H
