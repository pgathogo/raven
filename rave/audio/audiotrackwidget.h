#ifndef AUDIOTRACKWIDGET_H
#define AUDIOTRACKWIDGET_H

#include <memory>
#include <QWidget>

#include "audiotrackviewer.h"

class QVBoxLayout;

//class AudioTrackViewer;

namespace AUDIO
{
    class AudioTrackWidgetToolbar;

    class AudioTrackWidget : public QWidget
    {
        Q_OBJECT
    public:
        AudioTrackWidget();
        AudioTrackViewer* track_viewer();
        int selected_audio_id();

    signals:
        void selected_track(int) ;

    private slots:
        void track_selected(int);

    private:
        std::unique_ptr<AudioTrackWidgetToolbar> m_toolbar;
        std::unique_ptr<AudioTrackViewer> m_track_viewer;

        std::unique_ptr<QVBoxLayout> m_v_layout;

    };

}

#endif
