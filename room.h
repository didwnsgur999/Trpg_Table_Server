#ifndef ROOM_H
#define ROOM_H
#include "logworker.h"
#include <QHash>
#include <QString>
#include <QTcpSocket>
class Room
{
public:
    Room(QString rName,QString m_RMName,QTcpSocket* socket);
    ~Room();
    //getter setters
    QString getRName() const;
    QString getRMName() const;
    QHash<QString,QTcpSocket*>& getRMember() const;
    int getRCnt() const;
    //로그 메시지 저장.
    void logMessage(const QString& msg);

private:
    //방이름
    QString m_rName;
    //룸 마스터
    QString m_RMName;
    QHash<QString,QTcpSocket*> m_rMember;
    int m_rCnt;
    //로그용 로그 찍는 class
    LogWorker* m_logworker;
};

#endif // ROOM_H
