#include "serveruser.h"

ServerUser::ServerUser() {}

//유저 등록 제거
bool ServerUser::AssignUser(QTcpSocket* userSocket){
    if(m_curuser.contains(userSocket)){
        return false;
    }
    m_curuser[userSocket]=-1;
    return true;
}
bool ServerUser::ChangeUserName(QTcpSocket* userSocket,int userId){
    if(m_curuser.contains(userSocket))
        return false;
    m_curuser[userSocket]=userId;
    return true;
}
bool ServerUser::RemoveUser(QTcpSocket* userSocket){
    if(m_curuser.contains(userSocket))
        return false;
    m_curuser.remove(userSocket);
    return true;
}

//왠만하면 QString이랑 QTcpSocket*으로 상대방 찾을수 잇으면 좋겠는데?
int ServerUser::SearchNameSocket(QTcpSocket* userSocket){
    if(m_curuser.contains(userSocket)){
        return m_curuser[userSocket];
    }
    return -1;
}
QTcpSocket* ServerUser::SearchSocketName(int userId){
    for(auto it=m_curuser.begin(); it!=m_curuser.end(); it++){
        if(it.value()==userId)
            return it.key();
    }
    return nullptr;
}
