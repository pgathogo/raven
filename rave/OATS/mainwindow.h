#pragma once

#include <memory>
#include <vector>
#include <chrono>
#include <concepts>
#include <sstream>

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QMutex>
#include <QWheelEvent>

#include "scheduleitem.h"
#include "../audio/audio.h"
#include "../audio/artist.h"
#include "../audio/audiotool.h"
#include "../audio/audiocache.h"

#include "../framework/entitydatamodel.h"
#include "../framework/relationmapper.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//#define TEMP_SCHEDULE
#define DB_SCHEDULE
#define ENABLE_CACHE

// namespace FRAMEWORK {
//     class TreeViewModel;
// }

class QPushButton;
class QLabel;
class QStandardItemModel;
class QButtonGroup;


class BaseDataProvider;
class ScheduleItem;
//class TimeAnalyzerWidget;
class NodeData;
class TrackInfo;

namespace OATS{
    class DateTimeWidget;
    class PlayListControlWidget;
    class ScheduleItem;
    class ScheduleGridItem;
    class OutputPanel;
    class PlayModePanel;
    class Jingle;
    class JingleGrid;
    class CartPanel;
    class TimeAnalyzerTextWidget;
}

namespace DATA{
    class CommercialViewer;
}

namespace AUDIO{
   //class AudioTrackViewer;
   //class AudioLibItem;
   class TrackBrowser;
   class AudioPlayer;
   class AudioMeter;

   template<typename T>
   class AudioCacheManager;

   template<typename T>
   class AudioCacheQueueThread;

   template<typename T>
   class CacheUpdaterThread;

}

struct CurrentPlayItem{
    CurrentPlayItem():item{nullptr}{}
    OATS::ScheduleItem* item;
    int schedule_index{-1};
    int grid_index{-1};
};

struct CurrentDateTime{
    QDate current_date();
    int current_hour();
};

struct PlayedScheduleItem {
    int schedule_id{-1};
    bool db_persisted{false};
};

struct CommBreak{
    int booking_id{-1};
    int schedule_id{-1};
    int spot_id{-1};
    int book_hour{-1};
    int duration;
    int audio_id{-1};
    int played_audio_id{-1};
    QString comm_title;
    QString booking_status{""};
    QString filepath{""};
    QString file_extension{""};
    QString base_filename{""};
    QTime book_time;
    bool break_played{false};
    bool db_persisted{false};
    QDate play_date;
    QTime play_time;
};

template<typename T>
std::string join(const T& items, std::string delim)
{
    std::ostringstream s;
    for (const auto& i: items){
        if (&i != &items[0]){
            s << delim;
        }
        s << i;
    }
    return s.str();
}


template<typename T>
concept integral = std::is_integral_v<T>;

template<typename T>
concept floating_point = std::is_floating_point_v<T>;

template<typename T>
requires integral<T> || floating_point<T>
constexpr double Average(std::vector<T> const& v)
{
    const double sum = std::accumulate(v.begin(), v.end(), 0);
    return sum /v.size();
}

using History = std::map<int, std::vector<int>>;
using CurrentCuedItem = CurrentPlayItem;

using namespace std::chrono_literals;
using Hour = int;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    constexpr static int HOURS_IN_A_DAY = 24;
    constexpr static int MAX_GRID_ITEMS = 10;
    constexpr static int MAX_PLAYLIST_ITEMS = 10;

    constexpr static int YIELD_FADE_DELAY = 3000;
    constexpr static int YIELD_FADE_OUT = 7000;

    const std::string ChannelA = "A";
    const std::string ChannelB = "B";
    const std::string ChannelC = "C";

    const QString JinglePlayoutChannel = "C";

    const QString CACHE_DB_SQLITE = "audiocache.db";
    const QString AUDIO_CACHE_LOCATION = "d:/music/cache/";   // Temporary

    enum class ControlPage{Home, Commercial, Segue, Cart, Jingle, TrackInfo, Load};

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void compute_schedule_time();
    OATS::OutputPanel* create_output_panel(const QString);
//    void display_schedule(int start_index=0);
    void display_schedule(int grid_pos=0, int from_item_index=0);
    QTime schedule_time_at(int);

    void load_schedule(QDate, int);

    void load_schedule_bydate(QDate);
    void fetch_schedule_bydate(QDate);

    void fetch_temp_data(int);

    std::string play_channel();

    void set_playout_widgets();
    void set_datetime_widget();
    void set_time_analytics_widget();
    void set_playlist_control_widget();
    void set_current_play_item();
    void move_top_current_hour();
    OATS::ScheduleItem* schedule_item(int);
    std::vector<std::shared_ptr<OATS::ScheduleItem>>& get_schedule_item(int);

    void make_playlist_grid();
    void make_play_mode_panel();
    void make_output_panel();
    void make_comm_viewer_widget();
    void make_track_info_widget();
    void make_jingle_grid_widget();
    void draw_horizontal_line();

    int index_of(int);
    void play_audio(OATS::OutputPanel*);
    void stop_audio(OATS::OutputPanel*);
    void play_outputC(OATS::OutputPanel*);

    int next_output_panel_id(int);
    OATS::OutputPanel* find_output_panel(int);
    OATS::ScheduleItem* find_next_schedule_item(OATS::ScheduleItem*);
    void cue_commercial_item(OATS::ScheduleItem*, OATS::OutputPanel*);
    void cue_schedule_item(OATS::ScheduleItem*, OATS::OutputPanel*);

    void calculate_trigger_times();
    int calculate_yield_contribution(OATS::ScheduleItem*);
    void calculate_time_stats();

    void fetch_data_from_db(QDate, int);
    void fetch_commercials_bydate(QDate);
    void remove_empty_commercial_breaks();
    void build_hour_headers(QDate, int);
    void build_master_schedule();

    void set_schedule_fields(BaseDataProvider* provider,
                                     OATS::ScheduleItem* sched_item);

    QString output_string(OATS::ScheduleItem*);
    void create_track_view_headers();

    void print_schedule_items();
    void print_grid();

    void recompute_time(int);

    void load_item(int, int);
    void show_commercial(int);
    void show_next_break_commercial();
    void fetch_commercial_from_db(int);
    void show_track_info(int);
    History make_history(int);

    void start_timers();
    void print(QString);
    void print_output_status();

    int get_comm_duration(int);
    //void set_commercial_items_filepath(int);
    void set_scheduled_item_filepath(OATS::ScheduleItem*);
    void set_cache_last_play_dtime(OATS::ScheduleItem*);

    void scroll_down();
    void scroll_up();
    void style_page_controls();
    void style_footer_buttons();
    void refresh_data(int, int);

protected:
    void wheelEvent(QWheelEvent* event) override;


private slots:
    void close_win();
    void scroll_changed(int);
    void time_updated();
    void slow_flash();
    void fast_flash();
    //long long get_tick_count();
    void count_down();
    void status_timer();
    void on_calc_time_stats();

    void item_move_up(int, int);
    void item_move_down(int, int);
    void make_item_current(int, int);
    void grid_clicked(int, int);
    void delete_schedule_item(int, int);
    void reload_schedule(int, int);

    void transition_stop(int, int);
    void transition_mix(int, int);
    void transition_cut(int, int);

    void play_button(OATS::OutputPanel*);
    void stop_button(OATS::OutputPanel*);
    void go_current();
    void go_current_hour();
    void keep_current(bool);
    void play_jingle(OATS::Jingle*);
    void stop_all_jingles();
    //void play_jingle(const QString);

    void reprint_schedule(int);
    //void queue_for_caching(AUDIO::Audio*);
    void queue_for_caching(std::shared_ptr<AUDIO::Audio>);
//    void insert_schedule_item(int, std::unique_ptr<OATS::ScheduleItem>);
    void persist_cache();
    void end_of_play();
    void play_next();
    void audio_played(QString);
    void jingle_end_of_play();

    void play_cued_audio();

    void close_window();


private:
    void set_header_item(OATS::ScheduleItem*, int, QDate);
    void fill_schedule_headers(QDate);
    void setup_timers();
    void to_lower(std::string&);

    void mark_comm_as_played(int, const QString);
    void db_update_comm_break_play_status();
    void update_item_status();
    void create_schedule_item(const std::unique_ptr<OATS::ScheduleItem>&);
    void update_item(const std::unique_ptr<OATS::ScheduleItem>&);

    void print_comm_breaks();

    void test_concept();
    void uncheck_others(QString);

    void log_info(const QString);
    void log_error(const QString);


    Ui::MainWindow *ui;

   // std::unique_ptr<TimeAnalyzerWidget> m_taw;
    std::unique_ptr<OATS::TimeAnalyzerTextWidget> m_tta_widget;
    std::unique_ptr<OATS::DateTimeWidget> m_dtw;
    std::unique_ptr<OATS::PlayListControlWidget> m_pcw;
    std::unique_ptr<OATS::PlayModePanel> m_play_mode_panel;
    std::unique_ptr<QLabel> m_onair_label;

    std::vector<std::unique_ptr<OATS::ScheduleItem>> m_schedule_items;
    std::vector<std::unique_ptr<OATS::ScheduleGridItem>> m_schedule_grid;


    std::unique_ptr<OATS::ScheduleItem> m_temp_schedule_item;

    std::map<Hour, std::vector<std::shared_ptr<OATS::ScheduleItem>>> m_day_schedule_items;
    std::vector<std::shared_ptr<OATS::ScheduleItem>> m_master_schedule;

    CurrentPlayItem m_current_playing_item;
    CurrentCuedItem m_current_cued_item;

    OATS::OutputPanel* m_outputA;
    OATS::OutputPanel* m_outputB;
    std::unique_ptr<OATS::OutputPanel> m_outputC;

    std::vector<std::unique_ptr<OATS::OutputPanel>> m_output_panels;

    std::unique_ptr<QTimer> m_slow_flash_timer;
    std::unique_ptr<QTimer> m_fast_flash_timer;
    std::unique_ptr<QTimer> m_countdown_timer;
    std::unique_ptr<QTimer> m_main_player_timer;
    std::unique_ptr<QTimer> m_time_stats_timer;
    std::unique_ptr<QTimer> m_audio_playtime_db_update;

    // std::unique_ptr<FRAMEWORK::TreeViewModel> m_folder_model;
    // std::vector<NodeData*> m_folders;

    std::unique_ptr<EntityDataModel> m_audio_edm;

    std::unique_ptr<DATA::CommercialViewer> m_comm_viewer;
    std::unique_ptr<TrackInfo> m_track_info;
    std::unique_ptr<OATS::JingleGrid> m_jingle_grid;

    std::unique_ptr<AUDIO::TrackBrowser> m_track_browser;
    std::unique_ptr<OATS::CartPanel>m_cart_panel;

    static int s_sched_ref;
    static std::string s_channel;

    ControlPage m_control_page;

    AUDIO::AudioTool m_audio_tool;

    std::map<int, std::vector<CommBreak>> m_comm_breaks;

    std::shared_ptr<AUDIO::AudioCacheManager<SQLiteDatabaseManager>> m_audio_cache_manager;

    std::unique_ptr<AUDIO::AudioCacheQueueThread<SQLiteDatabaseManager>> m_cache_queue_thread;

    std::vector<std::unique_ptr<AUDIO::AudioCacheQueueThread<SQLiteDatabaseManager>>> m_cache_queue_threads;

    std::unique_ptr<AUDIO::CacheUpdaterThread<SQLiteDatabaseManager>> m_cache_updater;

    std::unique_ptr<QTimer> m_cache_updater_timer;

    std::shared_ptr<QMutex> m_queue_mutex;

    std::shared_ptr<AUDIO::AudioPlayer> m_audio_player;

    std::unique_ptr<AUDIO::AudioPlayer> m_jingle_player;
    std::unique_ptr<OATS::ScheduleItem> m_current_jingle_item;

    std::shared_ptr<QMutex> m_updatedb_mutex;
    std::unique_ptr<QTimer> m_comm_playtime_db_update;

    int m_schedule_top_index;
    int m_scrollbar_current_value;

    std::unique_ptr<QButtonGroup> m_control_buttons_group;

    std::shared_ptr<AUDIO::AudioMeter> m_audio_meter;

};

struct FindByRef{
    FindByRef(int ref):m_ref{ref}{}
    bool operator()(std::unique_ptr<OATS::ScheduleItem> const& item){
        return (item->schedule_ref() == m_ref);
    }


private:
    int m_ref;
};
