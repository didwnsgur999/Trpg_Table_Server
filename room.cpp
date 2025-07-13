#include "room.h"
//로그 찍는 worker만 만들어서 한다.
Room::Room(QString rName, int RMId, QTcpSocket *Socket)
    : m_rName(rName)
    , m_RMId(RMId)
{
    m_rMember[RMId] = Socket;
    m_logworker = new LogWorker(rName);
    m_logworker->start();
    addMem(RMId,Socket);
}

Room::~Room()
{
    m_logworker->stop();
    m_logworker->wait();
    delete m_logworker;
}

void Room::addMem(int Id,QTcpSocket* socket){
    if (!m_rMember.contains(Id)) {
        m_rMember[Id] = socket;
        m_rCnt++;
        qDebug() << "방 [" << m_rName << "]에 멤버 추가: ID " << Id << ", 현재 인원: " << m_rCnt;
    } else {
        qDebug() << "방 [" << m_rName << "]에 이미 존재하는 멤버: ID " << Id << " (추가 건너뜀)";
    }
}

void Room::deleteMem(int Id){
    m_rMember.remove(Id);
    m_rCnt--;
}

void Room::logMessage(const QString &msg)
{
    m_logworker->enqueueLog(msg);
}
