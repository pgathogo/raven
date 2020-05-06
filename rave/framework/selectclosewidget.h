#ifndef SELECTCLOSEWIDGET_H
#define SELECTCLOSEWIDGET_H

#include <QWidget>

class QPushButton;

namespace Ui {
class SelectCloseWidget;
}

class SelectCloseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SelectCloseWidget(QWidget *parent = nullptr);
    ~SelectCloseWidget();

    QPushButton* selectBtn();

private:
    Ui::SelectCloseWidget *ui;
};

#endif // SELECTCLOSEWIDGET_H
