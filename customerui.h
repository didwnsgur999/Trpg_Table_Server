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

private:
    Ui::CustomerUI *ui;
};

#endif // CUSTOMERUI_H
