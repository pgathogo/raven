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
#include "sedricscheduleitem.h"
#include "../utils/tools.h"

#include "../framework/entitydatamodel.h"
#include "../framework/ravenexception.h"
#include "../framework/databasemanager.h"
#include "../framework/tree.h"
#include "../framework/treeviewmodel.h"
#include "../framework/tableviewmodel.h"
#include "../audio/audiolibrary.h"
#include "../audio/audio.h"
#include "../audio/artist.h"
#include "../audio/audiotrackitem.h"
#include "../audio/audiotool.h"
#include "../audio/audiolibitem.h"
#include "../framework/entitydatamodel.h"
#include "../framework/schedule.h"
#include "../framework/choicefield.h"
#include "../utils/tools.h"

ScheduleHour::ScheduleHour()
{
    setMinimumWidth(150);
}

void ScheduleHour::hidePopup()
{
    QComboBox::hidePopup();
    emit hourComboBoxClosed();
}

void ScheduleHour::setText(QString text)
{
    QLineEdit* displayText = this->lineEdit();
    displayText->setText(text);
    displayText->setReadOnly(true);
}

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
    adjust_header_size();

    ui->tvTracks->horizontalHeader()->setStretchLastSection(true);
    ui->tvTracks->setModel(m_tracks_model);
    ui->tvTracks->verticalHeader()->setVisible(false);

    // For Width adjustment to be effected, it *MUST* be done after
    // the "setModel" method
    set_track_view_column_width();

    m_audio_lib_item = std::make_unique<AUDIO::AudioLibItem>(m_tracks_model);
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

    setup_hour_combobox();

    set_column_width();

    m_schedule_item = std::make_unique<SEDRIC::SedricScheduleItem>(m_schedule_model);

    connect(ui->btnNew, &QPushButton::clicked, this, &MainWindow::remove_all);
    connect(ui->btnRemove, &QPushButton::clicked, this, &MainWindow::remove_current_schedule_item);
    connect(ui->tvSchedule, &QTableView::clicked, this, &MainWindow::print_details);
    connect(ui->tvSchedule, &QTableView::doubleClicked, this, &MainWindow::insert_item_new);

    connect(ui->dtSchedule, &QDateEdit::dateChanged, this, &MainWindow::date_changed);

//    connect(ui->cbHours, QOverload<int>::of(&QComboBox::highlighted), this, &MainWindow::combo_highlight);
//    connect(ui->cbHours, QOverload<int>::of(&QComboBox::currentIndexChanged),
//            this, &MainWindow::hour_changed);

    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::save_schedule);

    ui->dtSchedule->setDate(QDate::currentDate());

    setWindowTitle("Raven - SeDRic");

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setup_hour_combobox()
{
    m_cb_model = new QStandardItemModel(24, 3);
    m_cb_hour = new ScheduleHour();
    m_cb_hour->setEditable(true);
    m_cb_hour->setText("");
    populate_hour_combobox();
    m_cb_hour->setModel(m_cb_model);

    connect(m_cb_model, &QStandardItemModel::dataChanged, this, &MainWindow::cb_data_changed);
    connect(m_cb_hour, &ScheduleHour::hourComboBoxClosed, this, &MainWindow::hour_cb_closed);

    ui->hlHour->addWidget(m_cb_hour);
}

void MainWindow::folder_clicked(const QModelIndex& index)
{
    int folder_id = index.data(Qt::UserRole).toInt();
    fetch_audio_new(folder_id);
}


std::tuple<int, int, QTime> MainWindow::get_sched_item_hour_time(const QModelIndex& index)
{
    auto time_col = ui->tvSchedule->model()->index(index.row(), 0);
    auto item_data = time_col.data(Qt::UserRole).toMap();

    auto row_id = item_data["row_id"].toInt();
    auto sched_hour = item_data["hour"].toInt();
    auto time_stamp = item_data["time"].toTime();

    auto results = std::make_tuple(row_id, sched_hour, time_stamp);
    return results;
}

void MainWindow::print_details(const QModelIndex& index)
{
    /*
    auto [row_id, sched_hour, time_stamp] = get_sched_item_hour_time(index);
    printstr("***");
    printstr(std::to_string(row_id));
    printstr(std::to_string(sched_hour));
    qDebug() << time_stamp.toString("hh:mm:ss");
    printstr("***");
    */
}

void MainWindow::insert_item_new(const QModelIndex& index)
{
    auto mod_index = ui->tvTracks->currentIndex();
    auto first_col = ui->tvTracks->model()->index(mod_index.row(), 0);
    auto audio_id = first_col.data(Qt::UserRole).toInt();

    if (audio_id == 0)
        return;

    AUDIO::Audio* audio = m_audio_lib_item->find_audio_by_id(audio_id);
    if (audio == nullptr)
        return;

    Schedule* schedule = new Schedule();

    auto schedule_audio = dynamic_cast<AUDIO::Audio*>(schedule->audio()->dataModel()->get_entity().get());
    auto schedule_artist = dynamic_cast<AUDIO::Artist*>(schedule_audio->artist()->dataModel()->get_entity().get());

    auto [row_id, sched_hour, time_stamp] = get_sched_item_hour_time(index);

    schedule->setId(-1);
    schedule->set_schedule_date(ui->dtSchedule->date());
    schedule->set_schedule_time(time_stamp);
    schedule->set_schedule_hour(sched_hour);

    schedule->set_auto_transition(SEDRIC::SedricScheduleItem::AudioTransition::Mix);
    schedule->set_schedule_item_type(audio->audio_type()->value());

    schedule->set_break_duration(audio->duration()->value());

    schedule_audio->setId(audio->id());
    schedule_audio->set_duration(audio->duration()->value());
    schedule_audio->set_title(audio->title()->value());
    schedule_audio->set_file_path(audio->file_path()->value());

    schedule->audio()->setValue(audio->id());

    schedule_artist->set_fullname(audio->artist()->displayName());

    if (audio->audio_type()->value() == "SONG")
        m_schedule_item->create_row_item<SEDRIC::SongItem>(schedule, index.row());

    if (audio->audio_type()->value() == "JINGLE")
        m_schedule_item->create_row_item<SEDRIC::JingleItem>(schedule, index.row());

    if (audio->audio_type()->value() == "COMM-AUDIO")
        m_schedule_item->create_row_item<SEDRIC::CommercialAudioItem>(schedule, index.row());

    m_schedule_item->update_time(sched_hour, index.row(), audio->duration()->value());

    add_activity(sched_hour);
}

void MainWindow::add_activity(int hour)
{
    auto it = m_activities.find(hour);
    if (it == m_activities.end()){
        m_activities[hour] = 1;
    }else{
        m_activities[hour]++;
    }
}

void MainWindow::remove_activity(int hour)
{
    auto it = m_activities.find(hour);
    if (it != m_activities.end()){
        m_activities[hour]--;
    }
}

void MainWindow::remove_current_schedule_item()
{
    auto index = ui->tvSchedule->currentIndex();
    if (index.row() < 0)
        return;

    auto [row_id, sched_hour, time_stamp] = get_sched_item_hour_time(index);

    auto schedule = m_schedule_item->find_schedule_item(row_id);
    if (schedule == nullptr)
        return;

    if (!is_item_deletable(stoq(schedule->schedule_item_type()->value())))
        return;

    m_schedule_item_builder->delete_row(row_id);
    remove_item(index.row());

    //update_schedule_time(sched_hour, index.row(), schedule->break_duration()->value()*-1);
    m_schedule_item->update_time(sched_hour, index.row(), schedule->break_duration()->value()*-1);

    remove_activity(sched_hour);
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
    if (item_type=="COMM-BREAK")
        return false;
    if (item_type=="END_MARKER")
        return false;

    return true;
}


void MainWindow::remove_all()
{
    if (QMessageBox::warning(this, tr("Sedric"),
                          tr("Clear all entered items?"),
                          QMessageBox::Ok | QMessageBox::Cancel,
                          QMessageBox::Cancel) == QMessageBox::Ok)
    {
        //remove_items_by_date_hour(ui->dtSchedule->date(), ui->cbHours->currentData().toInt());
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
        //update_schedule_time(hour, row, data->item_data()->duration*-1);
        m_schedule_item->update_time(hour, row, data->item_data()->duration*-1);
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
    populate_schedule(ui->dtSchedule->date(), get_hours());
}

void MainWindow::hour_changed(int h)
{
    //populate_schedule(ui->dtSchedule->date(), m_cb_hour->currentText().toStdString());
}

void MainWindow::combo_highlight(int)
{
}

void MainWindow::create_track_view_headers()
{
    m_tracks_model->setHorizontalHeaderItem(0, new QStandardItem("Track Title"));
    m_tracks_model->setHorizontalHeaderItem(1, new QStandardItem("Artist"));
    m_tracks_model->setHorizontalHeaderItem(2, new QStandardItem("Duration"));
    m_tracks_model->setHorizontalHeaderItem(3, new QStandardItem("Audio Type"));
    m_tracks_model->setHorizontalHeaderItem(4, new QStandardItem("Audio File"));
}

void MainWindow::adjust_header_size()
{
    QHeaderView* vert_header = ui->tvSchedule->verticalHeader();
    vert_header->setSectionResizeMode(QHeaderView::Fixed);
    vert_header->setDefaultSectionSize(20);
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

void MainWindow::populate_hour_combobox()
{
    for(int row=0; row < m_cb_model->rowCount(); ++row){
        QStandardItem* item = new QStandardItem(QString::number(row));
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);
        m_cb_model->setItem(row, 0, item);
        m_hours.push_back(item);
    }


}
void MainWindow::cb_data_changed(const QModelIndex& top_left, const QModelIndex bottom_right)
{
    /*
    if (m_hours.size() == 0)
        return;
    auto item = m_hours[top_left.row()];
    if (item->checkState() == Qt::Unchecked){
        qDebug() << "Un-checked";
    }else{
        qDebug() << "Checked";
    }
    */
}

void MainWindow::hour_cb_closed()
{
    QString sel_hrs{""};
    for (std::size_t i=0; i < m_hours.size(); ++i){
        auto item = m_hours[i];
        if (item->checkState() == Qt::Checked){
            if (!sel_hrs.isEmpty())
                sel_hrs += ", ";
            sel_hrs += QString::number(i);
        }
    }

    m_cb_hour->setText(sel_hrs);

    populate_schedule(ui->dtSchedule->date(), get_hours());
}

std::vector<int> MainWindow::get_hours()
{
    auto selected_hours = split_string(m_cb_hour->currentText().toStdString(), ',');
    std::vector<int> hours = vector_str_to_int(selected_hours);
    return hours;
}

std::vector<int> MainWindow::vector_str_to_int(std::vector<std::string>& vs)
{
    std::vector<int> ints;
    for (std::string item : vs){
        ints.push_back(std::stoi(item));
    }
    return ints;
}

void MainWindow::audio_folder_setup()
{
    m_tree_model->setHorizontalHeaderItem(0, new QStandardItem("Audio Library"));
}

void MainWindow::populate_schedule(QDate date, std::vector<int> hours)
{

    /*
    int schedule_ref = date.day()+date.month()+date.year()+hour;
    auto cached_items = m_schedule_item_builder->cached_data(schedule_ref);

    if (cached_items.size() > 0){
        m_schedule_model->clear();
        create_model_headers();
        set_column_width();
        show_cached_items(cached_items);
        return;
    }
    */

    // Read schedule from the database
    try{
        m_schedule_model->clear();
        create_model_headers();
        set_column_width();
        show_db_schedule_new(date, hours);
    }catch(DatabaseException& de){
        showMessage(de.errorMessage());
    }

}

void MainWindow::fetch_audio_new(int folder_id)
{
    m_edm = std::make_unique<EntityDataModel>(std::make_unique<AUDIO::Audio>());
    auto ad = std::make_unique<AUDIO::Audio>();

    auto folder_filter = std::make_tuple(
                ad->folder()->dbColumnName(),
                " = ",
                folder_id);

    std::string filter = m_edm->prepareFilter(folder_filter);
    m_edm->search(filter);

    for (auto&[name, entity] : m_edm->modelEntities()){
        AUDIO::Audio* audio = dynamic_cast<AUDIO::Audio*>(entity.get());

        if (audio->audio_type()->displayName() == "Song")
            m_audio_lib_item->create_row_item<AUDIO::SongAudioLibItem>(audio);

        if (audio->audio_type()->displayName() == "Jingle")
            m_audio_lib_item->create_row_item<AUDIO::JingleAudioLibItem>(audio);

        if (audio->audio_type()->displayName() == "Commercial")
            m_audio_lib_item->create_row_item<AUDIO::CommercialAudioLibItem>(audio);

    }

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

void MainWindow::show_db_schedule_new(QDate date, std::vector<int>& hours)
{
    if (hours.size() == 0)
        return;

    QString hours_str{""};
    for (int h : hours){
        if (!hours_str.isEmpty())
            hours_str += ", ";
        hours_str += QString::number(h);
    }

    int tmp_hour = hours[0];
    int last_hour = hours.back();

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

    EntityDataModel edm;
    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();

    if (provider->cacheSize() == 0){

        for (auto hour : hours){
            create_end_marker(hour);
        }
        return;
    }

    provider->cache()->first();
    do{

        Schedule* schedule = new Schedule();

        auto audio = dynamic_cast<AUDIO::Audio*>(schedule->audio()->dataModel()->get_entity().get());
        auto artist = dynamic_cast<AUDIO::Artist*>(audio->artist()->dataModel()->get_entity().get());

        // populate schedule fields
        set_schedule_fields(provider, schedule, audio, artist);

        if (schedule->schedule_hour()->value() != tmp_hour){
            Schedule* schedule_end_marker = new Schedule();
            schedule_end_marker->set_schedule_time(QTime(tmp_hour, 0, 0));
            schedule_end_marker->set_schedule_item_type("END_MARKER");
            m_schedule_item->create_row_item<SEDRIC::EndMarkerItem>(schedule_end_marker);

            tmp_hour = schedule->schedule_hour()->value();
        }

        if (schedule->schedule_item_type()->currText() == "SONG"){
            m_schedule_item->create_row_item<SEDRIC::SongItem>(schedule);
        }

        if (schedule->schedule_item_type()->currText() == "COMM-BREAK"){
            m_schedule_item->create_row_item<SEDRIC::CommercialBreakItem>(schedule);
        }

        if (schedule->schedule_item_type()->currText() == "COMM-AUDIO"){
            m_schedule_item->create_row_item<SEDRIC::CommercialAudioItem>(schedule);
        }

        if (schedule->schedule_item_type()->currText() == "JINGLE"){
            m_schedule_item->create_row_item<SEDRIC::JingleItem>(schedule);
        }

        provider->cache()->next();

    } while (!provider->cache()->isLast());


    create_end_marker(last_hour);

}

void MainWindow::create_end_marker(int hour)
{
    Schedule* schedule = new Schedule();
    schedule->set_schedule_hour(hour);
    schedule->set_schedule_time(QTime(hour, 0, 0));
    schedule->set_schedule_item_type("END_MARKER");
    m_schedule_item->create_row_item<SEDRIC::EndMarkerItem>(schedule);
}

void MainWindow::set_schedule_fields(BaseDataProvider* provider,
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

        if (field_name == "schedule_hour")
            schedule->set_schedule_hour(str_to_int(field_value));

        if (field_name == "schedule_time")
            schedule->set_schedule_time(QTime::fromString(QString::fromStdString(field_value), "hh:mm:ss"));

        if (field_name == "auto_transition")
            schedule->set_auto_transition(str_to_int(field_value));

        if (field_name == "play_date")
            schedule->set_play_date(QDate::fromString(QString::fromStdString(field_value), "yyyy/MM/dd"));

        if (field_name == "play_time")
            schedule->set_play_time(QTime::fromString(QString::fromStdString(field_value), "hh:mm:ss"));

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

void MainWindow::update_schedule_time(int hour, int from_row, int duration)
{
    auto model = ui->tvSchedule->model();
    for(int i=from_row; i < m_schedule_model->rowCount(); ++i){
        auto index = model->index(i, 0);

        auto data = index.data(Qt::UserRole).toMap();
        int row_id = data["row_id"].toInt();
        int sched_hour = data["hour"].toInt();
        QTime time_stamp = data["time"].toTime();

        if (sched_hour > hour)
            break;

        auto new_time = time_stamp.addSecs(duration);

        QMap<QString, QVariant> item_data;
        item_data["row_id"] = row_id;
        item_data["hour"] = sched_hour;
        item_data["time"] = new_time;

        model->setData(index, item_data, Qt::UserRole);
        model->setData(index, new_time.toString("hh:mm:ss"), Qt::DisplayRole);

        m_schedule_item->update_schedule_item_time(row_id, new_time);
    }
}


std::string MainWindow::make_insert_stmts()
{
//    Schedule schedule;
    std::string insert_stmts;
    VectorStruct<Field> fields;

    for (auto const& schedule_item : m_schedule_item->schedule_items()){

        auto schedule = schedule_item->schedule();

        if ( (schedule->schedule_item_type()->value() == "END_MARKER") ||
             (schedule->schedule_item_type()->value() == "COMM-BREAK") )
            continue;

        fields << schedule->schedule_date()
               << schedule->schedule_time()
               << schedule->schedule_hour()
               << schedule->auto_transition()
               << schedule->audio()
               << schedule->schedule_item_type();

        insert_stmts += schedule->make_insert_stmt(fields.vec);
        fields.clear();
    }

    return insert_stmts;

}

void MainWindow::print_activity_details()
{
    for (auto[hour, count] : m_activities){
        qDebug() << "Hour: "<< hour << " = " << count;
    }
}

void MainWindow::save_schedule()
{
    std::string delete_stmts = make_delete_stmts();

    std::string insert_stmts = make_insert_stmts();
    if (insert_stmts.empty())
        return;

    delete_current_schedule(delete_stmts);
    write_schedule_to_db(insert_stmts);
}

std::string MainWindow::make_delete_stmts()
{
    std::stringstream sql;

    std::string date_str = ui->dtSchedule->date().toString("yyyy/MM/dd").toStdString();

    for ( auto [hour, count] : m_activities){

        sql << "Delete from rave_schedule ";

        std::string where_filter = " Where schedule_date = '"+date_str+"'";
        std::string and_a_filter = " and schedule_hour = "+std::to_string(hour);
        std::string  and_b_filter = " and schedule_item_type <> 'COMM-BREAK'; ";

        sql << where_filter << and_a_filter << and_b_filter;
    }

    return sql.str();

}


void MainWindow::delete_current_schedule(std::string sql)
{
    EntityDataModel edm;

    try{
        edm.executeRawSQL(sql);
    } catch(DatabaseException& de){
        showMessage(de.errorMessage());
    }


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

std::vector<std::string> MainWindow::split_string(std::string source, char delim)
{
    std::vector<std::string> results;
    std::stringstream ss (source);
    std::string item;

    while(getline(ss, item, delim)){
        results.push_back(item);
    }

    return results;

}
