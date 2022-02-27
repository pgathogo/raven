#pragma once

#include <memory>
#include <vector>

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include "scheduleitem.h"

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

    constexpr static int YIELD_FADE_DELAY = 3000;
    constexpr static int YIELD_FADE_OUT = 7000;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void compute_schedule_time();
    OATS::OutputPanel* create_output_panel(const QString);
    void display_schedule(int start_index=0);

    void load_schedule(int);

    std::string play_channel();

    void set_widgets();
    void set_datetime_widget();
    void set_time_analytics_widget();
    void set_playlist_control_widget();
    void set_current_play_item();
    OATS::ScheduleItem* schedule_item(int);

    void make_playlist_grid();
    void make_play_mode_panel();
    void make_output_panel();
    void make_item_current(int);
    int index_of(int);
    void stop_audio(OATS::OutputPanel*);
    void play_audio(OATS::OutputPanel*);
    int next_output_panel_id(int);
    OATS::OutputPanel* find_output_panel(int);
    OATS::ScheduleItem* find_next_schedule_item(OATS::ScheduleItem*);
    void cue_next_schedule(OATS::ScheduleItem*, OATS::OutputPanel*);
    void calculate_trigger_times();
    int calculate_yield_contribution(OATS::ScheduleItem*);

public slots:
    void play_button();
    void stop_button();
    void go_current_clicked();

private slots:
    void close_win();
    void go_current();
    void scroll_changed(int);
    void time_updated();
    void slow_flash();
    void fast_flash();
    long long get_tick_count();
    void count_down();
    void status_timer();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<TimeAnalyzerWidget> m_taw;
    std::unique_ptr<OATS::DateTimeWidget> m_dtw;
    std::unique_ptr<OATS::PlayListControlWidget> m_pcw;
    std::unique_ptr<OATS::PlayModePanel> m_play_mode_panel;
    std::unique_ptr<QLabel> m_onair_label;

    std::vector<std::unique_ptr<OATS::ScheduleItem>> m_schedule_items;
    std::vector<std::unique_ptr<OATS::ScheduleGridItem>> m_schedule_grid;

    CurrentPlayItem m_current_playing_item;

    OATS::OutputPanel* m_outputA;
    OATS::OutputPanel* m_outputB;
    std::unique_ptr<OATS::OutputPanel> m_outputC;

    std::vector<std::unique_ptr<OATS::OutputPanel>> m_output_panels;

    std::unique_ptr<QTimer> m_slow_flash_timer;
    std::unique_ptr<QTimer> m_fast_flash_timer;
    std::unique_ptr<QTimer> m_countdown_timer;
    std::unique_ptr<QTimer> m_main_player_timer;

    static int s_sched_ref;
    static std::string s_channel;
};

struct FindByRef{
    FindByRef(int ref):m_ref{ref}{}
    bool operator()(std::unique_ptr<OATS::ScheduleItem> const& item){
        return (item->schedule_ref() == m_ref);
    }
private:
    int m_ref;
};
