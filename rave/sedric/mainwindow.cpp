#include <sstream>
#include <string>
#include <memory>
#include <algorithm>

#include <QStandardItemModel>
#include <QStandardItem>
#include <QHeaderView>
#include <QDebug>
#include <QVariant>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "scheduleitem.h"
#include "../utils/tools.h"

#include "../framework/entitydatamodel.h"
#include "../framework/ravenexception.h"
#include "../framework/databasemanager.h"
#include "../framework/tree.h"
#include "../framework/treeviewmodel.h"
#include "../framework/tableviewmodel.h"
#include "../audio/audiolibrary.h"
#include "../audio/audio.h"
#include "../audio/audiotrackitem.h"
#include "../audio/audiotool.h"
#include "../framework/entitydatamodel.h"
#include "../framework/schedule.h"
#include "../framework/choicefield.h"
#include "../utils/tools.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_schedule_model{nullptr}
    , m_tree_model{nullptr}
{
    ui->setupUi(this);

    m_schedule_item_builder = new ItemBuilder();

    m_track_item_builder = new TrackItemBuilder();

    AudioLibrary audio_lib;
    //auto node_data = audio_lib.read_data_from_file("tree.txt");
    auto node_data = audio_lib.read_data_from_db();
    m_tree_model = new TreeViewModel(node_data, this);
    ui->tvFolders->setModel(m_tree_model);
    audio_folder_setup();
    connect(ui->tvFolders, &QTreeView::clicked, this, &MainWindow::folder_clicked);

    m_tracks_model = new QStandardItemModel(this);
    create_track_view_headers();
    ui->tvTracks->horizontalHeader()->setStretchLastSection(true);
    ui->tvTracks->setModel(m_tracks_model);
    ui->tvTracks->verticalHeader()->setVisible(false);

    // For Width adjustment to be effected, it *MUST* be done after
    // the "setModel" method
    set_track_view_column_width();

//    ScheduleDelegate delegate;
//    ui->tvSchedule->setItemDelegate(&delegate);

    m_schedule_model = new QStandardItemModel(this);
    create_model_headers();
    ui->tvSchedule->setModel(m_schedule_model);

    ui->tvSchedule->setDragDropOverwriteMode(false);
    ui->tvSchedule->setDragEnabled(true);
    ui->tvSchedule->setDragDropMode(QAbstractItemView::InternalMove);

    ui->tvSchedule->horizontalHeader()->setStretchLastSection(true);
    ui->tvSchedule->verticalHeader()->setVisible(false);

    set_column_width();
    populate_hours();

    connect(ui->btnNew, &QPushButton::clicked, this, &MainWindow::remove_all);
    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::item_id);
    connect(ui->btnRemove, &QPushButton::clicked, this, &MainWindow::remove_current_schedule_item);
    connect(ui->dtSchedule, &QDateEdit::dateChanged, this, &MainWindow::date_changed);
    connect(ui->tvSchedule, &QTableView::doubleClicked, this, &MainWindow::insert_item);
    connect(ui->cbHours, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::hour_changed);
    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::save_schedule);

    ui->dtSchedule->setDate(QDate::currentDate());

    setWindowTitle("Raven - SeDRic");

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::folder_clicked(const QModelIndex& index)
{
    int folder_id = index.data(Qt::UserRole).toInt();
    fetch_audio(folder_id);
}

void MainWindow::insert_item(const QModelIndex& index)
{
    auto mod_index = ui->tvTracks->currentIndex();
    auto first_col = ui->tvTracks->model()->index(mod_index.row(), 0);
    auto track_id = first_col.data(Qt::UserRole).toInt();

    TrackItemData* data = m_track_item_builder->track_item_data(track_id);

    if (data == nullptr)
        return;

    auto schedule_time_col = ui->tvSchedule->model()->index(index.row(), 0);
    auto schedule_time = schedule_time_col.data(Qt::UserRole).toTime();

    ItemData* item_data = new ItemData();
    item_data->id = -1;
    item_data->table_row_id = 0;
    item_data->schedule_date = ui->dtSchedule->date();
    item_data->schedule_time = schedule_time;
    item_data->schedule_hour = ui->cbHours->currentData().toInt();

    item_data->schedule_ref = item_data->schedule_date.day()+
                              item_data->schedule_date.month()+
                              item_data->schedule_date.year()+
                              item_data->schedule_hour;

    item_data->auto_transition = ItemBuilder::AudioTransition::Mix;
    item_data->type = data->track_data()->audio_type;
    item_data->audio_id = data->track_data()->id;
    item_data->duration = data->track_data()->duration;
    item_data->item_title = data->track_data()->title;
    item_data->artist = data->track_data()->artist;
    item_data->filepath = data->track_data()->audio_file;

    auto item = m_schedule_item_builder->create_item<SongItem>(item_data);
    m_schedule_model->insertRow(index.row(), item);
    increment_time(index.row(), data->track_data()->duration*1000);
}

void MainWindow::remove_current_schedule_item()
{
    auto mod_index = ui->tvSchedule->currentIndex();
    if (mod_index.row() < 0)
        return;

    int row = mod_index.row();

    int row_id = get_schedule_row_id(row);

    auto data = m_schedule_item_builder->find_data_by_rowid(row_id);
    if (data == nullptr)
        return;

    if (!is_item_deletable(data->schedule_item_type()))
        return;

    m_schedule_item_builder->delete_row(row_id);
    remove_item(row);
    decrement_time(row, data->item_data()->duration);
}

int MainWindow::get_schedule_row_id(int row) const
{
    auto title_column = ui->tvSchedule->model()->index(row, 1);
    auto record_identifier = title_column.data(Qt::UserRole).toMap();
    return record_identifier["row_id"].toInt();
}

QString MainWindow::get_schedule_type(int row) const
{
    auto title_column = ui->tvSchedule->model()->index(row, 1);
    auto record_identifier = title_column.data(Qt::UserRole).toMap();
    return record_identifier["type"].toString();
}

void MainWindow::remove_item(int row)
{
    auto model = ui->tvSchedule->model();
    model->removeRow(row, QModelIndex());
}

bool MainWindow::is_item_deletable(const QString item_type)
{
    return ((item_type=="COMM-BREAK")||(item_type=="END_MARKER")) ? false : true;
}


void MainWindow::remove_all()
{
    if (QMessageBox::warning(this, tr("Sedric"),
                          tr("Clear all entered items?"),
                          QMessageBox::Ok | QMessageBox::Cancel,
                          QMessageBox::Cancel) == QMessageBox::Ok)
    {
        remove_items_by_date_hour(ui->dtSchedule->date(), ui->cbHours->currentData().toInt());
    }
}

void MainWindow::remove_items_by_date_hour(QDate date, int hour)
{
    ui->tvSchedule->setSelectionMode(QAbstractItemView::MultiSelection);

    std::vector<QModelIndex> selected_rows;

    for (int row =0; row <= m_schedule_model->rowCount(); ++row){

        QString schedule_type = get_schedule_type(row);

        if (schedule_type.isEmpty())
            continue;

        if (!is_item_deletable(schedule_type))
            continue;

        int row_id = get_schedule_row_id(row);
        auto data = m_schedule_item_builder->find_data_by_rowid(row_id);

        auto index = ui->tvSchedule->model()->index(row,0);

        selected_rows.push_back(index);

        m_schedule_item_builder->delete_row(row_id);
        decrement_time(row, data->item_data()->duration);
    }

    auto top_left_index = selected_rows[0];
    auto bottom_right_index = selected_rows[selected_rows.size()-1];

    auto item_selection = QItemSelection(top_left_index, bottom_right_index);
    ui->tvSchedule->selectionModel()->select(item_selection, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

    auto selected_indexes = ui->tvSchedule->selectionModel()->selectedRows();
    int sel_rows = selected_indexes.count();
    for(int i = sel_rows; i > 0; i--)
        ui->tvSchedule->model()->removeRow(selected_indexes.at(i-1).row(), QModelIndex());

    ui->tvSchedule->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MainWindow::date_changed()
{
    populate_schedule(ui->dtSchedule->date(), ui->cbHours->currentData().toInt());
}

void MainWindow::hour_changed(int h)
{
    populate_schedule(ui->dtSchedule->date(), ui->cbHours->currentData().toInt());
}

void MainWindow::create_track_view_headers()
{
    m_tracks_model->setHorizontalHeaderItem(0, new QStandardItem("Track Title"));
    m_tracks_model->setHorizontalHeaderItem(1, new QStandardItem("Artist"));
    m_tracks_model->setHorizontalHeaderItem(2, new QStandardItem("Duration"));
    m_tracks_model->setHorizontalHeaderItem(3, new QStandardItem("Audio Type"));
    m_tracks_model->setHorizontalHeaderItem(4, new QStandardItem("Audio File"));
}

void MainWindow::set_track_view_column_width()
{
    enum Column{Title, Artist, Duration, AudioType, AudioFile};

    ui->tvTracks->setColumnWidth(Column::Title, 250);
    ui->tvTracks->setColumnWidth(Column::Artist,250);
    ui->tvTracks->setColumnWidth(Column::Duration, 100);
    ui->tvTracks->setColumnWidth(Column::AudioType, 150);
    ui->tvTracks->setColumnWidth(Column::AudioFile, 250);
}

void MainWindow::create_model_headers()
{

    m_schedule_model->setHorizontalHeaderItem(0, new QStandardItem("Time"));
    m_schedule_model->setHorizontalHeaderItem(1, new QStandardItem("Title"));
    m_schedule_model->setHorizontalHeaderItem(2, new QStandardItem("Artist"));
    m_schedule_model->setHorizontalHeaderItem(3, new QStandardItem("Duration"));
    m_schedule_model->setHorizontalHeaderItem(4, new QStandardItem("Transition"));
    m_schedule_model->setHorizontalHeaderItem(5, new QStandardItem("Play Date"));
    m_schedule_model->setHorizontalHeaderItem(6, new QStandardItem("Play Time"));
    m_schedule_model->setHorizontalHeaderItem(7, new QStandardItem("Track Path"));
    m_schedule_model->setHorizontalHeaderItem(8, new QStandardItem("Comment"));

}

void MainWindow::set_column_width()
{
    enum Column{TIME, TITLE, ARTIST, DURATION, TRANSITION, PLAY_DATE, PLAY_TIME, TRACK_PATH, COMMENT};

    ui->tvSchedule->setColumnWidth(Column::TIME, 50);
    ui->tvSchedule->setColumnWidth(Column::TITLE, 250);
    ui->tvSchedule->setColumnWidth(Column::ARTIST, 200);
    ui->tvSchedule->setColumnWidth(Column::DURATION, 80);
    ui->tvSchedule->setColumnWidth(Column::TRANSITION, 80);
    ui->tvSchedule->setColumnWidth(Column::PLAY_DATE, 80);
    ui->tvSchedule->setColumnWidth(Column::PLAY_TIME, 80);
    ui->tvSchedule->setColumnWidth(Column::TRACK_PATH, 250);
    ui->tvSchedule->setColumnWidth(Column::COMMENT, 120);
}

void MainWindow::populate_hours()
{
    for (int h=0; h < 24; ++h){
        ui->cbHours->addItem(QString::number(h), QVariant(h));
    }

    ui->cbHours->setCurrentIndex(0);
}

void MainWindow::audio_folder_setup()
{
    m_tree_model->setHorizontalHeaderItem(0, new QStandardItem("Audio Library"));
}

void MainWindow::populate_schedule(QDate date, int hour)
{
    qDebug() << "Before Cache Size: "<< m_schedule_item_builder->cache_size();

    int schedule_ref = date.day()+date.month()+date.year()+hour;

    auto cached_items = m_schedule_item_builder->cached_data(schedule_ref);

    if (cached_items.size() > 0){
        m_schedule_model->clear();
        create_model_headers();
        set_column_width();
        show_cached_items(cached_items);
        qDebug() << "After Cache Size: "<< m_schedule_item_builder->cache_size();
        return;
    }

    // Read from the database
    try{
        m_schedule_model->clear();
        create_model_headers();
        set_column_width();
        show_db_schedule(date, hour);
    }catch(DatabaseException& de){
        showMessage(de.errorMessage());
    }

}

void MainWindow::fetch_audio(int folder_id)
{
    EntityDataModel edm = EntityDataModel(std::make_unique<AUDIO::Audio>());
    auto ad = std::make_unique<AUDIO::Audio>();

    auto folder_filter = std::make_tuple(
                ad->folder()->dbColumnName(),
                " = ",
                folder_id);

    std::string filter = edm.prepareFilter(folder_filter);
    edm.search(filter);

    AudioTool audio_tool;

    for (auto&[name, entity] : edm.modelEntities()){
        AUDIO::Audio* audio = dynamic_cast<AUDIO::Audio*>(entity.get());

        TrackData* track_data = new TrackData();

        track_data->id = audio->id();
        track_data->title = QString::fromStdString(audio->title()->value());
        track_data->artist = QString::fromStdString(audio->artist()->displayName());
        track_data->duration = audio->duration()->value();
        track_data->audio_type = QString::fromStdString(audio->audio_type()->value());
        track_data->audio_file = QString::fromStdString(audio->file_path()->value()+
                                                        audio_tool.generate_ogg_filename(audio->id())+".ogg");
        ItemColumns item;

        if (audio->audio_type()->displayName() == "Song")
            item = m_track_item_builder->create_item<SongTrackItem>(track_data);

        if (audio->audio_type()->displayName() == "Commercial")
            item = m_track_item_builder->create_item<CommercialTrackItem>(track_data);

        if (audio->audio_type()->displayName() == "Jingle")
            item = m_track_item_builder->create_item<JingleTrackItem>(track_data);

        if (item.size() > 0)
            m_tracks_model->appendRow(item);

    }

}

void MainWindow::test_schedule(QDate date, int hour)
{

    ItemData* sd = new ItemData();

    sd->id = 12;
    sd->schedule_ref = date.day()+date.month()+date.year()+hour;
    sd->schedule_date = date;
    sd->schedule_time = QTime(22,0,0);
    sd->comment = "";
    sd->auto_transition = 1;
    sd->type="COMM-BREAK";


    auto comm_item1 = m_schedule_item_builder->create_item<CommercialBreakItem>(sd);


    m_schedule_model->appendRow(comm_item1);

    ItemData* sd2 = new ItemData();
    sd2->schedule_time = QTime(22,0,0);
    sd2->play_time = QTime(0,0,0);
    sd->auto_transition = 1 ;
    sd2->type="COMM-BREAK";

    auto comm_item2 = m_schedule_item_builder->create_item<CommercialBreakItem>(sd);
    m_schedule_model->appendRow(comm_item2);

    ItemData* sd3 = new ItemData();
    sd3->schedule_time = QTime(22,0,0);
    sd3->play_time = QTime(0,0,0);
    sd->auto_transition = 1;
    sd3->type="COMM-BREAK";

    auto comm_item3 = m_schedule_item_builder->create_item<CommercialBreakItem>(sd);
    m_schedule_model->appendRow(comm_item3);

    ItemData* end_data = new ItemData();
    end_data->schedule_time = QTime(22,0,0);
    end_data->type = "END_MARKER";

    auto end_marker = m_schedule_item_builder->create_item<EndMarkerItem>(end_data);
    m_schedule_model->appendRow(end_marker);
}

void MainWindow::show_cached_items(std::vector<ScheduleData*> items)
{
    for (auto schedule_item : items){

        ItemColumns item_columns;

        if (schedule_item->item_data()->type == "COMM-BREAK")
            item_columns = m_schedule_item_builder->create_item<CommercialBreakItem>(schedule_item->item_data());

        if (schedule_item->item_data()->type == "COMM-AUDIO")
            item_columns = m_schedule_item_builder->create_item<CommercialAudioItem>(schedule_item->item_data());

        if (schedule_item->item_data()->type == "SONG")
            item_columns = m_schedule_item_builder->create_item<SongItem>(schedule_item->item_data());

        if (schedule_item->item_data()->type == "JINGLE")
            item_columns = m_schedule_item_builder->create_item<JingleItem>(schedule_item->item_data());

        if (item_columns.size() > 0)
            m_schedule_model->appendRow(item_columns);

    }
        ItemData* end_marker_data = new ItemData();
        end_marker_data->type = "END_MARKER";
        ItemColumns t_columns = m_schedule_item_builder->create_item<EndMarkerItem>(end_marker_data);
        m_schedule_model->appendRow(t_columns);
}

void MainWindow::show_db_schedule(QDate date, int hour)
{
    std::stringstream sql;

    sql << " SELECT a.id, a.schedule_date, a.schedule_time, "
        << " a.auto_transition, a.play_date, a.play_time, "
        << " a.schedule_item_type, a.comment, a.booked_spots, "
        << " a.audio_id, b.title, b.filepath, b.duration, c.fullname "
        << " FROM rave_schedule a  "
        << " left outer join rave_audio b on a.audio_id = b.id "
        << " left outer join rave_artist c on b.artist_id = c.id ";

    std::string where_filter = " WHERE a.schedule_date = '"+date.toString("yyyy/MM/dd").toStdString()+"'";
    std::string and_filter = " AND a.schedule_hour = "+std::to_string(hour);
    std::string order_by = " ORDER BY a.schedule_time ";

    sql << where_filter << and_filter << order_by;

    EntityDataModel edm;

    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();

    if (provider->cacheSize() > 0){
        provider->cache()->first();
        do{
            auto it_begin = provider->cache()->currentElement()->begin();
            auto it_end = provider->cache()->currentElement()->end();

            ItemData* data  = new ItemData();

            for (; it_begin != it_end; ++it_begin){
                std::string field_name = (*it_begin).first;
                std::string field_value = (*it_begin).second;

                if (field_name == "id")
                    data->id = str_to_int(field_value);

                if (field_name == "schedule_time")
                    data->schedule_time = QTime::fromString(QString::fromStdString(field_value), "hh:mm:ss");

                if (field_name == "auto_transition")
                    data->auto_transition = str_to_int(field_value);

                if (field_name == "play_date")
                    data->play_date = QDate::fromString(QString::fromStdString(field_value), "yyyy/MM/dd");

                if (field_name == "play_time")
                    data->play_time =  QTime::fromString(QString::fromStdString(field_value), "hh:mm:ss");

                if (field_name == "comment")
                    data->comment = QString::fromStdString(field_value);

                if (field_name == "schedule_item_type")
                    data->type = QString::fromStdString(field_value);

                if (field_name == "booked_spots")
                    data->booked_spots = str_to_int(field_value);

                if (field_name == "audio_id")
                    data->audio_id = str_to_int(field_value);

                if (field_name == "duration")
                    data->duration = str_to_int(field_value);

                if (field_name == "title")
                    data->item_title = QString::fromStdString(field_value);

                if (field_name == "filepath")
                    data->filepath = QString::fromStdString(field_value);

                if (field_name == "fullname")
                    data->artist = QString::fromStdString(field_value);

            }

            data->schedule_ref = date.day()+date.month()+date.year()+hour;

            ItemColumns item;

            if (data->type == "COMM-BREAK"){
                item = m_schedule_item_builder->create_item<CommercialBreakItem>(data);
            }

            if (data->type == "COMM-AUDIO"){
                item = m_schedule_item_builder->create_item<CommercialAudioItem>(data);
            }

            if (data->type == "SONG"){

                item = m_schedule_item_builder->create_item<SongItem>(data);
            }

            m_schedule_model->appendRow(item);

            provider->cache()->next();

        } while (!provider->cache()->isLast());
    }

    if (m_schedule_item_builder->item_exist("END_MARKER", QTime(hour,0,0)))
        return;

    ItemData* end_marker_data = new ItemData();
    end_marker_data->schedule_time = QTime(hour, 0, 0);
    end_marker_data->type = "END_MARKER";
    ItemColumns t_columns = m_schedule_item_builder->create_item<EndMarkerItem>(end_marker_data);
    m_schedule_model->appendRow(t_columns);
}

void MainWindow::item_id()
{
    auto mod_index = ui->tvSchedule->currentIndex();
    auto first_col = ui->tvSchedule->model()->index(mod_index.row(), 0);
    qDebug() << "ID: "<< first_col.data(Qt::UserRole).toInt();
}


void MainWindow::add_item()
{
    auto mod_index = ui->tvSchedule->currentIndex();
    auto second_col = ui->tvSchedule->model()->index(mod_index.row(), 0);
    auto current_time = second_col.data(Qt::UserRole).toTime();

    ItemData* sd = new ItemData();
    int duration = 225000;
    sd->schedule_time = current_time;
    sd->play_time = QTime(0,0,0);
    sd->auto_transition = 1;

    auto song_item1 = m_schedule_item_builder->create_item<SongItem>(sd);
    m_schedule_model->insertRow(mod_index.row(), song_item1);

    increment_time(mod_index.row()+1, duration);
}

void MainWindow::increment_time(int from_row, int duration)
{
    auto model = ui->tvSchedule->model();
    for (int i=from_row; i < m_schedule_model->rowCount(); ++i){
        auto index = model->index(i, 0);
        auto time = index.data(Qt::UserRole).toTime();
        auto new_time  = time.addMSecs(duration);
        model->setData(index, new_time, Qt::UserRole);
        model->setData(index, new_time.toString("hh:mm:ss"), Qt::DisplayRole);
    }
}

void MainWindow::decrement_time(int from_row, int duration)
{
    auto model = ui->tvSchedule->model();
    for(int i=from_row; i < m_schedule_model->rowCount(); ++i){
        auto index = model->index(i, 0);
        auto time = index.data(Qt::UserRole).toTime();
        int time_in_duration = time.msec();
        auto new_time = time.addMSecs(time_in_duration-(duration*1000));
        model->setData(index, new_time, Qt::UserRole);
        model->setData(index, new_time.toString("hh:m:ss"), Qt::DisplayRole);
    }
}

std::string MainWindow::make_insert_stmts()
{
    Schedule schedule;
    std::string insert_stmts;
    VectorStruct<Field> fields;

    for (auto data : m_schedule_item_builder->all_cached()){

        schedule.set_schedule_date(data->schedule_date());
        schedule.set_schedule_time(data->schedule_time());
        schedule.set_schedule_hour(data->schedule_hour());
        schedule.set_auto_transition(data->auto_transition());
        schedule.audio()->setValue(data->audio_id());


        fields << schedule.schedule_date()
               << schedule.schedule_time()
               << schedule.schedule_hour()
               << schedule.auto_transition()
               << schedule.audio();

        insert_stmts += schedule.make_insert_stmt(fields.vec);
        fields.clear();
    }

    return insert_stmts;

}

void MainWindow::save_schedule()
{
    std::string sql = make_insert_stmts();

    if (sql.empty())
        return;

    auto hours = m_schedule_item_builder->unique_hours();

    qDebug() << "Unique Hours: "<< hours.size();

    for ( auto h : hours){
        qDebug() << "Hour: "<< h;
    }

    //delete_current_schedule();
    //write_schedule_to_db(sql);
}

bool MainWindow::write_schedule_to_db(std::string sql)
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
