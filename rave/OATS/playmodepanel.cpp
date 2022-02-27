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
        m_play_mode_label = std::make_unique<QLabel>("Play Mode:");
        m_btn_go_current = std::make_unique<QPushButton>("Go Current");
        m_btn_keep_current = std::make_unique<QPushButton>("Keep Current");
        m_btn_keep_current->setCheckable(true);

        connect(m_btn_go_current.get(), &QPushButton::clicked, this, &PlayModePanel::go_current_clicked);

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

    QPushButton* PlayModePanel::go_current_button()
    {
        return m_btn_go_current.get();
    }

    bool PlayModePanel::keep_current_checked()
    {
        return m_btn_keep_current->isChecked();
    }
}
