#include "productui.h"
#include "ui_productui.h"

#include "mycore/backend.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSharedPointer>
#include <QToolTip>
#include <QBuffer>

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
    ui->PToolBox->setCurrentIndex(0);
}

ProductUI::~ProductUI()
{
    delete ui;
}
//add
void ProductUI::on_AddButton_clicked()
{
    //새로운 product생성
    int id = ui->PIDLineEdit->text().toInt();
    auto product = Backend::getInstance().searchProductId(id);
    if(id<=3000||id>=5000) {
        QMessageBox::warning(this,tr("바운더리"),tr("id를 3000~5000를 사용해주세요."));
        return;
    }
    if(product!=nullptr){
        QMessageBox::warning(this,tr("변경"),tr("기존 제품의 상태를 변경합니다."));
        product->setId(ui->PIDLineEdit->text().toInt());
        product->setName(ui->NameLineEdit->text());
        product->setPrice(ui->PriceLineEdit->text().toInt());
        product->setCnt(ui->CntLineEdit->text().toInt());
        product->setImage(m_selectedImage);
    }else{
        QSharedPointer<Product> newProduct = QSharedPointer<Product>::create();
        newProduct->setId(ui->PIDLineEdit->text().toInt());
        newProduct->setName(ui->NameLineEdit->text());
        newProduct->setPrice(ui->PriceLineEdit->text().toInt());
        newProduct->setCnt(ui->CntLineEdit->text().toInt());
        newProduct->setImage(m_selectedImage);
        Backend::getInstance().addProduct(newProduct);
    }
    loadProductTable(m_tableMain);
}
//remove
void ProductUI::on_DeleteButton_clicked() {
    Backend::getInstance().deleteProduct(ui->PIDLineEdit->text().toInt());
    loadProductTable(m_tableMain);
}

// 반환값 = bool, 인자 = QSharedPointer<Product>& 인 filter를 통해 가져온 backend데이터를 걸러서 보여주는 함수.
void ProductUI::loadProductTable(QStandardItemModel *table,
                                 std::function<bool(const QSharedPointer<Product> &)> filter)
{
    //항상 backend로부터 최신본을 받아온다, 테이블 모델 초기화
    const QVector<QSharedPointer<Product>> &temp = Backend::getInstance().getProductList();
    table->setRowCount(0);

    int row = 0;
    //제품번호 제품명, 가격, 수량 넣는다. QStandardItem은 QString만 받는다.
    for (const auto &cur : temp) {
        //필터 있으면 필터 통과해야 보여준다.
        if (filter && !filter(cur))
            continue;
        table->insertRow(row);
        table->setItem(row, 0, new QStandardItem(QString::number(cur->getId())));
        table->setItem(row, 1, new QStandardItem(cur->getName()));
        table->setItem(row, 2, new QStandardItem(QString::number(cur->getPrice())));
        table->setItem(row, 3, new QStandardItem(QString::number(cur->getCnt())));
        row++;
    }
    if (table == m_tableMain)
        ui->MainTableView->setModel(table);
    else
        ui->SearchTableView->setModel(table);
}

//search 탭에서 사용하는 함수들
void ProductUI::on_PToolBox_currentChanged(int index)
{
    if (index == 1) {
        loadProductTable(m_tableSearch);
    }
}

void ProductUI::on_SearchButton_clicked()
{
    //filter 생성
    QString attribute = ui->SearchComboBox->currentText();
    QString token = ui->SearchLineEdit->text();
    auto filter = makeProductSearchFilter(attribute, token);
    loadProductTable(m_tableSearch, filter);
}

//서치용 필터 만드는 함수.
std::function<bool(const QSharedPointer<Product> &)> ProductUI::makeProductSearchFilter(
    const QString &attribute, const QString &token)
{
    return [attribute, token](const QSharedPointer<Product> &p) -> bool {
        if (attribute == "id") {
            return p->getId() == token.toInt(); //동일 아이디 찾기.
        } else if (attribute == "name") {
            return p->getName().contains(token, Qt::CaseInsensitive); //동일 이름 찾기
        } else if (attribute == "price") {
            return p->getPrice() == token.toInt(); // 동일 가격 찾기
        } else if (attribute == "cnt") {
            return p->getCnt() == token.toInt(); // 동일 갯수 찾기 -> 추후 위아래로 변경가능.
        }
        return false;
    };
}

void ProductUI::on_ImageButton_clicked()
{
    openFile();
}

void ProductUI::openFile(){
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("이미지 선택"),
                                            "",
                                            tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    if(fileName.isEmpty())
        return;
    QPixmap pixmap;
    if(!pixmap.load(fileName)){
        QMessageBox::warning(this, tr("오류"), tr("이미지를 불러올 수 없습니다."));
        return;
    }
    m_selectedImage = pixmap;
    ui->ImageLabel->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio));
}

void ProductUI::on_MainTableView_entered(const QModelIndex &index)
{
    qDebug()<<"MainTable entered";
    if(!index.isValid())
        return;
    int row = index.row();
    int id= index.model()->data(index.model()->index(row,0)).toString().toInt();
    const QVector<QSharedPointer<Product>> &list = Backend::getInstance().getProductList();
    for(const auto& prod:list){
        if(prod->getId()==id){
            const QPixmap pix = prod->getImage();
            if(!pix.isNull()){
                qDebug()<<"pix";
                QPixmap pixPreview = pix.scaled(200,200,Qt::KeepAspectRatio);
                QToolTip::showText(QCursor::pos(),
                                   "<img src='data:image/png;base64," + encodePixmapToBase64(pixPreview) + "'>",
                                   ui->MainTableView);
            }
            break;
        }
    }
}
QString ProductUI::encodePixmapToBase64(const QPixmap& pixmap){
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);

    // PNG 형식으로 QPixmap을 QByteArray로 저장
    pixmap.save(&buffer, "PNG");

    // Base64 인코딩 후 QString으로 반환
    return QString::fromUtf8(byteArray.toBase64());
}


