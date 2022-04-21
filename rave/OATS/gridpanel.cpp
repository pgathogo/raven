#include <chrono>
#include <sstream>

#include <QTime>

#include "gridpanel.h"
#include "schedulegriditem.h"

namespace OATS{

    GridPanel::GridPanel(QWidget* parent)
        :m_parent{parent}
    {
      setFrameShape(QFrame::Panel);
      setFrameShadow(QFrame::Raised);

      auto size_policy = QSizePolicy();
      size_policy.setHorizontalPolicy(QSizePolicy::Expanding);
      size_policy.setHorizontalStretch(1);
      setSizePolicy(size_policy);
      setLineWidth(1);
      setFixedHeight(50);
    }

    GridPanel::~GridPanel()
    {
    }

    int GridPanel::index()
    {
        return dynamic_cast<ScheduleGridItem*>(m_parent)->grid_index();
    }

    int GridPanel::parent_index()
    {
        return dynamic_cast<ScheduleGridItem*>(m_parent)->subject_index();
    }

    int GridPanel::parent_ref()
    {
        return dynamic_cast<ScheduleGridItem*>(m_parent)->subject_ref();
    }

    QWidget* GridPanel::parent()
    {
        return m_parent;
    }

    QTime GridPanel::msec_to_time(int h, int ms)
    {
        auto msecs = std::chrono::milliseconds(ms);
        auto hr = std::chrono::duration_cast<std::chrono::hours>(msecs);
        msecs -= hr;
        auto mins = std::chrono::duration_cast<std::chrono::minutes>(msecs);
        msecs -= mins;
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(msecs);
        QTime t(h, mins.count(), secs.count());
        return t;
    }

    OATS::TimeFmt GridPanel::dur_to_tfmt(long ms)
    {
        long h = ms / 3600000;
        ms = ms - 3600000 * h;

        long m = ms / 60000;
        ms = ms - 60000 * m;

        long s = ms / 1000;
        ms = ms - 1000 * s;

        TimeFmt tf;
        tf.hr = h;
        tf.min = m;
        tf.sec = s;
        tf.msec = ms;

        return tf;
    }
}
