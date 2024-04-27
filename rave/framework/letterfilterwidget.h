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

    void tab_clicked(int);

    void style_tab_widget();

private:
    Ui::LetterFilterWidget *ui;

signals:
    void tab_changed(int);
    void tab_letter(QString letter);
};

