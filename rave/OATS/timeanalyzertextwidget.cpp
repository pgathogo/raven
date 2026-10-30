
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>

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

        // QLabel* music_remain_title = new QLabel("Music Remaining:");
        // music_remain_title->setStyleSheet(text_style);



        m_lbl_music_remain = new QLabel("00:00");
        //m_lbl_music_remain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_lbl_music_remain->setAlignment(Qt::AlignCenter);

        m_lbl_music_remain->setStyleSheet("QLabel{"
                                          "color:#F8153B;"
                                          "font-weight: bold; "
                                          "font-size: 25pt;"
                                          "background-color:transparent;}");

        // QFont font_music_remain("Arial", 16, QFont::Bold);
        // m_lbl_music_remain->setFont(font_music_remain);

        // Hour Remainining
        // QLabel* hour_remain_title = new QLabel("Hour Remaining: ");
        // hour_remain_title->setStyleSheet(text_style);

        m_lbl_hour_remain = new QLabel("00:00");
        m_lbl_hour_remain->setAlignment(Qt::AlignCenter);
        m_lbl_hour_remain->setStyleSheet("QLabel{color:#F56D4D; font-weight: bold; font-size: 25pt;"
                                         "background-color:transparent;}");


        QHBoxLayout* l_hlay = new QHBoxLayout();

        //Talk remaining
        // QLabel* talk_remain_title = new QLabel("Talk Time:");
        // talk_remain_title->setStyleSheet(text_style);

        m_lbl_talk_remain = new QLabel("00:00");
        m_lbl_talk_remain->setAlignment(Qt::AlignCenter);
        m_lbl_talk_remain->setStyleSheet("QLabel{color:#FECC5D; font-weight: bold; font-size: 25pt;"
                                         "background-color:transparent;}");

        QGroupBox* gb_music_remain = new QGroupBox("Music Remaining: ");
        //gb_music_remain->setFixedSize(200, 60);
        QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        gb_music_remain->setSizePolicy(policy);
        // gb_music_remain->setMinimumWidth(150);

        QVBoxLayout* vl_music_remain = new QVBoxLayout;
        vl_music_remain->addWidget(m_lbl_music_remain);
        gb_music_remain->setLayout(vl_music_remain);

        /*
        QGroupBox* gb_hour_remain = new QGroupBox("Hour Remaining: ");
        gb_hour_remain->setMinimumWidth(150);
        QVBoxLayout* vl_hour_remain =  new QVBoxLayout;
        vl_hour_remain->addWidget(m_lbl_hour_remain);
        gb_hour_remain->setLayout(vl_hour_remain);

        QGroupBox* gb_talk_time = new QGroupBox("Talk Time: ");
        gb_talk_time->setMinimumWidth(150);
        QVBoxLayout* vl_talk_time = new QVBoxLayout;
        vl_talk_time->addWidget(m_lbl_talk_remain);
        gb_talk_time->setLayout(vl_talk_time);
        */

        l_hlay->addWidget(gb_music_remain);

       // auto spacerb = new QSpacerItem(50,0, QSizePolicy::Expanding, QSizePolicy::Fixed);
       // l_hlay->addSpacerItem(spacerb);

        //l_hlay->addWidget(gb_hour_remain);


        auto spacerc = new QSpacerItem(50,0, QSizePolicy::Expanding, QSizePolicy::Fixed);
        l_hlay->addSpacerItem(spacerc);

        //l_hlay->addWidget((gb_talk_time));

        m_main_layout->addLayout(l_hlay);

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
