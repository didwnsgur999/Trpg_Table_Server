#ifndef PRODUCTUI_H
#define PRODUCTUI_H

#include <QWidget>
#include <QStandardItemModel>
#include "product.h"
namespace Ui {
class ProductUI;
}

class ProductUI : public QWidget
{
    Q_OBJECT

public:
    explicit ProductUI(QWidget *parent = nullptr);
    ~ProductUI();
    //getter,setter
    QStandardItemModel* getTableMain(){ return m_tableMain; }

    //productTable 정렬 및 디스플레이;
    void loadProductTable(QStandardItemModel* table, std::function<bool(const QSharedPointer<Product>&)> filter=nullptr);
    std::function<bool(const QSharedPointer<Product>&)> makeProductSearchFilter(const QString& attribute, const QString& token);

private slots:
    void on_AddButton_clicked();

    void on_toolBox_currentChanged(int index);

    void on_SearchButton_clicked();

    void on_DeleteButton_clicked();

private:
    Ui::ProductUI *ui;
    //table 관리 모델
    QStandardItemModel* m_tableMain;
    QStandardItemModel* m_tableSearch;
};

#endif // PRODUCTUI_H
