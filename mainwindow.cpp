#include "mainwindow.h"
#include <QDir>            //Directory
#include "backend.h"       //data
#include "serverchat.h"    //for chatserver open,close
#include "ui_mainwindow.h" //ui
#include <memory.h>        //uniqueptr

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 싱글턴 서버 열기 실패시 안된다고 말함.
    if (!ServerChat::getInstance().OpenServer()) {
        qDebug() << "unable to open server\n";
    } else {
        this->setWindowTitle(tr("서버 포트번호: %1").arg(ServerChat::getInstance().getPort()));
    }

    // UI 생성
    m_customerUI = new CustomerUI(this);
    m_productUI = new ProductUI(this);
    m_orderUI = new OrderUI(this);
    m_chatRoomUI = new ChatRoomUI(this);
    m_debugUI = new DebugUI(this);

    // 스택에 추가 먼저 넣은 순서대로 0, 1, 2... 이다.
    ui->MainTabWidget->addTab(m_customerUI, tr("고객 정보"));
    ui->MainTabWidget->addTab(m_productUI, tr("제품 정보"));
    ui->MainTabWidget->addTab(m_orderUI, tr("주문 정보"));
    ui->MainTabWidget->addTab(m_chatRoomUI,tr("채팅방 확인"));
    ui->MainTabWidget->addTab(m_debugUI,tr("디버그"));
    // 첫 화면은 고객정보
    ui->MainTabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    //종료시 저장
    Backend::getInstance().saveProductJson("products.json");
    Backend::getInstance().saveCustomerJson("customers.json");
    Backend::getInstance().saveOrderJson("orders.json");
    delete ui;
}

void MainWindow::on_MainTabWidget_currentChanged(int index)
{
    switch (index) {
    case 0:
        m_customerUI->loadCustomerTable(m_customerUI->getTableMain());
        //m_customerUI
        break;
    case 1:
        m_productUI->loadProductTable(m_productUI->getTableMain());
        break;
    case 2:
        //m_orderUI
        m_orderUI->loadOrderTable(m_orderUI->getTableMain());
        break;
    case 3:
        //m_ChatRoomUI
        m_chatRoomUI->addAllRoom();
        break;
    case 4:
        //debug
        break;
    }

}
