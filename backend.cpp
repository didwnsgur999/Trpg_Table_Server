#include "Backend.h"

Backend::Backend(QObject *parent)
    : QObject(parent)
{}

void Backend::addProduct(QSharedPointer<Product> prod)
{
    productList.push_back(prod);
}
void Backend::addCustomer(QSharedPointer<Customer> cust)
{
    customerList.push_back(cust);
}
void Backend::addOrder(QSharedPointer<Order> ord)
{
    orderList.push_back(ord);
}

const QVector<QSharedPointer<Product>> &Backend::getProductList() const
{
    return productList;
}
