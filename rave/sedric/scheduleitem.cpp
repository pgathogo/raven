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

ItemData* ScheduleData::item_data()
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

std::string ScheduleData::transition_to_string(int value) const
{
    std::map<int, std::string>::const_iterator iter = m_transitions.find(value);
    if (iter != m_transitions.end())
        return iter->second;

    return "";

}

QString ScheduleData::title() const{ return ""; }
QString ScheduleData::artist() const{ return ""; }
QString ScheduleData::track_path() const{return ""; }
QTime ScheduleData::duration() const{return QTime(); }

QString ScheduleData::transition() const
{
    return QString::fromStdString(transition_to_string(m_item_data->auto_transition));
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
    return "Song Song Song";
}

QString Song::artist() const
{
    return "";
}

QString Song::track_path() const
{
    return "";
}

QTime Song::duration() const
{
    return QTime();
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


std::vector<ScheduleData*> ItemBuilder::cached_data(int schedule_ref)
{
    std::vector<ScheduleData*> results(m_schedule_items.size());
    auto iter = std::copy_if(m_schedule_items.begin(), m_schedule_items.end(), results.begin(), FindRef(schedule_ref));
    results.resize(std::distance(results.begin(), iter));

    return results;
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
