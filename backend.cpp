#include "Backend.h"

Backend::Backend(QObject* parent) :QObject(parent){}

void Backend::addProduct(QSharedPointer<Product> prod) {
    productList.push_back(prod);
}

const QVector<QSharedPointer<Product>>& Backend::getProductList() const {
    return productList;
}
