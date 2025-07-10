#ifndef BACKEND_H
#define BACKEND_H
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QVector>
#include "customer.h"
#include "order.h"
#include "product.h"

class Backend : QObject
{
    Q_OBJECT
public:
    //backend도 싱글턴으로 만든다.
    static Backend &getInstance()
    {
        static Backend instance;
        return instance;
    }

    void addProduct(QSharedPointer<Product> prod);
    void addCustomer(QSharedPointer<Customer> cust);
    void addOrder(QSharedPointer<Order> ord);
    void deleteProduct(int id);
    void deleteCustomer(int id);
    void deleteOrder(int id);

    //ID 생성 프로토콜 - 나중에 템플릿으로 만들어도 될듯.
    void setMaxCusId();
    void setMaxProId();
    void setMaxOrdId();
    //1000 3000 5000
    int getNewCusId() {return MAX_CID++; }
    int getNewProId() {return MAX_PID++; }
    int getNewOrdId() {return MAX_OID++; }

    QSharedPointer<Product> searchProductName(QString name);
    QSharedPointer<Customer> searchCustomerLogin(QString name, QString pwd);
    QSharedPointer<Customer> searchCustomerId(int cid);
    //QSharedPointer<Order>& searchOrder(int id);

    //getProduct,customer,orderlist
    const QVector<QSharedPointer<Product>> &getProductList() const;
    void setProductList(const QVector<QSharedPointer<Product>> &list) { productList = list; }
    const QVector<QSharedPointer<Customer>> &getCustomerList() const;
    void setCustomerList(const QVector<QSharedPointer<Customer>> &list) { customerList = list; }
    const QVector<QSharedPointer<Order>> &getOrderList() const;
    void setOrderList(const QVector<QSharedPointer<Order>> &list) { orderList = list; }

    //save json
    bool saveProductJson(const QString &filename)
    {
        return saveJson<Product>(filename, productList);
    }
    bool saveCustomerJson(const QString &filename)
    {
        return saveJson<Customer>(filename, customerList);
    }
    bool saveOrderJson(const QString &filename)
    {
        return saveJson<Order>(filename, orderList);
    }

    //load json
    bool loadProductJson(const QString &filename)
    {
        return loadJson<Product>(filename, productList, [](const QJsonObject &obj) {
            return Product::fromJson(obj);
        });
    }
    bool loadCustomerJson(const QString &filename)
    {
        return loadJson<Customer>(filename, customerList, [](const QJsonObject &obj) {
            return Customer::fromJson(obj);
        });
    }
    bool loadOrderJson(const QString &filename)
    {
        return loadJson<Order>(filename, orderList, [](const QJsonObject &obj) {
            return Order::fromJson(obj);
        });
    }

    //===================================//
    // to save current Vector to Jsonfile
    //===================================//
    template<typename T>
    bool saveJson(const QString &filename, const QVector<QSharedPointer<T>> &data)
    {
        QJsonArray array;
        qDebug()<<"데이터 사이즈 : "<<data.size();
        for (const auto &d : data)
            array.append(d->toJson());

        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly))
            return false;

        QJsonDocument doc(array);
        file.write(doc.toJson());
        return true;
    }

    //===================================//
    // to load Jsonfile to current Vector
    //===================================//
    template<typename T>
    bool loadJson(const QString &filename,
                  QVector<QSharedPointer<T>> &dest,
                  std::function<QSharedPointer<T>(const QJsonObject &)> factory)
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
            return false;

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray array = doc.array();

        for (const auto &val : array)
            dest.push_back(factory(val.toObject()));

        return true;
    }

    //===========================================//
    //  show current list for debug
    //===========================================//
    void ShowProductList()
    {
        for (int i = 0; i < productList.size(); i++) {
            QSharedPointer<Product> now = productList[i];
            qDebug() << "product: " << now->getId() << now->getName() << now->getPrice()
                     << now->getCnt();
        }
    }
    void ShowCustomerList()
    {
        for (int i = 0; i < customerList.size(); i++) {
            QSharedPointer<Customer> now = customerList[i];
            qDebug() << "customer: " << now->getId() << now->getName() << now->getPwd();
            for (auto it = now->getProduct().begin(); it != now->getProduct().end(); ++it) {
                qDebug() << " - " << it.key() << ":" << it.value();
            }
        }
    }
    void ShowOrderList()
    {
        for (int i = 0; i < orderList.size(); i++) {
            QSharedPointer<Order> now = orderList[i];
            qDebug() << "order: " << now->getId() << now->getCID() << now->getPID()
                     << now->getCnt();
        }
    }

private:
    Backend(QObject *parent = nullptr);
    Backend(const Backend &) {}
    Backend &operator=(const Backend &) {}
    ~Backend() {}
    QVector<QSharedPointer<Product>> productList;
    QVector<QSharedPointer<Customer>> customerList;
    QVector<QSharedPointer<Order>> orderList;
    int MAX_CID;
    int MAX_PID;
    int MAX_OID;
};

#endif // BACKEND_H
