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
        void stop_audio();
        void fade_audio();

    private:
        QVBoxLayout* m_layout;
        std::unique_ptr<QLabel> m_item_status;
        std::unique_ptr<QLabel> m_output_name;

        QAction* m_act_play;
        QAction* m_act_stop;
        QAction* m_act_fade;


        OATS::ScheduleItem* m_schedule_item;

    };
}
