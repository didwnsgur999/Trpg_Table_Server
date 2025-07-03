#include "productui.h"
#include "ui_productui.h"

#include "backend.h"
#include "Product.h"

#include <QSharedPointer>

ProductUI::ProductUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProductUI)
{
    ui->setupUi(this);
    //table모델 생성
    m_tableMain = new QStandardItemModel(0, 4, this);
    m_tableMain->setHorizontalHeaderLabels({tr("제품번호"), tr("제품명"), tr("가격"), tr("수량")});
    m_tableSearch = new QStandardItemModel(0, 4, this);
    m_tableSearch->setHorizontalHeaderLabels({tr("제품번호"), tr("제품명"), tr("가격"), tr("수량")});
}

ProductUI::~ProductUI()
{
    delete ui;
}
//add/remove
void ProductUI::on_AddButton_clicked()
{
    //새로운 product생성
    QSharedPointer<Product> newProduct = QSharedPointer<Product>::create();
    //설정후 집어넣는다.
    newProduct->setId(ui->PIDLineEdit->text().toInt());
    newProduct->setName(ui->NameLineEdit->text());
    newProduct->setPrice(ui->PriceLineEdit->text().toInt());
    newProduct->setCnt(ui->PIDLineEdit->text().toInt());
    Backend::getInstance().addProduct(newProduct);
    loadProductTable(m_tableMain);
}
// 반환값 = bool, 인자 = QSharedPointer<Product>& 인 filter를 통해 가져온 backend데이터를 걸러서 보여주는 함수.
void ProductUI::loadProductTable(QStandardItemModel* table,std::function<bool(const QSharedPointer<Product>&)> filter){
    //항상 backend로부터 최신본을 받아온다, 테이블 모델 초기화
    const QVector<QSharedPointer<Product>>& temp = Backend::getInstance().getProductList();
    table->setRowCount(0);

    int row=0;
    //제품번호 제품명, 가격, 수량 넣는다. QStandardItem은 QString만 받는다.
    for(const auto& cur:temp){
        //필터 있으면 필터 통과해야 보여준다.
        if(filter&&!filter(cur)) continue;
        table->insertRow(row);
        table->setItem(row,0,new QStandardItem(QString::number(cur->getId())));
        table->setItem(row,1,new QStandardItem(cur->getName()));
        table->setItem(row,2,new QStandardItem(QString::number(cur->getPrice())));
        table->setItem(row,3,new QStandardItem(QString::number(cur->getCnt())));
        row++;
    }
    if(table==m_tableMain)
        ui->MainTableView->setModel(table);
    else
        ui->SearchTableView->setModel(table);
}

//search 탭에서 사용하는 함수들
void ProductUI::on_toolBox_currentChanged(int index)
{
    if(index==1){
        loadProductTable(m_tableSearch);
    }
}

void ProductUI::on_SearchButton_clicked()
{
    //filter 생성
    QString attribute = ui->SearchComboBox->currentText();
    QString token = ui->SearchLineEdit->text();
    auto filter=makeProductSearchFilter(attribute,token);
    loadProductTable(m_tableSearch,filter);
}
//서치용 필터 만드는 함수.
std::function<bool(const QSharedPointer<Product>&)> ProductUI::makeProductSearchFilter(const QString& attribute, const QString& token) {
    return [attribute, token](const QSharedPointer<Product>& p) -> bool {
        if (attribute == "id") {
            return p->getId()==token.toInt(); //동일 아이디 찾기.
        } else if (attribute == "name") {
            return p->getName().contains(token, Qt::CaseInsensitive); //동일 이름 찾기
        } else if (attribute == "price") {
            return p->getPrice()==token.toInt(); // 동일 가격 찾기
        } else if (attribute == "cnt") {
            return p->getCnt()==token.toInt();  // 동일 갯수 찾기 -> 추후 위아래로 변경가능.
        }
        return false;
    };
}
