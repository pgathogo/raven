#include "scheduleitem.h"

namespace OATS {

    Audio::Audio()
    {
    }

    int Audio::id()
    {
        return m_id;
    }

    std::string Audio::title()
    {
        return m_title;
    }

    std::string Audio::artist()
    {
        return m_artist;
    }

    std::string Audio::file_path()
    {
        return m_file_path;
    }

    int Audio::duration()
    {
        return m_duration;
    }

    void Audio::set_id(int id)
    {
        m_id = id;
    }

    void Audio::set_title(std::string title)
    {
        m_title =  title;
    }

    void Audio::set_artist(std::string art)
    {
        m_artist = art;
    }

    void Audio::set_file_path(std::string path)
    {
        m_file_path = path;
    }

    void Audio::set_duration(int dur)
    {
        m_duration = dur;
    }

    int Audio::fade_delay()
    {
        return m_fade_delay;
    }
    void Audio::set_fade_delay(int delay)
    {
        m_fade_delay = delay;
    }

    int Audio::intro()
    {
        return m_intro;
    }

    void Audio::set_intro(int i)
    {
        m_intro = i;
    }

    int Audio::extro()
    {
        return m_extro;
    }

    void Audio::set_extro(int e)
    {
        m_extro = e;
    }
    int Audio::fade_out()
    {
        return m_fade_out;
    }
    void Audio::set_fade_out(int fout)
    {
        m_fade_out = fout;
    }

    /* ----------- ScheduleItem ----------------- */

    ScheduleItem::ScheduleItem()
        :m_schedule_ref{-1}
        ,m_index{-1}
        ,m_current_time{0}
        ,m_transition_type{OATS::TransitionType::MIX}
    {
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

    std::string ScheduleItem::play_channel()
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

    Audio& ScheduleItem::audio()
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

    void ScheduleItem::set_schedule_type(ScheduleType sched_type)
    {
        m_schedule_type = sched_type;
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

    void ScheduleItem::set_audio(Audio audio)
    {
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
        case OATS::ItemStatus::STOP:
            return "STOP";
            break;
        default:
            return "WAITING";
        }
    }



    std::string ScheduleItem::transition_type_text()
    {
    enum class TransitionType{NONE=0, STOP, MIX, CUT, BACK, CENTER, EARLY, SYNCHRO};
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
        default:
            return "OTHERS";
        }
    }

}
