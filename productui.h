#ifndef PRODUCTUI_H
#define PRODUCTUI_H

#include <QStandardItemModel>
#include <QWidget>
#include "product.h"
class QFile;
class QProgressDialog;

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
    QStandardItemModel *getTableMain() { return m_tableMain; }

    //productTable 정렬 및 디스플레이;
    void loadProductTable(QStandardItemModel *table,
                          std::function<bool(const QSharedPointer<Product> &)> filter = nullptr);
    std::function<bool(const QSharedPointer<Product> &)> makeProductSearchFilter(
        const QString &attribute, const QString &token);


    //사진 가져오기, 디스플레이용 함수
    QFile *file;
    QString fileName;
    QString encodePixmapToBase64(const QPixmap& pixmap);

private slots:
    void on_AddButton_clicked();

    void on_toolBox_currentChanged(int index);

    void on_SearchButton_clicked();

    void on_DeleteButton_clicked();

    void on_ImageButton_clicked();

    void on_MainTableView_entered(const QModelIndex &index);



private:
    Ui::ProductUI *ui;
    //table 관리 모델
    QStandardItemModel *m_tableMain;
    QStandardItemModel *m_tableSearch;
    QPixmap m_selectedImage;

    //openfile
    void openFile();
};

#endif // PRODUCTUI_H
