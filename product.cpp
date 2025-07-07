#include "product.h"
#include <QByteArray>
#include <QBuffer>
//for QSharedptr
Product::Product()
    : m_id(0)
    , m_name("")
    , m_price(0)
    , m_cnt(0)
{}

Product::Product(int id, QString name, int price, int cnt)
    : m_id(id)
    , m_name(name)
    , m_price(price)
    , m_cnt(cnt)
{}

QJsonObject Product::toJson() const
{
    QJsonObject obj;
    obj["id"] = getId();
    obj["name"] = getName();
    obj["price"] = getPrice();
    obj["cnt"] = getCnt();
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);

    if(!m_image.isNull()){
        m_image.save(&buffer, "PNG");
        obj["image"] = QString::fromUtf8(byteArray.toBase64());
    }
    return obj;
}

QSharedPointer<Product> Product::fromJson(const QJsonObject &obj)
{
    auto newproduct = QSharedPointer<Product>::create(obj["id"].toInt(),
                                                      obj["name"].toString(),
                                                      obj["price"].toInt(),
                                                      obj["cnt"].toInt());

    if(obj.contains("image")){
        QString base64Str = obj["image"].toString();
        QByteArray imageData = QByteArray::fromBase64(base64Str.toUtf8());
        QPixmap pixmap;
        if(pixmap.loadFromData(imageData)){
            newproduct->setImage(pixmap);
        }else{
            qWarning() << "Failed to load pixmap from Base64";
        }
    }
    return newproduct;
}

void Product::setImage(const QPixmap& image){
    if(!image.isNull())
        m_image = image;
}

QPixmap Product::getImage() const {
    return m_image;
}
