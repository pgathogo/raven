#include <QVBoxLayout>
#include <QLabel>
#include <QFont>

#include "statusgridpanel.h"
#include "scheduleitem.h"

namespace OATS{

    StatusGridPanel::StatusGridPanel(QWidget* parent)
        :GridPanel(parent)
    {
        setStyleSheet("QFrame{background-color: #313131;}");
        setFixedWidth(70);

        m_layout = new QVBoxLayout();
        m_layout->setSpacing(0);
        m_layout->setContentsMargins(0,0,0,0);

        setObjectName("StatusPanel");
        //setFixedWidth(80);

        QFont f_statusTop("JetBrains Mono", 10, QFont::Bold);
        m_status1 = std::make_unique<QLabel>("");
        m_status1->setObjectName("StatusTopLabel");
        m_status1->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_status1->setFont(f_statusTop);

        QFont f_statusBottom("JetBrains Mono", 12, QFont::Bold);
        m_status2 = std::make_unique<QLabel>("");
        m_status2->setObjectName("StatusBottomLabel");
        m_status2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_status2->setFont(f_statusBottom);

        m_layout->addWidget(m_status1.get());
        m_layout->addWidget(m_status2.get());
        setLayout(m_layout);

    }

    StatusGridPanel::~StatusGridPanel()
    {
        delete m_layout;
    }

    void StatusGridPanel::update(OATS::ScheduleItem* schedule_item)
    {
        if (schedule_item->schedule_type() == OATS::ScheduleType::HOUR_HEADER){
            setStyleSheet("background-color: #222222;");
            m_status1->setText("");
            m_status2->setText("");
            return;
        }

        if (schedule_item->schedule_type() == OATS::ScheduleType::COMM){
            setStyleSheet("background-color: #222222;");
            m_status1->setText("");
            m_status2->setText("");
            return;
        }

        m_status1->setText(QString::fromStdString(schedule_item->item_status_text()));
        m_status2->setText(QString::fromStdString(schedule_item->play_channel()));

        setStyleSheet("background-color: #222222;");

        if (schedule_item->item_status() == OATS::ItemStatus::CUED){
            setStyleSheet("background-color: #EF6C00");
        }

        if (schedule_item->item_status() == OATS::ItemStatus::PLAYING){
            setStyleSheet("background-color: rgb(78, 207, 42)");
        }

        if (schedule_item->item_status() == OATS::ItemStatus::ERROR_01){
            setStyleSheet("background-color: #fb1d04");
            m_status2->setText("");
        }
    }


}
