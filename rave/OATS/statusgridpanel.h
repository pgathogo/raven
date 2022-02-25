#pragma once

#include <memory>

#include "gridpanel.h"

class QVBoxLayout;
class QLabel;

namespace OATS{
    class StatusGridPanel : public GridPanel
    {
        Q_OBJECT
    public:
        StatusGridPanel(QWidget*);
        ~StatusGridPanel() override;
        void update(OATS::ScheduleItem* schedule_item) override;
    private:
        QVBoxLayout* m_layout;
        std::unique_ptr<QLabel> m_status1;
        std::unique_ptr<QLabel> m_status2;
    };
}
