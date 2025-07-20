#ifndef DEBUGUI_H
#define DEBUGUI_H

#include <QWidget>
#include <QStringListModel>
#include <QGraphicsScene>
#include <QGraphicsItem>

namespace Ui {
class DebugUI;
}

class DebugUI : public QWidget
{
    Q_OBJECT

public:
    explicit DebugUI(QWidget *parent = nullptr);
    ~DebugUI();

    void loadProductList();
    void addProductImage(const QString& productName, const QPixmap& pixmap);

private slots:
    void on_reloadButton_clicked();

    void on_listView_doubleClicked(const QModelIndex &index);

private:
    Ui::DebugUI *ui;
    //제품 이름같은거 보여줌.
    QStringListModel *m_productListModel;
    QGraphicsScene* scene;
    QHash<QString, QGraphicsPixmapItem*> imageItems;
};

#endif // DEBUGUI_H
