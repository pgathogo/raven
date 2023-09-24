#include <sstream>
#include <string>
#include <memory>
#include <algorithm>
#include <ranges>

#include <QStandardItemModel>
#include <QStandardItem>
#include <QHeaderView>
#include <QDebug>
#include <QVariant>
#include <QFile>
#include <QCloseEvent>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "scheduleitem.h"
#include "sedricscheduleitem.h"
#include "saveas.h"
#include "../utils/tools.h"

#include "../framework/entitydatamodel.h"
#include "../framework/ravenexception.h"
#include "../framework/databasemanager.h"
#include "../framework/treeviewmodel.h"
#include "../framework/relationmapper.h"
#include "../framework/baseentity.h"
#include "../framework/entitydatamodel.h"
#include "../framework/schedule.h"
#include "../framework/choicefield.h"

#include "../audio/audiolibrary.h"
#include "../audio/audio.h"
#include "../audio/artist.h"
#include "../audio/audiotrackitem.h"
#include "../audio/audiotool.h"
#include "../audio/audiolibitem.h"
#include "../audio/audiohistoryform.h"

#include "../utils/tools.h"

#include "../audio/editor/audiowaveform.h"
#include "../audio/editor/audioplayer.h"

#include "../traffik/spotaudio.h"
#include "../traffik/spot.h"
#include "../traffik/client.h"

#include "bookedcommercialdlg.h"

namespace rv = std::ranges::views;

MainWindow::MainWindow(QApplication* app, const StationInfo& si, const ConnInfo& ci, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_qapp{app}
    , m_schedule_model{nullptr}
    , m_tree_model{nullptr}
    , m_save_as{nullptr}
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

    // For Width adjustment to take effect, it *MUST* be done after
    // the "setModel" method
    set_column_width();

    m_scheduler = std::make_unique<SEDRIC::SedricScheduleItem>(m_schedule_model);

    connect(ui->btnNew, &QPushButton::clicked, this, &MainWindow::new_schedule);
    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::save_schedule);
    connect(ui->btnSaveAs, &QPushButton::clicked, this, &MainWindow::save_as);
    connect(ui->btnRemove, &QPushButton::clicked, this, &MainWindow::remove_current_schedule_item);
    connect(ui->btnPrint, &QPushButton::clicked, this, &MainWindow::print_schedule);
    connect(ui->tvSchedule, &QTableView::clicked, this, &MainWindow::print_details);
    connect(ui->tvSchedule, &QTableView::doubleClicked, this, &MainWindow::insert_item);
    connect(ui->tvTracks, &QTableView::clicked, this, &MainWindow::track_selected);
//    connect(ui->dtSchedule, &QDateEdit::dateChanged, this, &MainWindow::date_changed);

    connect(ui->btnPlay, &QPushButton::clicked, this, &MainWindow::play_audio);
    connect(ui->btnStop, &QPushButton::clicked, this, &MainWindow::stop_play);
    connect(ui->btnHistory, &QPushButton::clicked, this, &MainWindow::show_audio_history);

    connect(ui->btnSearch, &QPushButton::clicked, this, &MainWindow::search_audio);
    connect(ui->edtTitle, &QLineEdit::returnPressed, this, &MainWindow::search_audio);
    connect(ui->edtArtist, &QLineEdit::returnPressed, this, &MainWindow::search_audio);

    connect(ui->btnDateTimeSelector, &QPushButton::clicked, this, &MainWindow::select_date_time);

    set_toolbutton_icons();

//    connect(ui->cbHours, QOverload<int>::of(&QComboBox::highlighted), this, &MainWindow::combo_highlight);
//    connect(ui->cbHours, QOverload<int>::of(&QComboBox::currentIndexChanged),
//            this, &MainWindow::hour_changed);

    m_audio_entity_data_model = std::make_unique<EntityDataModel>(std::make_shared<AUDIO::Audio>());

    //set_ui_style();

    m_datetime_selection.sel_date = QDate::currentDate();
    show_selection(m_datetime_selection);

    m_audio_history = std::make_unique<AudioHistoryForm>();

    ui->tvSchedule->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tvSchedule, &QTableView::customContextMenuRequested, this, &MainWindow::contextMenuRequested);

    //test_ranges();
    fetch_default_data();


    std::string uname = std::format("Username: {}      ",ci.username);
    std::string station = std::format("Station: {}      ", si.station_name.toStdString());
    std::string db_ip = std::format("Host: {}      ",si.ip_address.toStdString());

    QLabel* station_label = new QLabel( QString::fromStdString(station));
    station_label->setStyleSheet("font-weight: bold; color: red");

    QStatusBar* sb = new QStatusBar(this);
    sb->addWidget(new QLabel(QString::fromStdString(uname)));

    sb->addWidget(station_label);

    sb->addWidget(new QLabel(QString::fromStdString(db_ip)));

    setStatusBar(sb);

    QString title = QString("Raven-Sedric: %1").arg(si.station_name);

    setWindowTitle(title);
}

MainWindow::~MainWindow()
{
    if (m_save_as)
        delete m_save_as;
    delete ui;
}

void MainWindow::test_ranges()
{
    std::vector<int> vecs{1, 2, 3, 4, 5, 6, 7};
    for(int v: vecs | rv::reverse | rv::take(3) | rv::reverse){
        qDebug() << v << ",";
    }
    qDebug() << "\n";
}

void MainWindow::set_toolbutton_icons()
{
    ui->btnDateTimeSelector->setIconSize(QSize(50,50));
    ui->btnDateTimeSelector->setIcon(QIcon(":/images/icons/datetime_picker_sm.bmp"));
    ui->btnDateTimeSelector->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->btnNew->setIconSize(QSize(50,50));
    ui->btnNew->setIcon(QIcon(":/images/icons/new_schedule_sm.bmp"));
    ui->btnNew->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->btnSave->setIconSize(QSize(50,50));
    ui->btnSave->setIcon(QIcon(":/images/icons/save_sched2_sm.bmp"));
    ui->btnSave->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->btnPrint->setIconSize(QSize(50,50));
    ui->btnPrint->setIcon(QIcon(":/images/icons/print_schedule_sm.bmp"));
    ui->btnPrint->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->btnRemove->setIconSize(QSize(50,50));
    ui->btnRemove->setIcon(QIcon(":/images/icons/remove_music_sm.bmp"));
    ui->btnRemove->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->btnSaveAs->setIconSize(QSize(50,50));
    ui->btnSaveAs->setIcon(QIcon(":/images/icons/schedule_save_as.bmp"));
    ui->btnSaveAs->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

}

void MainWindow::set_ui_style()
{
    ui->btnNew->setStyleSheet(
                "QPushButton#btnNew{"
                "background-color: rgb(255, 170, 127);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-radius: 10px;"
                "border-color: beige;"
                "min-width: 6em;"
                "padding: 6px;}"

                "QPushButton#btnNew:pressed{"
                "background-color: rgb(217, 144, 108);"
                "border-style: inset;}"
                );

    ui->btnSave->setStyleSheet(
                "QPushButton#btnSave{"
                "background-color: rgb(255, 170, 127);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-radius: 10px;"
                "border-color: beige;"
                "min-width: 6em;"
                "padding: 6px;}"

                "QPushButton#btnSave:pressed{"
                "background-color: rgb(217, 144, 108);"
                "border-style: inset;}"
                );

    ui->btnSaveAs->setStyleSheet(
                "QPushButton#btnSaveAs{"
                "background-color: rgb(255, 170, 127);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-radius: 10px;"
                "border-color: beige;"
                "min-width: 6em;"
                "padding: 6px;}"

                "QPushButton#btnSaveAs:pressed{"
                "background-color: rgb(217, 144, 108);"
                "border-style: inset;}"
                );

    ui->btnRemove->setStyleSheet(
                "QPushButton#btnRemove{"
                "background-color: rgb(255, 85, 127);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-radius: 10px;"
                "border-color: beige;"
                "min-width: 6em;"
                "padding: 6px;}"

                "QPushButton#btnRemove:pressed{"
                "background-color: rgb(255, 0, 0);"
                "border-style: inset;}"
                );

    ui->btnPrint->setStyleSheet(
                "QPushButton#btnPrint{"
                "background-color: rgb(170, 255, 127);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-radius: 10px;"
                "border-color: beige;"
                "min-width: 6em;"
                "padding: 6px;}"

                "QPushButton#btnPrint:pressed{"
                "background-color: rgb(0, 255 ,127);"
                "border-style: inset;}"
                );



    ui->btnDateTimeSelector->setStyleSheet(
                "QPushButton#btnDateTimeSelector{"
                "background-color: rgb(170, 255, 127);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-radius: 10px;"
                "border-color: beige;"
                "min-width: 6em;"
                "padding: 6px;}"

                "QPushButton#btnDateTimeSelector:pressed{"
                "background-color: rgb(0, 255 ,127);"
                "border-style: inset;}"
                );

    // search button
    this->setStyleSheet("QPushButton#btnSearch{"
                        "background-color: rgb(170, 255, 127);"
                        "border-style: outset;"
                        "border-width: 2px;"
                        "border-radius: 10px;"
                        "border-color: beige;"
                        "min-width: 10em;"
                        "padding: 6px;}"

                        "QPushButton#btnSearch:pressed{"
                        "background-color: rgb(0, 255 ,127);"
                        "border-style: inset;}");

    /*
    ui->btnPlay->setStyleSheet(
                "QPushButton#btnPlay{"
                "background-color: rgb(0, 255, 127);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-radius: 10px;"
                "border-color: beige;"
                "min-width: 6em;"
                "padding: 6px;}"

                "QPushButton#btnPlay:pressed{"
                "background-color: rgb(170, 255, 127);"
                "border-style: inset;}"

                );

    ui->btnPlay->setIcon(QIcon(":/images/icons/play_green.png"));
//    ui->btnPlay->setIconSize(QSize(50,50));

    ui->btnStop->setStyleSheet(
                "QPushButton#btnStop{"
                "background-color: rgb(255, 85, 127);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-radius: 10px;"
                "border-color: beige;"
                "min-width: 6em;"
                "padding: 6px;}"

                "QPushButton#btnStop:pressed{"
                "background-color: rgb(255, 0, 0);"
                "border-style: inset;}"
                );

    ui->btnStop->setIcon(QIcon(":/images/icons/stop_play.png"));

    ui->btnHistory->setStyleSheet(
                "QPushButton#btnHistory{"
                "background-color: rgb(255, 170, 127);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-radius: 10px;"
                "border-color: beige;"
                "min-width: 6em;"
                "padding: 6px;}"

                "QPushButton#btnHistory:pressed{"
                "background-color: rgb(204, 136, 102);"
                "border-style: inset;}"
                );
    ui->btnHistory->setIcon(QIcon(":/images/icons/history02.png"));
    */

}

void MainWindow::contextMenuRequested(QPoint pos)
{
    QModelIndex index = ui->tvSchedule->indexAt(pos);
    m_schedule_context_menu = std::make_unique<QMenu>();


    auto selected_index = ui->tvSchedule->selectionModel()->currentIndex(); // selectedRows();

    SelectedScheduleItem ssi = get_sched_item_hour_time(selected_index);


    m_view_comm = std::make_unique<QAction>("View Commercial(s)...");

    connect(m_view_comm.get(), &QAction::triggered, this, [schedule_id = ssi.schedule_id, this](){
        view_commercial(schedule_id);});


    if (ssi.schedule_id <= 0)
        m_view_comm->setDisabled(true);

    m_schedule_context_menu->addAction(m_view_comm.get());
    m_schedule_context_menu->popup(ui->tvSchedule->mapToGlobal(pos));

}

void MainWindow::view_commercial(int schedule_id)
{

    if (m_booked_comm_dlg == nullptr)
        m_booked_comm_dlg = std::make_unique<BookedCommercialDlg>(this);

    m_booked_comm_dlg->show_booked_commercial(schedule_id);
    m_booked_comm_dlg->exec();

}

void MainWindow::selected_comm_break()
{
    auto selected_index = ui->tvSchedule->selectionModel()->currentIndex(); // selectedRows();
    SelectedScheduleItem ssi = get_sched_item_hour_time(selected_index);
}

void MainWindow::save_as()
{
    //m_save_as = new SaveAs(get_selected_hours_asInt(), this);
    if (m_datetime_selection.sel_hours.size() == 0){
        showMessage("Nothing selected for saving!", QMessageBox::Critical);
        return;
    }

    m_save_as = new SaveAs(m_datetime_selection.sel_hours, this);

    if (m_save_as->exec() == 1){
        Result result = m_save_as->save_result();
        m_scheduler->copy_schedule(m_datetime_selection.sel_date  , result.dest_date, result.dest_map);
    }
}

/*
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
*/

void MainWindow::folder_clicked(const QModelIndex& index)
{
    int folder_id = index.data(Qt::UserRole).toInt();

    auto audio = std::make_unique<AUDIO::Audio>();
    auto folder_filter = std::make_tuple(audio->folder()->dbColumnName(), " = ", folder_id);
    std::string filter_str = m_audio_entity_data_model->prepareFilter(folder_filter);

    qDebug() << stoq(filter_str);

    fetch_audio(filter_str);
}

SelectedScheduleItem MainWindow::get_sched_item_hour_time(const QModelIndex& index)
{
    SelectedScheduleItem si;

    auto time_col = ui->tvSchedule->model()->index(index.row(), 0);
    auto item_data = time_col.data(Qt::UserRole).toMap();

    si.row_id = item_data["row_id"].toInt();
    si.schedule_id = item_data["schedule_id"].toInt();
    si.schedule_type = item_data["schedule_type"].toString();
    si.schedule_hour = item_data["hour"].toInt();
    si.time_stamp = item_data["time"].toTime();

    return si;

}

void MainWindow::print_details(const QModelIndex& index)
{
}

void MainWindow::print_schedule()
{
}

AUDIO::Audio* MainWindow::get_selected_audio()
{
    auto mod_index = ui->tvTracks->currentIndex();
    auto first_col = ui->tvTracks->model()->index(mod_index.row(), 0);
    auto audio_id = first_col.data(Qt::UserRole).toInt();

    if (audio_id == 0)
        return nullptr;

    auto audio = m_audio_lib_item->find_audio_by_id(audio_id);
    return audio;
}

void MainWindow::track_selected(const QModelIndex& index)
{
    if (m_audio_history->isVisible()){
        show_audio_history();
    }
}

void MainWindow::insert_item(const QModelIndex& index)
{
    auto audio = this->get_selected_audio();
    if (audio == nullptr)
        return;

    if (!QFile::exists(audio->full_audio_filename())){
        showMessage("Audio file does not exists!", QMessageBox::Critical);
        return;
    }

    Schedule* schedule = new Schedule();

    SelectedScheduleItem ssi = get_sched_item_hour_time(index);

    schedule->setId(-1);
    schedule->set_schedule_date(m_datetime_selection.sel_date);
    schedule->set_schedule_time(ssi.time_stamp);
    schedule->set_schedule_hour(ssi.schedule_hour);

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

    m_scheduler->update_time(ssi.schedule_hour, index.row(), audio->duration()->value());

    m_scheduler->log_activity(m_datetime_selection.sel_date, ssi.schedule_hour);
}

void MainWindow::remove_current_schedule_item()
{
    auto index = ui->tvSchedule->currentIndex();
    if (index.row() < 0)
        return;

    SelectedScheduleItem ssi = get_sched_item_hour_time(index);

    auto schedule = m_scheduler->find_schedule_item(ssi.row_id);
    if (schedule == nullptr)
        return;

    if (!is_item_deletable(stoq(schedule->schedule_item_type()->value())))
        return;

    m_scheduler->delete_row(ssi.row_id);

    remove_item(index.row());

    m_scheduler->update_time(ssi.schedule_hour, index.row(), schedule->break_duration()->value()*-1);

    m_scheduler->log_activity(m_datetime_selection.sel_date, ssi.schedule_hour);
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
    return (item_type == "COMM-BREAK" || item_type == "END-MARKER") ? false : true;

    /*
    if (item_type=="COMM-BREAK")
        return false;
    if (item_type=="END-MARKER")
        return false;

    return true;
   */
}


void MainWindow::new_schedule()
{
    if (QMessageBox::warning(this, tr("Sedric"),
                          tr("This will clear all entered items for the selected date and time. Continue?"),
                          QMessageBox::Ok | QMessageBox::Cancel,
                          QMessageBox::Cancel) == QMessageBox::Ok)
    {
       if (m_datetime_selection.sel_hours.size()== 0)
           return;

       clear_schedule_model();

       m_scheduler->new_schedule(m_datetime_selection.sel_date, m_datetime_selection.sel_hours);
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
    m_tracks_model->setHorizontalHeaderItem(5, new QStandardItem("Folder"));
}

void MainWindow::adjust_header_size()
{
    QHeaderView* vert_header = ui->tvSchedule->verticalHeader();
    vert_header->setSectionResizeMode(QHeaderView::Fixed);
    vert_header->setDefaultSectionSize(20);
}

void MainWindow::set_track_view_column_width()
{
    enum Column{Title, Artist, Duration, AudioType, AudioFile, Folder};

    ui->tvTracks->setColumnWidth(Column::Title, 300);
    ui->tvTracks->setColumnWidth(Column::Artist,250);
    ui->tvTracks->setColumnWidth(Column::Duration, 100);
    ui->tvTracks->setColumnWidth(Column::AudioType, 150);
    ui->tvTracks->setColumnWidth(Column::AudioFile, 250);
    ui->tvTracks->setColumnWidth(Column::Folder, 250);
}

void MainWindow::create_model_headers()
{

    m_schedule_model->setHorizontalHeaderItem(0, new QStandardItem("Time"));
    m_schedule_model->setHorizontalHeaderItem(1, new QStandardItem("Title"));
    m_schedule_model->setHorizontalHeaderItem(2, new QStandardItem("Artist"));
    m_schedule_model->setHorizontalHeaderItem(3, new QStandardItem("Duration"));
    m_schedule_model->setHorizontalHeaderItem(4, new QStandardItem("Audio Type"));
    m_schedule_model->setHorizontalHeaderItem(5, new QStandardItem("Play Date"));
    m_schedule_model->setHorizontalHeaderItem(6, new QStandardItem("Play Time"));
    m_schedule_model->setHorizontalHeaderItem(7, new QStandardItem("Track Path"));
    m_schedule_model->setHorizontalHeaderItem(8, new QStandardItem("Comment"));

}

void MainWindow::set_column_width()
{
    enum Column{TIME, TITLE, ARTIST, DURATION, AUDIO_TYPE, PLAY_DATE, PLAY_TIME, TRACK_PATH, COMMENT};

    ui->tvSchedule->setColumnWidth(Column::TIME, 100);
    ui->tvSchedule->setColumnWidth(Column::TITLE, 250);
    ui->tvSchedule->setColumnWidth(Column::ARTIST, 200);
    ui->tvSchedule->setColumnWidth(Column::DURATION, 80);
    ui->tvSchedule->setColumnWidth(Column::AUDIO_TYPE, 120);
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

/*
void MainWindow::hour_cb_closed()
{
    const auto& hours = get_selected_hours_asInt();
    //fetch_data(QDate sel_date, const std::vector<int>& sel_hours)
}
*/

void MainWindow::fetch_data(QDate sel_date, const std::vector<int>& sel_hours)
{
    if (sel_hours.size() == 0)
        return

    clear_schedule_model();

    auto processed = fetch_schedule_from_cache(sel_date, sel_hours);

    std::vector<int> uncached_hours;
    std::transform(processed.begin(), processed.end(), back_inserter(uncached_hours), UnCachedHours());

    uncached_hours.erase(
                remove_if( uncached_hours.begin(), uncached_hours.end(),
                        [](int i){ return i == -1; }), uncached_hours.end());

    fetch_schedule_from_db(sel_date, uncached_hours);
    m_scheduler->show_items(sel_date, sel_hours); //selected_hours);

}


std::vector<int> MainWindow::get_selected_hours_asInt()
{
    std::vector<int> hours;

    if (m_cb_hour->count() == 0)
        return hours;

    auto selected_hours = split_string(m_cb_hour->currentText().toStdString(), ',');
    hours = vector_str_to_int(selected_hours);

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

std::map<int, int> MainWindow::fetch_schedule_from_cache(QDate date, const std::vector<int>& hours)
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

void MainWindow::set_track_view()
{
    m_tracks_model->clear();
    m_audio_lib_item->clear();
    create_track_view_headers();
    set_track_view_column_width();
}

void MainWindow::fetch_audio(const std::string filter)
{

    m_audio_entity_data_model->clearEntities();

    m_audio_entity_data_model->search(filter);

    show_audio_data();

}

void MainWindow::show_audio_data()
{
    set_track_view();

    for (auto&[name, entity] : m_audio_entity_data_model->modelEntities()){

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
    if (m_datetime_selection.sel_hours.size() == 0){
        showMessage("Nothing selected for saving!", QMessageBox::Critical);
        return;
    }

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

    auto audio = this->get_selected_audio();
    if (audio == nullptr)
        return;

    AUDIO::AudioTool audio_tool;
    auto ogg_file = audio_tool.make_audio_filename(audio->id())+".ogg";
    auto full_audio_name = audio->file_path()->value()+ogg_file;

    if (!QFile::exists(QString::fromStdString(full_audio_name))){
        showMessage("File does not exits! "+full_audio_name, QMessageBox::Critical);
        return;
    }

    AudioFile af(full_audio_name);
    m_audio_player = std::make_unique<AUDIO::AudioPlayer>(af);
    m_audio_player->play_audio("C", QString::fromStdString(full_audio_name));

}

void MainWindow::stop_play()
{
    if (m_audio_player != nullptr)
        m_audio_player->stop_play();
}

History MainWindow::make_history(int id)
{
    Schedule schedule;

    auto edm = std::make_unique<EntityDataModel>(std::make_shared<Schedule>());

    QString date_format = "yyyy-MM-dd";
    QString d_range1 = m_datetime_selection.sel_date.addDays(-7).toString(date_format);
    QString d_range2 = m_datetime_selection.sel_date.toString(date_format);

    QString date_range = "'"+d_range1+"' and '"+d_range2+ "'";

    auto date_filter = std::make_tuple(schedule.schedule_date()->dbColumnName(),
                                       " between ",
                                       date_range.toStdString());

    auto audio_filter = std::make_tuple(schedule.audio()->dbColumnName(),
                                        " = ",
                                        id);

    try{
        edm->search(edm->prepareFilter(date_filter, audio_filter));
    } catch (DatabaseException& de) {
        showMessage(de.errorMessage());
    }

    History hist;
    for (auto& [name, entity] : edm->modelEntities()){
        Schedule* sched = dynamic_cast<Schedule*>(entity.get());

       hist[sched->schedule_date()->value().dayOfWeek()].push_back(sched->schedule_hour()->value());

    }

    return hist;

}

void MainWindow::show_audio_history()
{

    auto audio = this->get_selected_audio();

    if (audio == nullptr)
        return;

    auto history =  make_history(audio->id());

    if(!ui->btnHistory->isChecked()){
        ui->vlHistory->removeWidget(m_audio_history.get());
        m_audio_history->hide();
    } else {
        m_audio_history->show_history(&history);
        ui->vlHistory->addWidget(m_audio_history.get());
        m_audio_history->show();
    }

}

void MainWindow::select_date_time()
{
    std::unique_ptr<DateTimeSelector> dts = std::make_unique<DateTimeSelector>(this, m_datetime_selection);
    if (dts->exec() == 1){
        m_datetime_selection = dts->selection();
        std::sort(m_datetime_selection.sel_hours.begin(), m_datetime_selection.sel_hours.end());

        fetch_data(m_datetime_selection.sel_date, m_datetime_selection.sel_hours);
        show_selection(m_datetime_selection);
    }

}

void MainWindow::fetch_default_data()
{
    int prev_hour = 0;
    int next_hour = 0;
    int curr_hour = QTime::currentTime().hour();

    constexpr int HOURS_IN_A_DAY = 23;

    if ( (curr_hour - 1) < 0 ){
        prev_hour = 0;
    } else {
        prev_hour = curr_hour - 1;
    }

    if ((curr_hour + 1) > HOURS_IN_A_DAY ){
        next_hour = HOURS_IN_A_DAY;
    } else {
        next_hour = curr_hour + 1;
    }

    std::vector<int> def_hours{prev_hour, curr_hour, next_hour};

    m_datetime_selection.sel_date = QDate::currentDate();
    m_datetime_selection.sel_hours = def_hours;

    fetch_data(QDate::currentDate(), def_hours);
    show_selection(m_datetime_selection);
}

void MainWindow::show_selection(DateTimeSelection selection)
{
    ui->btnDateTimeSelector->setText(selection.sel_date.toString("dd/MM/yyyy"));
    QString label = "[ ";
    QString h{""};
    std::sort(selection.sel_hours.begin(),selection.sel_hours.end());
    for (auto hr : selection.sel_hours){
        if (!h.isEmpty())
            h += ", ";
        h += QString::number(hr);
    }
    label += h;
    label += " ]";
    ui->lblDateTime->setText(label);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
}

void MainWindow::search_audio()
{
    m_audio_entity_data_model->clearEntities();

    AUDIO::Artist artist;
    AUDIO::Genre genre;
    AUDIO::Mood mood;
    AUDIO::Folder folder;

    auto const& audio = dynamic_cast<AUDIO::Audio*>(m_audio_entity_data_model->get_entity().get());

    auto track_filter = std::make_tuple(audio->title()->qualified_column_name<AUDIO::Audio>(),"like", ui->edtTitle->text().toStdString());
    auto artist_filter = std::make_tuple(artist.fullName()->qualified_column_name<AUDIO::Artist>(),"like", ui->edtArtist->text().toStdString());

    FRAMEWORK::RelationMapper* r_map = m_audio_entity_data_model->select_related(folder, artist, genre, mood)->filter(track_filter, artist_filter);

    try{
        m_audio_entity_data_model->readRaw(r_map->query());
    } catch(DatabaseException& de) {
        showMessage(de.errorMessage(), QMessageBox::Warning);
    }

    r_map->map_data();

    //TODO: Refactor this code

    for (auto const& [record_id, record] : r_map->mapped_entities()){

        auto audio_uPtr = std::make_unique<AUDIO::Audio>();

        bool audio_is_constructed = false;

        for (auto const& [table_name, entities] : record){

            for (auto const& entity : entities){

                if (audio_uPtr->tableName() == entity->tableName() &&
                    !audio_is_constructed){
                    if (entity->id() > -1 ){
                        auto audio_ptr = dynamic_cast<AUDIO::Audio*>(entity.get());
                        *audio_uPtr.get() = *audio_ptr;
                        audio_is_constructed = true;
                        break;
                    }
                }

                auto const& artist = audio_uPtr->artist()->data_model_entity();
                if (artist == nullptr){
                    continue;
                }

                if (artist->tableName() == entity->tableName()){
                    auto artist_ptr = dynamic_cast<AUDIO::Artist*>(entity.get());

                    auto artist_uptr = std::make_unique<AUDIO::Artist>();

                    *artist_uptr.get() = *artist_ptr;
                    audio->artist()->set_fk_entity(std::move(artist_uptr));
                }

            }

        }

        if (audio_uPtr->audio_type()->value() != ""){
            m_audio_entity_data_model->add_entity(std::move(audio_uPtr));
        }

    }
    show_audio_data();
}


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

