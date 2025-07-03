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
