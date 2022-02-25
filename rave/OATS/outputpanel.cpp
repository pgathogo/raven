#include <QLabel>
#include <QFont>
#include <QString>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressBar>

#include "scheduleitem.h"
#include "outputpanel.h"

namespace OATS{

    OutputPanel::OutputPanel(QString name)
        :m_panel_name{name}
        ,m_status{OATS::PanelStatus::WAITING}
    {
            //"border: 1px solid #9E9E9E;"
        QString btn_style(
            "background-color: #222222;"
            "height: 20;"
            "width:100;"
            "font-family: Roboto;"
            "font-size:12px;"
            "font:bold;"
            "color:#FFFFFF;"
            );

        setFrameShape(QFrame::Panel);
        setFrameShadow(QFrame::Raised);

        m_play_button = new QPushButton("PLAY "+name);
        m_play_button->setObjectName("PlayButton");
        connect(m_play_button, &QPushButton::clicked, this, &OutputPanel::play);

        m_play_button->setStyleSheet(btn_style);

        m_stop_button = new QPushButton("STOP "+name);
        m_stop_button->setObjectName("StopButton");
        connect(m_stop_button, &QPushButton::clicked, this, &OutputPanel::stop);

                        //"border:1px solid #FF0000;"
        m_stop_button->setStyleSheet(
                        "background-color: #222222;"
                        "height:20;"
                        "width:100;"
                        "font-family: Roboto;"
                        "font-size:12px;"
                        "font:bold;"
                        "color:#FFFFFF;"
                         );

        m_status_image = new QLabel("img");
        m_layout_buttons = new QHBoxLayout();

        m_layout_buttons->addWidget(m_play_button);
        m_layout_buttons->addWidget(m_stop_button);
        m_layout_buttons->addWidget(m_status_image);
        m_layout_buttons->addStretch(1);

        QFont f_title("JetBrains Mono", 14, QFont::Bold);

        m_title = new QLabel("Title "+name);
        m_title->setObjectName("TitleLabel");
        m_title->setFont(f_title);
        m_title->setAlignment(Qt::AlignHCenter);
        m_layout_title = new QHBoxLayout();
        m_layout_title->addWidget(m_title);

        QFont f_artist("JetBrains Mono", 10, QFont::Bold);
        f_artist.setBold(true);
        f_artist.setPointSize(10);

        m_artist = new QLabel("Artist "+name);
        m_artist->setObjectName("OutputArtistLabel");
        m_artist->setFont(f_artist);
        m_artist->setAlignment(Qt::AlignHCenter);
        m_layout_artist = new QHBoxLayout();
        m_layout_artist->addWidget(m_artist);

        m_time = new QLabel("00:00 / 00:00 / 00:00");
        m_time->setAlignment(Qt::AlignHCenter);
        m_time->setFont(QFont("DigifaceWide", 20));

        m_progress_bar = new QProgressBar();
//        m_progress_bar->setLayoutDirection(Qt::RightToLeft);
        m_progress_bar->setTextVisible(true);
        m_progress_bar->setFormat("00:00 / 00:00 / 00:00");
        m_progress_bar->setFont(QFont("DigifaceWide", 20));
        //m_progress_bar->setStyleSheet("background-color:#40D58A; border: 1px solid grey; border-radius: 5px;"
//        m_progress_bar->setStyleSheet("background-color:#40D58A; border: 0px solid grey; border-radius: 0px;"
//                                      "text-align: center;font: 75 italic 14pt 'DigifaceWide'; ");
        m_progress_bar->setStyleSheet("background-color:#212121;border: 0px solid; border-radius: 1px;"
                                      "text-align: center;font: 75 italic 14pt 'DigifaceWide';color:#FF0000;");
        m_progress_bar->setMinimum(0);
        m_progress_bar->setMaximum(100);
        m_progress_bar->setValue(0);
        //m_progress_bar->setAlignment(Qt::AlignCenter);

        m_layout_time = new QHBoxLayout();
//        m_layout_time->addWidget(m_time);
        m_layout_time->addWidget(m_progress_bar);

        m_main_panel = new QVBoxLayout();
        m_main_panel->addLayout(m_layout_buttons);
        m_main_panel->addLayout(m_layout_title);
        m_main_panel->addLayout(m_layout_artist);
        m_main_panel->addLayout(m_layout_time);

        setLayout(m_main_panel);

    }

    OutputPanel::~OutputPanel()
    {
        delete m_play_button;
        delete m_stop_button;
        delete m_status_image;
        delete m_title;
        delete m_artist;
        delete m_time;
        delete m_progress_bar;

        delete m_layout_buttons;
        delete m_layout_title;
        delete m_layout_artist;
        delete m_layout_time;

        //delete m_schedule_item;

        delete m_main_panel;
    }

    QLabel* OutputPanel::title()
    {
        return m_title;
    }

    QLabel* OutputPanel::artist()
    {
        return m_artist;
    }

    QLabel* OutputPanel::time()
    {
        return m_time;
    }

    OATS::PanelStatus OutputPanel::panel_status()
    {
        return m_status;
    }

    void OutputPanel::set_panel_status(OATS::PanelStatus p_status)
    {
        m_status = p_status;
    }

    QString OutputPanel::panel_status_text()
    {
        QString text{""};

        switch(m_status)
        {
         case OATS::PanelStatus::WAITING:
            text = "WAITING";
            break;
         case OATS::PanelStatus::CUED:
            text = "CUED";
            break;
         case OATS::PanelStatus::PLAYING:
            text = "PLAYING";
            break;
         case OATS::PanelStatus::STOP:
            text = "STOP";
            break;
         default:
            text = "NONE";
            break;
        }

        return text;
    }

    void OutputPanel::cue_item(ScheduleItem* s_item)
    {
        m_title->setText(QString::fromStdString(s_item->audio().title()));
        m_artist->setText(QString::fromStdString(s_item->audio().artist()));
        m_schedule_item = s_item;
    }

    void OutputPanel::play()
    {
        // play cued item
        //emit play_item(m_id, m_schedule_item);
        emit play_item(this);
        m_status = OATS::PanelStatus::PLAYING;
        set_panel_style();
    }

    void OutputPanel::stop()
    {
        // stop audio play
        //emit stop_play(m_id, m_schedule_item);
        emit stop_play(this);
        m_status = OATS::PanelStatus::STOP;
        set_panel_style();
    }

    int OutputPanel::id() const
    {
        return m_id;
    }

    void OutputPanel::set_id(int i)
    {
        m_id = i;
    }

    QString OutputPanel::panel_name()
    {
        return m_panel_name;
    }

    void OutputPanel::set_panel_style()
    {
        setStyleSheet("");

        if (m_status == OATS::PanelStatus::PLAYING){
            setStyleSheet("border-color: rgb(78, 207, 42)");
        }
    }

    QTime OutputPanel::play_start_time()
    {
        return m_play_start_time;
    }

    void OutputPanel::set_play_start_time(QTime time)
    {
        m_play_start_time = time;
    }

    long OutputPanel::time_remaining()
    {
        return m_time_remaining;
    }
    void OutputPanel::set_time_remaining(long time)
    {
        m_time_remaining = time;
    }

    long long OutputPanel::start_tick_stamp()
    {
        return m_start_tick_stamp;
    }

    void OutputPanel::set_start_tick_stamp(long long stamp)
    {
        m_start_tick_stamp = stamp;
    }

    long long OutputPanel::start_trigger_tick_stamp()
    {
        return m_start_trigger_tick_stamp;
    }
    void OutputPanel::set_start_trigger_tick_stamp(long long time)
    {
        m_start_trigger_tick_stamp = time;
    }

    long OutputPanel::fade_trigger_tick_stamp()
    {
        return m_fade_trigger_tick_stamp;
    }

    void OutputPanel::set_fade_trigger_tick_stamp(long long time)
    {
        m_fade_trigger_tick_stamp = time;
    }

    OATS::ScheduleItem* OutputPanel::schedule_item()
    {
        return m_schedule_item;
    }

    void OutputPanel::slow_flash_play_button()
    {
        if (!m_slow_flash_bit){
            m_play_button->setStyleSheet("background-color: #00C853");
            m_slow_flash_bit = true;
        }else{
            m_play_button->setStyleSheet("");
            m_slow_flash_bit = false;
        }
    }

    void OutputPanel::slow_flash_stop_button()
    {
        if (!m_stop_slow_flash_bit){
            m_stop_button->setStyleSheet("background-color: #FF0000");
            m_stop_slow_flash_bit = true;
        } else {
            m_stop_button->setStyleSheet("");
            m_stop_slow_flash_bit = false;
        }
    }

    void OutputPanel::fast_flash_play_button(OATS::ButtonFlashColor bfc)
    {
        QString flash_color{""};

        switch(bfc)
        {
        case OATS::ButtonFlashColor::NONE:
            break;
        case OATS::ButtonFlashColor::GREEN:
            flash_color = "background-color: rgb(170, 255, 0)";
            break;
        case OATS::ButtonFlashColor::ORANGE:
            flash_color =  "background-color: rgb(255, 195, 0)";
            break;
        case OATS::ButtonFlashColor::RED:
            flash_color = "background-color: rgb(255, 0, 0)";
            break;
        default:
            flash_color = "";
        }

        if (!m_fast_flash_on){
            m_play_button->setStyleSheet(flash_color);
            m_fast_flash_on = true;
        }else {
            m_play_button->setStyleSheet("");
            m_fast_flash_on = false;
        }
    }

    void OutputPanel::fast_flash_stop_button()
    {
        if (!m_stop_fast_flash_bit){
            m_stop_button->setStyleSheet(color_red);
            m_stop_fast_flash_bit = true;
        } else {
            m_stop_button->setStyleSheet("");
            m_stop_fast_flash_bit = false;
        }
    }

    /*
    long TimeStamp::time_remaining_ticks(OutputPanel* op)
    {
        return op->m_time_remaining;
    }
    */

    void OutputPanel::reset_play_button()
    {
        m_play_button->setStyleSheet("");
    }

    void OutputPanel::reset_stop_button()
    {
        m_stop_button->setStyleSheet("");
    }

    void OutputPanel::update_progress_bar(int value)
    {
        m_progress_bar->setValue(value);
    }

    void OutputPanel::set_cue_time_string(QString cue_string)
    {
        m_cue_time_string = cue_string;
        m_progress_bar->setFormat(cue_string);
    }

    QString OutputPanel::cue_time_string()
    {
        return m_cue_time_string;
    }

    void OutputPanel::set_progress_bar_background(OATS::ProgressBarBGColor bg_color)
    {
        switch(bg_color)
        {
        case OATS::ProgressBarBGColor::RED:
            //m_progress_bar->setStyleSheet("QProgressBar{background-color:#F51503; border: 0px solid grey;"
            m_progress_bar->setStyleSheet("QProgressBar{background-color:#212121;border: 0px solid grey;"
                                          "border-radius: 0px; text-align: center; text-align: center;"
                                          "font: 75 italic 14pt 'DigifaceWide';color:#FF0000;} "
                                          "QProgressBar::chunk{background-color:#00C853;}");
            break;
        case OATS::ProgressBarBGColor::BLUE:
            //m_progress_bar->setStyleSheet("QProgressBar{background-color:#0457E8; border: 0px solid grey; border-radius: 0px; "
            m_progress_bar->setStyleSheet("QProgressBar{background-color:#212121;border: 0px solid grey; border-radius: 0px; "
                                          "text-align: center;text-align: center;"
                                          "font: 75 italic 14pt 'DigifaceWide';color:#FF0000;}"
                                          "QProgressBar::chunk{background-color:#00C853;}");
            break;
        case OATS::ProgressBarBGColor::GREEN:
            //m_progress_bar->setStyleSheet("QProgressBar{background-color:#04E846; border: 0px solid grey;"
            m_progress_bar->setStyleSheet("QProgressBar{background-color:#212121;border: 0px solid grey;"
                                          "border-radius: 0px; text-align: center;text-align: center;"
                                          "font: 75 italic 14pt 'DigifaceWide';color:#FF0000;}"
                                          "QProgressBar::chunk{background-color:#00C853;}");
        default:
            //green
            m_progress_bar->setStyleSheet("QProgressBar{background-color:#04E846; border: 0px solid grey;"
                                          "border-radius: 0px; text-align: center;text-align: center;"
                                          "font: 75 italic 14pt 'DigifaceWide';color:#FF0000;}"
                                          "QProgressBar::chunk{background-color:#00C853;}");
        }

    }

}
