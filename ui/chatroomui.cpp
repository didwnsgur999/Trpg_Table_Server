#include "chatroomui.h"
#include "ui_chatroomui.h"
#include <QListView>
#include <QFile>
#include <QTextStream>
#include "mycore/roommanager.h"

ChatRoomUI::ChatRoomUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatRoomUI)
{
    ui->setupUi(this);
    m_logPath = "./logs/";
    ui->logTextBrowser->setMaximumWidth(0);
}

ChatRoomUI::~ChatRoomUI()
{
    delete ui;
}

void ChatRoomUI::loadLogFiles(const QString &path){
    ui->LogListWidget->clear();
    //파일 경로 설정.
    QDir dir(path);
    if(!dir.exists()){
        //경로오류
        qDebug()<<QDir::currentPath();
        return;
    }
    //필터 설정 로그파일만 가져오기
    QStringList filter;
    filter<< "*.log";
    //path에서 파일을 가져온다.
    QFileInfoList fileList = dir.entryInfoList(filter,QDir::Files);
    for(const auto& file :fileList){
        ui->LogListWidget->addItem(file.fileName());
    }
    m_logPath = path;
}

// void ChatRoomUI::addAllRoom() {
//     auto vec = RoomManager::getInstance().getAllRoom();
//     ui->LogListWidget->clear();
//     for(auto i:vec){
//         QString text = QString("%1 || 인원 : %2").arg(i->getRName(),QString::number(i->getRCnt()));
//         QStandardItem* item = new QStandardItem(text);
//         ui->LogListWidget->addItem(item);
//     }
// }

void ChatRoomUI::on_LogLoadButton_clicked()
{
    ChatRoomUI::loadLogFiles(m_logPath);
}
//로그 열기
void ChatRoomUI::on_LogListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString filePath = m_logPath + "/" + item->text();
    ui->logTextBrowser->setMaximumWidth(500);
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        ui->logTextBrowser->setPlainText(in.readAll());
        file.close();
        ui->logTextBrowser->moveCursor(QTextCursor::End);
    } else {
        ui->logTextBrowser->setPlainText("파일을 열 수 없습니다: " + filePath);
    }
}

