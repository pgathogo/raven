#pragma once

#include <QFrame>

class QWidget;
class QTime;

namespace OATS{

    class ScheduleItem;

    struct TimeFmt{
        long hr;
        long min;
        long sec;
        long msec;
    };

    class GridPanel : public QFrame
    {
    public:
        GridPanel(QWidget*);
        virtual ~GridPanel();
        virtual void update(OATS::ScheduleItem* schedule_item) = 0;
        int index();
        int parent_index();
        int parent_ref();

        QWidget* parent();
        TimeFmt dur_to_tfmt(long);
    private:
        QWidget* m_parent;
    };

}

