
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
        QLabel* music_remain_title = new QLabel("Music Remaining:");
        music_remain_title->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

        m_lbl_music_remain = new QLabel("00:00");
        m_lbl_music_remain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_lbl_music_remain->setFrameStyle(QFrame::Panel| QFrame::Sunken);
        m_lbl_music_remain->setAlignment(Qt::AlignCenter);

        QFont font_music_remain("Arial", 16, QFont::Bold);
        m_lbl_music_remain->setFont(font_music_remain);

        // Hour Remainining
        QLabel* hour_remain_title = new QLabel("Hour Remaining: ");
        hour_remain_title->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

        m_lbl_hour_remain = new QLabel("00:00");
        m_lbl_hour_remain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_lbl_hour_remain->setFrameStyle(QFrame::Panel| QFrame::Sunken);
        m_lbl_hour_remain->setAlignment(Qt::AlignCenter);

        QFont font_hour_remain("Arial", 16, QFont::Bold);
        m_lbl_hour_remain->setFont(font_hour_remain);

        QGridLayout* l_grid = new QGridLayout();

        l_grid->addWidget(music_remain_title, 0, 0);
        l_grid->addWidget(hour_remain_title, 1, 0);

        l_grid->addWidget(m_lbl_music_remain, 0, 1);
        l_grid->addWidget(m_lbl_hour_remain, 1, 1);

        //Talk remaining

        QLabel* talk_remain_title = new QLabel("Talk\nTime:");

        m_lbl_talk_remain = new QLabel("00:00");
        m_lbl_talk_remain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_lbl_talk_remain->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        m_lbl_talk_remain->setAlignment(Qt::AlignCenter);

        QFont font_talk_remain("Arial",16, QFont::Bold);
        m_lbl_talk_remain->setFont(font_talk_remain);

        QHBoxLayout* l_talk = new QHBoxLayout();
        l_talk->addWidget(talk_remain_title);
        l_talk->addWidget(m_lbl_talk_remain);
        l_talk->setContentsMargins(30,0,0,0);

        m_main_layout->addLayout(l_grid);
        m_main_layout->addLayout(l_talk);

        auto h_spacer = new QSpacerItem(0,0, QSizePolicy::Expanding, QSizePolicy::Fixed);
        m_main_layout->addSpacerItem(h_spacer);

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
