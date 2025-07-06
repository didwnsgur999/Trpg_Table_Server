#ifndef CHATROOMUI_H
#define CHATROOMUI_H

#include <QWidget>

namespace Ui {
class ChatRoomUI;
}

class ChatRoomUI : public QWidget
{
    Q_OBJECT

public:
    explicit ChatRoomUI(QWidget *parent = nullptr);
    ~ChatRoomUI();

private:
    Ui::ChatRoomUI *ui;
};

#endif // CHATROOMUI_H
