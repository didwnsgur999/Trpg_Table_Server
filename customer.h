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
    Customer(int id, QString name, int pwd);

    //Product를 Json으로 변경하는 함수
    QJsonObject toJson() const;
    //Json을 Product로 변경하는 함수.
    //외부에서 객체 없이 부르는 경우 사용해야 하므로 static이 붙는다.
    static QSharedPointer<Customer> fromJson(const QJsonObject& obj);

    //Getter Setter
    int getId() const { return m_id; }
    QString getName() const { return m_name; }
    int getPwd() const { return m_pwd;}
    QHash<QString,int>& getProduct() { return m_myProduct; }

private:
    //id, 이름, pwd, 제품이름, pid
    int m_id;
    QString m_name;
    int m_pwd;
    QHash<QString,int> m_myProduct;
};

#endif // CUSTOMER_H
