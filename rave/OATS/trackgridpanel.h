#pragma once

#include <memory>

#include <QLabel>
#include <QAction>

#include "gridpanel.h"

class QContextMenuEvent;
class QMouseEvent;
class QGridLayout;

namespace OATS{

    class ScheduleItem;

    class TrackGridPanel : public GridPanel
    {
        Q_OBJECT
    public:
        TrackGridPanel(QWidget*);
        ~TrackGridPanel() override;
        void update(OATS::ScheduleItem* schedule_item) override;
        void update_artist(const QString);
    protected:
        void contextMenuEvent(QContextMenuEvent* event) override;
//        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;

    private slots:
        void move_up();
        void move_down();
        void make_audio_current();
        void delete_item();
        void reload_schedule();


    private:
        QGridLayout* m_layout;
        std::unique_ptr<QLabel> m_track_label;
        std::unique_ptr<QLabel> m_artist_label;
        std::unique_ptr<QLabel> m_duration_label;

        std::unique_ptr<QAction> m_act_move_up;
        std::unique_ptr<QAction> m_act_move_down;
        std::unique_ptr<QAction> m_act_make_current;
        std::unique_ptr<QAction> m_act_delete_item;
        std::unique_ptr<QAction> m_act_reload_schedule;

        OATS::ScheduleItem* m_schedule_item;
    };
}
