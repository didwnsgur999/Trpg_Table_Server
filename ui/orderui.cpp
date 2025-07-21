#include "orderui.h"
#include "ui_orderui.h"

#include "mycore/backend.h"
#include <QSharedPointer>
#include <QMessageBox>

OrderUI::OrderUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OrderUI)
{
    ui->setupUi(this);
    m_tableMain = new QStandardItemModel(0, 4, this);
    m_tableMain->setHorizontalHeaderLabels({tr("주문ID"),tr("고객ID"), tr("제품ID"), tr("제품 수량")});
    m_tableSearch = new QStandardItemModel(0, 4, this);
    m_tableSearch->setHorizontalHeaderLabels({tr("주문ID"),tr("고객ID"), tr("제품ID"), tr("제품 수량")});
    ui->OToolBox->setCurrentIndex(0);
}

OrderUI::~OrderUI()
{
    delete ui;
}

void OrderUI::on_DeleteButton_clicked()
{
    Backend::getInstance().deleteOrder(ui->IDLineEdit->text().toInt());
    loadOrderTable(m_tableMain);
}

void OrderUI::loadOrderTable(QStandardItemModel *table,
                            std::function<bool(const QSharedPointer<Order> &)> filter)
{
    //항상 backend로부터 최신본을 받아온다, 테이블 모델 초기화
    const QVector<QSharedPointer<Order>> &temp = Backend::getInstance().getOrderList();
    table->setRowCount(0);

    int row = 0;
    //고객번호 고객명, 고객비밀번호 넣는다. QStandardItem은 QString만 받는다.
    for (const auto &cur : temp) {
        //필터 있으면 필터 통과해야 보여준다.
        if (filter && !filter(cur))
            continue;
        table->insertRow(row);
        table->setItem(row, 0, new QStandardItem(QString::number(cur->getId())));
        table->setItem(row, 1, new QStandardItem(QString::number(cur->getPID())));
        table->setItem(row, 2, new QStandardItem(QString::number(cur->getCID())));
        table->setItem(row, 3, new QStandardItem(QString::number(cur->getCnt())));
        row++;
    }
    if (table == m_tableMain)
        ui->MainTableView->setModel(table);
    else
        ui->SearchTableView->setModel(table);
}

std::function<bool(const QSharedPointer<Order> &)> OrderUI::makeOrderSearchFilter(
    const QString &attribute, const QString &token)
{
    return [attribute, token](const QSharedPointer<Order> &p) -> bool {
        if (attribute == "oid") {
            return p->getId() == token.toInt(); //동일 아이디 찾기.
        } else if (attribute == "pid") {
            return p->getPID()== token.toInt(); //동일 이름 찾기
        } else if (attribute == "cid") {
            return p->getCID()== token.toInt(); //동일 이름 찾기
        } else if (attribute == "pcnt") {
            return p->getCnt()== token.toInt(); //동일 이름 찾기
        }
        return false;
    };
}

void OrderUI::on_OToolBox_currentChanged(int index)
{
    if (index == 1) {
        loadOrderTable(m_tableSearch);
    }
}

void OrderUI::on_AddButton_clicked()
{
    int id = ui->IDLineEdit->text().toInt();
    if(id<5000) {
        QMessageBox::warning(this,tr("바운더리"),tr("id를 5000이상으로 사용해주세요."));
        return;
    }
    auto order = Backend::getInstance().searchOrderId(id);
    if(order != NULL){
        QMessageBox::warning(this,tr("변경"),tr("기존 주문의 상태를 변경합니다."));
        order->setId(ui->IDLineEdit->text().toInt());
        order->setPID(ui->ProductLineEdit->text().toInt());
        order->setCID(ui->CustomerLineEdit->text().toInt());
        order->setCnt(ui->ProductCntLineEdit->text().toInt());
    } else {
        QSharedPointer<Order> newOrder = QSharedPointer<Order>::create();
        newOrder->setId(ui->IDLineEdit->text().toInt());
        newOrder->setPID(ui->ProductLineEdit->text().toInt());
        newOrder->setCID(ui->CustomerLineEdit->text().toInt());
        newOrder->setCnt(ui->ProductCntLineEdit->text().toInt());
        Backend::getInstance().addOrder(newOrder);
    }
    loadOrderTable(m_tableMain);
}

