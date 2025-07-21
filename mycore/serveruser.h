#ifndef SERVERUSER_H
#define SERVERUSER_H
#include <QHash>
#include <QTcpSocket>
class ServerUser
{
public:
    static ServerUser &getInstance()
    {
        static ServerUser s;
        return s;
    }
    //유저 등록 제거
    bool AssignUser(QTcpSocket *userSocket);
    bool ChangeUserId(QTcpSocket *userSocket, int userId, QString userName);
    bool RemoveUser(QTcpSocket *userSocket);

    //왠만하면 Id랑 QTcpSocket*으로 상대방 찾을수 잇으면 좋겠는데?
    QHash<int,QString>& getUserName(){return m_userName;}
    int SearchIdSocket(QTcpSocket *);
    QTcpSocket *SearchSocketId(int userId);
    int SearchIdName(QString userName);
    QString SearchNameId(int userId);
    QString SearchNameSocket(QTcpSocket *userSocket);

private:
    ServerUser();
    ServerUser(const ServerUser &) {}
    ServerUser &operator=(const ServerUser &) {}
    ~ServerUser() {}
    //QHash
    QHash<QTcpSocket *, int> m_curuser;
    QHash<int, QString> m_userName;
};

#endif // SERVERUSER_H
