#pragma once

#include <QMainWindow>
#include <QComboBox>


class QStandardItemModel;
class QStandardItem;
class ItemBuilder;
class TrackItemBuilder;
class ScheduleData;
class TreeViewModel;
class TableViewModel;
class SaveAs;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class PostgresDatabaseManager;
class BaseDataProvider;
class NodeData;
class Schedule;
class EntityDataModel;
class CueEditor;

namespace AUDIO{
    class Audio;
    class Artist;
    class AudioLibItem;
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


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    enum class SearchBy{folder_id, audio_title, audio_artist};

    MainWindow(QWidget *parent = nullptr);
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
    std::tuple<int, int, QTime> get_sched_item_hour_time(const QModelIndex& index);

    void print_activity_details();
    std::vector<int> get_selected_hours_asInt();
    std::vector<int> vector_str_to_int(std::vector<std::string>& vs);

    void clear_schedule_model();
    QString get_selected_hours_asString();
    void fetch_data();
    AUDIO::Audio* get_audio();

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

public slots:
    void date_changed();
    void hour_changed(int);
    void combo_highlight(int);
    void folder_clicked(const QModelIndex&);
    void insert_item(const QModelIndex& index);
    void remove_current_schedule_item();

    void cb_data_changed(const QModelIndex&, const QModelIndex);
    void hour_cb_closed();
    void print_details(const QModelIndex&);
    void print_schedule();
    void copy_schedule();

    void play_audio();
    void stop_play();

    void search_audio();
    void show_audio_data();
    void set_track_view();
    void set_ui_style();

private:
    Ui::MainWindow *ui;
    QStandardItemModel* m_schedule_model;
    TreeViewModel* m_tree_model;
    QStandardItemModel* m_tracks_model;
    ItemBuilder* m_schedule_item_builder;
    TrackItemBuilder* m_track_item_builder;
    std::vector<ScheduleData*> m_test_data;

    QStandardItemModel* m_cb_model;
    std::vector<QStandardItem*> m_hours;
    ScheduleHour* m_cb_hour;

    SaveAs* m_save_as;
    std::unique_ptr<CueEditor> m_cue_editor;

    std::unique_ptr<SEDRIC::SedricScheduleItem> m_scheduler;
    std::unique_ptr<AUDIO::AudioLibItem> m_audio_lib_item;
    std::unique_ptr<EntityDataModel> m_audio_entity_data_model;

    //QList<QStandardItem*> commercial_record(const Schedule);
    void create_model_headers();
    std::map<int, int> fetch_schedule_from_cache(QDate, std::vector<int>);
    void fetch_schedule_from_db(QDate, std::vector<int>);
    void set_column_width();
};
