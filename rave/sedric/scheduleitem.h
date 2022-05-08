#pragma once

#include <QStandardItem>
#include <QTime>
#include <QString>
#include <QDate>
#include <QStyledItemDelegate>
#include <QDebug>


using ItemColumns = QList<QStandardItem*>;


struct ItemData
{
    int id;
    QDate schedule_date;
    QTime schedule_time;
    int schedule_hour{0};
    QString comment{""};
    QString type{"SONG"};
    int auto_transition;
    QDate play_date;
    QTime play_time;
    int schedule_ref{0}; // auto generated
    int table_row_id{0};
    int booked_spots{0};
    int audio_id{0};
    int duration{0};
    QString item_title{""};
    QString artist{""};
    QString filepath{""};
    bool new_item{true};
};

class ScheduleData{
public:
    ScheduleData(ItemData*);
    ~ScheduleData();
    int schedule_ref() const;
    int schedule_id() const;
    int row_id() const;
    QString schedule_item_type() const;
    virtual QString title() const;
    virtual QString artist() const;
    virtual QString track_path() const;
    virtual QTime duration() const;
    virtual QString transition() const;

    QDate schedule_date() const;
    QTime schedule_time() const;
    int schedule_hour() const;
    int auto_transition() const;
    QString schedule_type() const;
    int audio_id() const;

    ItemData* item_data() const;
    void set_table_row_id(int) const;
    std::string transition_to_string(int) const;
    void set_transitions();
    void set_schedule_time(QTime);
private:
    ItemData* m_item_data;
    std::map<int, std::string> m_transitions;
};

struct FindRef
{
    FindRef(int ref) : m_ref(ref){}
    bool operator()(const ScheduleData* data) const
    {
        return data->schedule_ref() == m_ref;
    }
private:
        int m_ref;
};

struct FindRowId
{
    FindRowId(int row_id)
        : m_row_id{ row_id } {}
    bool operator()(const ScheduleData* data) const
    {
        return data->row_id() ==  m_row_id;
    }
private:
    int m_row_id;
};

struct FindItem
{
    FindItem(QString item_type, QTime time)
        :m_item_type{item_type}
        ,m_time{time}
    {}
    bool operator()(const ScheduleData* data) const
    {
        return ((data->schedule_item_type() == m_item_type) &&
                (data->schedule_time() == m_time));
    }
private:
    QString m_item_type;
    QTime m_time;
};

struct FindUnique
{
    FindUnique(){};
    bool operator()(const ScheduleData* first_item, ScheduleData* second_item) const
    {
        return first_item->schedule_hour() == second_item->schedule_hour();
    }
};

struct SortItems
{
    bool operator()(const ScheduleData* first_item, ScheduleData* second_item) const
    {
        return (first_item->schedule_hour() < second_item->schedule_hour());
    }
};

class CommercialBreak : public ScheduleData
{
public:
    CommercialBreak(ItemData*);
    ~CommercialBreak();
    QString title() const override;
    QString artist() const override;
    QString track_path() const override;
    QTime duration() const override;
private:
    int m_break_id;
    QString m_title;
    QTime m_duration;
   // std::vector<BookedSpot> m_booked_spots;
};

class CommercialAudio : public ScheduleData
{
public:
    CommercialAudio(ItemData*);
    ~CommercialAudio();
    QString title() const override;
    QString artist() const override;
    QString track_path() const override;
    QTime duration() const override;
private:
    QString m_title;
    QString m_artist;
    QString m_track_path;
    QTime m_duration;
};

class Song : public ScheduleData
{
public:
    Song(ItemData*);
    QString title() const override;
    QString artist() const override;
    QString track_path() const override;
    QTime duration() const override;
private:
    QString m_title;
    QString m_artist;
    QString m_track_path;
    QTime m_duration;
};

class Jingle : public ScheduleData
{
public:
    Jingle(ItemData*);
    QString title() const override;
    QString artist() const override;
    QString track_path() const override;
    QTime duration() const override;
private:
    QString m_title;
    QString m_artist;
    QString m_track_path;
    QTime m_duration;
};

class EndMarker : public ScheduleData
{
public:
    EndMarker(ItemData*);
    QString title() const override;
    QString artist() const override;
    QString track_path() const override;
    QTime duration() const override;
    QString transition() const override;
};


class ScheduleItem : public QStandardItem
{
public:
    ScheduleItem();
    ScheduleItem(const QString);
    ~ScheduleItem();
    virtual ScheduleData* make_schedule_data(ItemData*) const = 0;
};


// CommercialItem
class CommercialBreakItem : public ScheduleItem
{
public:
    CommercialBreakItem();
    CommercialBreakItem(const QString);
    ScheduleData* make_schedule_data(ItemData*) const override;
};

class CommercialAudioItem : public ScheduleItem
{
public:
    CommercialAudioItem();
    CommercialAudioItem(const QString);
    ScheduleData* make_schedule_data(ItemData*) const override;
};

// SongItem
class SongItem : public ScheduleItem
{
public:
    SongItem();
    SongItem(const QString);
    ScheduleData* make_schedule_data(ItemData*) const override;
};

class JingleItem : public ScheduleItem
{
public:
    JingleItem();
    JingleItem(const QString);
    ScheduleData* make_schedule_data(ItemData*) const override;
};

class EndMarkerItem : public ScheduleItem
{
public:
    EndMarkerItem();
    EndMarkerItem(const QString);
    ScheduleData* make_schedule_data(ItemData*) const override;
};

class ScheduleItemHandler
{};


class ItemBuilder
{
public:
    ItemBuilder();
    ~ItemBuilder();

    enum AudioTransition{None=0, Mix, Cut, Back, Center, Early, Syncro, Stop};

    int get_last_item_id();
    void set_last_item_id(int id);
    int generate_row_id();

    std::vector<ScheduleData*> cached_data(int);
    std::vector<ScheduleData*> all_cached();
    std::size_t cache_size() const;
    void print_cache() const;
    std::vector<int> unique_hours();

    // make this a templated function
    ScheduleData* find_data_by_rowid(int);

    bool item_exist(QString, QTime);

    void delete_row(int);

    void update_schedule_item_time(QTime, int);

    template<typename T>
    ItemColumns create_item(ItemData* item_data){
        ItemColumns tr;

        T item;
        auto item_schedule_data = item.make_schedule_data(item_data);
        if (item_data->table_row_id == 0){
            item_schedule_data->set_table_row_id(this->generate_row_id());
        }

        auto time = new T(item_data->schedule_time.toString("hh:mm:ss"));

        time->setTextAlignment(Qt::AlignCenter);
        time->setData(item_data->schedule_time, Qt::UserRole);

        auto title = new T(item_schedule_data->title());
        QMap<QString, QVariant> record_identifier;
        record_identifier["id"] = item_data->id;
        record_identifier["row_id"] = item_schedule_data->item_data()->table_row_id;
        record_identifier["type"] = item_data->type;
        title->setData(record_identifier, Qt::UserRole);

        auto artist = new T(item_schedule_data->artist());
        auto duration = new T(item_schedule_data->duration().toString("hh:mm:ss"));
        duration->setData(item_data->duration, Qt::UserRole);
        duration->setTextAlignment(Qt::AlignCenter);

        auto transition = new T(item_schedule_data->transition());

        auto play_date = new T(item_data->play_date.toString("dd/MM/yyyy"));
        auto play_time = new T(item_data->play_time.toString("hh:mm:ss"));
        play_time->setTextAlignment(Qt::AlignCenter);

        auto track_path = new T(item_schedule_data->track_path());
        auto comment = new T(item_data->comment);
        comment->setData(item_data->type, Qt::UserRole);

        tr.append(time);
        tr.append(title);
        tr.append(artist);
        tr.append(duration);
        tr.append(transition);
        tr.append(play_date);
        tr.append(play_time);
        tr.append(track_path);
        tr.append(comment);

        if (item_data->new_item)
            m_schedule_items.push_back(item_schedule_data);

        return tr;
    }

private:
    int m_last_item_id{0};
    std::vector<ScheduleData*> m_schedule_items;
};

class ScheduleDelegate : public QStyledItemDelegate
{
public:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override{
    }
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override{
        QSize size = QStyledItemDelegate::sizeHint(option, index);
        auto model = index.model();
        auto model_index = model->index(index.row(), model->columnCount());
        auto item_type = model_index.data(Qt::UserRole).toString();
        if (item_type == "END-MARKER"){
            size.setHeight(50);
            return size;
        }
        return size;
    }
};

