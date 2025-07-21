#ifndef ORDERUI_H
#define ORDERUI_H

#include <QWidget>
#include <QStandardItemModel>
#include "mycore/order.h"

namespace Ui {
class OrderUI;
}

class OrderUI : public QWidget
{
    Q_OBJECT

public:
    explicit OrderUI(QWidget *parent = nullptr);
    ~OrderUI();

    QStandardItemModel *getTableMain(){ return m_tableMain; }
    void loadOrderTable(QStandardItemModel *table,
                        std::function<bool(const QSharedPointer<Order> &)> filter = nullptr);
                        std::function<bool(const QSharedPointer<Order> &)> makeOrderSearchFilter(
                        const QString &attribute, const QString &token);

private slots:
    void on_DeleteButton_clicked();

    void on_OToolBox_currentChanged(int index);

    void on_AddButton_clicked();

private:
    Ui::OrderUI *ui;
    QStandardItemModel *m_tableMain;
    QStandardItemModel *m_tableSearch;
};

#endif // ORDERUI_H
