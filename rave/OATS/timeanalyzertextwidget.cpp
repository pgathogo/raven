
#include <QHBoxLayout>
#include <QLabel>

#include "timeanalyzertextwidget.h"


namespace OATS
{

    TimeAnalyzerTextWidget::TimeAnalyzerTextWidget()
        :m_lbl_music_remain{nullptr}
        ,m_lbl_hour_remain{nullptr}
        ,m_lbl_talk_remain{nullptr}
    {
        m_main_layout = new QHBoxLayout();
        make_widget();
    }

    TimeAnalyzerTextWidget::~TimeAnalyzerTextWidget()
    {
        delete m_main_layout;
    }

    void TimeAnalyzerTextWidget::make_widget()
    {
        QString text_style = "QLabel{"
                             "color:#FFFFFF; "
                             "background-color:transparent;"
                             "font-size: 12pt;}";

        QLabel* music_remain_title = new QLabel("Music Remaining:");
        music_remain_title->setStyleSheet(text_style);

        m_lbl_music_remain = new QLabel("00:00");
        //m_lbl_music_remain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_lbl_music_remain->setAlignment(Qt::AlignCenter);

        m_lbl_music_remain->setStyleSheet("QLabel{"
                                          "color:#F8153B;"
                                          "font-weight: bold; "
                                          "font-size: 18pt;"
                                          "background-color:transparent;}");

        // QFont font_music_remain("Arial", 16, QFont::Bold);
        // m_lbl_music_remain->setFont(font_music_remain);

        // Hour Remainining
        QLabel* hour_remain_title = new QLabel("Hour Remaining: ");
        hour_remain_title->setStyleSheet(text_style);

        m_lbl_hour_remain = new QLabel("00:00");
        m_lbl_hour_remain->setAlignment(Qt::AlignCenter);
        m_lbl_hour_remain->setStyleSheet("QLabel{color:#F56D4D; font-weight: bold; font-size: 18pt;"
                                         "background-color:transparent;}");

        // QFont font_hour_remain("Arial", 16, QFont::Bold);
        // m_lbl_hour_remain->setFont(font_hour_remain);

        //QGridLayout* l_grid = new QGridLayout();
        QHBoxLayout* l_hlay = new QHBoxLayout();

        //Talk remaining
        QLabel* talk_remain_title = new QLabel("Talk Time:");
        talk_remain_title->setStyleSheet(text_style);

        m_lbl_talk_remain = new QLabel("00:00");
        m_lbl_talk_remain->setAlignment(Qt::AlignCenter);
        m_lbl_talk_remain->setStyleSheet("QLabel{color:#FECC5D; font-weight: bold; font-size: 18pt;"
                                         "background-color:transparent;}");

        // QFont font_talk_remain("Arial",16, QFont::Bold);
        // m_lbl_talk_remain->setFont(font_talk_remain);

        l_hlay->addWidget(music_remain_title);
        l_hlay->addWidget(m_lbl_music_remain);

        auto spacerb = new QSpacerItem(50,0, QSizePolicy::Expanding, QSizePolicy::Fixed);
        l_hlay->addSpacerItem(spacerb);
        l_hlay->addWidget(hour_remain_title);
        l_hlay->addWidget(m_lbl_hour_remain);

        // l_grid->addWidget(music_remain_title, 0, 0);
        // l_grid->addWidget(hour_remain_title, 1, 0);

        // l_grid->addWidget(m_lbl_music_remain, 0, 1);
        // l_grid->addWidget(m_lbl_hour_remain, 1, 1);

       // QHBoxLayout* l_talk = new QHBoxLayout();
       // l_talk->addWidget(talk_remain_title);
       // l_talk->addWidget(m_lbl_talk_remain);
       // l_talk->setContentsMargins(30,0,0,0);

        auto spacerc = new QSpacerItem(50,0, QSizePolicy::Expanding, QSizePolicy::Fixed);
        l_hlay->addSpacerItem(spacerc);
        l_hlay->addWidget(talk_remain_title);
        l_hlay->addWidget(m_lbl_talk_remain);

        m_main_layout->addLayout(l_hlay);
        //m_main_layout->addLayout(l_grid);
        // m_main_layout->addLayout(l_talk);

        // auto h_spacer = new QSpacerItem(0,0, QSizePolicy::Expanding, QSizePolicy::Fixed);
        // m_main_layout->addSpacerItem(h_spacer);

        setLayout(m_main_layout);
    }


    void TimeAnalyzerTextWidget::update_stats(std::tuple<QString, QString, QString> stats)
    {
        auto [music, hour, talk] = stats;

        m_lbl_music_remain->setText(music);
        m_lbl_hour_remain->setText(hour);
        m_lbl_talk_remain->setText(talk);
    }


}
