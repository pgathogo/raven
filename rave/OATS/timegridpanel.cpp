#include <QVBoxLayout>
#include <QMenu>
#include <QContextMenuEvent>
#include <QDebug>

#include "timegridpanel.h"
#include "scheduleitem.h"

namespace OATS{
    TimeGridPanel::TimeGridPanel(QWidget* parent)
        :GridPanel(parent)
    {
        m_layout = new QVBoxLayout();
        m_layout->setSpacing(0);
        m_layout->setContentsMargins(0,0,0,0);

        setObjectName("TimePanel");
        setFixedWidth(80);
        setStyleSheet("QFrame#TimePanel{background-color: #222222;}");

        QFont f("JetBrains Mono", 10, QFont::Bold);
        f.setBold(true);

        //m_label = std::make_unique<QLabel>("00:00:00");
        m_label = std::make_unique<QLabel>("");
        m_label->setObjectName("TimeLabel");
        m_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_label->setFont(f);

        m_layout->addWidget(m_label.get());
        setLayout(m_layout);

    }

    TimeGridPanel::~TimeGridPanel()
    {
        delete m_layout;
    }

    void TimeGridPanel::set_label(QString text)
    {
        m_label->setText(text);
    }

    void TimeGridPanel::time_menu()
    {
        qDebug() << "Time Panel... "<< index();
    }

    void TimeGridPanel::contextMenuEvent(QContextMenuEvent* event)
    {
        QMenu menu(this);
        auto act_time = new QAction("Time Menu");
        connect(act_time, &QAction::triggered, this, &TimeGridPanel::time_menu);
        menu.addAction(act_time);
        menu.exec(event->globalPos());
    }


    void TimeGridPanel::update(OATS::ScheduleItem* schedule_item)
    {

        if (schedule_item->schedule_type() != OATS::ScheduleType::HOUR_HEADER){
            QTime t = msec_to_time(schedule_item->hour(), schedule_item->current_time());
            set_label(t.toString("HH:mm:ss"));
        }
    }

    QVBoxLayout* TimeGridPanel::main_layout()
    {
        return m_layout;
    }
}
