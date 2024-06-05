#include <QWidget>
#include <QDebug>


//#include <QPushButton>
// #include <QFileSystemModel>
// #include <QDir>

#include <QGridLayout>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QSpacerItem>

#include "../framework/ravenexception.h"

#include "trackbrowser.h"
#include "audiofolderwidget.h"
#include "audiosearchwidget.h"
#include "audiotrackwidget.h"
#include "audiodatamodel.h"
#include "audio.h"

//#include "artist.h"

#include "audiotrackviewer.h"

namespace AUDIO
{
    TrackBrowser::TrackBrowser(QWidget *parent)
        : QWidget{parent}
    , m_current_selected_audio{nullptr}
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
        set_tab_widget_style();

    }

    void TrackBrowser::set_tab_widget_style()
    {
         m_tab_widget->setStyleSheet("QTabBar::tab{background: qlineargradient(x1:0 y1:0, x2:0 y2:1,"
                                     " stop:0 #47617B, stop:0.5 #2C3C4B, stop:1 #10161C);"
                                    "color:#FFFFFF; font-weight: bold;}"
                                    "QTabBar::tab:selected{"
                                       "background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, "
                                    "stop:0 #454500 , stop:1 #777700 );"
                                    "color:#FFFFFF; font-weight: bold;}"
                                    "QTabWidget::pane{background-color: #34424F; border: 1px solid lightgray;}"
                                    "QTabBar::tab:hover{background-color:#555D64;} "
                                     );

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

        auto folder_filter = std::make_tuple(audio->folder()->qualified_column_name<Audio>(),
                                             "=", folder_id);

        m_audio_data_model->fetch_filtered_audio(folder_filter);
    }

    void TrackBrowser::selected_audio(std::shared_ptr<Audio> audio)
    {
        m_current_selected_audio = audio;
    }

    std::shared_ptr<Audio> TrackBrowser::current_selected_audio()
    {
        //if (m_audio_track_widget->track_viewer()->get_selected_audio_id() < 0 )

        if (m_audio_track_widget->selected_audio_id() < 0 )
            return nullptr;

       return m_current_selected_audio;

    }

    std::shared_ptr<Audio> TrackBrowser::find_audio_by_id(int audio_id)
    {
       auto audio = m_audio_data_model->find_audio_by_id(audio_id);
        return audio;

    }

    void TrackBrowser::search_filter(Filter filter)
    {
        m_audio_data_model->fetch_filtered_audio(filter);
    }


}


