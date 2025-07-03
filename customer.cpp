#include "customer.h"

Customer::Customer() {}

Customer::Customer(int id, QString name, int phoneNum)
    :m_id(id),m_name(name),m_phoneNum(phoneNum)
{}

QJsonObject Customer::toJson() const {
    QJsonObject obj;
    obj["id"] = getId();
    obj["name"] = getName();
    obj["phonenum"] = getPhoneNum();
    QJsonObject productobj;
    for(auto it = m_myProduct.begin();it!=m_myProduct.end();it++){
        productobj[it.key()]=it.value();
    }
    obj["myproduct"] = productobj;
    return obj;
}
//int m_id;
//int m_phoneNum;
//QString m_name;
//QHash<QString,int> m_myProduct;

QSharedPointer<Customer> Customer::fromJson(const QJsonObject& obj) {
    int id = obj["id"].toInt();
    QString name = obj["name"].toString();
    int phonenum = obj["phonenum"].toInt();

    auto customer = QSharedPointer<Customer>::create(id, name, phonenum);

    if(obj.contains("myproduct")&&obj["myproduct"].isObject()){
        QJsonObject productObj=obj["myproduct"].toObject();
        for (auto it = productObj.begin(); it != productObj.end(); ++it) {
            customer->getProduct()[it.key()] = it.value().toInt();
        }
    }
    return customer;
}
