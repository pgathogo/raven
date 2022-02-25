#pragma once
#include <memory>
#include <QLabel>

#include <QString>
#include "gridpanel.h"

class QWidget;
class QVBoxLayout;
class QContextMenuEvent;

namespace OATS{

    class ScheduleItem;

    class TimeGridPanel : public GridPanel
    {
        Q_OBJECT
    public:
        TimeGridPanel(QWidget*);
        ~TimeGridPanel() override;

        void set_label(QString);
        void update(OATS::ScheduleItem* schedule_item) override;
        QVBoxLayout* main_layout();
    protected:
        void contextMenuEvent(QContextMenuEvent* event) override;
    private slots:
        void time_menu();
    private:
        std::unique_ptr<QLabel> m_label;
        QVBoxLayout* m_layout;
    };
}
