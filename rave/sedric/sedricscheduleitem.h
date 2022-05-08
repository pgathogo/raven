#pragma once

#include "../framework/schedule.h"
#include "../framework/choicefield.h"

using Columns = QList<QStandardItem*>;

class QStandardItemModel;
class QStandardItem;
class PostgresDatabaseManager;
class BaseDataProvider;

namespace AUDIO {
    class Audio;
    class Artist;
}

namespace SEDRIC
{
    template<typename T>
    struct VectorStruct{
        VectorStruct& operator<<(T* t)
        {
            vec.push_back(t);
            return *this;
        }
        void clear()
        {
            vec.clear();
        }
        std::vector<T*> vec;
    };

    class SedricScheduleItem
    {
    public:
        enum AudioTransition{None=0, Mix, Cut, Back, Center, Early, Syncro, Stop};

        SedricScheduleItem();
        SedricScheduleItem(Schedule*);
        SedricScheduleItem(QStandardItemModel*);
        ~SedricScheduleItem();

        virtual QString time();
        virtual QString title();
        virtual QString artist_name();
        virtual QString duration();
        virtual QString transition();
        virtual QString audio_type();
        virtual QString play_date();
        virtual QString play_time();
        virtual QString track_path();
        virtual QString comment();

        Schedule* schedule() const;
        int schedule_row_id() const;
        void set_schedule_row_id(int id);
        int generate_row_id();
        void update_time(int, int, int);
        void update_schedule_item_time(int, QTime);

        Schedule* find_schedule_item(int);
        void delete_row(int);

        AUDIO::Audio* audio();
        AUDIO::Artist* artist();

        std::vector<Schedule*> cached_items() const;
//        std::vector<std::unique_ptr<SedricScheduleItem>> const& find_schedule_by_datehour(QDate, int) const;
        std::map<int, int> fetch_cached_items(QDate, const std::vector<int>&);
        void fetch_db_items(QDate, std::vector<int>&);
        void set_schedule_fields(BaseDataProvider*, Schedule*, AUDIO::Audio*, AUDIO::Artist*);
        void create_end_marker(int);

        void clear_display_items();
        void clear_cached_items();
        void clear_cached_items_date_hours(QDate, std::vector<int>);

        void show_items(QDate, const std::vector<int>&);

        std::size_t display_item_count();
        bool is_schedule_cached(Schedule* schedule);

        void new_schedule(QDate, const std::vector<int>&);
        void log_activity(QDate, int);
        std::map<QDate, std::vector<int>> activities() const;

        std::string make_delete_stmts();
        std::string make_insert_stmts(const std::vector<Schedule*>&);
        std::string vec_to_str(const std::vector<int>);

        void copy_schedule(QDate, QDate, const std::map<int, int>&);
        void delete_current_schedule(std::string sql);
        bool write_schedule_to_db(std::string sql);

        bool log_find(QDate, int);

        template<typename T>
        void create_row_item(Schedule* schedule, int row=-1)
        {
            static_assert(std::is_base_of<SEDRIC::SedricScheduleItem, T>::value, "`T` must be derived from SedricScheduleItem");
            Columns columns;

            auto item = std::make_unique<T>(schedule);

            if (schedule->display_row_id()->value() == -1){
                schedule->set_display_row_id(generate_row_id());
            }

            auto time = new T(item->time());
            time->setTextAlignment(Qt::AlignCenter);

            QMap<QString, QVariant> item_data;
            item_data["row_id"] = schedule->display_row_id()->value();
            item_data["hour"] = schedule->schedule_hour()->value();
            item_data["time"] = schedule->schedule_time()->value();
            time->setData(item_data, Qt::UserRole);

            auto title = new T(item->title());

            auto artist = new T(item->artist_name());
            auto duration = new T(item->duration());
            auto transition = new T(item->audio_type());
            auto play_date  = new T(item->play_date());
            auto play_time = new T(item->play_time());
            auto track_path = new T(item->track_path());
            auto comment = new T(item->comment());

            columns.append(time);
            columns.append(title);
            columns.append(artist);
            columns.append(duration);
            columns.append(transition);
            columns.append(play_date);
            columns.append(play_time);
            columns.append(track_path);
            columns.append(comment);

            m_model->insertRow(row, columns);

            //m_model->appendRow(columns);

            if (schedule->schedule_item_type()->value() != "END_MARKER"){
                if (!is_schedule_cached(schedule))
                    m_cached_schedule_items.push_back(schedule);
            }
        }

    private:
        static int m_base_row_id;
        int m_schedule_row_id{-1};
        Schedule* m_schedule;
        QStandardItemModel* m_model;
        std::vector<Schedule*> m_cached_schedule_items;
        std::vector<Schedule*> m_display_items;
        AUDIO::Audio* m_audio;
        AUDIO::Artist* m_artist;
        std::map<QDate, std::vector<int>> m_activities;
    };

    class SongItem :  public SedricScheduleItem, public QStandardItem
    {
    public:
         SongItem(Schedule*);
         SongItem(QString);
        ~SongItem();
    };

    class CommercialBreakItem : public SedricScheduleItem, public QStandardItem
    {
    public:
        CommercialBreakItem(Schedule*);
        CommercialBreakItem(QString);
        ~CommercialBreakItem();

         QString title() override;
         QString artist_name() override;
         QString duration() override;
         QString transition() override;
         QString track_path() override;
         QString comment() override;
    };

    class CommercialAudioItem : public SedricScheduleItem, public QStandardItem
    {
    public:
        CommercialAudioItem(Schedule*);
        CommercialAudioItem(QString);
        ~CommercialAudioItem();
    };

    class JingleItem : public SedricScheduleItem, public QStandardItem
    {
    public:
        JingleItem(Schedule*);
        JingleItem(QString);
        ~JingleItem();
    };

    class EndMarkerItem : public SedricScheduleItem, public QStandardItem
    {
    public:
        EndMarkerItem(Schedule*);
        EndMarkerItem(QString);
        ~EndMarkerItem();

        QString title() override;
        QString artist_name() override;
        QString duration() override;
        QString transition() override;
        QString play_date() override;
        QString play_time() override;
        QString track_path() override;
        QString comment() override;
    };

    struct FindItemByDateHour{
        FindItemByDateHour(QDate date, int hour)
            :m_date{date}
            ,m_hour{hour}
        {
        }
        bool operator()(const Schedule* schedule) const
        {
            return ( (schedule->schedule_date()->value() == m_date) &&
                     (schedule->schedule_hour()->value() == m_hour) );
        }
    private:
        QDate m_date;
        int m_hour;
    };

    struct FindRowId{
        FindRowId(int row_id)
            :m_row_id{row_id}
        {
        }
        bool operator()(const Schedule* schedule) const
        {
            return ( schedule->display_row_id()->value() == m_row_id );
        }
     private:
        int m_row_id{-1};
    };


    struct FindScheduleByDate{
        FindScheduleByDate(QDate date)
            :m_date{date}
        {
        }
        bool operator()(const Schedule* schedule) const
        {
            return (schedule->schedule_date()->value() == m_date);

        }
     private:
        QDate m_date;

    };

    struct SortScheduleByHour{
        bool operator()(const Schedule* s1, const Schedule* s2)
        {
            return ( s1->schedule_date()->value() < s2->schedule_date()->value());
        }
    };


}

