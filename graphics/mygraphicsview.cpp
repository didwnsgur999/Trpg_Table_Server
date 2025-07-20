#include "mygraphicsview.h"
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsItem>

MyGraphicsView::MyGraphicsView(QWidget *parent)
    :QGraphicsView(parent)
{}

void MyGraphicsView::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        QList<QGraphicsItem*> selectedItems = scene()->selectedItems();
        for (QGraphicsItem* item : selectedItems) {
            QString key = item->data(0).toString();
            scene()->removeItem(item);
            delete item;
            //for debugui to delete hash item
            emit itemDeleted(key);
        }
    } else {
        QGraphicsView::keyPressEvent(event);
    }
}
