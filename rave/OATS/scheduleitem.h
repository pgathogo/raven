#pragma once

#include <string>

#include <QTime>

#include "subjectobserver.h"
#include "../../../audio/audio.h"

namespace OATS{

    enum class ScheduleType{SONG, COMM, JINGLE, DROP, COMM_AUDIO, HOUR_HEADER, NEWS_BITE};
    enum class ItemStatus{WAITING, CUED, PLAYING, PLAYED, STOP, ERROR_01};
    enum class TransitionType{NONE=0, STOP, MIX, CUT, BACK, CENTER, EARLY, SYNCHRO, SKIP};

    class ScheduleItem : public Subject
    {
    public:
        ScheduleItem();
        ScheduleItem(const ScheduleItem& other);
        ScheduleItem& operator=(const ScheduleItem& other);

        int schedule_ref();
        int id() const;
        int index();
        void set_index(int);

        ItemStatus item_status();
        std::string play_channel() const;
        ScheduleType schedule_type();
        TransitionType transition_type();
        int hour();
        int booked_spots();

        QDate schedule_date();
        QDate play_date();
        QTime play_time();
        std::string comment();

        QTime schedule_time();
        QTime play_start_time();
        int current_time();
        std::shared_ptr<AUDIO::Audio> audio();

        void set_id(int);
        void set_schedule_ref(int);
        void set_item_status(ItemStatus);
        void set_play_channel(std::string);
        void set_schedule_type(std::string);
        void set_transition_type(TransitionType);
        void set_hour(int);
        void set_booked_spots(int);

        void set_schedule_date(QDate);
        void set_play_date(QDate);
        void set_play_time(QTime);
        void set_comment(std::string);

        void set_schedule_time(QTime);
        void set_play_start_time(QTime);
        void set_current_time(int);
        void set_audio(std::shared_ptr<AUDIO::Audio>);

        std::string item_status_text();
        std::string transition_type_text();

        ScheduleType str_to_schedule_type(std::string);
        OATS::ItemStatus str_to_status(std::string);
        QString schedule_type_to_str(ScheduleType);
        QString schedule_type_to_str();

//        friend std::ostream operator<<(std::ostream& os,
//                                       const ScheduleItem& si);

        friend QDebug operator<<(QDebug qd, ScheduleItem& si);

    private:
        int m_id;
        int m_schedule_ref;
        int m_index;
        ItemStatus m_item_status;
        std::string m_play_channel;
        ScheduleType m_schedule_type;
        TransitionType m_transition_type;
        int m_hour;
        int m_booked_spots;

        // **
        QDate m_schedule_date;
        QDate m_play_date;
        QTime m_play_time;
        std::string m_comment;

        // **
        QTime m_schedule_time;    // Time from the scheduler - Sedric
        QTime m_play_start_time;  // Actual time the item was played
        int m_current_time;  // Computed time in millisecs
        //Audio m_audio;
        std::shared_ptr<AUDIO::Audio> m_audio;
    };

}
