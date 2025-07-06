#include "chatroomui.h"
#include "ui_chatroomui.h"

ChatRoomUI::ChatRoomUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatRoomUI)
{
    ui->setupUi(this);
}

ChatRoomUI::~ChatRoomUI()
{
    delete ui;
}
