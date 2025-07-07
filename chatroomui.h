#ifndef CHATROOMUI_H
#define CHATROOMUI_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class ChatRoomUI;
}

class ChatRoomUI : public QWidget
{
    Q_OBJECT

public:
    explicit ChatRoomUI(QWidget *parent = nullptr);
    ~ChatRoomUI();

    void addAllRoom();

private:
    Ui::ChatRoomUI *ui;
    QStandardItemModel* m_roomListView;
};

#endif // CHATROOMUI_H
