#pragma once

#include <map>
#include <QStandardItem>
#include <QTime>

enum TrackType{Song, Jingle, Drop, Commercial};

using TrackColumns = QList<QStandardItem*>;

struct TrackData{
    int id;
    QString title{""};
    QString artist{""};
    int duration{0};
    int auto_transition{0};
    QDate play_date;
    QTime play_time;
    QString audio_type{""};
    QString audio_file{""};
};

class AudioLibItem{
};

// SongAudioItem
// JingleAudioItem
// DropAudioItem
// CommAudioItem

class TrackItemData
{
public:
    TrackItemData(TrackData* const);
    ~TrackItemData();

    virtual QString title() const;
    virtual QString artist() const;
    virtual int duration() const;
    virtual QString audio_type() const;
    virtual QString audio_file() const;

    TrackData* track_data() const;
    QString duration_to_string(int duration);
private:
    TrackData* const m_track_data;
};


class SongItemData : public TrackItemData
{
public:
    SongItemData(TrackData* const);
};

class JingleItemData: public TrackItemData
{
public:
    JingleItemData(TrackData* const);
};

class CommercialItemData : public TrackItemData
{
public:
    CommercialItemData(TrackData* const);
};

/* ---- TrackItem --- */

class TrackItem : public QStandardItem
{
public:
    TrackItem();
    TrackItem(const QString);
    virtual TrackItemData* make_track_item_data(TrackData* const) const = 0;
private:
};

class SongTrackItem : public TrackItem
{
public:
    SongTrackItem();
    SongTrackItem(const QString);
    TrackItemData* make_track_item_data(TrackData* const) const override;
};

class JingleTrackItem : public TrackItem
{
public:
    JingleTrackItem();
    JingleTrackItem(const QString);
    TrackItemData* make_track_item_data(TrackData* const) const override;
};

class CommercialTrackItem : public TrackItem
{
public:
    CommercialTrackItem();
    CommercialTrackItem(const QString);
    TrackItemData* make_track_item_data(TrackData* const) const override;
};

class TrackItemBuilder
{
public:
    TrackItemBuilder();
    ~TrackItemBuilder();
    int string_to_track_type(std::string) const;
    void set_track_map();

    TrackItemData* track_item_data(int) const;

    template<typename T>
    TrackColumns create_item(TrackData* const track_data)
    {
        TrackColumns columns;

        T track_item;

        auto track_item_data = track_item.make_track_item_data(track_data);

        auto title = new T(track_data->title);
        title->setData(track_data->id, Qt::UserRole);

        auto artist = new T(track_data->artist);
        artist->setData(track_data->artist, Qt::UserRole);

        QString str_duration_msec = track_item_data->duration_to_string(track_data->duration);
        auto duration = new T(str_duration_msec);
        duration->setTextAlignment(Qt::AlignCenter);
        duration->setData(track_data->duration, Qt::UserRole);

        auto audio_type = new T(track_data->audio_type);
        audio_type->setData(track_data->audio_type, Qt::UserRole);

        auto audio_file = new T(track_data->audio_file);
        audio_file->setData(track_data->audio_file, Qt::UserRole);

        columns.append(title);
        columns.append(artist);
        columns.append(duration);
        columns.append(audio_type);
        columns.append(audio_file);

        m_tracks[track_data->id] = track_item_data;

        return columns;
    }
private:
    std::map<std::string, int> m_track_type_map;
    std::map<int, TrackItemData*> m_tracks;
};

