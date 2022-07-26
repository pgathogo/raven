#include <QGridLayout>
#include <QMenu>
#include <QMouseEvent>
#include <QFont>
#include <QContextMenuEvent>

#include "trackgridpanel.h"
#include "schedulegriditem.h"
#include "scheduleitem.h"

namespace OATS{

    TrackGridPanel::TrackGridPanel(QWidget* parent)
        :GridPanel(parent)
        ,m_schedule_item{nullptr}
    {
        constexpr static int COLUMN_0 = 0;
        constexpr static int COLUMN_1 = 1;
        constexpr static int ROW_0 = 0;
        constexpr static int ROW_1 = 1;

        setObjectName("TrackPanel");
        //setFixedWidth(450);
        setStyleSheet("QFrame#TrackPanel{background-color: #222222;}");

        m_layout = new QGridLayout();

        QFont f_track("JetBrains Mono", 12, QFont::Bold);
        f_track.setBold(true);

        m_track_label = std::make_unique<QLabel>("Track Title");
        m_track_label->setObjectName("TrackTitleLabel");
        m_track_label->setFont(f_track);

        QFont f_artist("JetBrains Mono", 10, QFont::Bold);
        f_artist.setBold(true);
        m_artist_label = std::make_unique<QLabel>("Artist");
        m_artist_label->setObjectName("ArtistLabel");
        m_artist_label->setFont(f_artist);

        m_duration_label = std::make_unique<QLabel>("03:02:07");
        m_duration_label->setObjectName("DurationLabel");
        m_duration_label->setFont(f_artist);

        m_layout->addWidget(m_track_label.get(), ROW_0, COLUMN_0);
        m_layout->addWidget(m_duration_label.get(), ROW_0, COLUMN_1);
        m_layout->addWidget(m_artist_label.get(), ROW_1, COLUMN_0);
        m_layout->setColumnStretch(COLUMN_0,1);
        m_layout->setContentsMargins(5,0,5,0);

        setLayout(m_layout);

    }


    TrackGridPanel::~TrackGridPanel()
    {
        delete m_layout;
    }

    void TrackGridPanel::contextMenuEvent(QContextMenuEvent* event)
    {
        QMenu menu;

        m_act_move_up = std::make_unique<QAction>("Move Up");
        m_act_move_down = std::make_unique<QAction>("Move Down");
        m_act_make_current = std::make_unique<QAction>("Make Current");

        if (m_schedule_item->item_status() == OATS::ItemStatus::ERROR_01)
            m_act_make_current->setEnabled(false);

        m_act_delete_item = std::make_unique<QAction>("Delete Item");


        connect(m_act_move_up.get(), &QAction::triggered, this, &TrackGridPanel::move_up);
        connect(m_act_move_down.get(), &QAction::triggered, this, &TrackGridPanel::move_down);
        connect(m_act_make_current.get(), &QAction::triggered, this, &TrackGridPanel::make_audio_current);
        connect(m_act_delete_item.get(), &QAction::triggered, this, &TrackGridPanel::delete_item);

        menu.addAction(m_act_move_up.get());
        menu.addAction(m_act_move_down.get());
        menu.addAction(m_act_make_current.get());
        menu.addAction(m_act_delete_item.get());

        menu.exec(event->globalPos());

    }

    void TrackGridPanel::mouseReleaseEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton){
            emit dynamic_cast<ScheduleGridItem*>(parent())->insert_item(parent_ref(), index());
        }
    }


    void TrackGridPanel::move_up()
    {
        emit dynamic_cast<OATS::ScheduleGridItem*>(parent())->move_up(parent_ref(), index());
    }

    void TrackGridPanel::move_down()
    {
        emit dynamic_cast<OATS::ScheduleGridItem*>(parent())->move_down(parent_ref(), index());
    }

    void TrackGridPanel::make_audio_current()
    {
        emit dynamic_cast<OATS::ScheduleGridItem*>(parent())->make_current(parent_ref(), index());
    }

    void TrackGridPanel::delete_item()
    {
        emit dynamic_cast<OATS::ScheduleGridItem*>(parent())->delete_item(parent_ref(), index());
    }

    void TrackGridPanel::update(OATS::ScheduleItem* schedule_item)
    {
        m_schedule_item = schedule_item;

        m_track_label->setText(QString::fromStdString(schedule_item->audio().title()));
        m_artist_label->setText(QString::fromStdString(schedule_item->audio().artist()));

        QTime t = msec_to_time(schedule_item->hour(), schedule_item->audio().duration());

        m_duration_label->setText(t.toString("mm:ss"));

        setStyleSheet("background-color: #636160;");

        if (schedule_item->schedule_type() == OATS::ScheduleType::COMM){
            m_duration_label->setText("");
            setStyleSheet("background-color: rgb(40, 133, 220)");
        }

        if (schedule_item->schedule_type() == OATS::ScheduleType::SONG){
            setStyleSheet("background-color: #E0E0E0");
        }

        if (schedule_item->item_status() == OATS::ItemStatus::ERROR_01){
            setStyleSheet("background-color: #fb1d04");
            m_artist_label->setText(m_artist_label->text()+": ERROR - FILE NOT FOUND");
        }

    }

}
