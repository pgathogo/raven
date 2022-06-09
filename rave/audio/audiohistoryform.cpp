#include <map>
#include <algorithm>

#include <QDate>
#include <QTime>
#include <QTableWidget>

#include "audiohistoryform.h"
#include "ui_audiohistoryform.h"

AudioHistoryForm::AudioHistoryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioHistoryForm)
{
    ui->setupUi(this);
    prepare_grid();
}

AudioHistoryForm::~AudioHistoryForm()
{
    delete ui;
}

void AudioHistoryForm::prepare_grid()
{
    std::map<int, QString> days;
    days[1] = "Mon";
    days[2] = "Tue";
    days[3] = "Wed";
    days[4] = "Thu";
    days[5] = "Fri";
    days[6] = "Sat";
    days[7] = "Sun";

    QDate curr_date = QDate::currentDate();

    QTime curr_time = QTime::currentTime();
    int curr_hour = curr_time.hour();

    for(int hr=0; hr <= hrs_in_day-1; ++hr){
        if (curr_hour == hr){
            auto item = new QTableWidgetItem(QString::number(hr));
            QFont f;
            f.setBold(true);
            item->setFont(f);
            ui->twHistory->setHorizontalHeaderItem(hr, item);
        } else {
            ui->twHistory->setHorizontalHeaderItem(hr, new QTableWidgetItem(QString::number(hr)));
        }

    }

    auto ms_date = curr_date.addDays(-6);
    QStringList dow_str;

    for(int d=1; d<= days_in_week; ++d){
        int dow = ms_date.dayOfWeek();
        dow_str << days[dow];
        ms_date = ms_date.addDays(1);
    }

    ui->twHistory->setVerticalHeaderLabels(dow_str);

    for (int r=0; r <= days_in_week-1; ++r){
        for (int c=0; c <= hrs_in_day-1; ++c) {
            ui->twHistory->setItem(r, c, new QTableWidgetItem);

            if (r == days_in_week-1)
                ui->twHistory->item(r, c)->setBackground(QColor(41, 121, 255));

            if (c == curr_hour)
                ui->twHistory->item(r, c)->setBackground(QColor(42, 121, 255));
        }
    }


}

void AudioHistoryForm::show_history(History* history)
{
    reset_history();
    prepare_grid();

    std::map<int, QString> days;
    days[1] = "Mon";
    days[2] = "Tue";
    days[3] = "Wed";
    days[4] = "Thu";
    days[5] = "Fri";
    days[6] = "Sat";
    days[7] = "Sun";

    for (auto [day, hours] : *history){
        auto dow = days[day];
        for (int r=0; r <= days_in_week-1; ++r){
            if (dow == ui->twHistory->verticalHeaderItem(r)->text()){
                for (int c=0; c <= hrs_in_day-1; ++c){
                    if (hours.size() > 0){
                        if (std::find(hours.begin(), hours.end(), c) != hours.end()){
                            ui->twHistory->item(r, c)->setBackground(Qt::red);
                        }
                    }
                }
            }
        }
    }

}

void AudioHistoryForm::reset_history()
{
    for (int r=0; r<= days_in_week-1; ++r){
        for(int c=0; c <= hrs_in_day-1; ++c){
            ui->twHistory->item(r, c)->setBackground(Qt::white);
        }
    }
}
