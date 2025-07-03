#ifndef BACKEND_H
#define BACKEND_H
#include <QObject>
#include <QVector>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include "product.h"
#include "customer.h"
#include "order.h"
#include <QSharedPointer>

class Backend : QObject
{
    Q_OBJECT
public:
    //backend도 싱글턴으로 만든다.
    static Backend& getInstance() {
        static Backend instance;
        return instance;
    }

    void addProduct(QSharedPointer<Product> prod);

    const QVector<QSharedPointer<Product>>& getProductList() const;
    void setProductList(const QVector<QSharedPointer<Product>>& list) { productList = list; }

    //save json
    bool saveProductJson(const QString& filename) {
        return saveJson<Product>(filename, productList);
    }
    bool saveCustomerJson(const QString& filename) {
        return saveJson<Customer>(filename, customerList);
    }
    bool saveOrderJson(const QString& filename) {
        return saveJson<Order>(filename, orderList);
    }

    //load json
    bool loadProductJson(const QString& filename)
    {
        return loadJson<Product>(filename,productList,[](const QJsonObject& obj) { return Product::fromJson(obj); });
    }
    bool loadCustomerJson(const QString& filename)
    {
        return loadJson<Customer>(filename,customerList,[](const QJsonObject& obj) { return Customer::fromJson(obj); });
    }
    bool loadOrderJson(const QString& filename)
    {
        return loadJson<Order>(filename,orderList,[](const QJsonObject& obj) { return Order::fromJson(obj); });
    }

    //===================================//
    // to save current Vector to Jsonfile
    //===================================//
    template<typename T>
    bool saveJson(const QString& filename, const QVector<QSharedPointer<T>>& data) {
        QJsonArray array;
        for (const auto& d : data)
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
    bool loadJson(const QString& filename, QVector<QSharedPointer<T>>& dest,
                  std::function<QSharedPointer<T>(const QJsonObject&)> factory) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
            return false;

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray array = doc.array();

        for (const auto& val : array)
            dest.push_back(factory(val.toObject()));

        return true;
    }

    //===========================================//
    //  show current list
    //===========================================//
    void ShowProductList(){
        for(int i=0; i<productList.size(); i++){
            QSharedPointer<Product> now=productList[i];
            qDebug()<<"product: "<<now->getId()<<now->getName()<<now->getPrice()<<now->getCnt();
        }
    }
    void ShowCustomerList(){
        for(int i=0; i<customerList.size(); i++){
            QSharedPointer<Customer> now=customerList[i];
            qDebug()<<"customer: "<<now->getId()<<now->getName()<<now->getPwd();
            for (auto it = now->getProduct().begin(); it != now->getProduct().end(); ++it) {
                qDebug() << " - " << it.key() << ":" << it.value();
            }
        }
    }
    void ShowOrderList(){
        for(int i=0; i<orderList.size(); i++){
            QSharedPointer<Order> now=orderList[i];
            qDebug()<<"order: "<<now->getId()<<now->getCID()<<now->getPID()<<now->getCnt();
        }
    }

private:
    Backend(QObject* parent = nullptr);
    Backend(const Backend& ) {}
    Backend& operator=(const Backend& ) {}
    ~Backend() {}
    QVector<QSharedPointer<Product>> productList;
    QVector<QSharedPointer<Customer>> customerList;
    QVector<QSharedPointer<Order>> orderList;
};

#endif // BACKEND_H
