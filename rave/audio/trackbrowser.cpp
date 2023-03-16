#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <QFileSystemModel>
#include <QDir>
#include <QGridLayout>
#include <QStandardItem>
#include <QStandardItemModel>

#include "trackbrowser.h"

#include "../framework/ravenexception.h"
#include "../framework/tree.h"
#include "../framework/treeviewmodel.h"

#include "../audio/audiotrackviewer.h"
#include "../audio/audiolibrary.h"

namespace AUDIO
{
    TrackBrowser::TrackBrowser(QWidget *parent)
        : QWidget{parent}
    {

        m_main_splitter	 = new QSplitter(Qt::Vertical);

        init_top_widget(m_main_splitter);
        init_bottom_widget(m_main_splitter);

        m_main_layout = std::make_unique<QVBoxLayout>();
        m_main_layout->addWidget(m_main_splitter);

        connect(m_audio_folder_widget.get(), &AudioFolderWidget::folder_clicked_signal, this, &TrackBrowser::folder_clicked);

        this->setLayout(m_main_layout.get());

    }

    void TrackBrowser::init_top_widget(QSplitter* splitter)
    {
        m_top_widget = std::make_unique<QWidget>(this);
        //m_top_widget->setStyleSheet("background-color: green;");

        auto v_layout = new QVBoxLayout(this);
        v_layout->setContentsMargins(0,0,0,0);
        m_top_widget->setLayout(v_layout);

        m_tab_widget = std::make_unique<QTabWidget>(m_top_widget.get());
        v_layout->addWidget(m_tab_widget.get());

        m_audio_folder_widget = std::make_unique<AudioFolderWidget>();
        m_tab_widget->addTab(m_audio_folder_widget.get(), "Browse Audio");

        m_search_widget = std::make_unique<AudioSearchWidget>();

        connect(m_search_widget.get(), &AudioSearchWidget::search_filter, this, &TrackBrowser::search_filter);

        m_tab_widget->addTab(m_search_widget.get(), "Search Audio");

        splitter->addWidget(m_top_widget.get());

    }

    void TrackBrowser::init_bottom_widget(QSplitter* splitter)
    {
        m_bottom_widget = std::make_unique<QWidget>(this);
        //m_bottom_widget->setStyleSheet("background-color: yellow;");

        auto v_layout = new QVBoxLayout(this);
        v_layout->setContentsMargins(0,0,0,0);
        m_bottom_widget->setLayout(v_layout);

        m_audio_track_widget = std::make_unique<AudioTrackWidget>();

        v_layout->addWidget(m_audio_track_widget.get());

        m_audio_data_model = std::make_unique<AudioDataModel>(m_audio_track_widget->track_viewer());
        connect(m_audio_data_model.get(), &AudioDataModel::selected_audio, this, &TrackBrowser::selected_audio);

        splitter->addWidget(m_bottom_widget.get());

        QList<int> sizes({50, 100});
        splitter->setSizes(sizes);

    }

    void TrackBrowser::folder_clicked(int folder_id)
    {
        auto audio = std::make_unique<AUDIO::Audio>();
        auto folder_filter = std::make_tuple(audio->folder()->qualified_column_name<AUDIO::Audio>(),
                                             "=", folder_id);

        m_audio_data_model->fetch_filtered_audio(folder_filter);
    }

    void TrackBrowser::selected_audio(AUDIO::Audio* audio)
    {
        m_current_selected_audio = audio;
    }

    AUDIO::Audio* TrackBrowser::current_selected_audio()
    {
        return m_current_selected_audio;
    }

    AUDIO::Audio* TrackBrowser::find_audio_by_id(int audio_id)
    {
        m_audio_data_model->find_audio_by_id(audio_id);

    }

    void TrackBrowser::search_filter(Filter filter)
    {
        m_audio_data_model->fetch_filtered_audio(filter);
    }

    /* ------ AudioFolderWidget ------ */

    AudioFolderWidget::AudioFolderWidget()
        :m_v_layout{nullptr}
        ,m_folder_view{nullptr}
    {
        m_v_layout = std::make_unique<QVBoxLayout>();
        layout_controls();
        setLayout(m_v_layout.get());
        set_model();
        connect(m_folder_view.get(), &QTreeView::clicked, this, &AudioFolderWidget::folder_clicked);
    }

    void AudioFolderWidget::layout_controls()
    {
        m_folder_view = std::make_unique<QTreeView>();
        m_v_layout->addWidget(m_folder_view.get());
    }

    void AudioFolderWidget::set_model()
    {
//        QFileSystemModel* model = new QFileSystemModel;
//        model->setRootPath(QDir::currentPath());

        try{
            AudioLibrary audio_lib;
            auto m_folders = audio_lib.read_data_from_db();
            m_folder_model = std::make_unique<TreeViewModel>(m_folders, this);
            m_folder_model->setHorizontalHeaderItem(0, new QStandardItem("Audio Library"));

            m_folder_view->setModel(m_folder_model.get());

        } catch (DatabaseException& de) {
            qDebug() << QString::fromStdString(de.errorMessage());
        }

    }

    void AudioFolderWidget::folder_clicked(const QModelIndex& index)
    {
        int folder_id = index.data(Qt::UserRole).toInt();
        emit folder_clicked_signal(folder_id);
    }

    AudioSearchWidget::AudioSearchWidget()
        :m_grid_layout{nullptr}
        ,m_lbl_title{nullptr}
        ,m_edt_title{nullptr}
    {
        layout_controls();
        setLayout(m_grid_layout.get());
    }

   void AudioSearchWidget::layout_controls()
   {
       m_grid_layout = std::make_unique<QGridLayout>();
       m_lbl_title = std::make_unique<QLabel>("Title: ", this);
       m_edt_title = std::make_unique<QLineEdit>(this);

       m_lbl_artist = std::make_unique<QLabel>("Artist: ", this);
       m_edt_artist = std::make_unique<QLineEdit>(this);

       m_btn_search = std::make_unique<QPushButton>("Search", this);
       connect(m_btn_search.get(), &QPushButton::clicked, this, &AudioSearchWidget::search_clicked);

       m_grid_layout->addWidget(m_lbl_title.get(), ROW0, COL0);
       m_grid_layout->addWidget(m_edt_title.get(), ROW0, COL1);

       m_grid_layout->addWidget(m_lbl_artist.get(), ROW1, COL0);
       m_grid_layout->addWidget(m_edt_artist.get(), ROW1, COL1);

       m_grid_layout->addWidget(m_btn_search.get(), ROW2, COL1, 1, 1);

   }

   void AudioSearchWidget::search_clicked()
   {
       if (!m_edt_title->text().isEmpty()){
           auto audio = std::make_unique<AUDIO::Audio>();
           auto title_filter = std::make_tuple(
                       audio->title()->qualified_column_name<AUDIO::Audio>(),
                       "like",
                       m_edt_title->text().toStdString());

           emit search_filter(title_filter);

       }

       if (!m_edt_artist->text().isEmpty()) {
           auto artist = std::make_unique<AUDIO::Artist>();
           auto artist_filter = std::make_tuple(
                       artist->fullName()->qualified_column_name<AUDIO::Artist>(),
                       "like",
                       m_edt_artist->text().toStdString());

           emit search_filter(artist_filter);
       }

   }

   AudioLibWidget::AudioLibWidget()
       :m_h_layout{nullptr}
       ,m_audio_folder_widget{nullptr}
       ,m_audio_search_widget{nullptr}
   {
       m_h_layout = std::make_unique<QVBoxLayout>();

       setStyleSheet("background-color: red;");

//       m_tab_widget = std::make_unique<QTabWidget>(this);

//       m_audio_folder_widget = std::make_unique<AudioFolderWidget>();
//       m_tab_widget->addTab(m_audio_folder_widget.get(), "Browser Audio Folders");

//       m_audio_search_widget  = std::make_unique<AudioSearchWidget>();
//       m_tab_widget->addTab(m_audio_search_widget.get(), "Search Audio");

//       m_h_layout->addWidget(m_tab_widget.get());

       //auto v_layout = std::make_unique<QHBoxLayout>();
       setLayout(m_h_layout.get());

   }

   /* --- AudioTrackWidget ----- */

   AudioTrackWidgetToolbar::AudioTrackWidgetToolbar()
       :m_h_layout{nullptr}
       ,m_btn_listen{nullptr}
       ,m_btn_info{nullptr}
       ,m_btn_history{nullptr}
   {
        layout_controls();
        setLayout(m_h_layout.get());
   }

   void AudioTrackWidgetToolbar::layout_controls()
   {
       m_btn_listen =  std::make_unique<QPushButton>("listen", this);
       m_btn_info = std::make_unique<QPushButton>("Audio Info", this);
       m_btn_history = std::make_unique<QPushButton>("Audio History", this);

       m_h_layout = std::make_unique<QHBoxLayout>();
       m_h_layout->setContentsMargins(0,0,0,0);
       m_h_layout->addWidget(m_btn_listen.get());
       m_h_layout->addWidget(m_btn_info.get());
       m_h_layout->addWidget(m_btn_history.get());
       m_h_layout->addSpacerItem(new QSpacerItem(0, 0 , QSizePolicy::Expanding, QSizePolicy::Ignored ));
   }

   /* ----- AudioTrackWidget --------- */

   AudioTrackWidget::AudioTrackWidget()
       :m_toolbar{nullptr}
       ,m_track_viewer{nullptr}
       ,m_v_layout{nullptr}
   {
        m_v_layout = std::make_unique<QVBoxLayout>();
        m_v_layout->setContentsMargins(0,0,0,0);

        m_toolbar = std::make_unique<AudioTrackWidgetToolbar>();
        m_track_viewer = std::make_unique<AUDIO::AudioTrackViewer>();

        m_v_layout->addWidget(m_toolbar.get());
        m_v_layout->addWidget(m_track_viewer.get());

        setLayout(m_v_layout.get());
   }

   AUDIO::AudioTrackViewer* AudioTrackWidget::track_viewer()
   {
       return m_track_viewer.get();
   }

   void AudioTrackWidget::track_selected(int track_id)
   {
       emit selected_track(track_id);
   }

  /* -----  AudioDataModel ------ */

  AudioDataModel::AudioDataModel(AUDIO::AudioTrackViewer* viewer)
       :m_viewer{viewer}
   {
        m_lib_item = std::make_unique<AUDIO::AudioLibItem>(m_viewer->model());
        m_audio_edm = std::make_unique<EntityDataModel>(std::make_unique<AUDIO::Audio>());

        connect(m_viewer, &AUDIO::AudioTrackViewer::track_selected, this, &AudioDataModel::track_selected);
   }

   void AudioDataModel::track_selected(int track_id)
   {
        AUDIO::Audio* audio = m_lib_item->find_audio_by_id(track_id);
        emit selected_audio(audio);
   }

   AUDIO::Audio* AudioDataModel::find_audio_by_id(int audio_id)
   {
       return m_lib_item->find_audio_by_id(audio_id);
   }


  void AudioDataModel::fetch_audio_data(FRAMEWORK::RelationMapper* r_mapper)
  {
    m_audio_edm->clearEntities();

    auto const& audio = dynamic_cast<AUDIO::Audio*>(m_audio_edm->get_entity().get());

    try{
        m_audio_edm->readRaw(r_mapper->query());
    } catch (DatabaseException& de) {
        showMessage(de.errorMessage());
    }

    r_mapper->map_data();

    for (auto const& [record_id, record] : r_mapper->mapped_entities())
    {
        auto audio_Uptr = std::make_unique<AUDIO::Audio>();
        bool audio_is_constructed = false;

        for(auto const& [table_name, entities] : record)
        {
            for (auto const& entity : entities)
            {
                if (audio_Uptr->tableName() == entity->tableName() &&
                    !audio_is_constructed)
                {
                    if (entity->id() > -1){
                    auto audio_ptr = dynamic_cast<AUDIO::Audio*>(entity.get());
                    *audio_Uptr.get() = *audio_ptr;
                    audio_is_constructed = true;
                    break;
                    }
                }

                auto const& artist = audio_Uptr->artist()->data_model_entity();

                if (artist == nullptr){
                    continue;
                }

                if (artist->tableName() == entity->tableName())
                {
                    auto artist_ptr = dynamic_cast<AUDIO::Artist*>(entity.get());
                    auto artist_uptr = std::make_unique<AUDIO::Artist>();
                    *artist_uptr.get() = *artist_ptr;
                    audio->artist()->set_fk_entity(std::move(artist_uptr));
                }
              }
        }

        if (audio_Uptr->audio_type()->value() != ""){
            m_audio_edm->add_entity(std::move(audio_Uptr));
        }
    }

    show_data();

  }


   void AudioDataModel::show_data()
   {
    m_viewer->clear();
    m_lib_item->clear();
    m_viewer->create_track_view_headers();

    for(auto&[name, entity] : m_audio_edm->modelEntities())
    {
        AUDIO::Audio* audio = dynamic_cast<AUDIO::Audio*>(entity.get());

        if (audio->audio_type()->displayName() == "Song")
            m_lib_item->create_row_item<AUDIO::SongAudioLibItem>(audio);

        if (audio->audio_type()->displayName() == "Jingle")
            m_lib_item->create_row_item<AUDIO::JingleAudioLibItem>(audio);

        if (audio->audio_type()->displayName() == "Commercial")
            m_lib_item->create_row_item<AUDIO::CommercialAudioLibItem>(audio);

    }

   }


}


