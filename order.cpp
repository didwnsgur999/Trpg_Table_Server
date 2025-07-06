#include "order.h"

Order::Order() {}

Order::Order(int id, int cid, int pid, int pcnt)
    : m_id(id)
    , m_CID(cid)
    , m_PID(pid)
    , m_cnt(pcnt)
{}

QJsonObject Order::toJson() const
{
    QJsonObject obj;
    obj["id"] = getId();
    obj["customerId"] = getCID();
    obj["productId"] = getPID();
    obj["productCnt"] = getCnt();
    return obj;
}

QSharedPointer<Order> Order::fromJson(const QJsonObject &obj)
{
    return QSharedPointer<Order>::create(obj["id"].toInt(),
                                         obj["customerId"].toInt(),
                                         obj["productId"].toInt(),
                                         obj["productCnt"].toInt());
}
