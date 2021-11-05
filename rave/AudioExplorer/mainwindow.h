#pragma once

#include <memory>

#include <QMainWindow>

class QStandardItemModel;
class QTableView;

class BaseEntity;
class DataToolBar;
class TreeViewModel;
class EntityDataModel;

namespace AUDIO{
  class Audio;
  class Artist;
  class AudioLibItem;
  class ArtistTypeItem;
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum ArtistColumn{FirstName=0, LastName, FullName, ArtistType, Notes};

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void fetch_audio(const std::string);
    void show_audio_data();
    void create_track_view_headers();
    void set_track_view();
    void set_track_view_column_width();
    void setup_audio_folders();
    void setup_artists_view();
    void create_artist_view_headers();
    void set_artist_view_columns_width();

    void fetch_artist(const std::string filter="");
    void show_artist_data();
    void update_table_view_record(QTableView* tv, const std::vector<std::string>);

    int selected_row_id(QTableView*);
    std::string get_search_value(const QTableView*, int);
    BaseEntity* find_selected_entity(EntityDataModel*);
    void delete_entity(EntityDataModel*, QTableView*);
    bool okay_to_delete(const BaseEntity* entity);
    void remove_tv_selected_row(QTableView*);

public slots:
    void add_artist();
    void edit_artist();
    void delete_artist();
    void folder_clicked(const QModelIndex& index);
    void artist_selected(const QModelIndex& index);
    void search_artist();

private:
    Ui::MainWindow *ui;
    DataToolBar* m_artist_toolbar;
    DataToolBar* m_genre_toolbar;

    QStandardItemModel* m_tracks_model;
    QStandardItemModel* m_artist_model;

    TreeViewModel* m_folder_model;
    std::unique_ptr<EntityDataModel> m_audio_entity_data_model;
    std::unique_ptr<EntityDataModel> m_artist_entity_data_model;
    std::unique_ptr<AUDIO::AudioLibItem> m_audio_lib_item;

    std::unique_ptr<AUDIO::ArtistTypeItem> m_artist_type_item;
};

