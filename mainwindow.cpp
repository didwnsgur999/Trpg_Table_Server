#include "mainwindow.h"
#include "ui_mainwindow.h"  //ui
#include "backend.h"        //data
#include <memory.h>         //uniqueptr
#include <QDir>             //Directory
#include "serverchat.h"     //for chatserver open,close

MainWindow::MainWindow(Backend* backend,QWidget *parent)
    : QMainWindow(parent),m_backend(backend)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ProductDebug_clicked()
{
    if(m_backend->saveJson("products.json", m_backend->getProducts())){
        qDebug() << "Current path:" << QDir::currentPath();
    }

    QVector<QSharedPointer<Product>> loadedProducts;
    if (m_backend->loadJson<Product>("products.json", loadedProducts,
                                     [](const QJsonObject& obj) { return Product::fromJson(obj); }))
    {
        for (const auto& prod : loadedProducts)
            qDebug() << prod->toJson();
    }
}


void MainWindow::on_serverOpenButton_clicked()
{
    if(!m_serverChat){
        m_serverChat = std::make_unique<ServerChat>();
        QString res = m_serverChat->OpenServer();
        ui->StateLabel->setText(res);
    }else{
        ui->StateLabel->setText(tr("already running port %1").arg(m_serverChat->getPort()));
    }
}


void MainWindow::on_serverCloseButton_clicked()
{

}

