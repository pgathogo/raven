#pragma once

#include <memory>
#include <string>
#include <map>
#include <vector>

#include <QString>

class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QTime;

namespace OATS{

    class DateTimeWidget : public QWidget
    {
    public:

        DateTimeWidget();
        ~DateTimeWidget();
        void set_time(QTime);
        void show_str_time(int, int);
        std::string minute_to_str(int);
        QString day_of_week(int);
    private:
        std::unique_ptr<QLabel> m_time_digit;
        std::unique_ptr<QLabel> m_time_text;
        std::unique_ptr<QLabel> m_date_text;

        QVBoxLayout* m_time_digit_layout;
        QVBoxLayout* m_time_date_layout;
        std::unique_ptr<QHBoxLayout> m_datetime_layout;

        std::vector<QString> m_days_of_week;
        std::map<int, std::string> m_hour_strings;

        void fill_hour_string();
        QString formatted_date();
    };
}
