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
    QString comment{""};
    QString type{"SONG"};
    int auto_transition;
    QDate play_date;
    QTime play_time;
    int schedule_ref{0}; // auto generated
    int table_row_id{0};
};

class ScheduleData{
public:
    ScheduleData(ItemData*);
    ~ScheduleData();
    int schedule_ref() const;
    virtual QString title() const;
    virtual QString artist() const;
    virtual QString track_path() const;
    virtual QTime duration() const;
    virtual QString transition() const;

    ItemData* item_data();
    void set_table_row_id(int) const;
    std::string transition_to_string(int) const;
    void set_transitions();
private:
    ItemData* m_item_data;
    std::map<int, std::string> m_transitions;
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


// SongItem
class SongItem : public ScheduleItem
{
public:
    SongItem();
    SongItem(const QString);
    ScheduleData* make_schedule_data(ItemData*) const override;
};

class EndMarkerItem : public ScheduleItem
{
public:
    EndMarkerItem();
    EndMarkerItem(const QString);
    ScheduleData* make_schedule_data(ItemData*) const override;
};

class ItemBuilder
{
public:
    ItemBuilder();
    ~ItemBuilder();

    int get_last_item_id();
    void set_last_item_id(int id);
    int generate_row_id();

    std::vector<ScheduleData*> cached_data(int);
    std::size_t cache_size() const;
    void print_cache() const;

    template<typename T>
    ItemColumns create_item(ItemData* item_data){
        ItemColumns tr;

        T item;
        auto item_schedule_data = item.make_schedule_data(item_data);

        auto time = new T(item_data->schedule_time.toString("hh:mm:ss"));

        time->setTextAlignment(Qt::AlignCenter);
        time->setData(item_data->schedule_time, Qt::UserRole);

        auto title = new T(item_schedule_data->title());
        title->setData(item_data->id, Qt::UserRole);

        auto artist = new T(item_schedule_data->artist());
        auto duration = new T(item_schedule_data->duration().toString("hh:mm:ss"));
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

        if (item_schedule_data->item_data()->table_row_id == 0){
            item_schedule_data->set_table_row_id(this->generate_row_id());
            m_schedule_items.push_back(item_schedule_data);
        }

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
        if (item_type == "END_MARKER"){
            size.setHeight(50);
            return size;
        }
        return size;
    }
};

