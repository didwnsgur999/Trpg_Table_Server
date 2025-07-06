#ifndef ORDER_H
#define ORDER_H

#include <QJsonObject>
#include <QSharedPointer>

class Order
{
public:
    Order();
    Order(int id, int cid, int pid, int pcnt);
    //Product를 Json으로 변경하는 함수
    QJsonObject toJson() const;
    //Json을 Product로 변경하는 함수.
    //외부에서 객체 없이 부르는 경우 사용해야 하므로 static이 붙는다.
    static QSharedPointer<Order> fromJson(const QJsonObject &obj);
    int getId() const { return m_id; }
    int getCID() const { return m_CID; }
    int getPID() const { return m_PID; }
    int getCnt() const { return m_cnt; }

private:
    //주문명, 고객ID, 제품ID, 갯수
    int m_id;
    int m_CID;
    int m_PID;
    int m_cnt;
};

#endif // ORDER_H
