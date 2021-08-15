#include <sstream>
#include <string>
#include <memory>

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
    , m_model{nullptr}
    , m_tree_model{nullptr}
{
    ui->setupUi(this);

    m_item_builder = new ItemBuilder();

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
    set_track_view_column_width();  // Width adjustment *MUST* happen after the "setModel" method

//    ScheduleDelegate delegate;
//    ui->tvSchedule->setItemDelegate(&delegate);

    m_model = new QStandardItemModel(this);
    create_model_headers();
    ui->tvSchedule->setModel(m_model);

    ui->tvSchedule->setDragDropOverwriteMode(false);
    ui->tvSchedule->setDragEnabled(true);
    ui->tvSchedule->setDragDropMode(QAbstractItemView::InternalMove);

    ui->tvSchedule->horizontalHeader()->setStretchLastSection(true);
    ui->tvSchedule->verticalHeader()->setVisible(false);

    set_column_width();
    populate_hours();

    connect(ui->btnNew, &QPushButton::clicked, this, &MainWindow::add_item);
    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::item_id);
    connect(ui->dtSchedule, &QDateEdit::dateChanged, this, &MainWindow::date_changed);
    connect(ui->cbHours, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::hour_changed);

    ui->dtSchedule->setDate(QDate::currentDate());

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

    m_model->setHorizontalHeaderItem(0, new QStandardItem("Time"));
    m_model->setHorizontalHeaderItem(1, new QStandardItem("Title"));
    m_model->setHorizontalHeaderItem(2, new QStandardItem("Artist"));
    m_model->setHorizontalHeaderItem(3, new QStandardItem("Duration"));
    m_model->setHorizontalHeaderItem(4, new QStandardItem("Transition"));
    m_model->setHorizontalHeaderItem(5, new QStandardItem("Play Date"));
    m_model->setHorizontalHeaderItem(6, new QStandardItem("Play Time"));
    m_model->setHorizontalHeaderItem(7, new QStandardItem("Track Path"));
    m_model->setHorizontalHeaderItem(8, new QStandardItem("Comment"));

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

    int schedule_ref = date.day()+date.month()+date.year()+hour;

    auto cached_items = m_item_builder->cached_data(schedule_ref);

    if (cached_items.size() > 0){
        m_model->clear();
        create_model_headers();
        set_column_width();
        show_cached_items(cached_items);
        return;
    }

    // Read from the database
    try{
        m_model->clear();
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

//    HorizontalHeaderItem(0, new QStandardItem("Track Title"));
//    m_tracks_model->setHorizontalHeaderItem(1, new QStandardItem("Artist"));
//    m_tracks_model->setHorizontalHeaderItem(2, new QStandardItem("Duration"));
//    m_tracks_model->setHorizontalHeaderItem(3, new QStandardItem("Audio Type"));
//    m_tracks_model->setHorizontalHeaderItem(4, new QStandardItem("Audio File"));

    QList<QStandardItem*> track_record;
    AudioTool audio_tool;

    for (auto&[name, entity] : edm.modelEntities()){
        AUDIO::Audio* audio = dynamic_cast<AUDIO::Audio*>(entity.get());

        TrackData* track_data = new TrackData();

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

        /*
        auto title = new QStandardItem(stoq(audio->title()->value()));
        title->setData(stoq(audio->title()->value()), Qt::UserRole);

        auto artist = new QStandardItem(stoq(audio->artist()->displayName()));

        QTime q_dur = QTime(QTime::fromMSecsSinceStartOfDay(audio->duration()->value()*1000));
        auto duration = new QStandardItem(

        track_record.append(title);
        track_record.append(artist);

        m_tracks_model->appendRow(track_record);

        track_record.clear();
        */
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
    sd->type="COMM";


    auto comm_item1 = m_item_builder->create_item<CommercialBreakItem>(sd);


    m_model->appendRow(comm_item1);

    ItemData* sd2 = new ItemData();
    sd2->schedule_time = QTime(22,0,0);
    sd2->play_time = QTime(0,0,0);
    sd->auto_transition = 1 ;
    sd2->type="COMM";

    auto comm_item2 = m_item_builder->create_item<CommercialBreakItem>(sd);
    m_model->appendRow(comm_item2);

    ItemData* sd3 = new ItemData();
    sd3->schedule_time = QTime(22,0,0);
    sd3->play_time = QTime(0,0,0);
    sd->auto_transition = 1;
    sd3->type="COMM";

    auto comm_item3 = m_item_builder->create_item<CommercialBreakItem>(sd);
    m_model->appendRow(comm_item3);

    ItemData* end_data = new ItemData();
    end_data->schedule_time = QTime(22,0,0);
    end_data->type = "END_MARKER";

    auto end_marker = m_item_builder->create_item<EndMarkerItem>(end_data);
    m_model->appendRow(end_marker);
}

void MainWindow::show_cached_items(std::vector<ScheduleData*> items)
{
    for (auto schedule_item : items){

        ItemColumns table_columns;

        if (schedule_item->item_data()->type == "COMM")
            table_columns = m_item_builder->create_item<CommercialBreakItem>(schedule_item->item_data());

        if (schedule_item->item_data()->type == "SONG")
            table_columns = m_item_builder->create_item<SongItem>(schedule_item->item_data());

        if (table_columns.size() > 0)
            m_model->appendRow(table_columns);

    }
        ItemData* end_marker_data = new ItemData();
        end_marker_data->type = "END_MARKER";
        ItemColumns t_columns = m_item_builder->create_item<EndMarkerItem>(end_marker_data);
        m_model->appendRow(t_columns);
}

void MainWindow::show_db_schedule(QDate date, int hour)
{
    std::stringstream sql;

    sql << " SELECT rave_schedule.id, rave_schedule.schedule_date, rave_schedule.schedule_time, "
        << " rave_schedule.auto_transition, rave_schedule.play_date, rave_schedule.play_time, "
        << " rave_schedule.schedule_item_type, rave_schedule.comment "
        << " FROM rave_schedule ";

    std::string where_filter = " WHERE rave_schedule.schedule_date = '"+date.toString("yyyy/MM/dd").toStdString()+"'";
    std::string and_filter = " AND rave_schedule.schedule_hour = "+std::to_string(hour);
    std::string order_by = " ORDER BY rave_schedule.schedule_time ";

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

            }

            data->schedule_ref = date.day()+date.month()+date.year()+hour;

            ItemColumns item;

            if (data->type == "COMM"){
                item = m_item_builder->create_item<CommercialBreakItem>(data);
            }

            m_model->appendRow(item);

            provider->cache()->next();

        } while (!provider->cache()->isLast());
    }
        ItemData* end_marker_data = new ItemData();
        end_marker_data->type = "END_MARKER";
        ItemColumns t_columns = m_item_builder->create_item<EndMarkerItem>(end_marker_data);
        m_model->appendRow(t_columns);

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

    auto song_item1 = m_item_builder->create_item<SongItem>(sd);
    m_model->insertRow(mod_index.row(), song_item1);

    update_time(mod_index.row()+1, duration);
}

void MainWindow::update_time(int from_row, int duration)
{
    auto model = ui->tvSchedule->model();
    for (int i=from_row; i < m_model->rowCount(); ++i){
        auto index = model->index(i, 0);
        auto time = index.data(Qt::UserRole).toTime();
        auto new_time  = time.addMSecs(duration);
        model->setData(index, new_time, Qt::UserRole);
        model->setData(index, new_time.toString("hh:mm:ss"), Qt::DisplayRole);
    }
}
