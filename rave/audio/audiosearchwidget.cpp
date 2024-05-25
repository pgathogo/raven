#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLabel>
#include <QPushButton>

#include "audiosearchwidget.h"
#include "audio.h"
#include "artist.h"

namespace AUDIO {

    AudioSearchWidget::AudioSearchWidget()
        :m_grid_layout{nullptr}
        ,m_lbl_title{nullptr}
        ,m_edt_title{nullptr}
    {
        layout_controls();
        setLayout(m_base_layout.get());
    }

   void AudioSearchWidget::layout_controls()
   {
       m_base_layout = std::make_unique<QVBoxLayout>();
       m_button_layout = std::make_unique<QHBoxLayout>();

       m_grid_layout = std::make_unique<QGridLayout>();
       m_lbl_title = std::make_unique<QLabel>("Title: ", this);
       m_edt_title = std::make_unique<QLineEdit>(this);

       m_lbl_title->setStyleSheet("QLabel{color: #FFFFFF; font-weight: bold;}");

       m_lbl_artist = std::make_unique<QLabel>("Artist: ", this);
       m_edt_artist = std::make_unique<QLineEdit>(this);

       m_lbl_artist->setStyleSheet("QLabel{color: #FFFFFF; font-weight: bold;}");

       m_btn_search = std::make_unique<QPushButton>("Search", this);
       m_btn_search->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
       style_search_button();

       connect(m_btn_search.get(), &QPushButton::clicked, this, &AudioSearchWidget::search_clicked);

       m_grid_layout->addWidget(m_lbl_title.get(), ROW0, COL0);
       m_grid_layout->addWidget(m_edt_title.get(), ROW0, COL1);

       m_grid_layout->addWidget(m_lbl_artist.get(), ROW1, COL0);
       m_grid_layout->addWidget(m_edt_artist.get(), ROW1, COL1);

       // auto l_spacer = new QSpacerItem(20,20, QSizePolicy::Expanding, QSizePolicy::Expanding);
       // m_grid_layout->addItem(l_spacer, ROW2, COL0, 1, 1);

       m_button_layout->addWidget(m_btn_search.get());
       m_base_layout->addLayout(m_grid_layout.get());
       m_base_layout->addLayout(m_button_layout.get());

//       auto r_spacer = new QSpacerItem(20,20,QSizePolicy::Expanding, QSizePolicy::Expanding);
//       m_grid_layout->addItem(r_spacer, ROW2, COL2, 1, 1);

   }

   void AudioSearchWidget::style_search_button()
   {
    QString btn_style(
        "QPushButton {background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 #4F565D, stop:0.5 #353A3F stop:1 #181B1D );"
        "border-radius: 20px;"
        "border-style: outset;"
        "border-bottom-width: 1px;"
        "border-bottom-color:#374148;"
        "border-radius: 3px;"
        "width: 100px;"
        "height: 55px;"
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

       m_btn_search->setStyleSheet(btn_style);

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


}
