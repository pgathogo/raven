#pragma once

#include <memory>
#include <QWidget>

#include "subjectobserver.h"

class QWidget;
class QGridLayout;
class QBoxLayout;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QContextMenuEvent;
class QPushButton;
class QProgressBar;

namespace OATS{

    class ScheduleItem;
    class TimeGridPanel;
    class TrackGridPanel;
    class StatusGridPanel;
    class TransitionModeGridPanel;

    enum class ItemStatus;

    class ScheduleGridItem : public QWidget, public OATS::Observer
    {
        Q_OBJECT
    public:

        ScheduleGridItem(ScheduleItem*);
        ~ScheduleGridItem();

        QHBoxLayout* main_layout();
        int grid_index();
        int subject_index();
        void set_subject(OATS::Subject*);
        void update(OATS::Subject* changed_subject) override;
        std::string format_message(Message message) override;
        std::string name() override;

        void set_item_index(int);
        ScheduleItem* schedule_item();
        void refresh();
        int subject_ref();

        OATS::ItemStatus item_status();
        void set_item_status(ItemStatus);

    signals:
        void move_up(int, int);
        void move_down(int, int);
        void delete_item(int);
        void insert_item(int, int);
        void make_current(int, int);

        void transition_stop(int, int);
        void transition_mix(int, int);
        void transition_cut(int, int);

    private:
        OATS::ItemStatus m_item_status;
        QHBoxLayout* m_layout;
        std::unique_ptr<OATS::TimeGridPanel> m_time_panel;
        std::unique_ptr<OATS::TrackGridPanel> m_track_panel;
        std::unique_ptr<OATS::StatusGridPanel> m_status_panel;
        std::unique_ptr<OATS::TransitionModeGridPanel> m_transition_mode_panel;

        ScheduleItem* m_schedule_item;

        int m_grid_index;
        int m_subject_index;

        static int g_index;
    };

}
