#ifndef AUDIOTRACKWIDGETTOOLBAR_H
#define AUDIOTRACKWIDGETTOOLBAR_H

#include <memory>

#include <QWidget>

class QHBoxLayout;
class QPushButton;

namespace AUDIO
{

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

        void style_buttons();
    };

}

#endif
