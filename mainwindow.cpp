#include "mainwindow.h"
#include "ui_mainwindow.h"  //ui
#include "backend.h"        //data
#include <memory.h>         //uniqueptr
#include <QDir>             //Directory
#include "serverchat.h"     //for chatserver open,close

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
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
    if(Backend::getInstance().saveJson("products.json", Backend::getInstance().getProducts())){
        qDebug() << "Current path:" << QDir::currentPath();
    }

    QVector<QSharedPointer<Product>> loadedProducts;
    if (Backend::getInstance().loadJson<Product>("products.json", loadedProducts,
                                     [](const QJsonObject& obj) { return Product::fromJson(obj); }))
    {
        for (const auto& prod : loadedProducts)
            qDebug() << prod->toJson();
    }
}


void MainWindow::on_serverOpenButton_clicked()
{
    QString res = ServerChat::getInstance().OpenServer();
    ui->StateLabel->setText(res);
}


void MainWindow::on_serverCloseButton_clicked()
{

}

