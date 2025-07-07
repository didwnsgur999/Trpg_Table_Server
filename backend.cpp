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
void Backend::deleteProduct(int id){
    for(int i=0; i<productList.size(); i++){
        if(productList[i]->getId()==id){
            productList.removeAt(i);
            break;
        }
    }
}
void Backend::deleteCustomer(int id){
    for(int i=0; i<customerList.size(); i++){
        if(customerList[i]->getId()==id){
            customerList.removeAt(i);
            break;
        }
    }
}
void Backend::deleteOrder(int id){
    for(int i=0; i<orderList.size(); i++){
        if(orderList[i]->getId()==id){
            orderList.removeAt(i);
            break;
        }
    }
}

const QVector<QSharedPointer<Product>> &Backend::getProductList() const
{
    return productList;
}
