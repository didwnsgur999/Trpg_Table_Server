#ifndef CUSTOMERUI_H
#define CUSTOMERUI_H

#include <QWidget>

namespace Ui {
class CustomerUI;
}

class CustomerUI : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerUI(QWidget *parent = nullptr);
    ~CustomerUI();

private slots:

    void on_AddButton_clicked();

    void on_DeleteButton_clicked();

private:
    Ui::CustomerUI *ui;
};

#endif // CUSTOMERUI_H
