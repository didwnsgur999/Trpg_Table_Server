#ifndef ROOM_H
#define ROOM_H
#include <QHash>
#include <QString>
#include <QTcpSocket>
class Room
{
public:
    Room();

    //getter setters
    QString getRName() const;
    QString getRMName() const;

private:
    //방이름
    QString m_rName;
    //룸 마스터
    QString m_RMName;
    QHash<QString,QTcpSocket*> m_rMember;
    int m_rCnt;
};

#endif // ROOM_H
