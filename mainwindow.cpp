#include "mainwindow.h"
#include "ui_mainwindow.h"  //ui
#include <memory.h>         //uniqueptr
#include <QDir>             //Directory
#include "serverchat.h"     //for chatserver open,close
#include "backend.h"        //data

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 싱글턴 서버 열기 실패시 안된다고 말함.
    if(!ServerChat::getInstance().OpenServer()){
        qDebug()<<"unable to open server\n";
    } else {
        this->setWindowTitle(tr("서버 포트번호: %1").arg(ServerChat::getInstance().getPort()));
    }

    // UI 생성
    m_customerUI = new CustomerUI(this);
    m_productUI = new ProductUI(this);

    // 스택에 추가 먼저 넣은 순서대로 0, 1, 2... 이다.
    ui->MainTabWidget->addTab(m_customerUI, tr("고객 정보"));
    ui->MainTabWidget->addTab(m_productUI, tr("제품 정보"));

    // 첫 화면은 고객정보
    ui->MainTabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    //종료시 저장
    Backend::getInstance().saveProductJson("products.json");
    Backend::getInstance().saveCustomerJson("customers.json");
    delete ui;

}

void MainWindow::on_MainTabWidget_currentChanged(int index)
{
    switch(index){
    case 0:

        break;

    case 1:
        m_productUI->loadProductTable(m_productUI->getTableMain());
        break;
    }
}

