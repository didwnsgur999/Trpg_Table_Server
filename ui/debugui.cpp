#include "debugui.h"
#include "ui_debugui.h"
#include "mycore/Backend.h"

DebugUI::DebugUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DebugUI)
{
    ui->setupUi(this);
    m_productListModel = new QStringListModel(this);
    ui->listView->setModel(m_productListModel);
    //graphicScene에 add로 넣는다.
    scene = new QGraphicsScene(this);
    ui->myGraphicsView->setScene(scene);
    connect(ui->myGraphicsView, &MyGraphicsView::itemDeleted, this, [=](const QString &key){
        imageItems.remove(key);
    });
}

DebugUI::~DebugUI()
{
    delete ui;
}

void DebugUI::on_reloadButton_clicked()
{
    loadProductList();
}

void DebugUI::loadProductList()
{
    QStringList productNames;
    auto productList = Backend::getInstance().getProductList();
    for (const auto& p : productList) {
        productNames << p->getName();  // 또는 ID 등
    }
    m_productListModel->setStringList(productNames);
}

void DebugUI::on_listView_doubleClicked(const QModelIndex &index)
{
    QString productName = m_productListModel->data(index, Qt::DisplayRole).toString();

    // productName으로 product 객체 가져오기
    auto product = Backend::getInstance().searchProductName(productName);
    if (!product) return;
    //product에서 image 뽑기.
    QPixmap pixmap = product->getImage();

    // QGraphicsScene 생성 및 설정
    addProductImage(productName,pixmap);
}

void DebugUI::addProductImage(const QString& productName, const QPixmap& pixmap)
{
    if (imageItems.contains(productName)) return;  // 중복 추가 방지

    auto *item = scene->addPixmap(pixmap.scaled(150, 150));
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    item->setFlag(QGraphicsItem::ItemIsSelectable);

    item->setData(0, productName);  // 추후 추적용

    // 위치는 랜덤 또는 기본
    item->setPos(QPointF(0, 0));

    imageItems.insert(productName, item);
}
