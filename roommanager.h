#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H
#include "room.h"
#include <QHash>    //for room manage
#include <QSharedPointer>
class RoomManager
{
public:
    //singleton
    static RoomManager& getInstance() {
        static RoomManager instance;
        return instance;
    }

    bool createRoom(QString name);
    bool removeRoom(QString name);
    //bool joinRoom(QString name);
    //bool leaveRoom(QString name);

private:
    // 복사 방지, 생성방지
    RoomManager(const RoomManager& ref){}
    RoomManager& operator=(const RoomManager& ref){}
    RoomManager();
    QHash<QString,QSharedPointer<Room>> m_rooms;
};

#endif // ROOMMANAGER_H
