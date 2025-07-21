#include "chatroomui.h"
#include "ui_chatroomui.h"
#include <QListView>
#include "mycore/roommanager.h"

ChatRoomUI::ChatRoomUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatRoomUI)
{
    ui->setupUi(this);
    m_roomListView = new QStandardItemModel(this);
    ui->RoomListView->setModel(m_roomListView);
}

ChatRoomUI::~ChatRoomUI()
{
    delete ui;
}

void ChatRoomUI::addAllRoom() {
    auto vec = RoomManager::getInstance().getAllRoom();
    m_roomListView->clear();
    for(auto i:vec){
        QString text = QString("%1 || 인원 : %2").arg(i->getRName(),QString::number(i->getRCnt()));
        QStandardItem* item = new QStandardItem(text);
        m_roomListView->appendRow(item);
    }
}

void ChatRoomUI::on_debugButton_clicked()
{
    ChatRoomUI::addAllRoom();
}

