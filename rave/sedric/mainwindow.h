#pragma once

#include <memory>
#include <concepts>

#include <QApplication>
#include <QMainWindow>
#include <QComboBox>

#include "../framework/datetimeselector.h"
#include "../security/structs.h"

class QStandardItemModel;
class QStandardItem;
class ItemBuilder;
class TrackItemBuilder;
class ScheduleData;
class TableViewModel;
class SaveAs;
class QCloseEvent;
class BookedCommercialDlg;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class PostgresDatabaseManager;
class BaseDataProvider;
class Schedule;
class EntityDataModel;
class CueEditor;
class AudioHistoryForm;

namespace FRAMEWORK {
    class TreeViewModel;
}

namespace AUDIO{
    class Audio;
    class Artist;
    class AudioLibItem;
    class AudioPlayer;
}

namespace SEDRIC{
    class SedricScheduleItem;
}

class ScheduleHour : public QComboBox
{
    Q_OBJECT
public:
    ScheduleHour();
    void hidePopup();
    void setText(QString);
signals:
    void hourComboBoxClosed();
};

struct SelectedScheduleItem{
    int row_id{-1};
    int schedule_id{-1};
    QString schedule_type;
    int schedule_hour{-1};
    QTime time_stamp;
};

using History = std::map<int, std::vector<int>>;


template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
int sum(T a, T b) requires Integral<T>
{
    return a + b;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    enum class SearchBy{folder_id, audio_title, audio_artist};

    MainWindow(QApplication* app, const StationInfo&, const ConnInfo&, QWidget *parent = nullptr);
    ~MainWindow();

    void populate_hour_combobox();

    void audio_folder_setup();
    void create_track_view_headers();
    void adjust_header_size();
    void set_track_view_column_width();
    void fetch_audio(const std::string);
    int get_schedule_row_id(int) const;
    void remove_item(int);
    bool is_item_deletable(const QString);
    void new_schedule();
    void remove_items_by_date_hour(QDate, int);
    QString get_schedule_type(int) const;
    void save_schedule();
    void setup_hour_combobox();
    std::vector<std::string> split_string(std::string, char);
    //std::tuple<int, int, int, QTime> get_sched_item_hour_time(const QModelIndex& index);
    SelectedScheduleItem get_sched_item_hour_time(const QModelIndex& index);

    void print_activity_details();
    std::vector<int> get_selected_hours_asInt();
    std::vector<int> vector_str_to_int(std::vector<std::string>& vs);

    void clear_schedule_model();
    QString get_selected_hours_asString();
    void fetch_data(QDate, const std::vector<int>&);
    AUDIO::Audio* get_selected_audio();
    void show_selection(DateTimeSelection);
    History make_history(int);
    void selected_comm_break();

    struct UnCachedHours{
        template <typename T>
        typename T::first_type operator()(T key_value_pair) const
        {
            return (key_value_pair.second == 0) ? key_value_pair.first : -1 ;
        }
    };

    template<typename T1, typename T2>
    T2 search_filter(std::string field, std::string op, T1 value)
    {
        return std::make_tuple(field, op, value);
    }

    inline void printlog(std::string msg)
    {
        qDebug() << __FILE__ << __LINE__ <<": "<< QString::fromStdString(msg);
    }


protected:
    void closeEvent(QCloseEvent* event) override;

public slots:
//    void date_changed();
    void hour_changed(int);
    void combo_highlight(int);
    void folder_clicked(const QModelIndex&);
    void insert_item(const QModelIndex& index);
    void remove_current_schedule_item();
    void track_selected(const QModelIndex& indClex);

    void cb_data_changed(const QModelIndex&, const QModelIndex);
//    void hour_cb_closed();
    void print_details(const QModelIndex&);
    void print_schedule();
    void save_as();

    void play_audio();
    void stop_play();
    void show_audio_history();

    void search_audio();
    void show_audio_data();
    void set_track_view();
    void set_ui_style();

    void select_date_time();
    void fetch_default_data();

    void contextMenuRequested(QPoint);
    void view_commercial(int);

    void test_ranges();



private:
    Ui::MainWindow *ui;
    QApplication* m_qapp;
    QStandardItemModel* m_schedule_model;
    FRAMEWORK::TreeViewModel* m_tree_model;
    QStandardItemModel* m_tracks_model;
    ItemBuilder* m_schedule_item_builder;
    TrackItemBuilder* m_track_item_builder;
    std::vector<ScheduleData*> m_test_data;

    QStandardItemModel* m_cb_model;
    std::vector<QStandardItem*> m_hours;
    ScheduleHour* m_cb_hour;

    SaveAs* m_save_as;
//    std::unique_ptr<CueEditor> m_cue_editor;

    std::unique_ptr<SEDRIC::SedricScheduleItem> m_scheduler;
    std::unique_ptr<AUDIO::AudioLibItem> m_audio_lib_item;
    std::unique_ptr<EntityDataModel> m_audio_entity_data_model;
    std::unique_ptr<AUDIO::AudioPlayer> m_audio_player;

    DateTimeSelection m_datetime_selection;

    std::unique_ptr<AudioHistoryForm> m_audio_history;
    QDate m_schedule_date;

    std::unique_ptr<QMenu> m_schedule_context_menu;
    std::unique_ptr<QAction> m_view_comm;
    std::unique_ptr<BookedCommercialDlg> m_booked_comm_dlg;

    void create_model_headers();
    std::map<int, int> fetch_schedule_from_cache(QDate, const std::vector<int>&);
    void fetch_schedule_from_db(QDate, std::vector<int>);
    void set_column_width();
    void set_toolbutton_icons();

};

