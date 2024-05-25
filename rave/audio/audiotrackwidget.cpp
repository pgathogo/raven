#include <QVBoxLayout>

#include "audiotrackwidget.h"
#include "audiotrackwidgettoolbar.h"

namespace AUDIO
{
   AudioTrackWidget::AudioTrackWidget()
       :m_toolbar{nullptr}
       ,m_track_viewer{nullptr}
       ,m_v_layout{nullptr}
   {
        m_v_layout = std::make_unique<QVBoxLayout>();
        m_v_layout->setContentsMargins(0,0,0,0);

        m_toolbar = std::make_unique<AudioTrackWidgetToolbar>();
        m_track_viewer = std::make_unique<AudioTrackViewer>();

        m_v_layout->addWidget(m_toolbar.get());
        m_v_layout->addWidget(m_track_viewer.get());

        setLayout(m_v_layout.get());
   }

   AudioTrackViewer* AudioTrackWidget::track_viewer()
   {
       return m_track_viewer.get();
   }

   void AudioTrackWidget::track_selected(int track_id)
   {
       emit selected_track(track_id);
   }

   int AudioTrackWidget::selected_audio_id()
   {
       return m_track_viewer->get_selected_audio_id();
   }


}
