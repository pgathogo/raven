#pragma once

#include <memory>
#include <vector>
#include <chrono>

#include <QMainWindow>
#include <QLabel>
#include <QTimer>

#include "scheduleitem.h"
#include "../audio/audio.h"
#include "../audio/artist.h"

#include "../framework/entitydatamodel.h"
#include "../framework/relationmapper.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//#define TEMP_SCHEDULE
#define DB_SCHEDULE

class QPushButton;
class QLabel;
class QStandardItemModel;

class TreeViewModel;
class BaseDataProvider;
class ScheduleItem;
class TimeAnalyzerWidget;
class NodeData;
class TrackInfo;

namespace OATS{
    class DateTimeWidget;
    class PlayListControlWidget;
    class ScheduleItem;
    class ScheduleGridItem;
    class OutputPanel;
    class PlayModePanel;
    class CommercialViewer;
    class JingleGrid;
   class CartPanel;
}

namespace AUDIO{
   class AudioTrackViewer;
   class AudioLibItem;
   class TrackBrowser;
}

struct CurrentPlayItem{
    CurrentPlayItem():item{nullptr}{}
    OATS::ScheduleItem* item;
    int schedule_index{-1};
    int grid_index{-1};
};

using History = std::map<int, std::vector<int>>;
using namespace std::chrono_literals;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    constexpr static int HOURS_IN_A_DAY = 23;
    constexpr static int MAX_GRID_ITEMS = 10;
    constexpr static int MAX_PLAYLIST_ITEMS = 10;

    constexpr static int YIELD_FADE_DELAY = 3000;
    constexpr static int YIELD_FADE_OUT = 7000;


    const std::string ChannelA = "A";
    const std::string ChannelB = "B";
    const std::string ChannelC = "C";


    enum class ControlPage{Home, Commercial, Segue, Cart, Jingle, TrackInfo, Load};

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void compute_schedule_time();
    OATS::OutputPanel* create_output_panel(const QString);
    void display_schedule(int start_index=0);

    void load_schedule(QDate, int);
    void fetch_temp_data(int);

    std::string play_channel();

    void set_playout_widgets();
    void set_datetime_widget();
    void set_time_analytics_widget();
    void set_playlist_control_widget();
    void set_current_play_item();
    OATS::ScheduleItem* schedule_item(int);

    void make_playlist_grid();
    void make_play_mode_panel();
    void make_output_panel();
    void make_comm_viewer_widget();
    void make_track_info_widget();
    void make_jingle_grid_widget();

    int index_of(int);
    void stop_audio(OATS::OutputPanel*);
    void play_audio(OATS::OutputPanel*);
    int next_output_panel_id(int);
    OATS::OutputPanel* find_output_panel(int);
    OATS::ScheduleItem* find_next_schedule_item(OATS::ScheduleItem*);
    void cue_next_schedule(OATS::ScheduleItem*, OATS::OutputPanel*);
    void calculate_trigger_times();
    int calculate_yield_contribution(OATS::ScheduleItem*);

    void fetch_db_data(QDate, int);
    void set_schedule_fields(BaseDataProvider* provider,
                                     OATS::ScheduleItem* sched_item);

    QString output_string(OATS::ScheduleItem*);
    void create_track_view_headers();

    void print_schedule_items();

    void recompute_time(int);

    void load_item(int, int);
    void show_commercial(int);
    void fetch_commercial_in_db(int);
    void show_track_info(int);
    History make_history(int);

    void start_timers();

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

    void item_move_up(int, int);
    void item_move_down(int, int);
    void make_item_current(int);
    void grid_clicked(int, int);
    void delete_schedule_item(int, int);

    void transition_stop(int, int);
    void transition_mix(int, int);
    void transition_cut(int, int);

    void play_button(OATS::OutputPanel*);
    void stop_button(OATS::OutputPanel*);
    void go_current_clicked();
    void play_jingle(const QString);

    void push_items_down(int);
    void reprint_schedule(int);
//    void insert_schedule_item(int, std::unique_ptr<OATS::ScheduleItem>);

private:
    Ui::MainWindow *ui;

   // std::unique_ptr<TimeAnalyzerWidget> m_taw;
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

    std::unique_ptr<TreeViewModel> m_folder_model;
    std::vector<NodeData*> m_folders;

    std::unique_ptr<EntityDataModel> m_audio_edm;

    std::unique_ptr<OATS::CommercialViewer> m_comm_viewer;
    std::unique_ptr<TrackInfo> m_track_info;
    std::unique_ptr<OATS::JingleGrid> m_jingle_grid;

    std::unique_ptr<AUDIO::TrackBrowser> m_track_browser;
    std::unique_ptr<OATS::CartPanel>m_cart_panel;

    static int s_sched_ref;
    static std::string s_channel;

    ControlPage m_control_page;

    void set_header_item(OATS::ScheduleItem*, int, QDate);
    void fill_schedule_headers(QDate, int);
};

struct FindByRef{
    FindByRef(int ref):m_ref{ref}{}
    bool operator()(std::unique_ptr<OATS::ScheduleItem> const& item){
        return (item->schedule_ref() == m_ref);
    }
private:
    int m_ref;
};
