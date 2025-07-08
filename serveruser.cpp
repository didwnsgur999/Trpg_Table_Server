#include "serveruser.h"
ServerUser::ServerUser() {}
//===========================================================
//진행중 헤더에 hash하나 추가되서 curuser랑 userName쪽 고쳐야됨.
//유저 등록 제거
bool ServerUser::AssignUser(QTcpSocket *userSocket)
{
    if (m_curuser.contains(userSocket)) {
        return false;
    }
    //임시로 저장.
    m_curuser[userSocket] = -1;
    qDebug()<<"assign user available";

    return true;
}
//유저 로그인시 불러올거임.
bool ServerUser::ChangeUserId(QTcpSocket *userSocket, int userId, QString userName)
{
    if (!m_curuser.contains(userSocket))
        return false;
    m_curuser[userSocket] = userId;
    m_userName[userId]=userName;

    return true;
}
//유저 접속 종료시 불러올거임.
bool ServerUser::RemoveUser(QTcpSocket *userSocket)
{
    if (m_curuser.contains(userSocket))
        return false;
    //hash정리하기
    if(m_userName.contains(m_curuser.value(userSocket)))
        m_userName.remove(m_curuser.value(userSocket));
    m_curuser.remove(userSocket);
    return true;
}
//진행중
//===============================================================

//순방향 역방향 다 탐색가능. 없으면 nullptr or -1 or nullString
int ServerUser::SearchIdSocket(QTcpSocket *userSocket)
{
    return m_curuser.value(userSocket,-1);
}
QTcpSocket *ServerUser::SearchSocketId(int userId)
{
    for (auto it = m_curuser.begin(); it != m_curuser.end(); it++) {
        if (it.value() == userId)
            return it.key();
    }
    return nullptr;
}
int ServerUser::SearchIdName(QString userName){
    for (auto it = m_userName.begin(); it != m_userName.end(); it++) {
        if (it.value() == userName)
            return it.key();
    }
    return -1;
}
QString ServerUser::SearchNameId(int userId){
    return m_userName.value(userId, QString());
}
QString ServerUser::SearchNameSocket(QTcpSocket *userSocket)
{
    int id = m_curuser.value(userSocket, -1);
    if (id == -1) return QString();
    return m_userName.value(id, QString());
}
