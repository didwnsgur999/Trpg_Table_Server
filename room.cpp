#include "room.h"
//로그 찍는 worker만 만들어서 한다.
Room::Room(QString rName,QString RMName,QTcpSocket* Socket)
    :m_rName(rName),m_RMName(RMName)
{
    m_rMember[RMName]=Socket;
    m_logworker = new LogWorker(rName);
    m_logworker->start();
}

Room::~Room(){
    m_logworker->stop();
    m_logworker->wait();
    delete m_logworker;
}

void Room::logMessage(const QString& msg){
    m_logworker->enqueueLog(msg);
}

