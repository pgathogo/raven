#include <map>
#include <sstream>
#include <algorithm>

#include <QStandardItemModel>
#include <QStandardItem>

#include "sedricscheduleitem.h"
#include "../framework/entitydatamodel.h"
#include "../framework/choicefield.h"
#include "../framework/ravenexception.h"
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
        if (schedule->audio()->entity() != nullptr)
        {
            m_audio = dynamic_cast<AUDIO::Audio*>(schedule->audio()->entity());
            m_artist = dynamic_cast<AUDIO::Artist*>(m_audio->artist()->entity());
        }

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
        clear_cached_items();
    }

    void SedricScheduleItem::clear_cached_items()
    {
        for(std::vector<Schedule*>::iterator it=m_cached_schedule_items.begin();
            it != m_cached_schedule_items.end(); ++it){
            Schedule* schedule = *it;
            delete schedule;
            schedule = nullptr;
        }

        m_cached_schedule_items.clear();
    }

    void SedricScheduleItem::clear_cached_items_date_hours(QDate date, std::vector<int> hours)
    {
        for (int hour : hours){
            m_cached_schedule_items.erase(
                        std::remove_if(m_cached_schedule_items.begin(), m_cached_schedule_items.end(),
                                       FindItemByDateHour(date, hour)), m_cached_schedule_items.end());
        }
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
                    m_audio->duration()->value()).toString("hh:mm:ss");
    }

    QString SedricScheduleItem::transition()
    {
        return stoq(schedule()->schedule_item_type()->value());
    }

    QString SedricScheduleItem::audio_type()
    {
        return stoq(schedule()->schedule_item_type()->value());
    }

    QString SedricScheduleItem::play_date()
    {
        return schedule()->play_date()->value().toString("dd/MM/yyyy");
    }
    QString SedricScheduleItem::play_time()
    {
        qDebug() << "Play Time: "<< schedule()->play_time()->value().toString("hhh:mm:ss");

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
        for (int i=from_row; i < m_model->rowCount(); ++i) {
            auto index = m_model->index(i, 0);  // first column
            auto data = index.data(Qt::UserRole).toMap();

            int row_id = data["row_id"].toInt();
            int schedule_hour = data["hour"].toInt();
            QTime time_stamp = data["time"].toTime();
            QString schedule_type = data["schedule_type"].toString();
            int schedule_id = data["schedule_id"].toInt();

            if (schedule_hour > hour)
                break;

            QTime new_time = time_stamp.addMSecs(duration);

            QMap<QString, QVariant> item_data;
            item_data["row_id"] = row_id;
            item_data["hour"] = schedule_hour;
            item_data["time"] = new_time;
            item_data["schedule_id"] = schedule_id;
            item_data["schedule_type"] = schedule_type;

            m_model->setData(index, item_data, Qt::UserRole);
            m_model->setData(index, new_time.toString("hh:mm:ss"), Qt::DisplayRole);

            update_schedule_item_time(row_id, new_time);
        }

    }

    void SedricScheduleItem::update_schedule_item_time(int row_id, QTime new_time)
    {
        auto iter = std::find_if(m_cached_schedule_items.begin(), m_cached_schedule_items.end(),
                                 FindRowId(row_id));

        if (iter != m_cached_schedule_items.end()){
            auto schedule = (*iter);
            if (schedule->schedule_item_type()->currText() != "COMM-BREAK"){
                schedule->set_schedule_time(new_time);
            }
        }
    }

    Schedule* SedricScheduleItem::find_schedule_item(int row_id)
    {
        auto iter = std::find_if(m_cached_schedule_items.begin(), m_cached_schedule_items.end(),
                                 FindRowId(row_id));

        if (iter != m_cached_schedule_items.end()){
            return *iter;
        }

        return nullptr;
    }

    void SedricScheduleItem::delete_row(int row_id)
    {
        m_cached_schedule_items.erase(
                    std::remove_if(m_cached_schedule_items.begin(), m_cached_schedule_items.end(),
                                   FindRowId(row_id)), m_cached_schedule_items.end());
    }

    AUDIO::Audio* SedricScheduleItem::audio()
    {
        return m_audio;
    }

    AUDIO::Artist* SedricScheduleItem::artist()
    {
        return m_artist;
    }


    std::vector<Schedule*> SedricScheduleItem::cached_items() const
    {
        return m_cached_schedule_items;
    }


    std::size_t SedricScheduleItem::display_item_count()
    {
        return m_display_items.size();
    }

    void SedricScheduleItem::clear_display_items()
    {
        if (m_display_items.size() == 0)
            return;

        auto it_start = m_display_items.begin();
        auto it_end = m_display_items.end();

        m_display_items.erase(it_start, it_end);
    }

    std::map<int, int> SedricScheduleItem::fetch_cached_items(QDate date, const std::vector<int>& hours)
    {
        std::map<int, int> processed;

        for (int hour : hours){

            processed[hour] = 0;

            for (auto schedule : m_cached_schedule_items){

                if ((schedule->schedule_hour()->value() == hour) &&
                    (schedule->schedule_date()->value() == date)){

                    m_display_items.push_back(schedule);

                    ++processed[hour];
                }

            }

        }

        return processed;
    }

    bool SedricScheduleItem::is_schedule_cached(Schedule* schedule)
    {
        auto iter = std::find_if(m_cached_schedule_items.begin(), m_cached_schedule_items.end(),
                                 FindRowId(schedule->display_row_id()->value()));

        return (iter != m_cached_schedule_items.end()) ? true : false;
    }

    void SedricScheduleItem::fetch_db_items(QDate date, std::vector<int>& hours)
    {
        if (hours.size() == 0)
            return;

        QString hours_str{""};
        for (int h : hours){
            if (!hours_str.isEmpty())
                hours_str += ", ";
            hours_str += QString::number(h);
        }

        std::stringstream sql;

        sql << " SELECT a.id, a.schedule_date, a.schedule_hour, a.schedule_time, "
            << " a.auto_transition, a.play_date, a.play_time, "
            << " a.schedule_item_type, a.comment, a.booked_spots, "
            << " a.audio_id, b.title, b.filepath, b.duration, c.fullname "
            << " FROM rave_schedule a  "
            << " left outer join rave_audio b on a.audio_id = b.id "
            << " left outer join rave_artist c on b.artist_id = c.id ";

        std::string where_filter = " WHERE a.schedule_date = '"+date.toString("yyyy/MM/dd").toStdString()+"'";
        std::string and_filter   = " AND a.schedule_hour in ("+hours_str.toStdString()+" )";
        std::string order_by     = " ORDER BY a.schedule_hour, a.schedule_time, a.id ";

        sql << where_filter << and_filter << order_by;

        qDebug() << stoq(sql.str());

        EntityDataModel edm;
        edm.readRaw(sql.str());

        auto provider = edm.getDBManager()->provider();

        if (provider->cacheSize() == 0)
            return;

        provider->cache()->first();

        int item_duration = 0;

        int current_hour = -1;

        do{
            Schedule* schedule = new Schedule();

            auto audio = dynamic_cast<AUDIO::Audio*>(schedule->audio()->dataModel()->get_entity().get());
            auto artist = dynamic_cast<AUDIO::Artist*>(audio->artist()->dataModel()->get_entity().get());

            // populate schedule fields
            set_schedule_fields(provider, schedule, audio, artist);

            if (schedule->schedule_hour()->value() != current_hour){
                item_duration = 0;
                current_hour = schedule->schedule_hour()->value();
            }

            if (schedule->schedule_item_type()->value() == "SONG" ||
                schedule->schedule_item_type()->value() == "COMM-AUDIO" ||
                schedule->schedule_item_type()->value() == "JINGLE")
            {
                if (item_duration > 0){
                    //schedule->set_schedule_time(schedule->schedule_time()->value().addMSecs(item_duration));
                }

                item_duration = item_duration + audio->duration()->value();
            }

            m_display_items.push_back(schedule);

            provider->cache()->next();

        } while (!provider->cache()->isLast());

    }


    void SedricScheduleItem::set_schedule_fields(BaseDataProvider* provider,
                                  Schedule* schedule,
                                  AUDIO::Audio* audio,
                                  AUDIO::Artist* artist)
    {
        auto it_begin = provider->cache()->currentElement()->begin();
        auto it_end = provider->cache()->currentElement()->end();

        for (; it_begin != it_end; ++it_begin){

            std::string field_name = (*it_begin).first;
            std::string field_value = (*it_begin).second;

            if (field_name == "id")
                schedule->setId(str_to_int(field_value));

            if (field_name == "schedule_date"){
                schedule->set_schedule_date(QDate::fromString(QString::fromStdString(field_value), "yyyy-MM-dd"));
            }

            if (field_name == "schedule_hour")
                schedule->set_schedule_hour(str_to_int(field_value));

            if (field_name == "schedule_time")
                schedule->set_schedule_time(QTime::fromString(QString::fromStdString(field_value), "hh:mm:ss"));

            if (field_name == "auto_transition")
                schedule->set_auto_transition(str_to_int(field_value));

            if (field_name == "play_date")
                schedule->set_play_date(QDate::fromString(QString::fromStdString(field_value), "yyyy/MM/dd"));

            if (field_name == "play_time") {
                qDebug() << stoq(field_value);
                schedule->set_play_time(QTime::fromString(QString::fromStdString(field_value), "hh:mm:ss"));
            }

            if (field_name == "comment")
                schedule->set_comment(field_value);

            if (field_name == "schedule_item_type")
                schedule->set_schedule_item_type(field_value);

            if (field_name == "booked_spots")
                schedule->set_booked_spots(str_to_int(field_value));

            if (field_name == "duration"){
                audio->set_duration(str_to_int(field_value));
                schedule->set_break_duration(str_to_int(field_value));
            }

            if (field_name == "audio_id"){
                schedule->set_audio(str_to_int(field_value));
                audio->setId(str_to_int(field_value));
            }

            if (field_name == "title")
                audio->set_title(field_value);

            if (field_name ==  "filepath")
                audio->set_file_path(field_value);

            if (field_name == "fullname")
                artist->set_fullname(field_value);
        }

    }

    void SedricScheduleItem::show_items(QDate current_date, const std::vector<int>& selected_hours)
    {
        std::vector<Schedule*> results;
        std::copy_if(m_display_items.begin(), m_display_items.end(), std::back_inserter(results),
                        FindScheduleByDate(current_date));

        std::sort(results.begin(), results.end(), SortScheduleByHour());

        for (int const& hour : selected_hours){

            for (auto schedule : results){
                if (schedule->schedule_hour()->value() == hour){

                    if (schedule->schedule_item_type()->currText() == "SONG"){
                        create_row_item<SEDRIC::SongItem>(schedule, m_model->rowCount());
                    }

                    if (schedule->schedule_item_type()->currText() == "COMM-BREAK"){
                        create_row_item<SEDRIC::CommercialBreakItem>(schedule, m_model->rowCount());
                    }

                    if (schedule->schedule_item_type()->currText() == "COMM-AUDIO"){
                        create_row_item<SEDRIC::CommercialAudioItem>(schedule, m_model->rowCount());
                    }

                    if (schedule->schedule_item_type()->currText() == "JINGLE"){
                        create_row_item<SEDRIC::JingleItem>(schedule, m_model->rowCount());
                    }

                }
            }

            create_end_marker(hour);

        }
    }

    void SedricScheduleItem::create_end_marker(int hour)
    {
        Schedule* schedule = new Schedule();
        schedule->set_schedule_hour(hour);
        schedule->set_schedule_time(QTime(hour, 0, 0));
        schedule->set_schedule_item_type("END_MARKER");
        create_row_item<SEDRIC::EndMarkerItem>(schedule, m_model->rowCount());
    }

    void SedricScheduleItem::new_schedule(QDate date, const std::vector<int>& hours)
    {
        for (int h : hours)
            log_activity(date, h);

        clear_cached_items_date_hours(date, hours);
        show_items(date, hours);
    }

    void SedricScheduleItem::log_activity(QDate date, int hour)
    {
        auto it = m_activities.find(date);
        if (it == m_activities.end()){
            m_activities[date].push_back(hour);
        }else{
            if (std::find(m_activities[date].begin(),
                          m_activities[date].end(),
                          hour) == m_activities[date].end())
                m_activities[date].push_back(hour);
        }
    }

    bool SedricScheduleItem::log_find(QDate date, int hour)
    {
        return ( (std::find(m_activities[date].begin(),
                          m_activities[date].end(), hour))== m_activities[date].end()) ? false : true;
    }


    std::map<QDate, std::vector<int>> SedricScheduleItem::activities() const
    {
        return m_activities;
    }

    std::string SedricScheduleItem::vec_to_str(const std::vector<int> ints)
    {
        QString str{""};
        for (int i : ints){
            if (!str.isEmpty())
                str += ", ";
            str += QString::number(i);
        }
        return str.toStdString();
    }

    std::string SedricScheduleItem::make_delete_stmts()
    {
        std::stringstream sql;

        for (auto [date, hours] : m_activities){
            std::string date_str = date.toString("yyyy/MM/dd").toStdString();
            std::string hour_str = vec_to_str(m_activities[date]);

            sql << "Delete From rave_schedule ";
            std::string where_filter = " Where schedule_date = '"+date_str+"'";
            std::string and_a_filter = " And schedule_hour in ("+hour_str+")";
            std::string and_b_filter = " And schedule_item_type <> 'COMM-BREAK'; ";

            sql << where_filter << and_a_filter << and_b_filter;

            }

        return sql.str();
    }

    std::string SedricScheduleItem::make_insert_stmts(const std::vector<Schedule*>& items)
    {
        if (m_activities.size() == 0)
            return "";

        std::string insert_stmts;
        VectorStruct<Field> fields;

        for (auto const schedule : items){

            if ( (schedule->schedule_item_type()->value() == "END_MARKER") ||
                 (schedule->schedule_item_type()->value() == "COMM-BREAK") )
                continue;

            if (log_find(schedule->schedule_date()->value(), schedule->schedule_hour()->value()))
            {
                fields << schedule->schedule_date()
                       << schedule->schedule_time()
                       << schedule->schedule_hour()
                       << schedule->auto_transition()
                       << schedule->audio()
                       << schedule->schedule_item_type()
                       << schedule->comment();

                insert_stmts += schedule->make_insert_stmt(fields.vec);
                fields.clear();
            }
        }

        return insert_stmts;

    }

    void SedricScheduleItem::copy_schedule(QDate curr_date, QDate dest_date, const std::map<int, int>& hour_map)
    {
        m_activities.clear();

        std::vector<Schedule*> tmp_copy;
        std::vector<Schedule*> sched_copy;

        for (auto [src_hour, dest_hour] : hour_map){
            tmp_copy.clear();
            std::copy_if(m_cached_schedule_items.begin(), m_cached_schedule_items.end(),
                        std::back_inserter(tmp_copy),
                         FindItemByDateHour(curr_date, src_hour));

            std::transform(tmp_copy.begin(), tmp_copy.end(), std::back_inserter(sched_copy),
                           [&](auto* schedule){

                int m = schedule->schedule_time()->value().minute();
                int s = schedule->schedule_time()->value().second();

                schedule->set_schedule_time(QTime(dest_hour, m, s));
                schedule->set_schedule_date(dest_date);
                schedule->set_schedule_hour(dest_hour);

                return schedule;
            });

            log_activity(dest_date, dest_hour);
        }

        std::string del_stmts = make_delete_stmts();
        std::string ins_stmts = make_insert_stmts(sched_copy);

        delete_current_schedule(del_stmts);
        write_schedule_to_db(ins_stmts);

    }


    void SedricScheduleItem::delete_current_schedule(std::string sql)
    {
        EntityDataModel edm;

        try{
            edm.executeRawSQL(sql);
        } catch(DatabaseException& de){
            showMessage(de.errorMessage());
        }
    }

    bool SedricScheduleItem::write_schedule_to_db(std::string sql)
    {
        try{
            EntityDataModel edm;
            edm.executeRawSQL(sql);
            showMessage("Schedule saved successfully.");
            return true;
        }catch (DatabaseException& de) {
            showMessage(de.errorMessage());
            return false;
        }
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
        setBackground(QBrush(QColor("#7393B3")));
        setForeground(QBrush(QColor("white")));
    }

    CommercialBreakItem::~CommercialBreakItem()
    {
    }

    QString CommercialBreakItem::title()
    {
        std::string comm = "Commercial Break ("+std::to_string(schedule()->booked_spots()->value())+")";
        return QString::fromStdString(comm);
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

    QString CommercialBreakItem::track_path()
    {
        return "";
    }

    QString CommercialBreakItem::comment()
    {
        return "";
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
        setBackground(QBrush(QColor("#FA8072")));
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
