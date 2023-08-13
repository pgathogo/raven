#pragma once

#include <memory>
#include <string>
#include <map>
#include <vector>

#include <QWidget>
#include <QString>
#include <QTimer>

class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QTime;
class QLCDNumber;

namespace OATS{

    class DateTimeWidget : public QWidget
    {
        Q_OBJECT
    public:
        DateTimeWidget();
        ~DateTimeWidget();
        void set_time(QTime);
        QString time_to_timestr(int,int);
        std::string minute_to_str(int);
        QString day_of_week(int);
        void start_timer(int interval=100);
        void stop_timer();
    private slots:
        void update_time();

    signals:
        void time_updated();

    private:
        //std::unique_ptr<QLabel> m_time_digit;
        std::unique_ptr<QLCDNumber> m_time_digit;
        std::unique_ptr<QLabel> m_time_text;
        std::unique_ptr<QLabel> m_date_text;

        QHBoxLayout* m_time_digit_layout;
        QVBoxLayout* m_time_date_layout;
        std::unique_ptr<QHBoxLayout> m_datetime_layout;

        std::vector<QString> m_days_of_week;
        std::map<int, std::string> m_hour_strings;

        std::unique_ptr<QTimer> m_datetime_timer;

        void fill_hour_string();
        QString formatted_date();
    };
}
