#include "room.h"
//로그 찍는 worker만 만들어서 한다.
Room::Room(QString rName, int RMId, QTcpSocket *Socket)
    : m_rName(rName)
    , m_RMId(RMId)
{
    m_rCnt=0;
    m_rMember[RMId] = Socket;
    m_logworker = new LogWorker(rName);
    m_logworker->start();
}

Room::~Room()
{
    m_logworker->stop();
    m_logworker->wait();
    delete m_logworker;
}

void Room::addMem(int Id,QTcpSocket* socket){
    m_rMember[Id]=socket;
    m_rCnt++;
}
void Room::deleteMem(int Id){
    m_rMember.remove(Id);
    m_rCnt--;
}

void Room::logMessage(const QString &msg)
{
    m_logworker->enqueueLog(msg);
}
