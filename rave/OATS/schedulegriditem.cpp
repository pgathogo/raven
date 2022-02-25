#include <QHBoxLayout>
#include "schedulegriditem.h"
#include "scheduleitem.h"
#include "timegridpanel.h"
#include "trackgridpanel.h"
#include "statusgridpanel.h"
#include "transitionmodegridpanel.h"

namespace OATS{

    int ScheduleGridItem::g_index{0};

    ScheduleGridItem::ScheduleGridItem(OATS::ScheduleItem* sched_item)
        :m_schedule_item{sched_item}
        ,m_item_status{OATS::ItemStatus::WAITING}
    {
        m_schedule_item->attach(this);

        m_grid_index = g_index++;

        //setStyleSheet(style);

        m_time_panel = std::make_unique<OATS::TimeGridPanel>(this);
        m_track_panel = std::make_unique<OATS::TrackGridPanel>(this);
        m_status_panel = std::make_unique<OATS::StatusGridPanel>(this);
        m_transition_mode_panel = std::make_unique<OATS::TransitionModeGridPanel>(this);

        m_layout = new QHBoxLayout();
        m_layout->setSpacing(0);
        m_layout->setContentsMargins(0,0,0,0);

        m_layout->addWidget(m_time_panel.get());
        m_layout->addWidget(m_track_panel.get());
        m_layout->addWidget(m_status_panel.get());
        m_layout->addWidget(m_transition_mode_panel.get());

        m_layout->setStretch(0,0);
        m_layout->setStretch(1,1);
        m_layout->setStretch(2,0);
        m_layout->setStretch(3,0);

        setLayout(m_layout);

    }

    ScheduleGridItem::~ScheduleGridItem()
    {
        delete m_layout;
    }

    int ScheduleGridItem::grid_index()
    {
        return m_grid_index;
    }

    int ScheduleGridItem::subject_index()
    {
        return m_schedule_item->index();
    }

    int ScheduleGridItem::subject_ref()
    {
        return m_schedule_item->schedule_ref();
    }

    void ScheduleGridItem::set_subject(Subject* subject)
    {
        update(subject);
    }

    void ScheduleGridItem::refresh()
    {
        update(m_schedule_item);
    }

    void ScheduleGridItem::set_item_index(int idx)
    {
        m_grid_index = idx;
    }

    ScheduleItem* ScheduleGridItem::schedule_item()
    {
        return m_schedule_item;
    }

    void ScheduleGridItem::update(OATS::Subject* changed_subject)
    {
        auto si = dynamic_cast<OATS::ScheduleItem*>(changed_subject);

        m_time_panel->update(si);
        m_track_panel->update(si);
        m_status_panel->update(si);
        m_transition_mode_panel->update(si);

        m_schedule_item = si;
    }

    std::string ScheduleGridItem::format_message(Message message)
    {
        return "";
    }

    std::string ScheduleGridItem::name()
    {
        return "";
    }

    OATS::ItemStatus ScheduleGridItem::item_status()
    {
        return m_item_status;
    }

    void ScheduleGridItem::set_item_status(OATS::ItemStatus i_status)
    {
        m_schedule_item->set_item_status(i_status);
    }
}
