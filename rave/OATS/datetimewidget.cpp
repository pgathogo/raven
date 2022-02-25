#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTime>
#include <QFont>

#include "datetimewidget.h"

namespace OATS{

    DateTimeWidget::DateTimeWidget()
    {
        m_days_of_week = {"Sunday", "Monday", "Tuesday", "Wednesday",
                         "Thursday", "Friday", "Saturday"};

        m_time_digit = std::make_unique<QLabel>("00:00:00");
        QFont td_font("DigifaceWide", 14, QFont::Bold);
        m_time_digit->setFont(td_font);

        m_time_text = std::make_unique<QLabel>("Time Text");
        QFont tt_font("JetBrains Mono", 14, QFont::Bold);
        m_time_text->setFont(tt_font);

        m_date_text = std::make_unique<QLabel>("Current date");
        QFont dt_font("JetBrains Mono", 8, QFont::Bold);
        m_date_text->setFont(dt_font);

        m_time_digit_layout = new QVBoxLayout();
        m_time_date_layout = new QVBoxLayout();

        m_time_digit_layout->addWidget(m_time_digit.get());
        m_time_date_layout->addWidget(m_time_text.get());
        m_time_date_layout->addWidget(m_date_text.get());

        m_datetime_layout = std::make_unique<QHBoxLayout>();
        m_datetime_layout->addLayout(m_time_digit_layout);
        m_datetime_layout->addLayout(m_time_date_layout);
        m_datetime_layout->setStretch(1, 2);
        m_datetime_layout->setMargin(0);
        //m_datetime_layout->setSpacing(10);

        setLayout(m_datetime_layout.get());

        fill_hour_string();

    }


    DateTimeWidget::~DateTimeWidget()
    {
        delete m_time_digit_layout;
        delete m_time_date_layout;
    }

    void DateTimeWidget::set_time(QTime time)
    {
        m_time_digit->setText(time.toString("HH:mm:ss"));

        show_str_time(time.hour(), time.minute());  // testing only!!

        if (time.second() < 2)
            show_str_time(time.hour(), time.minute());

        m_date_text->setText(formatted_date());
    }

    void DateTimeWidget::show_str_time(int hour, int minutes)
    {
        std::string str_link{""};
        std::string str_hours{""};
        std::string time_str{""};

        if (minutes > 30)
            ++hour;

        str_hours = m_hour_strings[hour];

        std::string str_minutes = minute_to_str(minutes);

        if (minutes > 30){
            str_link = " to ";
        } else if (minutes == 0) {
            str_link = " ";
        } else {
            str_link = " past ";
        }

        if (minutes == 0)
            str_hours = str_hours + " oclock";

        time_str = str_minutes + str_link + str_hours;

        m_time_text->setText(QString::fromStdString(time_str));

    }

    std::string DateTimeWidget::minute_to_str(int minutes)
    {
        if (minutes == 0) return "Exactly";
        if (minutes == 1 || minutes == 59) return "One minute";
        if (minutes == 2 || minutes == 58) return "Two minutes";
        if (minutes == 3 || minutes == 57) return "Three minutes";
        if (minutes == 4 || minutes == 56) return "Four minutes";
        if (minutes == 5 || minutes == 55) return "Five minutes";
        if (minutes == 6 || minutes == 54) return "Six minutes";
        if (minutes == 7 || minutes == 53) return "Seven minutes";
        if (minutes == 8 || minutes == 52) return "Eight minutes";
        if (minutes == 9 || minutes == 51) return "Nine minutes";
        if (minutes == 10 || minutes == 50) return "Ten minutes";
        if (minutes == 11 || minutes == 49) return "Eleven minutes";
        if (minutes == 12 || minutes == 48) return "Twelve minutes";
        if (minutes == 13 || minutes == 47) return "Thirteen minutes";
        if (minutes == 14 || minutes == 46) return "Fourteen minutes";
        if (minutes == 15 || minutes == 45) return "Quarter";
        if (minutes == 16 || minutes == 44) return "Sixteen minutes";
        if (minutes == 17 || minutes == 43) return "Seventeen minutes";
        if (minutes == 18 || minutes == 42) return "Eighteen minutes";
        if (minutes == 19 || minutes == 41) return "Nineteen minutes";
        if (minutes == 20 || minutes == 40) return "Twenty minutes";
        if (minutes == 21 || minutes == 39) return "Twenty-one minutes";
        if (minutes == 22 || minutes == 38) return "Twenty-two minutes";
        if (minutes == 23 || minutes == 37) return "Twenty-three minutes";
        if (minutes == 24 || minutes == 36) return "Twenty-four minutes";
        if (minutes == 25 || minutes == 35) return "Twenty-five minutes";
        if (minutes == 26 || minutes == 34) return "Twenty-six minutes";
        if (minutes == 27 || minutes == 33) return "Twenty-seven minutes";
        if (minutes == 28 || minutes == 32) return "Twenty-eight minutes";
        if (minutes == 29 || minutes == 31) return "Twenty-nine minutes";
        if (minutes == 30) return "Half";

        return "";
    }

    void DateTimeWidget::fill_hour_string()
    {
        m_hour_strings[0] = "Twelve";
        m_hour_strings[1] = "One";
        m_hour_strings[2] = "Two";
        m_hour_strings[3] = "Three";
        m_hour_strings[4] = "Four";
        m_hour_strings[5] = "Five";
        m_hour_strings[6] = "Six";
        m_hour_strings[7] = "Seven";
        m_hour_strings[8] = "Eight";
        m_hour_strings[9] = "Nine";
        m_hour_strings[10] = "Ten";
        m_hour_strings[11] = "Eleven";
        m_hour_strings[12] = "Twelve";
        m_hour_strings[13] = "One";
        m_hour_strings[14] = "Two";
        m_hour_strings[15] = "Three";
        m_hour_strings[16] = "Four";
        m_hour_strings[17] = "Five";
        m_hour_strings[18] = "Six";
        m_hour_strings[19] = "Seven";
        m_hour_strings[20] = "Eight";
        m_hour_strings[21] = "Nine";
        m_hour_strings[22] = "Ten";
        m_hour_strings[23] = "Eleven";
        m_hour_strings[24] = "Twelve";
    }

    QString DateTimeWidget::formatted_date()
    {
        auto curr_date = QDate::currentDate();
        auto day_str = day_of_week(curr_date.dayOfWeek());

        auto fmt_date = day_str+", "+curr_date.toString("dd MMMM yyyy");

        return fmt_date;
    }

    QString DateTimeWidget::day_of_week(int dow)
    {
        return m_days_of_week[dow];
    }

}
