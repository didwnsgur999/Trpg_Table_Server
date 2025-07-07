#ifndef ROOM_H
#define ROOM_H
#include <QHash>
#include <QString>
#include <QTcpSocket>
#include "logworker.h"
class Room
{
public:
    Room(QString rName, int m_RMId, QTcpSocket *socket);
    ~Room();
    void addMem(int Id,QTcpSocket* socket);
    void deleteMem(int Id);

    //getter setters
    QString getRName() const {return m_rName;}
    int getRMId() const {return m_RMId;}
    const QHash<int, QTcpSocket *> &getRMember() const {return m_rMember;}
    int getRCnt() const {return m_rCnt;}

    //로그 메시지 저장.
    void logMessage(const QString &msg);

private:
    //방이름
    QString m_rName;
    //룸 마스터 ID
    int m_RMId;
    QHash<int, QTcpSocket *> m_rMember;
    int m_rCnt;
    //로그용 로그 찍는 class
    LogWorker *m_logworker;
};

#endif // ROOM_H
