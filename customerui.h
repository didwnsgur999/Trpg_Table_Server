#ifndef CUSTOMERUI_H
#define CUSTOMERUI_H

#include <QStandardItemModel>
#include <QWidget>
#include "customer.h"

namespace Ui {
class CustomerUI;
}

class CustomerUI : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerUI(QWidget *parent = nullptr);
    ~CustomerUI();

    QStandardItemModel *getTableMain() { return m_tableMain; }

    //CustomerTable 정렬 및 디스플레이;
    void loadCustomerTable(QStandardItemModel *table,
                            std::function<bool(const QSharedPointer<Customer> &)> filter = nullptr);
                            std::function<bool(const QSharedPointer<Customer> &)> makeCustomerSearchFilter(
                            const QString &attribute, const QString &token);
private slots:

    void on_AddButton_clicked();

    void on_DeleteButton_clicked();

    void on_CToolBox_currentChanged(int index);

    void on_SearchButton_clicked();

private:
    Ui::CustomerUI *ui;
    QStandardItemModel *m_tableMain;
    QStandardItemModel *m_tableSearch;
};

#endif // CUSTOMERUI_H
