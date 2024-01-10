#include <QLabel>
#include <QPushButton>

#include "playmodepanel.h"

namespace OATS{

    PlayModePanel::PlayModePanel(QWidget *parent)
        : QWidget(parent)
        ,m_play_mode_label{nullptr}
        ,m_btn_go_current{nullptr}
        ,m_btn_keep_current{nullptr}
        ,m_layout{nullptr}
    {
        QString text_style = "QLabel{"
                             "color:#FFFFFF; "
                             "background-color:transparent;"
                             "font-size: 8pt;"
                             "font-weight: bold;}";

        m_play_mode_label = std::make_unique<QLabel>("Play Mode:");
        m_play_mode_label->setStyleSheet(text_style);

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
        m_layout->addWidget(m_btn_go_current.get());
        m_layout->addWidget(m_btn_keep_current.get());

        m_layout->setStretch(0, 1);
        m_layout->setStretch(1, 0);
        m_layout->setStretch(2, 0);

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
}
