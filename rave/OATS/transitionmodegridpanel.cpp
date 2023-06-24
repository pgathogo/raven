#include <QVBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>

#include "transitionmodegridpanel.h"
#include "scheduleitem.h"
#include "schedulegriditem.h"

namespace OATS{
    TransitionModeGridPanel::TransitionModeGridPanel(QWidget* parent)
        :GridPanel(parent)
        ,m_layout{nullptr}
        ,m_transition_label{nullptr}
    {
        setObjectName("TransitionMode");
        setFixedWidth(60);

        m_layout = new QVBoxLayout();
        m_layout->setSpacing(0);
        m_layout->setContentsMargins(0,0,0,0);

        QFont f_trans("JetBrains Mono", 10, QFont::Bold);

        m_transition_label = std::make_unique<QLabel>("MIX");
        m_transition_label->setObjectName("TransitionModeLabel");
        m_transition_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_transition_label->setFont(f_trans);

        m_layout->addWidget(m_transition_label.get());
        setLayout(m_layout);
    }

    TransitionModeGridPanel::~TransitionModeGridPanel()
    {
        delete m_layout;
    }

    void TransitionModeGridPanel::update(OATS::ScheduleItem* schedule_item)
    {
        m_schedule_item = schedule_item;

        m_transition_label->setText(QString::fromStdString(schedule_item->transition_type_text()));
        setStyleSheet("background-color: #222222;");

        if (schedule_item->transition_type() == OATS::TransitionType::MIX){
            //setStyleSheet("background-color: rgb(40, 133, 220)");
            setStyleSheet("background-color: #87CEEB");
        }

        if (schedule_item->transition_type() == OATS::TransitionType::STOP){
            //setStyleSheet("background-color: rgb(251, 27, 4)");
            setStyleSheet("background-color: #DC143C");
        }

        if (schedule_item->transition_type() == OATS::TransitionType::CUT){
            setStyleSheet("background-color: rgb(220, 135, 40)");
        }

        if (schedule_item->item_status() == OATS::ItemStatus::ERROR_01){
            setStyleSheet("background-color: #fb1d04");
            m_transition_label->setText("SKIP");
        }
    }

    void TransitionModeGridPanel::contextMenuEvent(QContextMenuEvent* event)
    {
        QMenu menu;
        m_act_stop = std::make_unique<QAction>("Stop");
        m_act_mix = std::make_unique<QAction>("Mix");
        m_act_cut = std::make_unique<QAction>("Cut");

        if (m_schedule_item->item_status() == OATS::ItemStatus::ERROR_01){
            m_act_stop->setEnabled(false);
            m_act_mix->setEnabled(false);
            m_act_cut->setEnabled(false);
        }

        connect(m_act_stop.get(), &QAction::triggered, this, &OATS::TransitionModeGridPanel::tran_mode_stop);
        connect(m_act_mix.get(), &QAction::triggered, this, &OATS::TransitionModeGridPanel::tran_mode_mix);
        connect(m_act_cut.get(), &QAction::triggered, this, &OATS::TransitionModeGridPanel::tran_mode_cut);

        menu.addAction(m_act_stop.get());
        menu.addAction(m_act_mix.get());
        menu.addAction(m_act_cut.get());

        menu.exec(event->globalPos());
    }

    void TransitionModeGridPanel::tran_mode_stop()
    {
        emit dynamic_cast<OATS::ScheduleGridItem*>(parent())->transition_stop(parent_ref(), index());
    }

    void TransitionModeGridPanel::tran_mode_mix()
    {
        emit dynamic_cast<OATS::ScheduleGridItem*>(parent())->transition_mix(parent_ref(), index());
    }

    void TransitionModeGridPanel::tran_mode_cut()
    {
        emit dynamic_cast<OATS::ScheduleGridItem*>(parent())->transition_cut(parent_ref(), index());
    }

}
