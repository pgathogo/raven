#include <QTabWidget>
#include "letterfilterwidget.h"
#include "ui_letterfilterwidget.h"

LetterFilterWidget::LetterFilterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LetterFilterWidget)
{
    ui->setupUi(this);

    ui->tabLetters->setTabPosition(QTabWidget::South);
    ui->tabLetters->setCurrentIndex(0);
}

LetterFilterWidget::~LetterFilterWidget()
{
    delete ui;
}

QTabWidget* LetterFilterWidget::get_tabwidget()
{
    return ui->tabLetters;
}
