#ifndef CHATROOMUI_H
#define CHATROOMUI_H

#include <QWidget>
#include <QListWidgetItem>
#include <QDir>

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
    void loadLogFiles(const QString &path);
    const QString& getPath() const {return m_logPath;}

private slots:
    void on_LogLoadButton_clicked();

    void on_LogListWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::ChatRoomUI *ui;
    QString m_logPath;
};

#endif // CHATROOMUI_H
