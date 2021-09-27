#include <sstream>
#include <string>
#include <memory>
#include <algorithm>

#include <QStandardItemModel>
#include <QStandardItem>
#include <QHeaderView>
#include <QDebug>
#include <QVariant>
#include <QFile>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "scheduleitem.h"
#include "sedricscheduleitem.h"
#include "saveas.h"
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

#include "../audiolib/headers/cueeditor.h"
#include "../audiolib/headers/audioplayer.h"

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
    , m_cue_editor{nullptr}
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

    m_scheduler = std::make_unique<SEDRIC::SedricScheduleItem>(m_schedule_model);

    connect(ui->btnNew, &QPushButton::clicked, this, &MainWindow::new_schedule);
    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::save_schedule);
    connect(ui->btnSaveAs, &QPushButton::clicked, this, &MainWindow::copy_schedule);
    connect(ui->btnRemove, &QPushButton::clicked, this, &MainWindow::remove_current_schedule_item);
    connect(ui->btnPrint, &QPushButton::clicked, this, &MainWindow::print_schedule);
    connect(ui->tvSchedule, &QTableView::clicked, this, &MainWindow::print_details);
    connect(ui->tvSchedule, &QTableView::doubleClicked, this, &MainWindow::insert_item);
    connect(ui->dtSchedule, &QDateEdit::dateChanged, this, &MainWindow::date_changed);

    connect(ui->btnPlay, &QPushButton::clicked, this, &MainWindow::play_audio);
    connect(ui->btnStop, &QPushButton::clicked, this, &MainWindow::stop_play);

//    connect(ui->cbHours, QOverload<int>::of(&QComboBox::highlighted), this, &MainWindow::combo_highlight);
//    connect(ui->cbHours, QOverload<int>::of(&QComboBox::currentIndexChanged),
//            this, &MainWindow::hour_changed);


    ui->dtSchedule->setDate(QDate::currentDate());

    setWindowTitle("Raven - SeDRic");
}


MainWindow::~MainWindow()
{
    delete m_save_as;
    delete ui;
}

void MainWindow::copy_schedule()
{
    m_save_as = new SaveAs(get_selected_hours_asInt(), this);
    if (m_save_as->exec() == 1){
        Result result = m_save_as->save_result();
        m_scheduler->copy_schedule(ui->dtSchedule->date(), result.dest_date, result.dest_map);
    }
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
}

void MainWindow::print_schedule()
{
}

AUDIO::Audio* MainWindow::get_audio()
{
    auto mod_index = ui->tvTracks->currentIndex();
    auto first_col = ui->tvTracks->model()->index(mod_index.row(), 0);
    auto audio_id = first_col.data(Qt::UserRole).toInt();

    if (audio_id == 0)
        return nullptr;

    AUDIO::Audio* audio = m_audio_lib_item->find_audio_by_id(audio_id);

    return audio;
}

void MainWindow::insert_item(const QModelIndex& index)
{
    auto audio = get_audio();
    if (audio == nullptr)
        return;

    Schedule* schedule = new Schedule();

    auto [row_id, sched_hour, time_stamp] = get_sched_item_hour_time(index);

    schedule->setId(-1);
    schedule->set_schedule_date(ui->dtSchedule->date());
    schedule->set_schedule_time(time_stamp);
    schedule->set_schedule_hour(sched_hour);

    schedule->set_auto_transition(SEDRIC::SedricScheduleItem::AudioTransition::Mix);
    schedule->set_schedule_item_type(audio->audio_type()->value());

    schedule->set_break_duration(audio->duration()->value());
    schedule->audio()->setValue(audio->id());

    if (audio->audio_type()->value() == "SONG")
        m_scheduler->create_row_item<SEDRIC::SongItem>(schedule, index.row());

    if (audio->audio_type()->value() == "JINGLE")
        m_scheduler->create_row_item<SEDRIC::JingleItem>(schedule, index.row());

    if (audio->audio_type()->value() == "COMM-AUDIO")
        m_scheduler->create_row_item<SEDRIC::CommercialAudioItem>(schedule, index.row());

    m_scheduler->update_time(sched_hour, index.row(), audio->duration()->value());

    m_scheduler->log_activity(ui->dtSchedule->date(), sched_hour);
}

void MainWindow::remove_current_schedule_item()
{
    auto index = ui->tvSchedule->currentIndex();
    if (index.row() < 0)
        return;

    auto [row_id, sched_hour, time_stamp] = get_sched_item_hour_time(index);

    auto schedule = m_scheduler->find_schedule_item(row_id);
    if (schedule == nullptr)
        return;

    if (!is_item_deletable(stoq(schedule->schedule_item_type()->value())))
        return;

    m_scheduler->delete_row(row_id);

    remove_item(index.row());

    m_scheduler->update_time(sched_hour, index.row(), schedule->break_duration()->value()*-1);

    m_scheduler->log_activity(ui->dtSchedule->date(), sched_hour);
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


void MainWindow::new_schedule()
{
    if (QMessageBox::warning(this, tr("Sedric"),
                          tr("This will clear all entered items for the selected date and time. Continue?"),
                          QMessageBox::Ok | QMessageBox::Cancel,
                          QMessageBox::Cancel) == QMessageBox::Ok)
    {
       clear_schedule_model();
       m_scheduler->new_schedule(ui->dtSchedule->date(), get_selected_hours_asInt());
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
        m_scheduler->update_time(hour, row, data->item_data()->duration*-1);
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

void MainWindow::clear_schedule_model()
{
    m_scheduler->clear_display_items();
    m_schedule_model->clear();
    create_model_headers();
    set_column_width();
}

void MainWindow::date_changed()
{
    fetch_data();
}

void MainWindow::hour_changed(int h)
{
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
        if (row == 0)
            item->setCheckState(Qt::Checked);
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

QString MainWindow::get_selected_hours_asString()
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

    return sel_hrs;
}

void MainWindow::hour_cb_closed()
{
    fetch_data();
}

void MainWindow::fetch_data()
{
    m_cb_hour->setText(get_selected_hours_asString());

    QDate current_date = ui->dtSchedule->date();
    auto selected_hours = get_selected_hours_asInt();

    clear_schedule_model();
    auto processed = fetch_schedule_from_cache(current_date, selected_hours);

    std::vector<int> uncached_hours;
    std::transform(processed.begin(), processed.end(), back_inserter(uncached_hours), UnCachedHours());

    uncached_hours.erase(
                remove_if( uncached_hours.begin(), uncached_hours.end(),
                        [](int i){ return i == -1; }), uncached_hours.end());

    fetch_schedule_from_db(current_date, uncached_hours);
    m_scheduler->show_items(current_date, selected_hours);
}

std::vector<int> MainWindow::get_selected_hours_asInt()
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

std::map<int, int> MainWindow::fetch_schedule_from_cache(QDate date, std::vector<int> hours)
{
    auto processed = m_scheduler->fetch_cached_items(date, hours);
    return processed;
}

void MainWindow::fetch_schedule_from_db(QDate date, std::vector<int> hours)
{
    try{
        m_scheduler->fetch_db_items(date, hours);
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

void MainWindow::print_activity_details()
{
}

void MainWindow::save_schedule()
{
    std::string delete_stmts = m_scheduler->make_delete_stmts();

    std::string insert_stmts = m_scheduler->make_insert_stmts(m_scheduler->cached_items());
    if (insert_stmts.empty())
        return;

    m_scheduler->delete_current_schedule(delete_stmts);
    m_scheduler->write_schedule_to_db(insert_stmts);
}

// split string for single character delimeter
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

void MainWindow::play_audio()
{
    QItemSelectionModel* select = ui->tvTracks->selectionModel();
    if (select->selectedRows().size() == 0)
        return;

    AUDIO::Audio* audio = get_audio();
    if (audio == nullptr)
        return;

    AudioTool audio_tool;
    auto ogg_file = audio_tool.generate_ogg_filename(audio->id())+".ogg";
    auto full_audio_name = audio->file_path()->value()+ogg_file;

    if (!QFile::exists(QString::fromStdString(full_audio_name))){
        showMessage("File does not exits! "+full_audio_name);
        return;
    }

    AudioFile af(full_audio_name);
    m_cue_editor = std::make_unique<CueEditor>(af, full_audio_name);
    m_cue_editor->play_audio();

}

void MainWindow::stop_play()
{
    if (m_cue_editor != nullptr)
        m_cue_editor->stop_audio();
}
