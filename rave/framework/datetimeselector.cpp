#include <QDebug>
#include "datetimeselector.h"
#include "ui_datetimeselector.h"

DateTimeSelector::DateTimeSelector(DateTimeSelection selection, QWidget *parent)
    :QDialog(parent)
    ,m_selection{selection}
    ,ui(new Ui::DateTimeSelector)
{
    ui->setupUi(this);
    build_time_buttons();

    connect(ui->btnOK, &QPushButton::clicked, this, &DateTimeSelector::pick_selection);
    connect(ui->btnCancel, &QPushButton::clicked, this, &DateTimeSelector::cancel_dialog);
    connect(ui->btnClear, &QPushButton::clicked, this, &DateTimeSelector::clear_selection);
}

DateTimeSelector::~DateTimeSelector()
{
    delete ui;
}

void DateTimeSelector::build_time_buttons()
{
    int hour = 0;
    time_buttons("AM", hour);
    time_buttons("PM", hour);

    ui->calWidget->setSelectedDate(m_selection.sel_date);
    set_selected_buttons();
}

void DateTimeSelector::set_selected_buttons()
{
    for (int hr : m_selection.sel_hours){
        for (auto [text, btn_data] : m_hour_buttons){
            if (btn_data.long_hour_fmt == hr){
                btn_data.button->setChecked(true);
            }
        }
    }
}


void DateTimeSelector::time_buttons(const QString time_symbol, int& long_hour)
{
    std::vector<QString> time_text_am{"12", "1", "2", "3", "4", "5",
                                      "6", "7", "8", "9", "10", "11"};
    const int ROWS = 5;

    int row = 0;
    int	col = 0;

    for (int hr=0; hr < time_text_am.size(); ++hr){
        QString btn_text = time_text_am[hr]+" "+time_symbol;
        QPushButton* btn = new QPushButton(btn_text, this);
        btn->setMinimumHeight(40);
        btn->setCheckable(true);
        connect(btn, &QPushButton::clicked, this,
                [&, btn, btn_text](){ m_hour_buttons[btn_text].is_selected = btn->isChecked(); });

        if (time_symbol == "AM"){
            ui->gridTimeAM->addWidget(btn, row, col);
        }else{
            ui->gridTimePM->addWidget(btn, row, col);
        }

        if (row < ROWS){
            ++row;
        }else{
            row = 0;
            ++col;
        }

        HourButtonData btn_data;
        btn_data.long_hour_fmt = long_hour++;
        btn_data.button = btn;

        m_hour_buttons[btn_text] = btn_data; //std::make_tuple(long_hour++, false);

    }

}

void DateTimeSelector::pick_selection()
{
    m_selection.sel_date = ui->calWidget->selectedDate();

    for (auto [text, btn_data] : m_hour_buttons){
        if (btn_data.is_selected)
            m_selection.sel_hours.push_back(btn_data.long_hour_fmt);
    }


    accept();

}

void DateTimeSelector::cancel_dialog()
{
    reject();
}

DateTimeSelection DateTimeSelector::selection()
{
    return m_selection;
}

void DateTimeSelector::clear_selection()
{
    for (auto [text, data] : m_hour_buttons){
        data.button->setChecked(false);
    }


}




