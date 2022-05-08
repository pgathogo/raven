#include <string>
#include <algorithm>
#include "scheduleitem.h"

ScheduleData::ScheduleData(ItemData* item_data)
    :m_item_data{item_data}
{
    set_transitions();
}

ScheduleData::~ScheduleData()
{
    delete m_item_data;
}

int ScheduleData::schedule_ref() const
{
    return m_item_data->schedule_ref;
}

int ScheduleData::schedule_id() const
{
    return m_item_data->id;
}

int ScheduleData::row_id() const
{
    return m_item_data->table_row_id;
}

ItemData* ScheduleData::item_data() const
{
    return m_item_data;
}

void ScheduleData::set_table_row_id(int row_id) const
{
    m_item_data->table_row_id = row_id;
}

void ScheduleData::set_transitions()
{
    m_transitions[0] = "None";
    m_transitions[1] = "Mix";
    m_transitions[2] = "Cut";
    m_transitions[3] = "Back";
    m_transitions[4] = "Center";
    m_transitions[5] = "Early";
    m_transitions[6] = "Synchro";
    m_transitions[7] = "Stop";
}

void ScheduleData::set_schedule_time(QTime time)
{
    m_item_data->schedule_time = time;
}

std::string ScheduleData::transition_to_string(int value) const
{
    std::map<int, std::string>::const_iterator iter = m_transitions.find(value);
    if (iter != m_transitions.end())
        return iter->second;

    return "Mix";

}

QString ScheduleData::title() const{ return ""; }
QString ScheduleData::artist() const{ return ""; }
QString ScheduleData::track_path() const{return ""; }
QTime ScheduleData::duration() const{return QTime(); }

QString ScheduleData::transition() const
{
    return QString::fromStdString(transition_to_string(m_item_data->auto_transition));
}

QDate ScheduleData::schedule_date() const
{
    return item_data()->schedule_date;
}

QTime ScheduleData::schedule_time() const
{
    return item_data()->schedule_time;

}

int ScheduleData::schedule_hour() const
{
    return item_data()->schedule_hour;
}

int ScheduleData::auto_transition() const
{
    return item_data()->auto_transition;
}

QString ScheduleData::schedule_type() const
{
    return schedule_item_type();
}

int ScheduleData::audio_id() const
{
    return item_data()->audio_id;
}

CommercialBreak::CommercialBreak(ItemData* item_data)
    :ScheduleData(item_data)
    ,m_break_id{item_data->id}
    ,m_title{""}
    ,m_duration{}
{
}

QString CommercialBreak::title() const
{
    return "Commercial Break";
}

QString CommercialBreak::artist() const
{
    return "";
}

QString CommercialBreak::track_path() const
{
    return "";
}

QTime CommercialBreak::duration() const
{
    return QTime();
}

CommercialAudio::CommercialAudio(ItemData* item_data)
    :ScheduleData(item_data)
    ,m_title{""}
    ,m_artist{""}
    ,m_track_path{""}
    ,m_duration{}
{
}

QString CommercialAudio::title() const
{
    return item_data()->item_title;
}

QString CommercialAudio::artist() const
{
    return item_data()->artist;
}

QString CommercialAudio::track_path() const
{
    return item_data()->filepath;
}

QTime CommercialAudio::duration() const
{
    return QTime::fromMSecsSinceStartOfDay(
                item_data()->duration*1000);
}

Song::Song(ItemData* item_data)
    :ScheduleData(item_data)
    ,m_title{""}
    ,m_artist{""}
    ,m_track_path{""}
    ,m_duration{}
{
}

QString Song::title() const
{
    return item_data()->item_title;
}

QString Song::artist() const
{
    return item_data()->artist;
}

QString Song::track_path() const
{
    return item_data()->filepath;
}

QTime Song::duration() const
{
    return QTime::fromMSecsSinceStartOfDay(item_data()->duration*1000);
}

Jingle::Jingle(ItemData* item_data)
    :ScheduleData(item_data)
{
}

QString Jingle::title() const
{
    return item_data()->item_title;
}

QString Jingle::artist() const
{
    return item_data()->artist;
}

QString Jingle::track_path() const
{
    return item_data()->filepath;

}

QTime Jingle::duration() const
{
    return QTime::fromMSecsSinceStartOfDay(
                item_data()->duration*1000);
}


EndMarker::EndMarker(ItemData* item_data)
    :ScheduleData(item_data)
{
}

QString EndMarker::title() const
{
    return "END";
}

QString EndMarker::artist() const
{
    return "";
}

QString EndMarker::track_path() const
{
    return "";
}

QTime EndMarker::duration() const
{
    return QTime();
}

QString EndMarker::transition() const
{
    return "";
}

ScheduleItem::ScheduleItem()
{
}

ScheduleItem::ScheduleItem(const QString text)
{
    setText(text);
    setSizeHint(QSize(text.length(), 100));
    setEditable(false);
}

ScheduleItem::~ScheduleItem()
{
}

CommercialBreakItem::CommercialBreakItem()
    :ScheduleItem()
{
}

CommercialBreakItem::CommercialBreakItem(const QString text)
    :ScheduleItem(text)
{
    setBackground(QBrush(QColor("darkCyan")));
    setForeground(QBrush(QColor("white")));
    setFlags(flags() & ~(Qt::ItemIsDropEnabled));

}

ScheduleData* CommercialBreakItem::make_schedule_data(ItemData* item_data) const
{
    return new CommercialBreak(item_data);
}

CommercialAudioItem::CommercialAudioItem()
    :ScheduleItem()
{
}

CommercialAudioItem::CommercialAudioItem(const QString text)
    :ScheduleItem(text)
{
    setForeground(QBrush(QColor("darkCyan")));
}

ScheduleData* CommercialAudioItem::make_schedule_data(ItemData* item_data) const
{
    return new CommercialAudio(item_data);
}

SongItem::SongItem()
    :ScheduleItem()
{
}

SongItem::SongItem(const QString text)
    :ScheduleItem(text)
{
    setBackground(QBrush(QColor(234, 249, 142)));
    setFlags(flags()  & ~(Qt::ItemIsDropEnabled));
}

ScheduleData* SongItem::make_schedule_data(ItemData* item_data) const
{
    return new Song(item_data);
}

JingleItem::JingleItem()
{
}

JingleItem::JingleItem(const QString text)
    :ScheduleItem(text)
{
    setForeground(QBrush(QColor("green")));
}

ScheduleData* JingleItem::make_schedule_data(ItemData* item_data) const
{
    return new Jingle(item_data);
}


EndMarkerItem::EndMarkerItem()
    :ScheduleItem()
{
}

EndMarkerItem::EndMarkerItem(const QString text)
    :ScheduleItem(text)
{
    setBackground(QBrush(QColor("red")));
}

ScheduleData* EndMarkerItem::make_schedule_data(ItemData* item_data) const
{
    return new EndMarker(item_data);
}

ItemBuilder::ItemBuilder()
    :m_last_item_id{0}
{
}

ItemBuilder::~ItemBuilder()
{
    m_schedule_items.clear();
}

int ItemBuilder::get_last_item_id()
{
    return m_last_item_id;
}

void ItemBuilder::set_last_item_id(int id)
{
    m_last_item_id = id;
}

int ItemBuilder::generate_row_id()
{
    int id = get_last_item_id()+1;
    set_last_item_id(id);
    return id;
}

std::vector<ScheduleData*> ItemBuilder::all_cached()
{
    return m_schedule_items;
}

std::vector<ScheduleData*> ItemBuilder::cached_data(int schedule_ref)
{
    std::vector<ScheduleData*> results(m_schedule_items.size());
    auto iter = std::copy_if(m_schedule_items.begin(), m_schedule_items.end(), results.begin(), FindRef(schedule_ref));
    results.resize(std::distance(results.begin(), iter));

    return results;
}

std::vector<int> ItemBuilder::unique_hours()
{
    std::sort(m_schedule_items.begin(), m_schedule_items.end(), SortItems());

    std::vector<ScheduleData*> results(m_schedule_items.size());
    auto iter = std::unique_copy(m_schedule_items.begin(), m_schedule_items.end(), results.begin(), FindUnique());
    results.resize(std::distance(results.begin(), iter));

    std::vector<int> hours;


    for (auto result : results){
        qDebug() << "Result: " << result->title();
        if (result->schedule_type() != "END-MARKER")
            hours.push_back(result->schedule_hour());
    }

    return hours;
}

std::size_t ItemBuilder::cache_size() const
{
    return m_schedule_items.size();
}

void ItemBuilder::print_cache() const
{
    for(auto& schedule_item : m_schedule_items){
        qDebug() << "Schedule Ref: " << schedule_item->schedule_ref();
        qDebug() << "Title: " << schedule_item->title();
        qDebug() << "Row ID: " << schedule_item->item_data()->table_row_id;
    }
}

QString ScheduleData::schedule_item_type() const
{
    return m_item_data->type;
}

ScheduleData* ItemBuilder::find_data_by_rowid(int row_id)
{
    auto iter = std::find_if(m_schedule_items.begin(), m_schedule_items.end(),
                             FindRowId(row_id));
    if (iter != m_schedule_items.end())
        return *iter;

    return nullptr;
}

void ItemBuilder::delete_row(int row_id)
{
    m_schedule_items.erase(
                std::remove_if(m_schedule_items.begin(), m_schedule_items.end(),
                               FindRowId(row_id)), m_schedule_items.end());
}

bool ItemBuilder::item_exist(QString item_type, QTime time)
{
    auto iter = std::find_if(m_schedule_items.begin(), m_schedule_items.end(),
                             FindItem(item_type, time));
    return (iter == m_schedule_items.end()) ? false : true;
}

void ItemBuilder::update_schedule_item_time(QTime time, int row_id)
{
    auto iter = std::find_if(m_schedule_items.begin(), m_schedule_items.end(),
                             FindRowId(row_id));

    if (iter != m_schedule_items.end()){
        ScheduleData* data = *iter;
        if (data->schedule_item_type() == "COMM-BREAK")
            data->set_schedule_time(time);
    }

}
