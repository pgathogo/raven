#pragma once

#include "../framework/schedule.h"

using Columns = QList<QStandardItem*>;

class QStandardItemModel;
class QStandardItem;

namespace AUDIO {
    class Audio;
    class Artist;
}

namespace SEDRIC{

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
        virtual QString play_date();
        virtual QString play_time();
        virtual QString track_path();
        virtual QString comment();

        Schedule* schedule() const;
        int schedule_row_id() const;
        void set_schedule_row_id(int id);
        int generate_row_id();
        void update_schedule_item_time(int, QTime);

        Schedule* find_schedule_item(int);
        void delete_row(int);

        AUDIO::Audio* audio();
        AUDIO::Artist* artist();

        template<typename T>
        void create_row_item(Schedule* schedule, int row=-1)
        {
            static_assert(std::is_base_of<SEDRIC::SedricScheduleItem, T>::value, "`T` must be derived from SedricScheduleItem");
            Columns columns;

            auto item = std::make_unique<T>(schedule);
            if (item->schedule_row_id() == -1){
                item->set_schedule_row_id(generate_row_id());
            }

            auto time = new T(item->time());
            time->setTextAlignment(Qt::AlignCenter);

            QMap<QString, QVariant> item_data;
            item_data["row_id"] = item->schedule_row_id();
            item_data["hour"] = schedule->schedule_hour()->value();
            item_data["time"] = schedule->schedule_time()->value();
            time->setData(item_data, Qt::UserRole);

            auto title = new T(item->title());
            auto artist = new T(item->artist_name());
            auto duration = new T(item->duration());
            auto transition = new T(item->transition());
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

            if (row > -1){
                m_model->insertRow(row, columns);
            }else{
                m_model->appendRow(columns);
            }

            m_schedule_items.push_back(std::move(item));
        }

    private:
        static int m_base_row_id;
        int m_schedule_row_id{-1};
        Schedule* m_schedule;
        QStandardItemModel* m_model;
        std::vector<std::unique_ptr<SedricScheduleItem>> m_schedule_items;
        AUDIO::Audio* m_audio;
        AUDIO::Artist* m_artist;
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
        bool operator()(const SedricScheduleItem* item) const
        {
            return ( (item->schedule()->schedule_date()->value() == m_date) &&
                     (item->schedule()->schedule_hour()->value() == m_hour) );
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
        bool operator()(const std::unique_ptr<SedricScheduleItem>& item) const
        {
            return ( item->schedule_row_id() == m_row_id );
        }
     private:
        int m_row_id{-1};
    };

}

