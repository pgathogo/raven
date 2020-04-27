#ifndef SELECTCLOSEWIDGET_H
#define SELECTCLOSEWIDGET_H

#include <QWidget>

namespace Ui {
class SelectCloseWidget;
}

class SelectCloseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SelectCloseWidget(QWidget *parent = nullptr);
    ~SelectCloseWidget();

private:
    Ui::SelectCloseWidget *ui;
};

#endif // SELECTCLOSEWIDGET_H
