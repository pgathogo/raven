#pragma once

#include <memory>

#include "gridpanel.h"

class QContextMenuEvent;
class QVBoxLayout;
class QLabel;
class QAction;

namespace OATS{
    class ScheduleItem;

    enum class TransitionType;

    class TransitionModeGridPanel : public GridPanel
    {
        Q_OBJECT
    public:
        TransitionModeGridPanel(QWidget*);
        ~TransitionModeGridPanel() override;
        void update(ScheduleItem* schedule_item) override;
    protected:
        void contextMenuEvent(QContextMenuEvent* event);
    private slots:
        void tran_mode_stop();
        void tran_mode_mix();
        void tran_mode_cut();
    private:
        QVBoxLayout* m_layout;
        std::unique_ptr<QLabel> m_transition_label;
        std::unique_ptr<QAction> m_act_stop;
        std::unique_ptr<QAction> m_act_mix;
        std::unique_ptr<QAction> m_act_cut;
    };
}

