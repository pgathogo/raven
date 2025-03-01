#include <chrono>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

#include "playmodepanel.h"

using namespace std::chrono_literals;

namespace OATS{

    PlayModePanel::PlayModePanel(QWidget *parent)
        : QWidget(parent)
        ,m_play_mode_label{nullptr}
        ,m_btn_go_current{nullptr}
        ,m_btn_keep_current{nullptr}
        ,m_layout{nullptr}
        ,m_status_flash_timer{nullptr}
        ,m_fading_timer{nullptr}
    {
        QString text_style = "QLabel{"
                             "color:#FFFFFF; "
                             "background-color:transparent;"
                             "font-size: 8pt;"
                             "font-weight: bold;}";

        const QString status_label_style = "QLabel{"
                                          "color:#FFFFFF; "
                                          "background-color:transparent;"
                                         "font-size: 24pt;"
                                          "font-weight: bold;}";

        m_play_mode_label = std::make_unique<QLabel>("Play Mode:");
        m_play_mode_label->setStyleSheet(text_style);

        m_output_status_label = std::make_unique<QLabel>("");
        m_output_status_label->setStyleSheet(status_label_style);
        m_status_flash_timer = std::make_unique<QTimer>(this);
        connect(m_status_flash_timer.get(), &QTimer::timeout, this, &PlayModePanel::flash_status_label);

        m_fading_timer = std::make_unique<QTimer>(this);

        m_btn_go_current = std::make_unique<QPushButton>("Go Current");
        m_btn_go_current->setObjectName("go_current");

        // m_btn_go_current->setStyleSheet({
        //    "background-color:#40464C;"
        // });
        QString btn_style(
        "QPushButton{background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 #555D64 , stop:1 #374148 );"
        "border-radius: 20px;"
        "border-style: outset;"
        "border-bottom-width: 1px;"
        "border-bottom-color:#0479B0;"
        "border-left-color:#0479B0;"
        "border-right-color:#0479B0;"
        "border-radius: 3px;"
        "color:#FFFFFF;"
       "font-weight:bold;"
        "height: 50px;"
        "width: 100px;}"
       "QPushButton:hover{background-color:#555D64;border:none; }"
       "QPushButton:pressed {"
        "background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 #555D64 , stop:1 #555D64 );"
        "border-radius: 20px;"
        "border-style: outset;"
        "border-bottom-width: 1px;"
        "border-bottom-color:#374148;"
        "border-radius: 3px;"
        "color:#FFFFFF;"
       "font-weight:bold;}"
       "QPushButton:checked {"
        "background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 #454500 , stop:1 #777700 );"
        "border-radius: 20px;"
        "border-style: outset;"
        "border-bottom-width: 1px;"
        "border-radius: 3px;"
        "color:#FFFFFF;"
       "font-weight:bold;}"
         );


        m_btn_keep_current = std::make_unique<QPushButton>("Keep Current");
        m_btn_keep_current->setCheckable(true);

        m_btn_go_current->setStyleSheet(btn_style);
        m_btn_keep_current->setStyleSheet(btn_style);

        connect(m_btn_go_current.get(), &QPushButton::clicked, this, &PlayModePanel::go_current_clicked);
        connect(m_btn_keep_current.get(), &QPushButton::clicked, this, &PlayModePanel::keep_current_clicked);

        m_layout = new QHBoxLayout();

        m_layout->addWidget(m_play_mode_label.get());
        m_layout->addWidget(m_output_status_label.get());
        m_layout->addWidget(m_btn_go_current.get());
        m_layout->addWidget(m_btn_keep_current.get());

        // Stretch
        //[ PLAY MODE        ] [   OUTPUT STATUS                 ] [GO-CURRENT][KEEP-CURRENT]
        m_layout->setStretch(0, 1);
        m_layout->setStretch(1, 2);
        m_layout->setStretch(2, 0);
        m_layout->setStretch(3, 0);

        setLayout(m_layout);
    }

    PlayModePanel::~PlayModePanel()
    {
        delete m_layout;
    }

    void PlayModePanel::go_current_clicked()
    {
        emit go_current();
    }

    void PlayModePanel::keep_current_clicked()
    {
        emit keep_current(m_btn_keep_current->isChecked());
    }

    QPushButton* PlayModePanel::go_current_button()
    {
        return m_btn_go_current.get();
    }

    bool PlayModePanel::keep_current_checked()
    {
        return m_btn_keep_current->isChecked();
    }

    void PlayModePanel::update_ouput_status_label(OATS::PlayMode pm, const QString target)
    {
        m_output_status_label->setText("");
        m_status_flash_timer->stop();

        switch(pm)
        {
        case PlayMode::Stop:
            m_output_status_label->setText("");
            m_status_flash_timer->stop();
            break;

        case PlayMode::Playing:
            m_output_status_label->setText(QString(" PLAYING - %1 ").arg(target));
            set_playing_status_style();
            break;

        case PlayMode::Paused:
            m_output_status_label->setText(QString(" %1 - PAUSED ").arg(target));
            auto FLASH_INTERVAL = 400ms;
            m_status_flash_timer->start(FLASH_INTERVAL);
            break;

        case PlayMode::Fade:
            m_output_status_label->setText(QString(" Fading - %1...").arg(target));
            set_fading_status_style();
            break;
        }
    }

    void PlayModePanel::set_playing_status_style()
    {
        // https://stackoverflow.com/questions/76748476/how-to-change-text-color-of-qlabel-using-qanimation

        QString green =  "color:#00ff00; ";

        QString status_style = "QLabel{"
                               "%1"
                               "background-color:transparent;"
                               "font-size: 24pt;"
                               "font-weight: bold;}";

        m_output_status_label->setStyleSheet(status_style.arg(green));
    }

    void PlayModePanel::set_fading_status_style()
    {
        QString magenta = "color:#aa00ff; ";

        QString status_style = "QLabel{"
                               "%1"
                               "background-color:transparent;"
                               "font-size: 24pt;"
                               "font-weight: bold;}";

        m_output_status_label->setStyleSheet(status_style.arg(magenta));

        connect(m_fading_timer.get(), &QTimer::timeout, this, [&](){
            m_output_status_label->setText("");
            m_fading_timer->stop();
         ;});

        auto FADE_DURATION = 5000;
        m_fading_timer->start(FADE_DURATION);
    }

    void PlayModePanel::flash_status_label()
    {

         QString white = "color:#FFFFFF; ";
         QString yellow = "color:#ffff00; ";

        QString status_style = "QLabel{"
                               "%1"
                               "background-color:transparent;"
                               "font-size: 24pt;"
                               "font-weight: bold;}";

        if (m_stop_status_flash) {
            m_output_status_label->setStyleSheet(status_style.arg(white));
            m_stop_status_flash = false;
        } else {
            m_output_status_label->setStyleSheet(status_style.arg(yellow));
            m_stop_status_flash = true;
        }

    }


}
