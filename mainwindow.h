#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void on_ProductDebug_clicked();
    //서버 오픈됨.
    void on_serverOpenButton_clicked();
    //종료시 저장해야되고, 다른 client 다 닫아줘야한다.
    void on_serverCloseButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
