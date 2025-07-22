#include "room.h"
//로그 찍는 worker만 만들어서 한다.
Room::Room(QString rName, int RMId, QTcpSocket *Socket)
    : m_rName(rName)
    , m_RMId(RMId)
{
    Max_iid=0;
    m_rCnt=0;

    m_logworker = new LogWorker(rName);
    m_logworker->start();
    //아래는 join안한다는 가정하에 하는거고.
    //m_rMember[RMId] = Socket;
    //addMem(RMId,Socket);
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

//item 추가 제거
RoomItem Room::addRoomItem(int pId, QString pName){
    int x=20, y=20, z = 0;
    int iid=Max_iid++;
    while(!searchColXYItem(x,y)){}
    RoomItem item = {iid,pId,pName,x,y,z};
    m_item.append(item);
    return item;
}
bool Room::delRoomItem(int iid){
    for(int i=0; i<m_item.size(); i++){
        if(m_item[i].iid==iid){
            m_item.removeAt(i);
            return 1;
        }
    }
    return 0;
}
bool Room::movRoomItem(int iid,int x,int y,int z,int* finx,int *finy,int *finz){
    for(int i=0; i<m_item.size();i++){
        if(m_item[i].iid==iid){
            m_item[i].x=x;
            m_item[i].y=y;
            m_item[i].z=z;
            *finx=x;
            *finy=y;
            *finz=z;
            return 1;
        }
    }
    return 0;
}

bool Room::searchColXYItem(int &x, int &y){
    for(RoomItem& i:m_item){
        if(i.x==x&&i.y==y){
            x++;y++;
            return 0;
        }
    }
    return 1;
}

void Room::clearall(){

}

void Room::logMessage(const QString &msg)
{
    m_logworker->enqueueLog(msg);
}

