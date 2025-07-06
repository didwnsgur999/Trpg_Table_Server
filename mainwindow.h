#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "customerui.h"
#include "productui.h"
#include "orderui.h"
#include "chatroomui.h"
#include <memory.h>

class Backend;
class ServerChat;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_MainTabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    CustomerUI *m_customerUI;
    ProductUI *m_productUI;
    OrderUI *m_orderUI;
    ChatRoomUI *m_chatRoomUI;
};
#endif // MAINWINDOW_H
