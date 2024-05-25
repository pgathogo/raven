#ifndef AUDIOLIBWIDGET_H
#define AUDIOLIBWIDGET_H

#include <memory>

#include "audiosearchwidget.h"
#include "audiofolderwidget.h"

class QVBoxLayout;
class QTabWidget;

namespace AUDIO {

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

}


#endif
