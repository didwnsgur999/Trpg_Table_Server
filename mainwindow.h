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
    MainWindow(Backend* backend,QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ProductDebug_clicked();

    void on_serverOpenButton_clicked();

    void on_serverCloseButton_clicked();

private:
    Backend* m_backend;
    Ui::MainWindow *ui;

    std::unique_ptr<ServerChat> m_serverChat;
};
#endif // MAINWINDOW_H
