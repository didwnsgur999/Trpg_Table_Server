#ifndef ROOM_H
#define ROOM_H
#include <QHash>
#include <QString>
#include <QTcpSocket>
#include "logworker.h"
#include "roomitem.h"
class Room
{
public:
    Room(QString rName, int m_RMId, QTcpSocket *socket);
    ~Room();
    //방에 멤버 넣기
    void addMem(int Id,QTcpSocket* socket);
    void deleteMem(int Id);
    //방 방장 변경하기

    //getter setters
    QString getRName() const {return m_rName;}
    int getRMId() const {return m_RMId;}
    const QHash<int, QTcpSocket *> &getRMember() const {return m_rMember;}
    int getRCnt() const {return m_rCnt;}
    //int getItem() const {return }
    //room item 추가제거
    RoomItem addRoomItem(int pId, QString pName);
    bool delRoomItem(int iid);
    bool movRoomItem(int iid,int x,int y,int z,int* finx,int *finy,int *finz);
    //room clear;
    void clearall();
    //충돌 방어용.
    bool searchColXYItem(int &x, int &y);

    //로그 메시지 저장.
    void logMessage(const QString &msg);

private:
    //방이름
    QString m_rName;
    //룸 마스터 ID
    int m_RMId;
    QHash<int, QTcpSocket *> m_rMember;
    int m_rCnt;
    //룸 오브젝트
    QList<RoomItem> m_item;
    int Max_iid;
    //로그용 로그 찍는 class
    LogWorker *m_logworker;
};

#endif // ROOM_H
