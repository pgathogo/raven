#pragma once

#include <memory>
#include <vector>

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class TimeAnalyzerWidget;
class QPushButton;
class QLabel;

namespace OATS{
    class DateTimeWidget;
    class PlayListControlWidget;
    class ScheduleItem;
    class ScheduleGridItem;
    class OutputPanel;
    class PlayModePanel;
}

struct CurrentPlayItem{
    OATS::ScheduleItem* item;
    int schedule_index{-1};
    int grid_index{-1};
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    constexpr static int MAX_GRID_ITEMS = 5;
    constexpr static int MAX_PLAYLIST_ITEMS = 10;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void set_widgets();
    void set_datetime_widget();
    void set_time_analytics_widget();
    void set_playlist_control_widget();
    void load_schedule(int);
    void make_playlist_grid();
    void make_play_mode_panel();

    OATS::ScheduleItem* schedule_item(int);
    std::string play_channel();

    void make_output_panel();
    OATS::OutputPanel* create_output_panel(const QString);

public slots:
    void play_button();
    void stop_button();

private slots:
    void close_win();
    void go_current();
    void keep_current();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<TimeAnalyzerWidget> m_taw;
    std::unique_ptr<OATS::DateTimeWidget> m_dtw;
    std::unique_ptr<OATS::PlayListControlWidget> m_pcw;
    std::unique_ptr<OATS::PlayModePanel> m_play_mode_panel;

    std::vector<std::unique_ptr<OATS::ScheduleItem>> m_schedule_items;
    std::vector<std::unique_ptr<OATS::ScheduleGridItem>> m_schedule_grid;

    CurrentPlayItem m_current_playing_item;

    OATS::OutputPanel* m_outputA;
    OATS::OutputPanel* m_outputB;
    std::unique_ptr<OATS::OutputPanel> m_outputC;

    std::vector<std::unique_ptr<OATS::OutputPanel>> m_output_panels;

    std::unique_ptr<QLabel> m_onair_label;

    static int s_sched_ref;
    static std::string s_channel;
};
