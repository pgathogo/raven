#include "selectclosewidget.h"
#include "ui_selectclosewidget.h"

SelectCloseWidget::SelectCloseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectCloseWidget)
{
    ui->setupUi(this);
}

SelectCloseWidget::~SelectCloseWidget()
{
    delete ui;
}

QPushButton* SelectCloseWidget::selectBtn()
{
    return ui->btnSelect;
}

QPushButton* SelectCloseWidget::closeBtn()
{
    return ui->btnClose;
}
