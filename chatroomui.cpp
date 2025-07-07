#include "chatroomui.h"
#include "ui_chatroomui.h"
#include <QListView>
#include "roommanager.h"

#define MAX 10
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
    for(auto i:vec){
        QString text = QString("%1 || 인원 : %2)").arg(i->getRName(),i->getRCnt());
        QStandardItem* item = new QStandardItem(text);
        m_roomListView->appendRow(item);
    }
}
