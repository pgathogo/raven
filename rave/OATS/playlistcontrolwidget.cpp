#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

#include "playlistcontrolwidget.h"

namespace OATS {

    PlayListControlWidget::PlayListControlWidget(QWidget *parent)
        :QWidget(parent)
    {
        m_title = std::make_unique<QLabel>("Playlist Mode:");
        m_go_current = std::make_unique<QPushButton>("Go Current");
        m_go_current->setStyleSheet(
            "background-color:#00A36C;"
            "color: #FFFFFF;"
            "font:bold;"
            );
        connect(m_go_current.get(), &QPushButton::clicked, this, &PlayListControlWidget::go_current_clicked);

        m_keep_current = std::make_unique<QPushButton>("Keep Current");
        connect(m_keep_current.get(), &QPushButton::clicked, this, &PlayListControlWidget::keep_current_clicked);

        m_main_layout = new QHBoxLayout();

        m_main_layout->addWidget(m_title.get());
        m_main_layout->addWidget(m_go_current.get());
        m_main_layout->addWidget(m_keep_current.get());
        m_main_layout->addStretch(2);

        setLayout(m_main_layout);
    }

    void PlayListControlWidget::style_button(const QString style)
    {
       m_go_current->setStyleSheet(style);

        setStyleSheet(style);

    }

    PlayListControlWidget::~PlayListControlWidget()
    {
        delete m_main_layout;
    }

    void PlayListControlWidget::go_current_clicked()
    {
        emit go_current();
    }
    void PlayListControlWidget::keep_current_clicked()
    {
        emit keep_current();
    }

}
