#ifndef TRACKBROWSER_H
#define TRACKBROWSER_H

#include <memory>
#include <QWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QTreeView>
#include <QLabel>
#include <QLineEdit>
#include <QModelIndex>

#include "../framework/baseentity.h"
#include "../framework/entitydatamodel.h"
#include "../framework/relationmapper.h"

#include "../audio/audio.h"
#include "../audio/artist.h"
#include "../audio/audiolibitem.h"

class NodeData;
class TreeViewModel;
class EntityDataModel;

namespace AUDIO
{
    class AudioTrackViewer;
    class AudioFolderWidget;
    class AudioSearchWidget;
    class AudioLibWidget;
    class AudioTrackWidget;
    class AudioLibItem;
    class AudioDataModel;

    using Filter = std::tuple<std::string, std::string, std::string>;

    class TrackBrowser : public QWidget
    {
        Q_OBJECT
    public:
        explicit TrackBrowser(QWidget *parent = nullptr);

        void init_top_widget(QSplitter*);
        void init_bottom_widget(QSplitter*);

        std::shared_ptr<AUDIO::Audio> current_selected_audio();
        std::shared_ptr<AUDIO::Audio> find_audio_by_id(int);

    signals:

    private slots:
        void folder_clicked(int);
        void selected_audio(std::shared_ptr<AUDIO::Audio>);
        void search_filter(Filter);

    private:
       std::unique_ptr<QWidget> m_top_widget;
       std::unique_ptr<QWidget> m_bottom_widget;
       std::unique_ptr<QVBoxLayout> m_main_layout;

       std::unique_ptr<QTabWidget> m_tab_widget;
       QHBoxLayout* btn_h_layout;

       std::unique_ptr<QPushButton> m_btn_listen;
       std::unique_ptr<QPushButton> m_btn_info;
       std::unique_ptr<QPushButton> m_btn_history;

       std::unique_ptr<AudioFolderWidget> m_audio_folder_widget;
       std::unique_ptr<AudioSearchWidget> m_search_widget;

       std::unique_ptr<AudioTrackWidget> m_audio_track_widget;
       std::unique_ptr<AudioDataModel> m_audio_data_model;

       QSplitter* m_main_splitter;

       std::shared_ptr<AUDIO::Audio> m_current_selected_audio;
    };


    class AudioFolderWidget : public QWidget
    {
        Q_OBJECT
    public:
        AudioFolderWidget();
        void layout_controls();
        void set_model();

    private slots:
        void folder_clicked(const QModelIndex& index);

    signals:
        void folder_clicked_signal(int);

    private:
        std::unique_ptr<QVBoxLayout> m_v_layout;
        std::unique_ptr<QTreeView> m_folder_view;
        std::unique_ptr<NodeData*> m_folders;
        std::unique_ptr<TreeViewModel> m_folder_model;
    };


    class AudioSearchWidget : public QWidget
    {
        Q_OBJECT
    public:
        const int COL0 = 0;
        const int COL1 = 1;
        const int COL2 = 2;
        const int ROW0 = 0;
        const int ROW1 = 1;
        const int ROW2 = 2;

        AudioSearchWidget();
        void layout_controls();
    signals:
        void search_filter(Filter);
    private slots:
        void search_clicked();
    private:
        std::unique_ptr<QGridLayout> m_grid_layout;

        std::unique_ptr<QLabel> m_lbl_title;
        std::unique_ptr<QLineEdit> m_edt_title;

        std::unique_ptr<QLabel> m_lbl_artist;
        std::unique_ptr<QLineEdit>m_edt_artist;

        std::unique_ptr<QPushButton>m_btn_search;
    };


    class AudioLibWidget : public QWidget
    {
        Q_OBJECT
    public:
        AudioLibWidget();
    private:
        std::unique_ptr<QVBoxLayout> m_h_layout;
        std::unique_ptr<QTabWidget> m_tab_widget;
        std::unique_ptr<AudioFolderWidget> m_audio_folder_widget;
        std::unique_ptr<AudioSearchWidget> m_audio_search_widget;
    };

    class AudioTrackWidgetToolbar : public QWidget
    {
        Q_OBJECT
    public:
        AudioTrackWidgetToolbar();
        void layout_controls();
    private:
        std::unique_ptr<QHBoxLayout> m_h_layout;
        std::unique_ptr<QPushButton> m_btn_listen;
        std::unique_ptr<QPushButton> m_btn_info;
        std::unique_ptr<QPushButton> m_btn_history;
    };

    class AudioTrackWidget : public QWidget
    {
        Q_OBJECT
    public:
        AudioTrackWidget();

        AUDIO::AudioTrackViewer* track_viewer();

    signals:
        void selected_track(int) ;

    private slots:
        void track_selected(int);

    private:
        std::unique_ptr<AudioTrackWidgetToolbar> m_toolbar;
        std::unique_ptr<AUDIO::AudioTrackViewer> m_track_viewer;

        std::unique_ptr<QVBoxLayout> m_v_layout;

    };

    class AudioDataModel : public QObject
    {
        Q_OBJECT
    public:
        AudioDataModel(AUDIO::AudioTrackViewer* viewer);
        void fetch_audio_data(FRAMEWORK::RelationMapper*);
        void show_data();
        std::shared_ptr<AUDIO::Audio> find_audio_by_id(int);

        template<typename T>
        void fetch_filtered_audio(T arg)
        {
           AUDIO::Artist artist;
           AUDIO::Audio audio;

           auto [field_name, op, value] = arg;
           auto base_filter = std::make_tuple(field_name, op, value);
           auto active_audio = std::make_tuple(audio.is_deleted()->qualified_column_name<AUDIO::Audio>(),
                                               "=", false);
           FRAMEWORK::RelationMapper* r_mapper = new FRAMEWORK::RelationMapper();
           r_mapper = m_audio_edm->select_related(artist)->filter(base_filter, active_audio);

           fetch_audio_data(r_mapper);
        }
    signals:
        void selected_audio(std::shared_ptr<AUDIO::Audio>);

    private slots:
        void track_selected(int);

    private:
       AUDIO::AudioTrackViewer* m_viewer;
       std::unique_ptr<AUDIO::AudioLibItem> m_lib_item;
       std::unique_ptr<EntityDataModel> m_audio_edm;
    };



}

#endif // TRACKBROWSER_H
