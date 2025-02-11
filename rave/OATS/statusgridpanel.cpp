#include <QVBoxLayout>
#include <QLabel>
#include <QFont>
#include <QMenu>
#include <QContextMenuEvent>

#include "statusgridpanel.h"
#include "schedulegriditem.h"
#include "scheduleitem.h"

namespace OATS{

    StatusGridPanel::StatusGridPanel(QWidget* parent)
        :GridPanel(parent)
         ,m_schedule_item{nullptr}
    {
        setStyleSheet("QFrame{background-color: #313131;}");
        setFixedWidth(70);

        m_layout = new QVBoxLayout();
        m_layout->setSpacing(0);
        m_layout->setContentsMargins(0,0,0,0);

        setObjectName("StatusPanel");
        //setFixedWidth(80);

        QFont f_statusTop("JetBrains Mono", 10, QFont::Bold);
        m_item_status = std::make_unique<QLabel>("");
        m_item_status->setObjectName("StatusTopLabel");
        m_item_status->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_item_status->setFont(f_statusTop);

        QString text_style = "QLabel{"
                             "color:#FFFFFF; "
                             "background-color:transparent;"
                             "font-size: 12pt;  font-weight: bold; }";

        //QFont f_statusBottom("JetBrains Mono", 12, QFont::Bold);
        m_output_name = std::make_unique<QLabel>("");
        m_output_name->setObjectName("StatusBottomLabel");
        m_output_name->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_output_name->setStyleSheet(text_style);
        // m_output_name->setFont(f_statusBottom);

        m_layout->addWidget(m_item_status.get());
        m_layout->addWidget(m_output_name.get());
        setLayout(m_layout);

    }

    StatusGridPanel::~StatusGridPanel()
    {
        delete m_layout;
    }

    void StatusGridPanel::update(OATS::ScheduleItem* schedule_item)
    {
        m_schedule_item = schedule_item;

        if (schedule_item->schedule_type() == OATS::ScheduleType::HOUR_HEADER){
            //setStyleSheet("background-color: #222222;");
            setStyleSheet("background-color: rgb(40, 133, 220)");
            m_item_status->setText("");
            m_output_name->setText("");
            return;
        }

        if (schedule_item->schedule_type() == OATS::ScheduleType::COMM){
            setStyleSheet("background-color: #222222;");
        }


        m_item_status->setText(QString::fromStdString(schedule_item->item_status_text()));
        m_output_name->setText(QString::fromStdString(schedule_item->play_channel()));

        //setStyleSheet("background-color: #222222;");
        setStyleSheet("background-color: rgb(40, 133, 220)");

        if (schedule_item->item_status() == OATS::ItemStatus::WAITING){
            setStyleSheet("background-color: #899499");
        }

        if (schedule_item->item_status() == OATS::ItemStatus::CUED){
            setStyleSheet("background-color: #EF6C00");
        }

        if (schedule_item->item_status() == OATS::ItemStatus::PLAYING){
            setStyleSheet("background-color: rgb(78, 207, 42)");
        }

        if (schedule_item->item_status() == OATS::ItemStatus::PAUSED){
            setStyleSheet("background-color: rgb(170, 85, 127)");
        }

        if (schedule_item->item_status() == OATS::ItemStatus::ERROR_01){
            setStyleSheet("background-color: #fb1d04");
            m_output_name->setText("");
        }
    }

    void StatusGridPanel::contextMenuEvent(QContextMenuEvent* event)
    {
        QMenu menu;

        m_act_play = new QAction("Play");
        connect(m_act_play, &QAction::triggered, this, &StatusGridPanel::play_audio);

        if (m_schedule_item->item_status() != OATS::ItemStatus::CUED)
            m_act_play->setEnabled(false);

        // Stop action
       m_act_stop = new QAction("Stop");
       connect(m_act_stop, &QAction::triggered, this, &StatusGridPanel::stop_audio);
       if (m_schedule_item->item_status() != OATS::ItemStatus::PLAYING)
           m_act_stop->setEnabled(false);

       // Fade action
       m_act_fade = new QAction("Fade");
       connect(m_act_fade, &QAction::triggered, this, &StatusGridPanel::fade_audio);
       if (m_schedule_item->item_status() != OATS::ItemStatus::PLAYING)
           m_act_fade->setEnabled(false);


        menu.addAction(m_act_play);
        menu.addAction(m_act_stop);
        menu.addAction(m_act_fade);
        menu.exec(event->globalPos());
    }

    void StatusGridPanel::play_audio()
    {
        emit dynamic_cast<OATS::ScheduleGridItem*>(parent())->play_audio(parent_ref(), index());

    }

    void StatusGridPanel::stop_audio()
    {
        emit dynamic_cast<OATS::ScheduleGridItem*>(parent())->stop_audio(parent_ref(), index());
    }

    void StatusGridPanel::fade_audio()
    {
        emit dynamic_cast<OATS::ScheduleGridItem*>(parent())->fade_audio(parent_ref(), index());
    }

}
