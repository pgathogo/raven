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

    connect(ui->tabLetters, &QTabWidget::currentChanged, this, &LetterFilterWidget::tab_clicked);

    style_tab_widget();
}

void LetterFilterWidget::style_tab_widget()
{
    QString tab_style(
        "QTabWidget::pane {"
        "border: 1px solid lightgray;"
        "top:-1px;"
        "background: red;"
        "}"

        "QTabBar::tab {"
        "background: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 #555D64 , stop:1 #374148 );"
        "border: 1px #374148;"
        "padding: 5px;"
        "color: #FFFFFF"
        "}"

        "QTabBar::tab:!selected:hover{"
        "background-color:#555D64;"
        "border-width:1px;"
        "border-color:#0479B0;"
        "}"

        "QTabBar::tab:selected { "
        "background:  qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 #454500 , stop:1 #777700 );"
        "margin-bottom: -1px;"
        "}"

        );

    ui->tabLetters->setStyleSheet(tab_style);
}

LetterFilterWidget::~LetterFilterWidget()
{
    delete ui;
}

QTabWidget* LetterFilterWidget::get_tabwidget()
{
    return ui->tabLetters;
}

void LetterFilterWidget::tab_clicked(int index)
{
    emit tab_letter(ui->tabLetters->tabText(index));
}
