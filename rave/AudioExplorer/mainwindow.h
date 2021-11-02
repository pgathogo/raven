#pragma once

#include <memory>

#include <QMainWindow>

class DataToolBar;
class TreeViewModel;
class QStandardItemModel;
class EntityDataModel;

namespace AUDIO{
  class Audio;
  class Artist;
  class AudioLibItem;
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void fetch_audio(const std::string);
    void show_audio_data();
    void create_track_view_headers();
    void set_track_view();
    void set_track_view_column_width();
    void setup_audio_folders();
    void setup_artists();
    void create_artist_view_headers();

public slots:
    void add_artist();
    void edit_artist();
    void delete_artist();
    void folder_clicked(const QModelIndex& index);

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
};

