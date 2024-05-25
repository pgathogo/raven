#include <QHBoxLayout>
#include <QPushButton>

#include "audiotrackwidgettoolbar.h"

namespace AUDIO
{
   AudioTrackWidgetToolbar::AudioTrackWidgetToolbar()
       :m_h_layout{nullptr}
       ,m_btn_listen{nullptr}
       ,m_btn_info{nullptr}
       ,m_btn_history{nullptr}
   {
        layout_controls();
        setLayout(m_h_layout.get());
        style_buttons();
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

   void AudioTrackWidgetToolbar::style_buttons()
   {

    QString style(
        "QPushButton {background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 #4F565D, stop:0.5 #353A3F stop:1 #181B1D );"
        "border-radius: 20px;"
        "border-style: outset;"
        "border-bottom-width: 1px;"
        "border-bottom-color:#374148;"
        "border-radius: 3px;"
        "width: 80px;"
        "height: 25px;"
        "color:#FFFFFF;"
       "font-weight:normal;}"

        "QPushButton:hover{"
            "background-color:#555D64; "
            "border-width:1px;"
            "border-color:#0479B0;"
            " }"

       "QPushButton:pressed {"
        "background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 #555D64 , stop:1 #555D64 );"
        "border-radius: 20px;"
        "border-style: outset;"
        "border-bottom-width: 1px;"
        "border-radius: 3px;"
        "border-color:#0479B0;"
        "color:#FFFFFF;"
       "font-weight:normal;}"

        );

       m_btn_listen->setStyleSheet(style);
       m_btn_info->setStyleSheet(style);
       m_btn_history->setStyleSheet(style);

   }
}
