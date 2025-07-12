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
const QVector<QSharedPointer<Customer>> &Backend::getCustomerList() const
{
    return customerList;
}
const QVector<QSharedPointer<Order>> &Backend::getOrderList() const
{
    return orderList;
}
QSharedPointer<Product> Backend::searchProductName(QString Name){
    for(int i=0; i<productList.size(); i++){
        if(productList[i]->getName()==Name){
            return productList[i];
        }
    }
    return nullptr;
}

QSharedPointer<Customer> Backend::searchCustomerLogin(QString name, QString pwd){
    for(int i=0; i<customerList.size(); i++){
        if(customerList[i]->getName()==name&&customerList[i]->getPwd()==pwd){
            return customerList[i];
        }
    }
    return nullptr;
}
void Backend::setMaxCusId(){
    if(customerList.empty()){
        MAX_CID=1000;
        return;
    }
    for(auto i:customerList){
        if(i->getId()>MAX_CID) MAX_CID=i->getId();
    }
}
void Backend::setMaxProId(){
    if(productList.empty()){
        MAX_PID=3000;
        return;
    }
    for(auto i:productList){
        if(i->getId()>MAX_PID) MAX_PID=i->getId();
    }
}
void Backend::setMaxOrdId(){
    if(orderList.empty()){
        MAX_OID=5000;
        return;
    }
    for(auto i:orderList){
        if(i->getId()>MAX_OID) MAX_OID=i->getId();
    }
}
