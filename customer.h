#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "Info.h"
#include <QString>
#include <QHash>
#include <QJsonObject>
#include <QSharedPointer>
class Customer : public Info{
public:
    //고객은 첫 생성시에는 id name 전화번호만 있으면 된다.
    Customer();
    Customer(int id, QString name, int phoneNum);

    //Product를 Json으로 변경하는 함수
    QJsonObject toJson() const ;
    //Json을 Product로 변경하는 함수.
    //외부에서 객체 없이 부르는 경우 사용해야 하므로 static이 붙는다.
    static QSharedPointer<Customer> fromJson(const QJsonObject& obj);

    int getId() const { return m_id; }
    QString getName() const { return m_name; }
    int getPhoneNum() const { return m_phoneNum;}
    QHash<QString,int>& getProduct() { return m_myProduct; }

private:
    //id, 전화번호, 이름, 제품이름,pid만.
    int m_id;
    QString m_name;
    int m_phoneNum;
    QHash<QString,int> m_myProduct;
};

#endif // CUSTOMER_H
