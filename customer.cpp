#include "customer.h"

Customer::Customer() {}

Customer::Customer(int id, QString name, QString pwd)
    : m_id(id)
    , m_name(name)
    , m_pwd(pwd)
{}

QJsonObject Customer::toJson() const
{
    QJsonObject obj;
    obj["id"] = getId();
    obj["name"] = getName();
    obj["pwd"] = getPwd();
    QJsonObject productobj;
    for (auto it = m_myProduct.begin(); it != m_myProduct.end(); it++) {
        productobj[it.key()] = it.value();
    }
    obj["myproduct"] = productobj;
    return obj;
}

QSharedPointer<Customer> Customer::fromJson(const QJsonObject &obj)
{
    int id = obj["id"].toInt();
    QString name = obj["name"].toString();
    QString pwd = obj["pwd"].toString();

    auto customer = QSharedPointer<Customer>::create(id, name, pwd);

    if (obj.contains("myproduct") && obj["myproduct"].isObject()) {
        QJsonObject productObj = obj["myproduct"].toObject();
        for (auto it = productObj.begin(); it != productObj.end(); ++it) {
            customer->getProduct()[it.key()] = it.value().toInt();
        }
    }
    return customer;
}
