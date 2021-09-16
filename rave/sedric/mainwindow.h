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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class PostgresDatabaseManager;
class BaseDataProvider;
class NodeData;
class Schedule;
class EntityDataModel;

namespace AUDIO{
    class Audio;
    class Artist;
    class AudioLibItem;
}

namespace SEDRIC{
    class SedricScheduleItem;
}

template<typename T>
struct VectorStruct{
    VectorStruct& operator<<(T* t)
    {
        vec.push_back(t);
        return *this;
    }
    void clear()
    {
        vec.clear();
    }
    std::vector<T*> vec;
};

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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void update_schedule_time(int, int);
    void show_cached_items(std::vector<ScheduleData*>);
    void show_db_schedule_new(QDate, std::vector<int>&);
    void populate_hour_combobox();

    void audio_folder_setup();
    void create_track_view_headers();
    void set_track_view_column_width();
    void fetch_audio_new(int folder_id);
    int get_schedule_row_id(int) const;
    void remove_item(int);
    bool is_item_deletable(const QString);
    void remove_all();
    void remove_items_by_date_hour(QDate, int);
    QString get_schedule_type(int) const;
    std::string make_insert_stmts();
    void save_schedule();
    bool write_schedule_to_db(std::string);
    void add_activity(int);
    void remove_activity(int);
    void delete_current_schedule(std::string);
    std::string make_delete_stmts();
    void set_schedule_fields(BaseDataProvider*,
                      Schedule*,
                      AUDIO::Audio*,
                      AUDIO::Artist*);
    void setup_hour_combobox();
    std::vector<std::string> split_string(std::string, char);
    std::tuple<int, int, QTime> get_sched_item_hour_time(const QModelIndex& index);
    void create_end_marker(int hour);

    void print_activity_details();
    std::vector<int> get_hours();
    std::vector<int> vector_str_to_int(std::vector<std::string>& vs);

public slots:
    void date_changed();
    void hour_changed(int);
    void combo_highlight(int);
    void folder_clicked(const QModelIndex&);
    void insert_item_new(const QModelIndex& index);
    void remove_current_schedule_item();

    void cb_data_changed(const QModelIndex&, const QModelIndex);
    void hour_cb_closed();
    void print_details(const QModelIndex&);

private:
    Ui::MainWindow *ui;
    QStandardItemModel* m_schedule_model;
    TreeViewModel* m_tree_model;
    QStandardItemModel* m_tracks_model;
    ItemBuilder* m_schedule_item_builder;
    TrackItemBuilder* m_track_item_builder;
    std::vector<ScheduleData*> m_test_data;
    std::map<int, int> m_activities;

    QStandardItemModel* m_cb_model;
    std::vector<QStandardItem*> m_hours;
    ScheduleHour* m_cb_hour;

    std::unique_ptr<SEDRIC::SedricScheduleItem> m_schedule_item;
    std::unique_ptr<AUDIO::AudioLibItem> m_audio_lib_item;
    std::unique_ptr<EntityDataModel> m_edm;

    //QList<QStandardItem*> commercial_record(const Schedule);
    void create_model_headers();
    void populate_schedule(QDate, std::vector<int>);
    void set_column_width();
};
