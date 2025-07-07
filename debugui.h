#ifndef DEBUGUI_H
#define DEBUGUI_H

#include <QWidget>

namespace Ui {
class DebugUI;
}

class DebugUI : public QWidget
{
    Q_OBJECT

public:
    explicit DebugUI(QWidget *parent = nullptr);
    ~DebugUI();

private:
    Ui::DebugUI *ui;
};

#endif // DEBUGUI_H
