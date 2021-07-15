#pragma once

#include <QWidget>

class QTabWidget;

namespace Ui {
class LetterFilterWidget;
}

class LetterFilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LetterFilterWidget(QWidget *parent = nullptr);
    ~LetterFilterWidget();

    QTabWidget* get_tabwidget();

private:
    Ui::LetterFilterWidget *ui;
};

