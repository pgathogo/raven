#pragma once

#include <QMainWindow>

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
class NodeData;

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


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void increment_time(int, int);
    void decrement_time(int, int);
    void show_cached_items(std::vector<ScheduleData*>);
    void show_db_schedule(QDate, int);
    void test_schedule(QDate, int);
    void populate_hours();

    void audio_folder_setup();
    void create_track_view_headers();
    void set_track_view_column_width();
    void fetch_audio(int);
     int get_schedule_row_id(int) const;
    void remove_item(int);
    bool is_item_deletable(const QString);
    void remove_all();
    void remove_items_by_date_hour(QDate, int);
    QString get_schedule_type(int) const;
    std::string make_insert_stmts();
    void save_schedule();
    bool write_schedule_to_db(std::string);

public slots:
    void add_item();
    void item_id();
    void date_changed();
    void hour_changed(int);
    void folder_clicked(const QModelIndex&);
    void insert_item(const QModelIndex& index);
    void remove_current_schedule_item();

private:
    Ui::MainWindow *ui;
    QStandardItemModel* m_schedule_model;
    TreeViewModel* m_tree_model;
    QStandardItemModel* m_tracks_model;
    ItemBuilder* m_schedule_item_builder;
    TrackItemBuilder* m_track_item_builder;
    std::vector<ScheduleData*> m_test_data;

    //QList<QStandardItem*> commercial_record(const Schedule);
    void create_model_headers();
    void populate_schedule(QDate, int);
    void set_column_width();
};
