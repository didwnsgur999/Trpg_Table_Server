#include "customerui.h"
#include "ui_customerui.h"

CustomerUI::CustomerUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomerUI)
{
    ui->setupUi(this);
}

CustomerUI::~CustomerUI()
{
    delete ui;
}

void CustomerUI::on_AddButton_clicked()
{

}


void CustomerUI::on_DeleteButton_clicked()
{

}

