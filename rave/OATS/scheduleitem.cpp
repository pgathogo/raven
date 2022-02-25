#include "scheduleitem.h"

namespace OATS {

    Audio::Audio()
    {
    }

    std::string Audio::title()
    {
        return m_title;
    }

    std::string Audio::artist()
    {
        return m_artist;
    }

    int Audio::duration()
    {
        return m_duration;
    }

    void Audio::set_title(std::string title)
    {
        m_title =  title;
    }

    void Audio::set_artist(std::string art)
    {
        m_artist = art;
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

    Audio ScheduleItem::audio()
    {
        return m_audio;
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

    void ScheduleItem::set_transition_type(OATS::TransitionType t_type)
    {
        m_transition_type = t_type;
        notify();
    }

    void ScheduleItem::set_hour(int hr)
    {
        m_hour = hr;
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
