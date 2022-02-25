#pragma once

#include <string>

#include <QTime>

#include "subjectobserver.h"

namespace OATS{

    enum class ScheduleType{SONG, COMM, JINGLE, DROP};
    enum class ItemStatus{WAITING, CUED, PLAYING, PLAYED, STOP};
    enum class TransitionType{NONE=0, STOP, MIX, CUT, BACK, CENTER, EARLY, SYNCHRO};

    class Audio{
    public:
        Audio();
        std::string title();
        std::string artist();
        int duration();

        void set_title(std::string);
        void set_artist(std::string);
        void set_duration(int);

        int fade_delay();
        void set_fade_delay(int);
        int fade_out();
        void set_fade_out(int);
        int intro();
        void set_intro(int);
        int extro();
        void set_extro(int);

    private:
        std::string m_title;
        std::string m_artist;
        int m_duration{0};
        int m_fade_delay{0};
        int m_fade_out{0};
        int m_intro{0};
        int m_extro{0};
    };


    class ScheduleItem : public Subject
    {
    public:

        ScheduleItem();
        ScheduleItem(const ScheduleItem& other);

        int schedule_ref();
        int index();
        void set_index(int);

        ItemStatus item_status();
        std::string play_channel();
        ScheduleType schedule_type();
        TransitionType transition_type();
        int hour();
        QTime schedule_time();
        QTime play_start_time();
        int current_time();
        Audio audio();

        void set_schedule_ref(int);
        void set_item_status(ItemStatus);
        void set_play_channel(std::string);
        void set_schedule_type(ScheduleType);
        void set_transition_type(TransitionType);
        void set_hour(int);
        void set_schedule_time(QTime);
        void set_play_start_time(QTime);
        void set_current_time(int);
        void set_audio(Audio);

        std::string item_status_text();
        std::string transition_type_text();

    private:
        int m_schedule_ref;
        int m_index;
        ItemStatus m_item_status;
        std::string m_play_channel;
        ScheduleType m_schedule_type;
        TransitionType m_transition_type;
        int m_hour;
        QTime m_schedule_time;    // Time from the scheduler - Sedric
        QTime m_play_start_time;  // Actual time the item was played
        int m_current_time;  // Computed time in millisecs
        Audio m_audio;

    };

}
