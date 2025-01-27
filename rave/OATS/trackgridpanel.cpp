#include <QGridLayout>
#include <QMenu>
#include <QMouseEvent>
#include <QFont>
#include <QContextMenuEvent>

#include "trackgridpanel.h"
#include "schedulegriditem.h"
#include "scheduleitem.h"

#include "../audio/audiotool.h"

namespace OATS{

    TrackGridPanel::TrackGridPanel(QWidget* parent)
        :GridPanel(parent)
        ,m_schedule_item{nullptr}
    {
        constexpr static int COLUMN_0 = 0;
        constexpr static int COLUMN_1 = 1;
        constexpr static int ROW_0 = 0;
        constexpr static int ROW_1 = 1;

        m_schedule_item = dynamic_cast<ScheduleGridItem*>(parent)->schedule_item();

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
                // "{background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, "
                //  "stop:0 #555D64 , stop:1 #374148 );"
        // QString menu_style(
        //     "QMenu::item{"
        //         "background-color: #374148;"
        //          "border-radius: 20px;"
        //          "border-bottom-width: 1px;"
        //          "border-bottom-color:#374148;"
        //          "color:#FFFFFF;"
        //          "font-weight:bold;}"
        //          "QMenu::item:selected{"
        //          "background-color: rgb(0, 85, 127);"
        //          "color: rgb(255, 255, 255);"
        //         "}"
        //     );

        // QString menu_style2(
        //     "QMenu::item{"
        //     "background-color: rgb(0, 170, 0);"
        //     "color: rgb(255, 255, 255);"
        //     "}"
        //     "QMenu::item:selected{"
        //     "background-color: rgb(0, 85, 127);"
        //     "color: rgb(255, 255, 255);"
        //     "}"
        //     );

        QMenu menu;

        m_act_move_up = std::make_unique<QAction>("Move Up");
        m_act_move_down = std::make_unique<QAction>("Move Down");
        m_act_make_current = std::make_unique<QAction>("Make Current");
        m_act_delete_item = std::make_unique<QAction>("Remove Item");
        m_act_reload_schedule = std::make_unique<QAction>("Reload Schedule");

        if (m_schedule_item->item_status() == OATS::ItemStatus::ERROR_01)
            m_act_make_current->setEnabled(false);

        if ((m_schedule_item->schedule_type() == OATS::ScheduleType::HOUR_HEADER) ||
            (m_schedule_item->item_status() == OATS::ItemStatus::PLAYING)){
            m_act_move_up->setEnabled(false);
            m_act_move_down->setEnabled(false);
            m_act_make_current->setEnabled(false);
            m_act_delete_item->setEnabled(false);
        }

        int current_hour = QTime::currentTime().hour();
        if (m_schedule_item->hour() <= current_hour){
            m_act_reload_schedule->setEnabled(false);
        }

        connect(m_act_move_up.get(), &QAction::triggered, this, &TrackGridPanel::move_up);
        connect(m_act_move_down.get(), &QAction::triggered, this, &TrackGridPanel::move_down);
        connect(m_act_make_current.get(), &QAction::triggered, this, &TrackGridPanel::make_audio_current);
        connect(m_act_delete_item.get(), &QAction::triggered, this, &TrackGridPanel::delete_item);
        connect(m_act_reload_schedule.get(), &QAction::triggered, this, &TrackGridPanel::reload_schedule);

        menu.addAction(m_act_move_up.get());
        menu.addAction(m_act_move_down.get());
        menu.addAction(m_act_make_current.get());
        menu.addAction(m_act_delete_item.get());
        menu.addSeparator();
        menu.addAction(m_act_reload_schedule.get());

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

    void TrackGridPanel::reload_schedule()
    {
        emit dynamic_cast<OATS::ScheduleGridItem*>(parent())->reload_schedule(parent_ref(), index());
    }

    void TrackGridPanel::update(OATS::ScheduleItem* schedule_item)
    {
        m_schedule_item = schedule_item;

        if (schedule_item->schedule_type() == OATS::ScheduleType::HOUR_HEADER){
            m_artist_label->setText("");
            m_duration_label->setText("");
        } else {
            m_artist_label->setText(QString::fromStdString(schedule_item->audio()->artist_fullname()));
        }

        m_track_label->setText(QString::fromStdString(schedule_item->audio()->title()->value()));

        AUDIO::AudioTool at;
        QTime t = at.msec_to_time(schedule_item->hour(), schedule_item->audio()->playable_duration());

        m_duration_label->setText(t.toString("mm:ss"));

        setStyleSheet("background-color: #636160;");
        m_track_label->setStyleSheet("color: #FFFFFF");

        if (schedule_item->schedule_type() == OATS::ScheduleType::COMM){
            //m_duration_label->setText("");
            m_track_label->setStyleSheet("color: #353935");
            setStyleSheet("background-color: #FFDB58");
        }
            //setStyleSheet("background-color: rgb(40, 133, 220)");

        if (schedule_item->schedule_type() == OATS::ScheduleType::SONG){
            setStyleSheet("background-color: #40B5AD");

        }
            //setStyleSheet("background-color: #088F8F");
        if (schedule_item->item_status() == OATS::ItemStatus::ERROR_01){
            setStyleSheet("background-color: #fb1d04");
            m_artist_label->setText(m_artist_label->text()+": ERROR - FILE NOT FOUND");
        }

    }

}
