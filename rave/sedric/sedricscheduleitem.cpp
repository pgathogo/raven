#include <QStandardItemModel>
#include <QStandardItem>

#include "sedricscheduleitem.h"
#include "../framework/entitydatamodel.h"
#include "../framework/choicefield.h"
#include "../audio/artist.h"
#include "../audio/audio.h"

namespace SEDRIC{

    int SedricScheduleItem::m_base_row_id{-1};

    SedricScheduleItem::SedricScheduleItem()
        :m_schedule{nullptr}
        ,m_audio{nullptr}
        ,m_artist{nullptr}
    {
    }

    SedricScheduleItem::SedricScheduleItem(Schedule* schedule)
        :m_schedule{schedule}
    {
        m_audio = dynamic_cast<AUDIO::Audio*>(schedule->audio()->dataModel()->get_entity().get());
        m_artist = dynamic_cast<AUDIO::Artist*>(m_audio->artist()->dataModel()->get_entity().get());
    }

    SedricScheduleItem::SedricScheduleItem(QStandardItemModel* model)
        :m_schedule{nullptr}
        ,m_model{ model }
        ,m_audio{nullptr}
        ,m_artist{nullptr}
    {
    }

    SedricScheduleItem::~SedricScheduleItem()
    {
    }

    QString SedricScheduleItem::time()
    {
        return schedule()->schedule_time()->value().toString("hh:mm:ss");
    }

    QString SedricScheduleItem::title()
    {
        return stoq(m_audio->title()->value());
    }

    QString SedricScheduleItem::artist_name()
    {
        return stoq(m_artist->fullName()->value());
    }

    QString SedricScheduleItem::duration()
    {
        return QTime::fromMSecsSinceStartOfDay(
                    m_audio->duration()->value()*1000).toString("hh:mm:ss");
    }
    QString SedricScheduleItem::transition()
    {
        return stoq(schedule()->schedule_item_type()->value());
    }
    QString SedricScheduleItem::play_date()
    {
        return schedule()->play_date()->value().toString("dd/MM/yyyy");
    }
    QString SedricScheduleItem::play_time()
    {
        return schedule()->play_time()->value().toString("hh:mm:ss");
    }
    QString SedricScheduleItem::track_path()
    {
        return stoq(m_audio->file_path()->value());
    }
    QString SedricScheduleItem::comment()
    {
        return stoq(schedule()->comment()->value());
    }

    Schedule* SedricScheduleItem::schedule() const
    {
        return m_schedule;
    }

    int SedricScheduleItem::schedule_row_id() const
    {
        return m_schedule_row_id;
    }

    void SedricScheduleItem::set_schedule_row_id(int id)
    {
        m_schedule_row_id = id;
    }

    int SedricScheduleItem::generate_row_id()
    {
        m_base_row_id +=1;
        return m_base_row_id;
    }


    void SedricScheduleItem::update_time(int hour, int from_row, int duration)
    {
        for (int i=from_row; i < m_model->rowCount(); ++i){
            auto index = m_model->index(i, 0);  // first column
            auto data = index.data(Qt::UserRole).toMap();

            int row_id = data["row_id"].toInt();
            int schedule_hour = data["hour"].toInt();
            QTime time_stamp = data["time"].toTime();

            if (schedule_hour > hour)
                break;

            QTime new_time = time_stamp.addSecs(duration);

            QMap<QString, QVariant> item_data;
            item_data["row_id"] = row_id;
            item_data["hour"] = schedule_hour;
            item_data["time"] = new_time;

            m_model->setData(index, item_data, Qt::UserRole);
            m_model->setData(index, new_time.toString("hh:mm:ss"), Qt::DisplayRole);

            update_schedule_item_time(row_id, new_time);
        }

    }

    void SedricScheduleItem::update_schedule_item_time(int row_id, QTime new_time)
    {
        auto iter = std::find_if(m_schedule_items.begin(), m_schedule_items.end(),
                                 FindRowId(row_id));

        if (iter != m_schedule_items.end()){
            auto schedule = (*iter)->schedule();
            if (schedule->schedule_item_type()->currText() != "COMM-BREAK"){
                schedule->set_schedule_time(new_time);
            }
        }
    }

    Schedule* SedricScheduleItem::find_schedule_item(int row_id)
    {
        auto iter = std::find_if(m_schedule_items.begin(), m_schedule_items.end(),
                                 FindRowId(row_id));

        if (iter != m_schedule_items.end()){
            return (*iter)->schedule();
        }

        return nullptr;
    }

    void SedricScheduleItem::delete_row(int row_id)
    {
        m_schedule_items.erase(
                    std::remove_if(m_schedule_items.begin(), m_schedule_items.end(),
                                   FindRowId(row_id)), m_schedule_items.end());
    }

    AUDIO::Audio* SedricScheduleItem::audio()
    {
        return m_audio;
    }

    AUDIO::Artist* SedricScheduleItem::artist()
    {
        return m_artist;
    }


    std::vector<std::unique_ptr<SedricScheduleItem>> const& SedricScheduleItem::schedule_items() const
    {
        return m_schedule_items;
    }

    /* ---------- SongItem ------------- */

    SongItem::SongItem(QString text)
        :SedricScheduleItem()
    {
        setText(text);
    }

    SongItem::SongItem(Schedule* schedule)
        :SedricScheduleItem(schedule)
    {
    }

    SongItem::~SongItem()
    {
    }

    /* ---------- CommmercialBreakItem ------------- */

    CommercialBreakItem::CommercialBreakItem(Schedule* schedule)
        :SedricScheduleItem(schedule)
    {
    }

    CommercialBreakItem::CommercialBreakItem(QString text)
        :SedricScheduleItem()
    {
        setText(text);
        setBackground(QBrush(QColor("darkCyan")));
        setForeground(QBrush(QColor("white")));
    }

    CommercialBreakItem::~CommercialBreakItem()
    {
    }

    QString CommercialBreakItem::title()
    {
        return "Commercial Break";
    }

    QString CommercialBreakItem::artist_name()
    {
        return "";
    }

    QString CommercialBreakItem::duration()
    {
        return "00:00:00";
    }

    QString CommercialBreakItem::transition()
    {
        return "MIX";
    }


    /* ---------- CommmercialAudioItem ------------- */

    CommercialAudioItem::CommercialAudioItem(Schedule* sched)
        :SedricScheduleItem(sched)
    {
    }

    CommercialAudioItem::CommercialAudioItem(QString text)
        :SedricScheduleItem()
    {
        setText(text);
    }

    CommercialAudioItem::~CommercialAudioItem()
    {
    }

    /* ------- JingleItem ---------------- */

    JingleItem::JingleItem(Schedule* schedule)
        :SedricScheduleItem(schedule)
    {
    }

    JingleItem::JingleItem(QString text)
        :SedricScheduleItem()
    {
        setText(text);
    }

    JingleItem::~JingleItem()
    {
    }

    /* ------- EndMarkerItem ---------------- */

    EndMarkerItem::EndMarkerItem(Schedule* sched)
        :SedricScheduleItem(sched)
    {
    }

    EndMarkerItem::EndMarkerItem(QString text)
        :SedricScheduleItem()
    {
        setText(text);
        setBackground(QBrush(QColor("red")));
    }

    EndMarkerItem::~EndMarkerItem()
    {
    }

    QString EndMarkerItem::title()
    {
        return "END";
    }

    QString EndMarkerItem::artist_name()
    {
        return "";
    }

    QString EndMarkerItem::transition()
    {
        return "";
    }

    QString EndMarkerItem::duration()
    {
        return "";
    }

    QString EndMarkerItem::play_date()
    {
        return "";
    }
    QString EndMarkerItem::play_time()
    {
        return "";
    }

    QString EndMarkerItem::track_path()
    {
        return "";
    }

    QString EndMarkerItem::comment()
    {
        return "";
    }

}
