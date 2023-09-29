#include <iostream>
#include "scheduleitem.h"


namespace OATS {

/*
std::ostream operator<<(std::ostream& os,
                        const ScheduleItem& si)
{
    os << "ID: " << si.m_id;
    os << "Output: "<< si.m_play_channel;
}
*/


QDebug operator<<(QDebug qd, ScheduleItem& si)
{
    qd << "ID: " << si.id();
    qd << "OutCH: " << QString::fromStdString(si.play_channel());
    qd << "Audio: " << si.audio()->title()->to_qstring();

    std::string ttype = si.transition_type_text();
    qd << "TTYP: " << stoq(ttype);

    return qd;
}

    /* ----------- ScheduleItem ----------------- */

    ScheduleItem::ScheduleItem()
        :m_schedule_ref{-1}
        ,m_id{0}
        ,m_index{-1}
        ,m_current_time{0}
        ,m_transition_type{OATS::TransitionType::MIX}
        ,m_audio{nullptr}
    {
    }

    ScheduleItem& ScheduleItem::operator=(const ScheduleItem& other)
    {
        if (this == &other) return *this;

        m_id = other.m_id;
        m_schedule_ref = other.m_schedule_ref;
        m_index = other.m_index;
        m_item_status = other.m_item_status;

        std::cout << "Other CH: "<< other.m_play_channel << '\n';

        m_play_channel = other.m_play_channel;

        m_schedule_type = other.m_schedule_type;
        m_transition_type = other.m_transition_type;
        m_hour = other.m_hour;
        m_booked_spots = other.m_booked_spots;

        // **
        m_schedule_date = other.m_schedule_date;
        m_play_date = other.m_play_date;
        m_play_time = other.m_play_time;
        m_comment = other.m_comment;

        // **
        m_schedule_time = other.m_schedule_time;
        m_play_start_time = other.m_play_start_time;
        m_current_time = other.m_current_time;
        //Audio m_audio;
        m_audio = other.m_audio;

        return *this;

    }


    int ScheduleItem::id() const
    {
        return m_id;
    }


    int ScheduleItem::schedule_ref()
    {
        return m_schedule_ref;
    }

    int ScheduleItem::index()
    {
        return m_index;
    }

    void ScheduleItem::set_index(int i)
    {
        m_index = i;
    }

    OATS::ItemStatus ScheduleItem::item_status()
    {
        return m_item_status;
    }

    std::string ScheduleItem::play_channel() const
    {
        return m_play_channel;
    }

    OATS::ScheduleType ScheduleItem::schedule_type()
    {
        return m_schedule_type;
    }

    OATS::TransitionType ScheduleItem::transition_type()
    {
        return m_transition_type;
    }

    int ScheduleItem::hour()
    {
        return m_hour;
    }

    int ScheduleItem::booked_spots()
    {
        return m_booked_spots;
    }

    QDate ScheduleItem::schedule_date()
    {
        return m_schedule_date;
    }

    QDate ScheduleItem::play_date()
    {
        return m_play_date;
    }

    QTime ScheduleItem::play_time()
    {
        return m_play_time;
    }

    std::string ScheduleItem::comment()
    {
        return m_comment;
    }

    QTime ScheduleItem::schedule_time()
    {
        return m_schedule_time;
    }

    QTime ScheduleItem::play_start_time()
    {
        return m_play_start_time;
    }

    int ScheduleItem::current_time()
    {
        return m_current_time;
    }

    std::shared_ptr<AUDIO::Audio> ScheduleItem::audio()
    {
        return m_audio;
    }

    void ScheduleItem::set_id(int id)
    {
        m_id = id;
    }

    void ScheduleItem::set_schedule_ref(int sched_ref)
    {
        m_schedule_ref = sched_ref;
    }

    void ScheduleItem::set_item_status(OATS::ItemStatus item_status)
    {
        m_item_status = item_status;
        notify();
    }

    void ScheduleItem::set_play_channel(std::string channel)
    {
        m_play_channel = channel;
    }

    void ScheduleItem::set_schedule_type(std::string sched_type)
    {
        m_schedule_type = str_to_schedule_type(sched_type);
    }

    ScheduleType ScheduleItem::str_to_schedule_type(std::string s_type)
    {
        if (s_type == "SONG")
            return ScheduleType::SONG;
        if (s_type == "COMM-AUDIO")
            return ScheduleType::COMM_AUDIO;
        if (s_type == "COMM-BREAK")
            return ScheduleType::COMM;
        if (s_type == "JINGLE")
            return ScheduleType::JINGLE;
        if (s_type == "DROP")
            return ScheduleType::DROP;
        if (s_type == "HOUR_HEADER")
            return ScheduleType::HOUR_HEADER;
        if (s_type == "NBITE")
            return ScheduleType::NEWS_BITE;

        return ScheduleType::SONG;
    }

    QString ScheduleItem::schedule_type_to_str(ScheduleType s_type)
    {
        switch(s_type){
        case ScheduleType::SONG:
            return "SONG";
        case ScheduleType::COMM_AUDIO:
            return "COMM_AUDIO";
        case ScheduleType::COMM:
            return "COMM";
        case ScheduleType::JINGLE:
            return "JINGLE";
        case ScheduleType::DROP:
            return "DROP";
        case ScheduleType::HOUR_HEADER:
            return "HOUR_HEADER";
        default:
            return "UNKNOWN TYPE";
        }

    }

    QString ScheduleItem::schedule_type_to_str()
    {
        switch(m_schedule_type){
        case ScheduleType::SONG:
            return "SONG";
        case ScheduleType::COMM_AUDIO:
            return "COMM_AUDIO";
        case ScheduleType::COMM:
            return "COMM";
        case ScheduleType::JINGLE:
            return "JINGLE";
        case ScheduleType::DROP:
            return "DROP";
        case ScheduleType::HOUR_HEADER:
            return "HOUR_HEADER";
        default:
            return "UNKNOWN TYPE";
        }

    }

    OATS::ItemStatus ScheduleItem::str_to_status(std::string s_status)
    {
        if (s_status == "WAITING")
            return OATS::ItemStatus::WAITING;
        if (s_status == "CUED")
            return OATS::ItemStatus::CUED;
        if (s_status =="PLAYING")
            return OATS::ItemStatus::PLAYING;
        if (s_status == "PLAYED")
            return OATS::ItemStatus::PLAYED;
        if (s_status == "STOP")
            return OATS::ItemStatus::STOP;
        if (s_status == "ERROR_01")
            return OATS::ItemStatus::ERROR_01;

        return OATS::ItemStatus::WAITING;
    }

    void ScheduleItem::set_transition_type(OATS::TransitionType t_type)
    {
        m_transition_type = t_type;
        notify();
    }

    void ScheduleItem::set_hour(int hr)
    {
        m_hour = hr;
    }

    void ScheduleItem::set_booked_spots(int spots)
    {
        m_booked_spots = spots;
    }

    void ScheduleItem::set_schedule_date(QDate date)
    {
        m_schedule_date = date;
    }

    void ScheduleItem::set_play_date(QDate date)
    {
        m_play_date = date;
    }

    void ScheduleItem::set_play_time(QTime time)
    {
        m_play_time = time;
    }

    void ScheduleItem::set_comment(std::string comment)
    {
        m_comment = comment;
    }

    void ScheduleItem::set_schedule_time(QTime time)
    {
        m_schedule_time = time;
    }

    void ScheduleItem::set_play_start_time(QTime time)
    {
        m_play_start_time = time;
    }

    void ScheduleItem::set_current_time(int msec)
    {
        m_current_time = msec;
    }

    void ScheduleItem::set_audio(std::shared_ptr<AUDIO::Audio> audio)
    {
//        if (m_audio == nullptr)
//            m_audio = std::make_shared<AUDIO::Audio>();

        m_audio = audio;
    }

    std::string ScheduleItem::item_status_text()
    {
        switch (item_status())
        {
        case OATS::ItemStatus::WAITING:
            return "WAITING";
            break;
        case OATS::ItemStatus::CUED:
            return "CUED";
            break;
        case OATS::ItemStatus::PLAYING:
            return "PLAYING";
            break;
        case OATS::ItemStatus::PLAYED:
            return "PLAYED";
            break;
        case OATS::ItemStatus::STOP:
            return "STOP";
            break;
        case OATS::ItemStatus::ERROR_01:
            return "ERROR_01";
            break;
        default:
            return "WAITING";
        }
    }



    std::string ScheduleItem::transition_type_text()
    {
    enum class TransitionType{NONE=0, STOP, MIX, CUT, BACK, CENTER, EARLY, SYNCHRO, SKIP};
        switch (transition_type())
        {
        case OATS::TransitionType::NONE:
            return "NONE";
            break;
        case OATS::TransitionType::STOP:
            return "STOP";
            break;
        case OATS::TransitionType::MIX:
            return "MIX";
            break;
        case OATS::TransitionType::CUT:
            return "CUT";
            break;
        case OATS::TransitionType::SKIP:
            return "SKIP";
            break;
        default:
            return "OTHERS";
        }
    }

}
