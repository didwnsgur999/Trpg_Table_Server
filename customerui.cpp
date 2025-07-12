#include "customerui.h"
#include "ui_customerui.h"

#include "backend.h"
#include <QSharedPointer>

CustomerUI::CustomerUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomerUI)
{
    ui->setupUi(this);
    m_tableMain = new QStandardItemModel(0, 3, this);
    m_tableMain->setHorizontalHeaderLabels({tr("고객번호"), tr("고객명"), tr("고객PWD")});
    m_tableSearch = new QStandardItemModel(0, 3, this);
    m_tableSearch->setHorizontalHeaderLabels({tr("고객번호"), tr("고객명"), tr("고객PWD")});
    ui->CToolBox->setCurrentIndex(0);
}

CustomerUI::~CustomerUI()
{
    delete ui;
}

void CustomerUI::on_AddButton_clicked() {
    QSharedPointer<Customer> newCustomer = QSharedPointer<Customer>::create();
    newCustomer->setId(ui->PIDLineEdit->text().toInt());
    newCustomer->setName(ui->NameLineEdit->text());
    newCustomer->setPwd(ui->PwdLineEdit->text());
    qDebug()<<"onAddButtonClicked";
    Backend::getInstance().addCustomer(newCustomer);
    loadCustomerTable(m_tableMain);
}

void CustomerUI::on_DeleteButton_clicked() {
    Backend::getInstance().deleteCustomer(ui->PIDLineEdit->text().toInt());
    loadCustomerTable(m_tableMain);
}
void CustomerUI::loadCustomerTable(QStandardItemModel *table,
                                 std::function<bool(const QSharedPointer<Customer> &)> filter)
{
    //항상 backend로부터 최신본을 받아온다, 테이블 모델 초기화
    const QVector<QSharedPointer<Customer>> &temp = Backend::getInstance().getCustomerList();
    table->setRowCount(0);

    int row = 0;
    //고객번호 고객명, 고객비밀번호 넣는다. QStandardItem은 QString만 받는다.
    for (const auto &cur : temp) {
        //필터 있으면 필터 통과해야 보여준다.
        if (filter && !filter(cur))
            continue;
        table->insertRow(row);
        table->setItem(row, 0, new QStandardItem(QString::number(cur->getId())));
        table->setItem(row, 1, new QStandardItem(cur->getName()));
        table->setItem(row, 2, new QStandardItem(cur->getPwd()));
        row++;
    }
    if (table == m_tableMain)
        ui->MainTableView->setModel(table);
    else
        ui->SearchTableView->setModel(table);
}

void CustomerUI::on_CToolBox_currentChanged(int index)
{
    if (index == 1) {
        loadCustomerTable(m_tableSearch);
    }
}

void CustomerUI::on_SearchButton_clicked()
{
    QString attribute = ui->SearchComboBox->currentText();
    QString token = ui->SearchLineEdit->text();
    auto filter = makeCustomerSearchFilter(attribute, token);
    loadCustomerTable(m_tableSearch, filter);
}

std::function<bool(const QSharedPointer<Customer> &)> CustomerUI::makeCustomerSearchFilter(
    const QString &attribute, const QString &token)
{
    return [attribute, token](const QSharedPointer<Customer> &p) -> bool {
        if (attribute == "id") {
            return p->getId() == token.toInt(); //동일 아이디 찾기.
        } else if (attribute == "name") {
            return p->getName().contains(token, Qt::CaseInsensitive); //동일 이름 찾기
        }
        return false;
    };
}
