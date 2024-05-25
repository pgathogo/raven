#include <QVBoxLayout>

#include "audiolibwidget.h"

namespace AUDIO {

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


}
