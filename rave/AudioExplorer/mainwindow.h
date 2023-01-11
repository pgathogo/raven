#pragma once

#include <memory>

#include <QApplication>
#include <QMainWindow>

#include "artistmanager.h"

#include "../audio/audio.h"
#include "../audio/artist.h"
//#include "../../audiolib/cueeditor/audioplayer.h"
#include "../framework/relationmapper.h"
#include "../framework/entitydatamodel.h"
#include "../framework/tree.h"

class QStandardItemModel;
class QTableView;

class BaseEntity;
class DataToolBar;
class TreeViewModel;
//class EntityDataModel;
class LetterFilterWidget;


namespace AUDIO{
  class Audio;
  class Artist;
  class AudioLibItem;
  class ArtistTypeItem;
  class GenreTypeItem;
  class Mp3ToOggConverter;
  class AudioWaveFormGenerator;
  class AudioConverter;
  class AudioWaveForm;
  class AudioPlayer;
}

//class CueEditor;
class RavenSetup;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


using FileOperationResult = std::tuple<bool, std::string>;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum ArtistColumn{FirstName=0, LastName, FullName, ArtistType, Notes};
    enum GenreColumn{GenreName};
    enum TrackColumns : int{Title=0, Artist=1, Duration=2, AudioType=3, AudioFileName=4};

    struct ImportResult{
        int audio_id = -1;
        std::vector<std::string> temp_files;
    };

    MainWindow(QApplication* qapp, QWidget *parent = nullptr);
    ~MainWindow();

    void fetch_audio(const std::string);
    void fetch_folder_audio(FRAMEWORK::RelationMapper* r_mapper);
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

    void setup_genre_view();
    void set_genre_view_columns_width();
    void create_genre_view_headers();
    void fetch_genre(const std::string filter="");
    void show_genre_data();

    int selected_row_id(QTableView*);
    std::string get_search_value(const QTableView*, int);
    BaseEntity* find_selected_entity(EntityDataModel*, QTableView*, int);
    void delete_entity(EntityDataModel*, QTableView*, int);
    bool okay_to_delete(const BaseEntity* entity);
    void remove_tv_selected_row(QTableView*);
    std::string make_selection(EntityDataModel* edm, std::string, int);
    AUDIO::Audio* get_selected_audio();

    void play_audio();
    void stop_play();
    void move_audio_to_current_folder();
    void show_letter_filter();
    int create_folder_to_db(const std::string&, int);
    void delete_folder_db(int);
    bool is_folder_empty(int);
    void connect_toolbutton_signals();
    void attach_folder_model();
    void update_folder_name_db(int,std::string);
    void update_folder_parent(int, int);
    void update_folder_view(int);
    void delete_audio_from_db(const std::string);

    int write_audio_to_db(std::unique_ptr<AUDIO::Audio>);
    FileOperationResult copy_new_audio_to_library(int);
    FileOperationResult copy_wave_file_to_library(int);
    FileOperationResult create_adf_file(int, AudioFile);

    void rollback_import_process(ImportResult& import_result);
    bool convert_audio(std::shared_ptr<AUDIO::Audio>);

    template<typename T>
    void fetch_filtered_audio(T arg)
    {
        AUDIO::Artist artist;
        AUDIO::Folder folder;
        AUDIO::Genre genre;
        AUDIO::Audio audio;

        auto [field_name, op, value] = arg;
        auto folder_filter = std::make_tuple(field_name, op, value);
        auto active_audio = std::make_tuple(audio.is_deleted()->qualified_column_name<AUDIO::Audio>(), "=", false);
        FRAMEWORK::RelationMapper* r_mapper = new FRAMEWORK::RelationMapper();
        r_mapper = m_audio_entity_data_model->select_related(folder, artist, genre)->filter(folder_filter, active_audio);
        fetch_folder_audio(r_mapper);

    }



public slots:
    void open_trash_can();

    void search_audio();

    void add_artist();
    void edit_artist();
    void delete_artist();
    void folder_clicked(const QModelIndex&);
    void artist_selected(const QModelIndex&);
    void search_artist();

    void add_genre();
    void edit_genre();
    void delete_genre();
    void genre_selected(const QModelIndex&);

    void import_audio();
    void audio_properties();
    void track_double_clicked(const QModelIndex&);

    void play_btn_clicked();
    void cue_edit();
    void audio_editor();

    void cut_audio();
    void paste_audio();
    void delete_audio();
    void filter_audio_by_letter(int);

    void folder_context_menu(const QPoint&);
    void track_context_menu(const QPoint&);
    void create_new_folder();
    void rename_folder();
    void delete_folder();

    void cut_folder();
    void paste_folder();

    void end_of_play();
    void open_settings();

private:
    Ui::MainWindow *ui;
    QApplication* m_qapp;
    DataToolBar* m_artist_toolbar;
    DataToolBar* m_genre_toolbar;

    QStandardItemModel* m_tracks_model;
    QStandardItemModel* m_artist_model;
    QStandardItemModel* m_genre_model;
    TreeViewModel* m_folder_model;

    std::unique_ptr<EntityDataModel> m_audio_entity_data_model;
    //std::unique_ptr<EntityDataModel> m_artist_entity_data_model;
    std::unique_ptr<EntityDataModel> m_genre_entity_data_model;

    std::unique_ptr<EntityDataModel> m_audio_edm;

    std::unique_ptr<AUDIO::AudioLibItem> m_audio_lib_item;
    std::unique_ptr<AUDIO::ArtistTypeItem> m_artist_type_item;
    std::unique_ptr<AUDIO::GenreTypeItem> m_genre_type_item;

    std::unique_ptr<AUDIO::AudioPlayer> m_audio_player;

//    std::unique_ptr<CueEditor> m_cue_editor;
    std::vector<int> m_cut_audios;
    int m_cut_folder_id;

    std::unique_ptr<LetterFilterWidget>m_letter_filter_widget;

    AUDIOEXP::ArtistManager m_artist_manager;

    std::vector<NodeData*> m_folders;

    std::unique_ptr<AUDIO::AudioConverter> m_audio_converter;
    std::unique_ptr<AUDIO::AudioWaveFormGenerator> m_wave_gen;

    std::unique_ptr<AUDIO::AudioWaveForm> m_audio_wave_form;

    RavenSetup* m_setup;
    std::unique_ptr<EntityDataModel> m_setup_edm;

    int selected_folder_id();

};


