#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <QHash>
#include <QJsonObject>
#include <QSharedPointer>
#include <QString>
#include "Info.h"
class Customer : public Info
{
public:
    //고객은 첫 생성시에는 id name 전화번호만 있으면 된다.
    Customer();
    Customer(int id, QString name, QString pwd);

    //Product를 Json으로 변경하는 함수
    QJsonObject toJson() const;
    //Json을 Product로 변경하는 함수.
    //외부에서 객체 없이 부르는 경우 사용해야 하므로 static이 붙는다.
    static QSharedPointer<Customer> fromJson(const QJsonObject &obj);
    static QSharedPointer<Customer> fromJsonEXID(const QJsonObject &obj);
    //Getter Setter
    int getId() const { return m_id; }
    void setId(int id) { m_id=id;}
    QString getName() const { return m_name; }
    void setName(QString name) { m_name=name;}
    QString getPwd() const { return m_pwd; }
    void setPwd(QString pwd) { m_pwd=pwd;}

    QHash<QString, int> &getProduct() { return m_myProduct; }

private:
    //id, 이름, pwd, 제품이름, pid
    int m_id;
    QString m_name;
    QString m_pwd;
    QHash<QString, int> m_myProduct;
};

#endif // CUSTOMER_H
