#ifndef SERVERUSER_H
#define SERVERUSER_H
#include <QHash>
#include <QTcpSocket>
class ServerUser
{
public:
    static ServerUser& getInstance() {
        static ServerUser s;
        return s;
    }
    //유저 등록 제거
    bool AssignUser(QTcpSocket* userSocket);
    bool ChangeUserName(QTcpSocket* userSocket,int userId);
    bool RemoveUser(QTcpSocket* userSocket);

    //왠만하면 QString이랑 QTcpSocket*으로 상대방 찾을수 잇으면 좋겠는데?
    int SearchNameSocket(QTcpSocket*);
    QTcpSocket* SearchSocketName(int userId);

private:
    ServerUser();
    ServerUser(const ServerUser& ) {}
    ServerUser& operator=(const ServerUser& ) {}
    ~ServerUser() {}
    //QHash로 저장.
    QHash<QTcpSocket*,int> m_curuser;
};

#endif // SERVERUSER_H
