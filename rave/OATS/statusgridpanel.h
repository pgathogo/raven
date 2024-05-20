#pragma once

#include <memory>

#include <QAction>

#include "gridpanel.h"

class QVBoxLayout;
class QLabel;

class ScheduleItem;

namespace OATS{
    class StatusGridPanel : public GridPanel
    {
        Q_OBJECT
    public:
        StatusGridPanel(QWidget*);
        ~StatusGridPanel() override;
        void update(OATS::ScheduleItem* schedule_item) override;
    protected:
        void contextMenuEvent(QContextMenuEvent*) override;

    private slots:
        void play_audio();

    private:
        QVBoxLayout* m_layout;
        std::unique_ptr<QLabel> m_status1;
        std::unique_ptr<QLabel> m_status2;

        QAction* m_act_play;

        OATS::ScheduleItem* m_schedule_item;

    };
}
