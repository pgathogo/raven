#pragma once

#include <QWidget>

namespace Ui {
class AudioHistoryForm;
}

using History = std::map<int, std::vector<int>>;

class AudioHistoryForm : public QWidget
{
    Q_OBJECT

public:
    static int constexpr days_in_week = 7;
    static int constexpr hrs_in_day = 24;

    explicit AudioHistoryForm(QWidget *parent = nullptr);
    ~AudioHistoryForm();

    void prepare_grid();
    void show_history(History* history);
    void reset_history();

private:
    Ui::AudioHistoryForm *ui;
};

