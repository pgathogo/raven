#pragma once

#include <QDialog>
#include <QDate>

namespace Ui {
class DateTimeSelector;
}

using IsSelected = bool;

struct DateTimeSelection{
    QDate sel_date;
    std::vector<int> sel_hours{};
};

struct HourButtonData{
    int long_hour_fmt{0};
    bool is_selected{false};
    QPushButton* button{nullptr};
};

class DateTimeSelector : public QDialog
{
    Q_OBJECT

public:
    explicit DateTimeSelector(QWidget *parent = nullptr, DateTimeSelection dts = {});
    ~DateTimeSelector();

    void build_time_buttons();
    void time_buttons(const QString, int);
    DateTimeSelection selection();
    void set_selected_buttons();

public slots:
    void pick_selection();
    void cancel_dialog();
    void clear_selection();

private:
    Ui::DateTimeSelector *ui;
    std::vector<QPushButton*> m_am_buttons;
    std::map<QString, HourButtonData> m_hour_buttons;
    DateTimeSelection m_selection;
};

