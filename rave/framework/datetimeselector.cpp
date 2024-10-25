#include <QDebug>
#include "datetimeselector.h"
#include "ui_datetimeselector.h"

DateTimeSelector::DateTimeSelector(QWidget *parent, DateTimeSelection dts)
    :QDialog(parent)
    ,ui(new Ui::DateTimeSelector)
    ,m_selection{dts}
{
    ui->setupUi(this);

    if (m_selection.sel_hours.size() == 0){
        m_selection.sel_hours.push_back(QTime::currentTime().hour());
    }

    build_time_buttons();

    connect(ui->btnOK, &QPushButton::clicked, this, &DateTimeSelector::pick_selection);
    connect(ui->btnCancel, &QPushButton::clicked, this, &DateTimeSelector::cancel_dialog);
    connect(ui->btnClear, &QPushButton::clicked, this, &DateTimeSelector::clear_selection);

    if (dts.sel_hours.size() > 0)
    {
        m_selection.sel_hours = dts.sel_hours;
        set_selected_buttons();
    }

    ui->calWidget->setSelectedDate(dts.sel_date);

    setWindowTitle("Date & Time Selection");
}

DateTimeSelector::~DateTimeSelector()
{
    delete ui;
}

void DateTimeSelector::build_time_buttons()
{
    //int hour = 0;
    std::vector<QString> time_text_am{"12", "1", "2", "3", "4", "5",
                                      "6", "7", "8", "9", "10", "11"};

    std::vector<QString> time_text_pm{"12", "13", "14", "15", "16", "17",
                                      "18", "19", "20", "21", "22", "23"};
    time_buttons("AM", time_text_am);
    time_buttons("PM", time_text_pm);

    ui->calWidget->setSelectedDate(m_selection.sel_date);
    set_selected_buttons();
}

void DateTimeSelector::set_selected_buttons()
{
    for (int hr : m_selection.sel_hours){
        for (auto [text, btn_data] : m_hour_buttons){
            if (btn_data.long_hour_fmt == hr) {

                //qDebug() << "Button Long Hour Fmt: " << btn_data.long_hour_fmt;
                //qDebug() << "Selected Hour: " << hr;

                btn_data.button->setChecked(true);
            }
        }
    }
}


void DateTimeSelector::time_buttons(const QString time_symbol, std::vector<QString>& time_text)
{

    QString btn_style(
        "QPushButton {background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0  #C1F5C7, stop:1 #D0F6B9, stop:2 #E4F9AA, stop:3 #EFFCA2 );"
        "border-radius: 20px;"
        "border-style: inset;"
        "border-bottom-width: 2px;"
        "border-radius: 3px;"
        "border-bottom-color:#F2FCA3;"
        "color:#FFFFFF;"
        "height:35;"
        "font-weight:bold;}" );

    /*
        "QPushButton:hover{"
            "background-color:#555D64; "
            "border-width:1px;"
            "border-color:#0479B0;"
            " }"

        "QPushButton:pressed{background-color:#555D64; border:none;}"
            );

    */

    // std::vector<QString> time_text_am{"12", "1", "2", "3", "4", "5",
    //                                   "6", "7", "8", "9", "10", "11"};

    int long_hour = 0;
    const int ROWS = 5;

    int row = 0;
    int	col = 0;

    int current_hour = QTime::currentTime().hour();
    QString am_pm = QTime::currentTime().toString("AP");

    for (int hr=0; hr < time_text.size(); ++hr)
    {
        QString btn_text = time_text[hr]+" "+time_symbol;

        QPushButton* btn = new QPushButton(btn_text, this);

        btn->setMinimumHeight(40);
        btn->setCheckable(true);
        connect(btn, &QPushButton::clicked, this,
                [&, btn, btn_text](){ m_hour_buttons[btn_text].is_selected = btn->isChecked(); });

        if (time_symbol == "AM"){
            ui->gridTimeAM->addWidget(btn, row, col);
            if (hr == current_hour && am_pm == "AM"){
                btn->setStyleSheet( "background-color: #7FFF00");
            }
        }else{

            // if (current_hour > 12)
            //     current_hour = current_hour - 12;

            if (hr == current_hour && am_pm == "PM"){
                btn->setStyleSheet(
                "background-color: #7FFF00"
                );
            }

            ui->gridTimePM->addWidget(btn, row, col);
        }

        if (row < ROWS){
            ++row;
        }else{
            row = 0;
            ++col;
        }


        HourButtonData btn_data;

        btn_data.long_hour_fmt = time_text[hr].toInt();
        btn_data.button = btn;

        m_hour_buttons[btn_text] = btn_data; //std::make_tuple(long_hour++, false);

    }

}

void DateTimeSelector::pick_selection()
{
    m_selection.sel_date = ui->calWidget->selectedDate();

    for(auto& h : m_selection.sel_hours) {
        qDebug() << h;
    }

    for (auto [text, btn_data] : m_hour_buttons){
        if (btn_data.is_selected) {


            if (std::find(m_selection.sel_hours.begin(),
                      m_selection.sel_hours.end(),
                       btn_data.long_hour_fmt) == m_selection.sel_hours.end()) {

                    m_selection.sel_hours.push_back(btn_data.long_hour_fmt);
            }
        }
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




