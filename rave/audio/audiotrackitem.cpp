#include "audiotrackitem.h"

TrackItemData::TrackItemData(TrackData* const track_data)
    :m_track_data{track_data}
{
}

TrackItemData::~TrackItemData()
{
    delete m_track_data;
}

QString TrackItemData::title() const
{
}

QString TrackItemData::artist() const
{
}

int TrackItemData::duration() const
{
}

QString TrackItemData::audio_type() const
{
}

QString TrackItemData::audio_file() const
{
}

QString TrackItemData::duration_to_string(int duration)
{
    if (duration > 0){
        QTime time = QTime(QTime::fromMSecsSinceStartOfDay(duration*1000));
        return time.toString("hh:mm:ss");
    } else {
        return "00:00:00";
    }
}

TrackData* TrackItemData::track_data() const
{
    return m_track_data;
}

/* -- SongItemData --- */
SongItemData::SongItemData(TrackData* const track_data)
    :TrackItemData(track_data)
{
}

/* -- JingletemData --- */
JingleItemData::JingleItemData(TrackData* const track_data)
    :TrackItemData(track_data)
{
}

CommercialItemData::CommercialItemData(TrackData* const track_data)
    :TrackItemData(track_data)
{
}


TrackItem::TrackItem()
{
}

TrackItem::TrackItem(const QString text)
{
    setText(text);
    setEditable(false);
}


SongTrackItem::SongTrackItem()
{
}

SongTrackItem::SongTrackItem(const QString text)
    :TrackItem(text)
{
}

TrackItemData* SongTrackItem::make_track_item_data(TrackData* const track_data) const
{
    return new SongItemData(track_data);
}

JingleTrackItem::JingleTrackItem()
{
}

JingleTrackItem::JingleTrackItem(const QString text)
    :TrackItem(text)
{
    setForeground(QBrush(QColor(6,144,2)));
}

TrackItemData* JingleTrackItem::make_track_item_data(TrackData* const track_data) const
{
    return new JingleItemData(track_data);
}

CommercialTrackItem::CommercialTrackItem()
{
}
CommercialTrackItem::CommercialTrackItem(const QString text)
    :TrackItem(text)
{
    setForeground(QBrush(QColor(144,2,9)));
}

TrackItemData* CommercialTrackItem::make_track_item_data(TrackData* const track_data) const
{
    return new CommercialItemData(track_data);
}

TrackItemBuilder::TrackItemBuilder()
{
}

TrackItemBuilder::~TrackItemBuilder()
{
}

void TrackItemBuilder::set_track_map()
{
    m_track_type_map["Song"]       = 1;
    m_track_type_map["Jingle"]     = 2;
    m_track_type_map["Drop"]       = 3;
    m_track_type_map["Commercial"] = 4;
}

TrackItemData* TrackItemBuilder::track_item_data(int value) const
{
   std::map<int, TrackItemData*>::const_iterator iter = m_tracks.find(value);
   if (iter != m_tracks.end())
       return iter->second;
   return nullptr;
}

int TrackItemBuilder::string_to_track_type(std::string value) const
{
    std::map<std::string, int>::const_iterator iter = m_track_type_map.find(value);
    if (iter != m_track_type_map.end())
        return iter->second;
    return -1;
}
