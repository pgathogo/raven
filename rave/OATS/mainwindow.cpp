#include <map>
#include <chrono>
#include <thread>

#include <math.h>
#include <algorithm>
#include <concepts>
#include <iostream>
#include <sstream>
#include <format>

#include <QVBoxLayout>
#include <QPushButton>
#include <QTime>
#include <QScrollBar>
#include <QDebug>
#include <QStandardItemModel>
#include <QSplitter>
#include <QStandardItem>
#include <QList>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QVariant>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "../framework/entitydatamodel.h"
#include "../framework/baseentity.h"
#include "../framework/ravenexception.h"
#include "../framework/tree.h"
#include "../framework/treeviewmodel.h"
#include "../framework/schedule.h"
#include "../framework/databasemanager.h"

#include "../audio/audiolibrary.h"
#include "../audio/audiotrackviewer.h"
#include "../audio/audio.h"
#include "../audio/artist.h"
#include "../audio/audiotrackitem.h"
#include "../audio/audiolibitem.h"
#include "../audio/trackpickerdialog.h"
#include "../audio/trackbrowser.h"
#include "../audio/audiocachemanager.h"
#include "../audio/audiocachequeuethread.h"
#include "../audio/cacheupdaterthread.h"
#include "../audio/audiocache.h"
#include "../audio/editor/audioplayer.h"
#include "../audio/audiofile.h"

#include "../data/commercialviewer.h"

//#include "timeanalyzerwidget.h"
#include "timeanalyzertextwidget.h"
#include "datetimewidget.h"
#include "playlistcontrolwidget.h"
#include "scheduleitem.h"
#include "schedulegriditem.h"
#include "outputpanel.h"
#include "playmodepanel.h"
#include "trackinfo.h"
#include "jinglegrid.h"
#include "cartpanel.h"

int MainWindow::s_sched_ref{0};
std::string MainWindow::s_channel{"A"};

using namespace std::chrono_literals;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dtw{nullptr}
    , m_outputA{nullptr}
    , m_outputB{nullptr}
{
    ui->setupUi(this);

    m_current_cued_item.item = nullptr;

    // First load any cached audio data
    m_audio_cache_manager = std::make_shared<AUDIO::AudioCacheManager<SQLiteDatabaseManager>>(CACHE_DB_SQLITE, AUDIO_CACHE_LOCATION);
    auto rec_count = m_audio_cache_manager->load_cached_audio();

    m_queue_mutex = std::make_shared<QMutex>();

    auto thread1 =  std::make_unique<AUDIO::AudioCacheQueueThread<SQLiteDatabaseManager>>(m_audio_cache_manager, m_queue_mutex, this);
    auto thread2 =  std::make_unique<AUDIO::AudioCacheQueueThread<SQLiteDatabaseManager>>(m_audio_cache_manager, m_queue_mutex, this);
    auto thread3 =  std::make_unique<AUDIO::AudioCacheQueueThread<SQLiteDatabaseManager>>(m_audio_cache_manager, m_queue_mutex, this);
    auto thread4 =  std::make_unique<AUDIO::AudioCacheQueueThread<SQLiteDatabaseManager>>(m_audio_cache_manager, m_queue_mutex, this);

    m_cache_queue_threads.push_back(std::move(thread1));
    m_cache_queue_threads.push_back(std::move(thread2));
    m_cache_queue_threads.push_back(std::move(thread3));
    m_cache_queue_threads.push_back(std::move(thread4));

    m_cache_updater = std::make_unique<AUDIO::CacheUpdaterThread<SQLiteDatabaseManager>>(m_audio_cache_manager, m_queue_mutex, this);

    m_cache_updater_timer = std::make_unique<QTimer>(this);
    connect(m_cache_updater_timer.get(), &QTimer::timeout, this, &MainWindow::persist_cache);
    auto five_seconds = 5000ms;
    m_cache_updater_timer->start(five_seconds);

    // Load data from DB

    //load_schedule_bydate(QDate::currentDate());

    load_schedule(QDate::currentDate(), 0);

    set_playout_widgets();

    compute_schedule_time();

    setup_timers();

    ui->swMain->setCurrentIndex(0);
//    ui->twLoad->setCurrentIndex(0);

//    connect(ui->gridScroll, &QScrollBar::valueChanged, this, &MainWindow::scroll_changed);
    connect(m_play_mode_panel.get(), &OATS::PlayModePanel::go_current, this, &MainWindow::go_current);
    connect(m_play_mode_panel.get(), &OATS::PlayModePanel::keep_current, this, &MainWindow::keep_current);
    connect(m_dtw.get(), &OATS::DateTimeWidget::time_updated, this, &MainWindow::time_updated);
    connect(m_jingle_grid.get(), &OATS::JingleGrid::play_jingle, this, &MainWindow::play_jingle);
    connect(m_jingle_grid.get(), &OATS::JingleGrid::stop_all_jingles, this, &MainWindow::stop_all_jingles);

    connect(ui->btnHome, &QPushButton::clicked, this, [&](){ ui->swMain->setCurrentIndex(0); m_control_page = ControlPage::Home; });
    connect(ui->btnComm, &QPushButton::clicked, this, [&](){ ui->swMain->setCurrentIndex(1); m_control_page = ControlPage::Commercial; });
//    connect(ui->btnSegue, &QPushButton::clicked, this, [&](){ ui->swMain->setCurrentIndex(2); m_control_page = ControlPage::Segue; });
    connect(ui->btnCarts, &QPushButton::clicked, this, [&](){ui->swMain->setCurrentIndex(3); m_control_page = ControlPage::Cart; });
    connect(ui->btnJingles, &QPushButton::clicked, this, [&](){ui->swMain->setCurrentIndex(4); m_control_page = ControlPage::Jingle; });
    connect(ui->btnTrackInfo, &QPushButton::clicked, this, [&](){ui->swMain->setCurrentIndex(5); m_control_page = ControlPage::TrackInfo; });
    connect(ui->btnLoad, &QPushButton::clicked, this, [&](){ui->swMain->setCurrentIndex(7); m_control_page = ControlPage::Load; });

    connect(ui->btnExit, &QPushButton::clicked, this, &MainWindow::close_window);

    connect(ui->btnPrint1, &QPushButton::clicked, this, [&](){m_audio_cache_manager->print_queue();});
    connect(ui->btnPrint2, &QPushButton::clicked, this, [&](){m_audio_cache_manager->print_cache();});
    connect(ui->btnPrint3, &QPushButton::clicked, this, [&](){m_audio_cache_manager->cache_audio();});
    connect(ui->btnClearQue, &QPushButton::clicked, this, [&](){m_audio_cache_manager->clear_queue();});
    connect(ui->btnClearCache, &QPushButton::clicked, this, [&](){m_audio_cache_manager->clear_cache();});

    connect(ui->btnBreaks, &QPushButton::clicked, this, &MainWindow::print_comm_breaks);

//    connect(ui->btnPrintCache, &QPushButton::clicked, this, [&](){m_audio_cache_manager->print_cache();});

    // Audio Library Page

//    connect(ui->btnSearch, &QPushButton::clicked, this, &MainWindow::search_audio);

    m_track_browser = std::make_unique<AUDIO::TrackBrowser>();
    ui->vlTrackBrowser->addWidget(m_track_browser.get());

    m_cart_panel = std::make_unique<OATS::CartPanel>();
    ui->vlCart->addWidget(m_cart_panel.get());

    start_timers();

    m_audio_player = std::make_unique<AUDIO::AudioPlayer>();
    connect(m_audio_player.get(), &AUDIO::AudioPlayer::end_of_play, this, &MainWindow::end_of_play);
    connect(m_audio_player.get(), &AUDIO::AudioPlayer::play_next, this, &MainWindow::play_next);

    m_jingle_player = std::make_unique<AUDIO::AudioPlayer>();
    connect(m_jingle_player.get(), &AUDIO::AudioPlayer::end_of_play, this, &MainWindow::jingle_end_of_play);

    m_current_jingle_item = std::make_unique<OATS::ScheduleItem>();
    //test_concept();

    style_page_controls();

    calculate_time_stats();

    m_updatedb_mutex = std::make_shared<QMutex>();

    m_updatedb_timer = std::make_unique<QTimer>(this);
    connect(m_updatedb_timer.get(), &QTimer::timeout, this, &MainWindow::db_update_comm_break_play_status);
    auto two_seconds = 2s;
    m_updatedb_timer->start(two_seconds);

    //setStyleSheet("background-color: #222222;");
    //QMainWindow::showFullScreen();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::style_page_controls()
{
    QString style(
            "background-color:#00A36C;"
            "color: #FFFFFF;"
            "font:bold;"
            "height:30;"
        );

    ui->btnHome->setStyleSheet(style);
    ui->btnComm->setStyleSheet(style);
//    ui->btnSegue->setStyleSheet(style);
    ui->btnCarts->setStyleSheet(style);
    ui->btnJingles->setStyleSheet(style);
    ui->btnTrackInfo->setStyleSheet(style);
    ui->btnLoad->setStyleSheet(style);
//    ui->btnPrintCache->setStyleSheet(style);
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
    QRect rect = ui->vlPlaylist->contentsRect();
    QPointF pos = event->position();

    if( pos.x() >= rect.topLeft().x()+5 &&
        pos.x() < rect.bottomRight().x()-5 &&
        pos.y() > rect.topLeft().y() &&
        pos.y() <= rect.bottomRight().y()  )
    {
        QPoint degree = event->angleDelta() / 8;
        QPoint step = degree/15;

        ui->gridScroll->setValue(ui->gridScroll->value() + step.y());
    }
}

void MainWindow::test_concept()
{
    std::vector ints{1, 2, 3,4,4,3,6,7};
    std::cout << Average(ints);
}


void MainWindow::setup_timers()
{        
    m_slow_flash_timer = std::make_unique<QTimer>(this);
    connect(m_slow_flash_timer.get(), &QTimer::timeout, this, &MainWindow::slow_flash);

    m_fast_flash_timer = std::make_unique<QTimer>(this);
    connect(m_fast_flash_timer.get(), &QTimer::timeout, this, &MainWindow::fast_flash);

    m_countdown_timer = std::make_unique<QTimer>(this);
    connect(m_countdown_timer.get(), &QTimer::timeout, this, &MainWindow::count_down);

    m_main_player_timer = std::make_unique<QTimer>(this);
    connect(m_main_player_timer.get(), &QTimer::timeout, this, &MainWindow::status_timer);

    m_time_stats_timer = std::make_unique<QTimer>(this);
    connect(m_time_stats_timer.get(), &QTimer::timeout, this, &MainWindow::on_calc_time_stats);
}

void MainWindow::start_timers()
{
    auto SLOW_FLASH_INTERVAL = 400ms;
    auto FAST_FLASH_INTERVAL = 250ms;
    auto MAIN_TIMER_INTERVAL = 25ms;
    auto COUNT_DOWN_INTERVAL = 50ms;
    auto TIME_STATS_INTERVAL = 3000ms;

    m_slow_flash_timer->start(SLOW_FLASH_INTERVAL);
    m_fast_flash_timer->start(FAST_FLASH_INTERVAL);
    m_countdown_timer->start(COUNT_DOWN_INTERVAL);
    m_main_player_timer->start(MAIN_TIMER_INTERVAL);
    m_time_stats_timer->start(TIME_STATS_INTERVAL);
}

void MainWindow::set_playout_widgets()
{
    set_datetime_widget();
    set_playlist_control_widget();
    set_time_analytics_widget();

    make_output_panel();
    make_play_mode_panel();
    make_comm_viewer_widget();
    make_track_info_widget();
    make_jingle_grid_widget();

}

void MainWindow::close_win()
{
    close();
}

void MainWindow::set_datetime_widget()
{
    m_dtw = std::make_unique<OATS::DateTimeWidget>();

    auto curr_time = QTime::currentTime();
    m_dtw->set_time(curr_time);
    ui->hlDateTime->addWidget(m_dtw.get());

    m_onair_label = std::make_unique<QLabel>("On-Air");
    m_onair_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    QPixmap onair(":/images/icons/OnAir1.png");
    m_onair_label->setPixmap(onair);
    ui->hlDateTime->addWidget(m_onair_label.get());

    ui->hlDateTime->setStretch(1, 1);

    m_dtw->start_timer(100);
}

void MainWindow::set_time_analytics_widget()
{

    m_tta_widget = std::make_unique<OATS::TimeAnalyzerTextWidget>();
    ui->hlTimeAnalytics->addWidget(m_tta_widget.get());

    /*
    auto m_taw = std::make_unique<TimeAnalyzerWidget>();
    std::map<QString, float> time_slots;

    time_slots["music_remain"] = 0.1;
    time_slots["time_remain"] = 0.1;
    time_slots["talk_time"] = 0.1;

    m_taw->set_time_bar(time_slots);
    ui->hlTimeAnalytics->addWidget(m_taw.get());
   */

}

void MainWindow::set_playlist_control_widget()
{
    m_pcw = std::make_unique<OATS::PlayListControlWidget>();
    connect(m_pcw.get(), &OATS::PlayListControlWidget::go_current, this, &MainWindow::go_current);

    QString style(
            "background-color:#00A36C;"
            "color: #FFFFFF;"
            "font:bold;"
        );

    m_pcw->setStyleSheet(style);

    //ui->hlPlayMode->addWidget(m_pcw.get());
}

void MainWindow::compute_schedule_time()
{
    if (m_current_playing_item.item == nullptr)
        return;

    int time_counter = m_current_playing_item.item->audio()->duration()->value();

    m_current_playing_item.item->set_current_time(time_counter);
    m_current_playing_item.item->notify();

    time_counter = time_counter + m_current_playing_item.item->audio()->duration()->value();

    for (int i=m_current_playing_item.schedule_index+1; i < m_schedule_items.size()-1; ++i){
        auto si = schedule_item(i);
        si->set_current_time(time_counter);
        time_counter = time_counter + si->audio()->duration()->value();

        //si->notify();
    }
}

void MainWindow::load_schedule_bydate(QDate date)
{
    build_hour_headers(date, 0);

    fetch_schedule_bydate(date);

    fetch_commercials_bydate(date);

    remove_empty_commercial_breaks();

    build_master_schedule();

}

void MainWindow::build_hour_headers(QDate date, int hr)
{
    for (int hour=hr; hour < HOURS_IN_A_DAY; ++hour)
    {
        auto header = std::make_unique<OATS::ScheduleItem>();
        set_header_item(header.get(), hour, date);
        m_schedule_items.push_back(std::move(header));
    }

}


void MainWindow::fetch_schedule_bydate(QDate date)
{
    assert(m_day_schedule_items.size() == HOURS_IN_A_DAY-1);

    std::stringstream sql;

    sql << " SELECT a.id, a.schedule_date, a.schedule_hour, a.schedule_time, "
        << " a.auto_transition, a.play_date, a.play_time, "
        << " a.schedule_item_type, a.comment, a.booked_spots, "
        << " a.audio_id, b.title, b.filepath, b.duration, b.file_extension,"
        << " b.audio_type, c.id AS artist_id, c.fullname "
        << " FROM rave_schedule a  "
        << " left outer join rave_audio b on a.audio_id = b.id "
        << " left outer join rave_artist c on b.artist_id = c.id ";

    std::string date_filter = std::format(" WHERE a.schedule_date ='{}'",date.toString("yyyy/MM/dd").toStdString());
    std::string order_by    = " ORDER BY a.schedule_hour, a.schedule_time, a.id ";

    sql << date_filter <<  order_by;
    EntityDataModel edm;
    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();

    if (provider->cacheSize() == 0)
        return;

    provider->cache()->first();
    do{

        auto schedule_item = std::make_shared<OATS::ScheduleItem>();
        set_schedule_fields(provider, schedule_item.get());

        m_day_schedule_items[schedule_item->hour()].push_back(schedule_item);

        provider->cache()->next();
    } while(!provider->cache()->isLast());

}

void MainWindow::remove_empty_commercial_breaks()
{
    std::vector<int> s_ids;
    for(auto& [key, value]: m_comm_breaks){
        s_ids.push_back(key);
    }

    auto schedule_not_found = [&](std::shared_ptr<OATS::ScheduleItem> s_item){
        if (s_item->schedule_type() == OATS::ScheduleType::COMM){
            return (std::find(s_ids.begin(), s_ids.end(), s_item->id()) == s_ids.end()) ? true : false;
        }
        return false;
    };

    for(auto& [hour, schedule_items]: m_day_schedule_items) {
        auto deleted_breaks = std::erase_if(schedule_items, schedule_not_found);
    }

}


void MainWindow::build_master_schedule()
{
    for (int hr=0; hr <= HOURS_IN_A_DAY; ++hr)
    {
        for(auto& schedule: m_day_schedule_items[hr]){
            m_master_schedule.push_back(std::move(schedule));
        }
    }
}



void MainWindow::load_schedule(QDate date, int hr)
{
//    fetch_cached_data(hr);

#ifdef DB_SCHEDULE
    fetch_data_from_db(date, hr);
#endif

#ifdef TEMP_SCHEDULE
    fetch_temp_data(hr);
#endif
    make_playlist_grid();

    set_current_play_item();

    //move_top_current_hour();
    connect(ui->gridScroll, &QScrollBar::valueChanged, this, &MainWindow::scroll_changed);

}

void MainWindow::fill_schedule_headers(QDate date)
{
    for(int hr=0; hr <= HOURS_IN_A_DAY-1; ++hr){
        auto header_item = std::make_unique<OATS::ScheduleItem>();
        set_header_item(header_item.get(), hr, date);
        m_schedule_items.push_back(std::move(header_item));
    }
}

void MainWindow::fetch_data_from_db(QDate date, int hr)
{
    build_hour_headers(QDate::currentDate(), hr);

    fetch_commercials_bydate(date);

    std::stringstream sql;

    sql << " SELECT a.id, a.schedule_date, a.schedule_hour, a.schedule_time, "
        << " a.auto_transition, a.play_date, a.play_time, "
        << " a.schedule_item_type, a.comment, a.booked_spots, "
        << " a.audio_id, b.title, b.filepath, b.duration, b.file_extension,"
        << " b.audio_type, c.id AS artist_id, c.fullname "
        << " FROM rave_schedule a  "
        << " left outer join rave_audio b on a.audio_id = b.id "
        << " left outer join rave_artist c on b.artist_id = c.id ";

    std::string where_filter = " WHERE a.schedule_date = '"+date.toString("yyyy/MM/dd").toStdString()+"'";
    std::string time_filter = " AND a.schedule_hour >= "+ std::to_string(hr);
    std::string order_by     = " ORDER BY a.schedule_hour, a.schedule_time, a.id ";

    sql << where_filter << time_filter << order_by;

    EntityDataModel edm;
    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();

    if (provider->cacheSize() == 0)
        return;

    int item_duration = 0;
    int total_duration = 0;

    provider->cache()->first();
    do{
        auto sched_item = std::make_unique<OATS::ScheduleItem>();

        set_schedule_fields(provider, sched_item.get());

        if (sched_item->schedule_type() == OATS::ScheduleType::COMM)
        {
            auto comm_duration = get_comm_duration(sched_item->id());

            // Don't show empty commercial breaks
            if (comm_duration > 0)
            {
                QTime item_time = sched_item->schedule_time().addMSecs(comm_duration);
                sched_item->set_current_time(comm_duration);
                //sched_item->set_schedule_time(item_time);

                sched_item->audio()->set_duration(comm_duration);
                sched_item->audio()->set_title(
                    sched_item->schedule_time().toString("HH:mm").toStdString()+
                       " Commercial Break ("+std::to_string(sched_item->booked_spots())+" items)");

                sched_item->set_transition_type(OATS::TransitionType::CUT);

                item_duration += comm_duration;

                m_schedule_items.push_back(std::move(sched_item));
            }
        }else{
                item_duration = item_duration + sched_item->audio()->duration()->value();
                total_duration = (item_duration == 0) ?  0 : sched_item->current_time()+item_duration;

                QTime item_time = sched_item->schedule_time().addMSecs(total_duration);
                sched_item->set_current_time(total_duration);
                sched_item->set_schedule_time(item_time);

                m_schedule_items.push_back(std::move(sched_item));
        }

        provider->cache()->next();

    } while (!provider->cache()->isLast());


    std::stable_sort(m_schedule_items.begin(), m_schedule_items.end(),
                     [](std::unique_ptr<OATS::ScheduleItem> const& lhs, std::unique_ptr<OATS::ScheduleItem> const& rhs){
        return lhs->hour() < rhs->hour(); });

}



int MainWindow::get_comm_duration(int schedule_id)
{
    auto comm_breaks = m_comm_breaks[schedule_id];
    int duration=0;
    for (auto& comm : comm_breaks){
        duration += comm.duration;
    }
    return duration;
}


void MainWindow::fetch_commercials_bydate(QDate date)
{
    std::stringstream sql;

    sql << " select a.id, a.schedule_id, a.spot_id,  a.booking_status, "
           " a.book_date, a.book_hour, a.book_time, c.audio_id, "
           " d.title, d.duration, d.file_extension, d.filepath "
           " From rave_orderbooking a "
           " left outer join rave_spot b on b.id = a.spot_id "
           " left outer join rave_spotaudio c on c.spot_id = b.id "
           " left outer join rave_audio d on d.id = c.audio_id ";

    std::string date_filter = std::format(" WHERE a.book_date = '{}'",date.toString("yyyy/MM/dd").toStdString());
    std::string audio_filter = " AND c.audio_id IS NOT NULL ";

    std::string order_by     = " ORDER BY a.book_time, a.book_seq ";

    sql << date_filter << audio_filter << order_by;

    EntityDataModel edm;
    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();
    if (provider->cacheSize() == 0)
        return;

    provider->cache()->first();

    do{
        auto it_begin = provider->cache()->currentElement()->begin();
        auto it_end = provider->cache()->currentElement()->end();

        CommBreak cb;

        for (; it_begin != it_end; ++it_begin)
        {
            std::string field_name = (*it_begin).first;
            std::string field_value = (*it_begin).second;

            if (field_name == "id"){ cb.booking_id = str_to_int(field_value); }
            if (field_name == "schedule_id"){ cb.schedule_id = str_to_int(field_value); }
            if (field_name == "spot_id"){ cb.spot_id = str_to_int(field_value); }
            if (field_name == "booking_status") {cb.booking_status = QString::fromStdString(field_value); }
            if (field_name == "book_hour"){cb.book_hour = str_to_int(field_value);}
            if (field_name == "book_time") {cb.book_time = QTime::fromString(QString::fromStdString(field_value), "hh:mm:ss"); }
            if (field_name == "audio_id") {cb.audio_id = str_to_int(field_value); }
            if (field_name == "title") {cb.comm_title = QString::fromStdString(field_value);}
            if (field_name == "duration"){ cb.duration = str_to_int(field_value); }
            if (field_name == "file_extension"){ cb.file_extension = QString::fromStdString(field_value); }

            if (field_name == "filepath")
            {
                if (cb.audio_id > -1)
                {
                    auto cached_audio_path = m_audio_cache_manager->get_cached_audio_path(cb.audio_id);

                    qDebug() << "CACHE: "<< stoq(cached_audio_path);

                    cb.filepath = (!cached_audio_path.empty()) ? stoq(cached_audio_path) : stoq(field_value);
                }
            }
        }

        if (m_comm_breaks.contains(cb.schedule_id)){
           m_comm_breaks[cb.schedule_id].push_back(cb) ;
        } else {
            std::vector<CommBreak> comm_breaks;
            comm_breaks.push_back(cb);
            m_comm_breaks[cb.schedule_id] = comm_breaks;
        }

        provider->cache()->next();

    } while(!provider->cache()->isLast());

}


void MainWindow::set_header_item(OATS::ScheduleItem* header, int hr, QDate dt)
{
    //header->set_schedule_type(OATS::ScheduleType::HOUR_HEADER);
    header->set_schedule_type("HOUR_HEADER");

    auto audio = std::make_shared<AUDIO::Audio>();
    audio->set_title("HOUR: "+std::to_string(hr)+" Header "+dt.toString("dd/MM/yyyy").toStdString());
    audio->set_duration(0);
    header->set_transition_type(OATS::TransitionType::STOP);
    header->set_schedule_ref(s_sched_ref++);
    header->set_schedule_date(dt);
    header->set_hour(hr);
    header->set_audio(std::move(audio));
    header->set_play_channel("X");
}

void MainWindow::set_schedule_fields(BaseDataProvider* provider,
                                     OATS::ScheduleItem* schedule)
{
    std::shared_ptr<AUDIO::Audio> audio = std::make_shared<AUDIO::Audio>();

    auto it_begin = provider->cache()->currentElement()->begin();
    auto it_end = provider->cache()->currentElement()->end();

    for (; it_begin != it_end; ++it_begin){

        std::string field_name = (*it_begin).first;
        std::string field_value = (*it_begin).second;

        if (field_name == "id"){
            schedule->set_id(str_to_int(field_value));
        }

        schedule->set_schedule_ref(s_sched_ref++);
        /* ********* */

        if (field_name == "live_transition"){
            schedule->set_item_status(schedule->str_to_status(field_value));
        }

        schedule->set_play_channel(play_channel());

        if (field_name == "schedule_item_type"){
            schedule->set_schedule_type(field_value);
        }

        if (field_name == "schedule_hour")
            schedule->set_hour(str_to_int(field_value));

        if (field_name == "schedule_time")
            schedule->set_schedule_time(QTime::fromString(QString::fromStdString(field_value), "hh:mm:ss"));

        if (field_name == "schedule_date"){
            schedule->set_schedule_date(QDate::fromString(QString::fromStdString(field_value), "yyyy-MM-dd"));
        }

//        if (field_name == "auto_transition")
//            schedule->set_auto_transition(str_to_int(field_value));


        if (field_name == "play_date"){
            schedule->set_play_date(QDate::fromString(QString::fromStdString(field_value), "yyyy/MM/dd"));
        }

        if (field_name == "play_time")
            schedule->set_play_time(QTime::fromString(QString::fromStdString(field_value), "hh:mm:ss"));

        if (field_name == "comment")
            schedule->set_comment(field_value);

        if (field_name == "booked_spots")
            schedule->set_booked_spots(str_to_int(field_value));

        if (field_name == "duration"){
            audio->set_duration(str_to_int(field_value));
            //schedule->set_break_duration(str_to_int(field_value));
        }

        if (field_name == "audio_id"){
            //schedule->set_audio(str_to_int(field_value));
            if (!field_value.empty())
                audio->setId(str_to_int(field_value));
        }

        if (field_name == "title")
            audio->set_title(field_value);

        if (field_name ==  "filepath")
            audio->set_file_path(field_value);

//        if (field_name == "fullname")
//            audio.set_artist(field_value);

        if (field_name == "artist_id")
            if(!field_value.empty())
                audio->set_artist(std::stoi(field_value));

        if (field_name == "file_extension")
            audio->set_file_extension(field_value);

        if (field_name == "audio_type"){
            if (!field_value.empty())
                audio->set_audio_type(field_value);
        }

    }

    schedule->set_audio(std::move(audio));
}

void MainWindow::fetch_temp_data(int hr)
{
}

void MainWindow::make_playlist_grid()
{
    ui->vlPlaylist->setSpacing(1);
    ui->vlPlaylist->setContentsMargins(0,0,0,2);

    std::string ch = ChannelA;

    for (int i=0; i < MAX_GRID_ITEMS; ++i)
    {
        auto si = schedule_item(i);

        if (si->schedule_type() != OATS::ScheduleType::HOUR_HEADER){
            si->set_play_channel(ch);
            ch = (ch == ChannelA) ? ChannelB : ChannelA;
        }

        auto grid_item = std::make_unique<OATS::ScheduleGridItem>(si);
        si->notify();;

        connect(grid_item.get(), &OATS::ScheduleGridItem::move_up, this, &MainWindow::item_move_up);
        connect(grid_item.get(), &OATS::ScheduleGridItem::move_down, this, &MainWindow::item_move_down);
        connect(grid_item.get(), &OATS::ScheduleGridItem::make_current, this, &MainWindow::make_item_current);
        connect(grid_item.get(), &OATS::ScheduleGridItem::insert_item, this, &MainWindow::grid_clicked);
        connect(grid_item.get(), &OATS::ScheduleGridItem::delete_item, this, &MainWindow::delete_schedule_item);
        connect(grid_item.get(), &OATS::ScheduleGridItem::reload_schedule, this, &MainWindow::reload_schedule);

        connect(grid_item.get(), &OATS::ScheduleGridItem::transition_stop, this, &MainWindow::transition_stop);
        connect(grid_item.get(), &OATS::ScheduleGridItem::transition_mix, this, &MainWindow::transition_mix);
        connect(grid_item.get(), &OATS::ScheduleGridItem::transition_cut, this, &MainWindow::transition_cut);

        ui->vlPlaylist->addWidget(grid_item.get());
        m_schedule_grid.push_back(std::move(grid_item));

    }

    //ui->vlPlaylist->insertStretch(-1,1);

    if (m_schedule_items.size() > 0){
        ui->gridScroll->setMaximum(m_schedule_items.size() - MAX_GRID_ITEMS );
    } else {
        ui->gridScroll->setMaximum(MAX_GRID_ITEMS);
    }

    //ui->gridScroll->setPageStep(MAX_GRID_ITEMS/2);
    ui->gridScroll->setPageStep(1);

    //go_current_hour();

    m_scrollbar_current_value = ui->gridScroll->value();

    //connect(ui->gridScroll, &QScrollBar::valueChanged, this, &MainWindow::scroll_changed);
}



OATS::ScheduleItem* MainWindow::schedule_item(int index)
{
    if (index <= m_schedule_items.size()-1){
        return m_schedule_items[index].get();
    } else {
        return nullptr;
    }

}

std::vector<std::shared_ptr<OATS::ScheduleItem>>& MainWindow::get_schedule_item(int hour)
{
    return m_day_schedule_items[hour];
}

std::string MainWindow::play_channel()
{
    return (s_channel == ChannelA ) ? ChannelB : ChannelA ;
}

void MainWindow::make_output_panel()
{
    m_outputA = create_output_panel("A");
    m_outputB = create_output_panel("B");
    m_outputC = std::make_unique<OATS::OutputPanel>("C");
    ui->hlOutput->addWidget(m_outputC.get());
}

OATS::OutputPanel* MainWindow::create_output_panel(const QString panel_name)
{
    auto panel = std::make_unique<OATS::OutputPanel>(panel_name);
    panel->set_id(m_output_panels.size());
    ui->hlOutput->addWidget(panel.get());

    connect(panel.get(), &OATS::OutputPanel::play_item, this, &MainWindow::play_button);
    connect(panel.get(), &OATS::OutputPanel::stop_play, this, &MainWindow::stop_button);

    auto rPtr = panel.get();
    m_output_panels.push_back(std::move(panel));

    return rPtr;
}

void MainWindow::play_button(OATS::OutputPanel* op)
{
    print_output_status();

    op->set_fade_trigger_tick_stamp(1);
    play_audio(op);
}

void MainWindow::stop_button(OATS::OutputPanel* op)
{
    if (op->panel_status() == OATS::PanelStatus::PLAYING){
        op->schedule_item()->set_transition_type(OATS::TransitionType::STOP);
        stop_audio(op);

        for(auto const& op : m_output_panels){
            op->set_start_trigger_tick_stamp(-1);
            op->set_fade_trigger_tick_stamp(-1);
        }
        QString audio_name = op->schedule_item()->audio()->title()->to_qstring();
        QString filename = op->schedule_item()->audio()->full_audio_filename();
        qInfo() << "Manual Stop: OP: "<< op->panel_name() << ": Audio: "<< audio_name << "-"<< filename;

        calculate_trigger_times();
    }
}


void MainWindow::make_play_mode_panel()
{
    m_play_mode_panel = std::make_unique<OATS::PlayModePanel>(this);
    ui->hlPlayMode->addWidget(m_play_mode_panel.get());
}

void MainWindow::make_comm_viewer_widget()
{
    m_comm_viewer = std::make_unique<DATA::CommercialViewer>(this);
    ui->vlCommViewer->addWidget(m_comm_viewer.get());
}

void MainWindow::make_track_info_widget()
{
    m_track_info = std::make_unique<TrackInfo>(this);
    QVBoxLayout* vlTrackInfo = new QVBoxLayout();
    vlTrackInfo->addWidget(m_track_info.get());
    ui->pgTrackInfo->setLayout(vlTrackInfo);
}

void MainWindow::make_jingle_grid_widget()
{
    m_jingle_grid = std::make_unique<OATS::JingleGrid>();

    ui->vlJingle->addWidget(m_jingle_grid.get());
}


void MainWindow::scroll_down()
{
    std::rotate(m_schedule_items.begin(), m_schedule_items.begin()+1, m_schedule_items.end());
}

void MainWindow::scroll_up()
{
    std::rotate(m_schedule_items.rbegin(), m_schedule_items.rbegin()+1, m_schedule_items.rend());
}


void MainWindow::display_schedule(int grid_pos, int from_item_index)
{

    std::string ch = "";
    for (int index=grid_pos; index <= MAX_GRID_ITEMS-1; ++index)
    {

        auto schedule = schedule_item(from_item_index);

        if (schedule->audio()->title()->value() == "")
            continue;

        QTime schedule_time = schedule_time_at(from_item_index);

        schedule->set_schedule_time(schedule_time);

        if (ch.empty()){
            ch = schedule->play_channel();
        }

        ch = (ch == ChannelA) ? ChannelB : ChannelA;
        schedule->set_play_channel(ch);

        m_schedule_grid[index]->set_subject(schedule);

        ++from_item_index;

    }

}



void MainWindow::reprint_schedule(int from_pos)
{
    for(int i=from_pos; i<MAX_GRID_ITEMS; ++i){
        auto schedule = schedule_item(i);
        if (schedule != nullptr)
            m_schedule_grid[i]->set_subject(schedule);
    }
}

void MainWindow::recompute_time(int from_pos)
{
    QTime new_time;
    QTime prev_schedule_time;

    std::string play_channel{""};

    if (from_pos > 0){
        play_channel = m_schedule_grid[from_pos-1]->schedule_item()->play_channel();
    }else{
        play_channel = m_schedule_grid[from_pos]->schedule_item()->play_channel();
    }

    for(int i=from_pos; i<MAX_GRID_ITEMS-1; ++i)
    {
        if (i > 0)
        {

            auto prev_schedule_item = schedule_item(i-1);
            //auto prev_schedule_item = m_schedule_grid[i-1]->schedule_item();

            prev_schedule_time  = prev_schedule_item->schedule_time();

            auto current_schedule = schedule_item(i);
            //auto current_schedule = m_schedule_grid[i]->schedule_item();

            if (current_schedule->schedule_type() == OATS::ScheduleType::COMM)
                continue;


            if (prev_schedule_item->schedule_type() != OATS::ScheduleType::HOUR_HEADER){
                new_time = prev_schedule_time.addMSecs(prev_schedule_item->audio()->duration()->value());
            }else{
                new_time = current_schedule->schedule_time();
            }

            play_channel = (play_channel == ChannelA) ? ChannelB : ChannelA;
            current_schedule->set_play_channel(play_channel);
            current_schedule->set_schedule_time(new_time);
            m_schedule_grid[i]->schedule_item()->set_schedule_time(new_time);
            m_schedule_grid[i]->set_subject(current_schedule);
        }
    }
}

QTime MainWindow::schedule_time_at(int index)
{
    QTime new_time;

    auto curr_schedule_item = schedule_item(index);

    if (index == 0)
        return curr_schedule_item->schedule_time();

    auto prev_schedule_item = schedule_item(index-1);

    if (prev_schedule_item->schedule_type() != OATS::ScheduleType::HOUR_HEADER){
        new_time = prev_schedule_item->schedule_time().addMSecs(prev_schedule_item->audio()->duration()->value());
    }else{
        new_time = curr_schedule_item->schedule_time();
    }

    return new_time;

}


void MainWindow::scroll_changed(int new_pos)
{
    if (new_pos > m_scrollbar_current_value){
        scroll_down();
    }else{
        scroll_up();
    }
    m_scrollbar_current_value = new_pos;
    display_schedule();
}

void MainWindow::go_current()
{
    if (m_current_playing_item.grid_index > 0)
    {
       std::rotate(m_schedule_items.begin(), m_schedule_items.begin()+m_current_playing_item.grid_index, m_schedule_items.end());
       display_schedule();
       m_current_playing_item.grid_index = 0;
       //ui->gridScroll->setValue(m_current_playing_item.grid_index);
    }
}

void MainWindow::keep_current(bool checked)
{
    if (checked)
        go_current();
}

void MainWindow::play_jingle(OATS::Jingle* jingle)
{

    m_current_jingle_item->set_schedule_date(QDate::currentDate());
    m_current_jingle_item->set_hour(QTime::currentTime().hour());
    m_current_jingle_item->set_schedule_type("JINGLE");
    m_current_jingle_item->set_transition_type(OATS::TransitionType::STOP);
    m_current_jingle_item->set_id(-1);
    m_current_jingle_item->set_schedule_ref(999);
    m_current_jingle_item->set_comment("JINGLE-PLAY");
    m_current_jingle_item->set_schedule_time(QTime::currentTime());

    m_current_jingle_item->set_audio(jingle->audio());
    m_current_jingle_item->set_play_channel("C");

    cue_schedule_item(m_current_jingle_item.get(), m_outputC.get());

    play_outputC(m_outputC.get());

//    m_jingle_player->play_audio(JinglePlayoutChannel, QString::fromStdString(audio_filename));

}

void MainWindow::stop_all_jingles()
{
    m_jingle_player->stop_play();
}

void MainWindow::move_top_current_hour()
{
    int index = 0;

    for (const auto& s_item : m_schedule_items){
        if (s_item->hour() == QDateTime::currentDateTime().time().hour()){
            break;
        }
        ++index;
    }

    if (index > MAX_GRID_ITEMS){
        ui->gridScroll->setValue((ui->gridScroll->maximum() - (ui->gridScroll->maximum()-index)));
    }else{
        ui->gridScroll->setValue(index+2);
    }

     std::rotate(m_schedule_items.begin(), m_schedule_items.begin()+index, m_schedule_items.end());
     display_schedule();
}

void MainWindow::set_current_play_item()
{
    if (m_schedule_items.size() > 0)
    {
        auto s_item = schedule_item(0);

        if (s_item == nullptr)
            return;

        if (s_item->schedule_type() == OATS::ScheduleType::HOUR_HEADER)
            return;

        m_current_playing_item.item = s_item;
        m_current_playing_item.schedule_index = s_item->index();
        m_current_playing_item.grid_index = 0;

        make_item_current(s_item->schedule_ref(), m_current_playing_item.grid_index);
    }
}


void MainWindow::time_updated()
{
    if (m_play_mode_panel->keep_current_checked())
        go_current();

}

void MainWindow::persist_cache()
{
    // Starts a thread that writes cache to database
    m_cache_updater->start();
}

void MainWindow::slow_flash()
{
    auto TEN_SECONDS  = 10000ms;
    auto FIVE_SECONDS =  5000ms;

    auto current_time = m_audio_tool.get_tick_count();

    // Output A

    if (m_outputA->panel_status() == OATS::PanelStatus::CUED ){

        auto tick_count_diff_OPA = m_outputA->start_trigger_tick_stamp() - current_time;
        if ( tick_count_diff_OPA < TEN_SECONDS.count() && tick_count_diff_OPA > FIVE_SECONDS.count() ) {
            m_outputA->slow_flash_play_button();
        }
    } else if (m_outputA->panel_status() == OATS::PanelStatus::PLAYING) {

        if ((m_outputA->time_remaining() < TEN_SECONDS.count()) &&
            (m_outputA->time_remaining() > FIVE_SECONDS.count()) ){
            m_outputA->slow_flash_stop_button();
        }
    }

    // Output B

    if (m_outputB->panel_status() == OATS::PanelStatus::CUED){
        auto tick_count_diff_OPB = m_outputB->start_trigger_tick_stamp() - current_time;
        if ( tick_count_diff_OPB < TEN_SECONDS.count() && tick_count_diff_OPB > FIVE_SECONDS.count() ){
            m_outputB->slow_flash_play_button();
        }
    } else if (m_outputB->panel_status() == OATS::PanelStatus::PLAYING){
        if ((m_outputB->time_remaining() < TEN_SECONDS.count()) &&
            (m_outputB->time_remaining() > FIVE_SECONDS.count()) ){
            m_outputB->slow_flash_stop_button();
        }
    }

    // Output C

    if (m_outputC->panel_status() == OATS::PanelStatus::CUED) {
        auto tick_count_diff_OPC = m_outputC->start_trigger_tick_stamp() - current_time;
        if (tick_count_diff_OPC < TEN_SECONDS.count() &&
                tick_count_diff_OPC > FIVE_SECONDS.count()) {
            m_outputC->slow_flash_play_button();
        }
        else if (m_outputC->panel_status() == OATS::PanelStatus::PLAYING) {
            if ((m_outputC->time_remaining() < TEN_SECONDS.count()) &&
               (m_outputC->time_remaining() > FIVE_SECONDS.count()) ) {
               m_outputC->slow_flash_stop_button();
            }

        }

    }

}

void MainWindow::fast_flash()
{
    auto FIVE_SECONDS = 5000ms;

    auto current_time = m_audio_tool.get_tick_count();

    // Output A
    if (m_outputA->panel_status() == OATS::PanelStatus::CUED) {
        auto tick_count_diff_OPA = m_outputA->start_trigger_tick_stamp() - current_time;
        if ( (tick_count_diff_OPA < FIVE_SECONDS.count()) && (tick_count_diff_OPA > 0) ) {
            m_outputA->fast_flash_play_button(OATS::ButtonFlashColor::GREEN);
        }
    } else {
        if (m_outputA->panel_status() == OATS::PanelStatus::PLAYING){
            if ( (m_outputA->time_remaining() < FIVE_SECONDS.count() ) &&
                 ( m_outputA->time_remaining() > 0 ) ) {
                m_outputA->fast_flash_stop_button();
            }
        }
    }

    // Output B
    if (m_outputB->panel_status() == OATS::PanelStatus::CUED){
        auto tick_count_diff_OPB = m_outputB->start_trigger_tick_stamp() - current_time;
        if ( (tick_count_diff_OPB < FIVE_SECONDS.count()) && (tick_count_diff_OPB > 0) ){
            m_outputB->fast_flash_play_button(OATS::ButtonFlashColor::GREEN);
        }
    } else {
        if (m_outputB->panel_status() == OATS::PanelStatus::PLAYING){
            if ((m_outputB->time_remaining() < FIVE_SECONDS.count()) &&
                (m_outputB->time_remaining() > 0) ){
                m_outputB->fast_flash_stop_button();
            }
        }
    }

    // Output C
    if (m_outputC->panel_status() == OATS::PanelStatus::CUED){
        auto tick_count_diff_OPC = m_outputC->start_trigger_tick_stamp() - current_time;
        if ((tick_count_diff_OPC < FIVE_SECONDS.count()) &&
            (tick_count_diff_OPC > 0) ){
            m_outputC->fast_flash_play_button(OATS::ButtonFlashColor::GREEN);
        }
        else {
            if (m_outputC->panel_status() == OATS::PanelStatus::PLAYING){
                if ((m_outputC->time_remaining() < FIVE_SECONDS.count()) &&
                    (m_outputC->time_remaining() > 0)){
                    m_outputC->fast_flash_stop_button();
                }
            }
        }


    }
}

void MainWindow::count_down()
{
    auto trigger_ticker = m_audio_tool.get_tick_count();

    switch (m_outputA->panel_status())
    {
    case OATS::PanelStatus::WAITING:
        break;
    case OATS::PanelStatus::CUED:
        break;

    case OATS::PanelStatus::PLAYING: {
        long long elapsed = trigger_ticker - m_outputA->start_tick_stamp();
        long long remaining = m_outputA->schedule_item()->audio()->duration()->value() - elapsed;

        if ( elapsed < m_outputA->schedule_item()->audio()->intro_marker()->value() ){
            // intro is still in progress
            auto duration_seconds = (int)(m_outputA->schedule_item()->audio()->intro_marker()->value() - elapsed)/100;
            int ds_u1 = (int)duration_seconds/10;
            int ds_u2 = duration_seconds % 10;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputA->set_cue_time_string(cue_string);
            m_outputA->set_progress_bar_background(OATS::ProgressBarBGColor::RED);

            m_outputA->update_progress_bar( 100 - round(( ((float)(m_outputA->schedule_item()->audio()->intro_marker()->value() - elapsed ) /
                                     m_outputA->schedule_item()->audio()->intro_marker()->value()) * 100 )
                    ) );
        } else if ( remaining < m_outputA->schedule_item()->audio()->fade_out_marker()->value()) {
            int duration_seconds = (int)remaining / 1000;
            int ds_u1 = (int)duration_seconds / 60;
            int ds_u2 = duration_seconds % 60;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputA->set_cue_time_string(cue_string);
            m_outputA->set_progress_bar_background(OATS::ProgressBarBGColor::GREEN);

            if (m_outputA->schedule_item()->audio()->fade_out_marker()->value() > 0) {
                m_outputA->update_progress_bar(
                            100 - round( ((float)m_outputA->time_remaining() /
                                          m_outputA->schedule_item()->audio()->fade_out_marker()->value()
                                          ) * 100 )
                            );
            }
        } else {
            int duration_seconds = (int)remaining / 1000;
            int ds_u1 = (int)duration_seconds / 60;
            int ds_u2 = duration_seconds % 60;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputA->set_cue_time_string(cue_string);

            m_outputA->set_progress_bar_background(OATS::ProgressBarBGColor::BLUE);

            /*
            // Chunck Color!!!!!
            if (m_outputA->schedule_item()->audio().fade_out() > 0) {
                m_outputA->set_progress_bar_background();
            } else {
                m_outputA->set_progress_bar_background();
            }
            */

            m_outputA->update_progress_bar(100 - round(  ((float)m_outputA->time_remaining() /
                                 (m_outputA->schedule_item()->audio()->duration()->value() -
                                  m_outputA->schedule_item()->audio()->intro_marker()->value()) ) * 100 )
                    );
        }
    }
    break;

    case OATS::PanelStatus::STOP:
        calculate_trigger_times();
        break;

    }

    // Output B
    switch (m_outputB->panel_status())
    {
     case OATS::PanelStatus::WAITING:
        break;
     case OATS::PanelStatus::CUED:
        break;
     case OATS::PanelStatus::PLAYING:
      {
        long long elapsed = trigger_ticker - m_outputB->start_tick_stamp();
        long long remaining = m_outputB->schedule_item()->audio()->duration()->value() - elapsed;

        if ( elapsed < m_outputB->schedule_item()->audio()->intro_marker()->value() ){
            // intro is still in progress
            auto duration_seconds = (int)(m_outputB->schedule_item()->audio()->intro_marker()->value() - elapsed)/100;
            int ds_u1 = (int)duration_seconds/10;
            int ds_u2 = duration_seconds % 10;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputB->set_cue_time_string(cue_string);
            m_outputB->set_progress_bar_background(OATS::ProgressBarBGColor::RED);

            m_outputB->update_progress_bar(
                        100 - round(((float)(m_outputB->schedule_item()->audio()->intro_marker()->value() - elapsed ) /
                                     m_outputB->schedule_item()->audio()->intro_marker()->value()) * 100 )
                    );
        } else if ( remaining < m_outputB->schedule_item()->audio()->fade_out_marker()->value()) {
            int duration_seconds = (int)remaining / 1000;
            int ds_u1 = (int)duration_seconds / 60;
            int ds_u2 = duration_seconds % 60;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputB->set_cue_time_string(cue_string);
            m_outputB->set_progress_bar_background(OATS::ProgressBarBGColor::GREEN);

            if (m_outputB->schedule_item()->audio()->fade_out_marker()->value() > 0) {
                m_outputB->update_progress_bar(
                            100 - round( ((float)m_outputB->time_remaining() /
                                          m_outputB->schedule_item()->audio()->fade_out_marker()->value()
                                          ) * 100 )
                            );
            }
        } else {
            int duration_seconds = (int)remaining / 1000;
            int ds_u1 = (int)duration_seconds / 60;
            int ds_u2 = duration_seconds % 60;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputB->set_cue_time_string(cue_string);

            m_outputB->set_progress_bar_background(OATS::ProgressBarBGColor::BLUE);

            // Chunck Color!!!!!
            /*
            if (m_outputA->schedule_item()->audio().fade_out() > 0) {
                m_outputA->set_progress_bar_background();
            } else {
                m_outputA->set_progress_bar_background();
            }
            */

            m_outputB->update_progress_bar(100 - round( ((float)m_outputB->time_remaining() /
                                      (m_outputB->schedule_item()->audio()->duration()->value() -
                                       m_outputB->schedule_item()->audio()->intro_marker()->value())
                                      ) * 100 ) );

        }
      }
      break;
    case OATS::PanelStatus::STOP:
        calculate_trigger_times();
        break;

    }

    switch (m_outputC->panel_status())
    {
    case OATS::PanelStatus::WAITING:
        break;
    case OATS::PanelStatus::CUED:
        break;
    case OATS::PanelStatus::PLAYING: {
        long long elapsed = trigger_ticker - m_outputC->start_tick_stamp();
        long long remaining = m_outputC->schedule_item()->audio()->duration()->value() - elapsed;

        if (elapsed < m_outputC->schedule_item()->audio()->intro_marker()->value() ) {
            // intro is still in progress
            auto duration_seconds = (int)(m_outputC->schedule_item()->audio()->intro_marker()->value() - elapsed)/100;
            int ds_u1 = (int)duration_seconds / 10;
            int ds_u2 = duration_seconds % 10;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                   .arg(ds_u2, 2, 10, QChar('0'));
            m_outputC->set_cue_time_string(cue_string);
            m_outputC->set_progress_bar_background(OATS::ProgressBarBGColor::RED);

            m_outputC->update_progress_bar( 100 - round(( ((float)(m_outputC->schedule_item()->audio()->intro_marker()->value() - elapsed ) /
                                     m_outputC->schedule_item()->audio()->intro_marker()->value()) * 100 )
                    ) );
        } else if ( remaining < m_outputC->schedule_item()->audio()->fade_out_marker()->value()) {
            int duration_seconds = (int)remaining / 1000;
            int ds_u1 = (int)duration_seconds / 60;
            int ds_u2 = duration_seconds % 60;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputC->set_cue_time_string(cue_string);
            m_outputC->set_progress_bar_background(OATS::ProgressBarBGColor::GREEN);

            if (m_outputC->schedule_item()->audio()->fade_out_marker()->value() > 0) {
                m_outputC->update_progress_bar(
                            100 - round( ((float)m_outputC->time_remaining() /
                                          m_outputC->schedule_item()->audio()->fade_out_marker()->value()
                                          ) * 100 )
                            );
            }
        } else {
            int duration_seconds = (int)remaining / 1000;
            int ds_u1 = (int)duration_seconds / 60;
            int ds_u2 = duration_seconds % 60;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputC->set_cue_time_string(cue_string);

            m_outputC->set_progress_bar_background(OATS::ProgressBarBGColor::BLUE);

            /*
            // Chunck Color!!!!!
            if (m_outputA->schedule_item()->audio().fade_out() > 0) {
                m_outputA->set_progress_bar_background();
            } else {
                m_outputA->set_progress_bar_background();
            }
            */

            m_outputC->update_progress_bar(100 - round(  ((float)m_outputC->time_remaining() /
                                 (m_outputC->schedule_item()->audio()->duration()->value() -
                                  m_outputC->schedule_item()->audio()->intro_marker()->value()) ) * 100 )
                    );
        }
    }
    break;

    case OATS::PanelStatus::STOP:
        calculate_trigger_times();
        break;


        }

}

void MainWindow::status_timer()
{
    int elapsed{0};
    int remaining{0};
    auto trigger_ticker = m_audio_tool.get_tick_count();

    switch (m_outputA->panel_status())
    {
     case OATS::PanelStatus::STOP:
        m_outputA->set_start_trigger_tick_stamp(-1);
        break;
     case OATS::PanelStatus::WAITING:
        m_outputA->set_start_trigger_tick_stamp(-1);
        break;
     case OATS::PanelStatus::CUED:
        if ( (m_outputA->start_trigger_tick_stamp() > -1) &&
             (m_outputA->start_trigger_tick_stamp() <= (trigger_ticker)))
        {
            if (m_outputB->panel_status() == OATS::PanelStatus::PLAYING)
                stop_audio(m_outputB);

            play_audio(m_outputA);
        }
        // Do some outputC stuff
        break;

    case OATS::PanelStatus::PLAYING:
            //fade_audio(m_outputA);
          if (m_outputA->start_tick_stamp() > 0){

            elapsed = trigger_ticker - m_outputA->start_tick_stamp();

            remaining = m_outputA->schedule_item()->audio()->duration()->value() - elapsed;
            m_outputA->set_time_remaining(remaining);

            if ( (remaining <= 0) || (m_outputA->start_tick_stamp() < 0) ){
                stop_audio(m_outputA);
            }

          }
    }

    // Output B

    switch(m_outputB->panel_status())
    {
     case OATS::PanelStatus::STOP:
        m_outputB->set_start_trigger_tick_stamp(-1);
        break;
     case OATS::PanelStatus::WAITING:
        m_outputB->set_start_trigger_tick_stamp(-1);
        break;
     case OATS::PanelStatus::CUED:
        if ((m_outputB->start_trigger_tick_stamp() > -1 ) &&
            (m_outputB->start_trigger_tick_stamp() <= (trigger_ticker)))
        {
            if (m_outputA->panel_status() == OATS::PanelStatus::PLAYING)
                stop_audio(m_outputA);

            play_audio(m_outputB);
        }
        break;

     case OATS::PanelStatus::PLAYING:
         if (m_outputB->start_tick_stamp() > 0)
         {
            elapsed = trigger_ticker - m_outputB->start_tick_stamp();
            remaining = m_outputB->schedule_item()->audio()->duration()->value()-elapsed;
            m_outputB->set_time_remaining(remaining);

            if ((remaining <= 0) || (m_outputB->start_tick_stamp() < 0))
            {
                stop_audio(m_outputB);
            }
         }
    }


    // Output C

    switch(m_outputC->panel_status())
    {
       case OATS::PanelStatus::STOP:
        m_outputC->set_start_trigger_tick_stamp(-1);
        break;
       case OATS::PanelStatus::WAITING:
        m_outputC->set_start_trigger_tick_stamp(-1);
        break;
       case OATS::PanelStatus::CUED:
        break;
       case OATS::PanelStatus::PLAYING:
        if (m_outputC->start_tick_stamp() > 0) {
            elapsed = trigger_ticker - m_outputC->start_tick_stamp();
            remaining = m_outputC->schedule_item()->audio()->duration()->value() - elapsed;
            m_outputC->set_time_remaining(remaining);

            if ((remaining <= 0 ) || (m_outputC->start_tick_stamp() < 0)){
                stop_audio(m_outputC.get());
            }

        }

    }

}

void MainWindow::on_calc_time_stats()
{
    calculate_time_stats();
}


int MainWindow::index_of(int item_ref)
{
    auto it = std::find_if(m_schedule_items.begin(), m_schedule_items.end(),
                           FindByRef(item_ref));
    if (it != m_schedule_items.end()){
        return std::distance(m_schedule_items.begin(), it);
    }else{
        return -1;
    }
}

int MainWindow::next_output_panel_id(int current_panel_id)
{
    return ((current_panel_id+1) <= m_output_panels.size()-1 ? ++current_panel_id : --current_panel_id);
}

OATS::OutputPanel* MainWindow::find_output_panel(int panel_id)
{
    for (auto const& panel : m_output_panels){
        if (panel->id() == panel_id)
            return panel.get();
    }

    return nullptr;
}

OATS::ScheduleItem* MainWindow::find_next_schedule_item(OATS::ScheduleItem* current_item)
{
    auto index = index_of(current_item->schedule_ref());

    auto next_schedule_item = current_item;

    for(int i=index; i < m_schedule_items.size()-1; ++i){
        auto next_item = schedule_item(i+1);

        // Skip items with errors
        if (next_item->item_status() == OATS::ItemStatus::ERROR_01)
            continue;

        // Skip hour headers
        if (next_item->schedule_type()== OATS::ScheduleType::HOUR_HEADER)
            continue;

        next_schedule_item = next_item;
        break;
    }

    return next_schedule_item;
}

void MainWindow::cue_commercial_item(OATS::ScheduleItem* next_schedule_item,
                                     OATS::OutputPanel* next_output_panel)
{

    //set_commercial_items_filepath(next_schedule_item->id());

    next_schedule_item->set_item_status(OATS::ItemStatus::CUED);
    next_output_panel->cue_item(next_schedule_item);
    next_output_panel->set_panel_status(OATS::PanelStatus::CUED);

    m_current_cued_item.item = next_schedule_item;

    auto comm_duration = get_comm_duration(next_schedule_item->id());

    auto duration_seconds = (int)comm_duration/1000;
    auto intro_seconds = 0;
    auto fade_seconds = 0;

    int is_u1 = (int)intro_seconds/60;
    int is_u2 = intro_seconds % 60;

    int ds_u1 = (int)duration_seconds/60;
    int ds_u2 = duration_seconds % 60;

    int fs_u1 = (int)fade_seconds/60;
    int fs_u2 = fade_seconds % 60;

    QString cue_string = QString("%1:%2 / %3:%4 / %5:%6").arg(is_u1, 2, 10, QChar('0'))
                                                         .arg(is_u2, 2, 10, QChar('0'))
                                                         .arg(ds_u1, 2, 10, QChar('0'))
                                                         .arg(ds_u2, 2, 10, QChar('0'))
                                                         .arg(fs_u1, 2, 10, QChar('0'))
                                                         .arg(fs_u2, 2, 10, QChar('0'));
    next_output_panel->set_cue_time_string(cue_string);

    next_output_panel->update_progress_bar(0);

    if (next_schedule_item->audio()->intro_marker()->value() > 0) {
        next_output_panel->set_progress_bar_background(OATS::ProgressBarBGColor::RED);
    } else {
        next_output_panel->set_progress_bar_background(OATS::ProgressBarBGColor::BLUE);
    }

    calculate_trigger_times();

    // TODO: Cache all commercial audio items
//    queue_for_caching(next_schedule_item->audio());

}

void MainWindow::cue_schedule_item(OATS::ScheduleItem* next_schedule_item, OATS::OutputPanel* next_output_panel)
{

    set_scheduled_item_filepath(next_schedule_item);

    next_schedule_item->set_item_status(OATS::ItemStatus::CUED);
    next_output_panel->cue_item(next_schedule_item);
    next_output_panel->set_panel_status(OATS::PanelStatus::CUED);

    m_current_cued_item.item = next_schedule_item;

    auto duration_seconds = (int)next_schedule_item->audio()->duration()->value()/1000;
    auto intro_seconds = (int)next_schedule_item->audio()->intro_marker()->value()/1000;
    auto fade_seconds = (int)next_schedule_item->audio()->fade_delay_marker()->value()/1000;

    int is_u1 = (int)intro_seconds/60;
    int is_u2 = intro_seconds % 60;

    int ds_u1 = (int)duration_seconds/60;
    int ds_u2 = duration_seconds % 60;

    int fs_u1 = (int)fade_seconds/60;
    int fs_u2 = fade_seconds % 60;

    QString cue_string = QString("%1:%2 / %3:%4 / %5:%6").arg(is_u1, 2, 10, QChar('0'))
                                                         .arg(is_u2, 2, 10, QChar('0'))
                                                         .arg(ds_u1, 2, 10, QChar('0'))
                                                         .arg(ds_u2, 2, 10, QChar('0'))
                                                         .arg(fs_u1, 2, 10, QChar('0'))
                                                         .arg(fs_u2, 2, 10, QChar('0'));
    next_output_panel->set_cue_time_string(cue_string);

    next_output_panel->update_progress_bar(0);

    if (next_schedule_item->audio()->intro_marker()->value() > 0) {
        next_output_panel->set_progress_bar_background(OATS::ProgressBarBGColor::RED);
    } else {
        next_output_panel->set_progress_bar_background(OATS::ProgressBarBGColor::BLUE);
    }

    calculate_trigger_times();

    queue_for_caching(next_schedule_item->audio());

}

/*
void MainWindow::set_commercial_items_filepath(int schedule_id)
{
    auto comm_breaks = m_comm_breaks[schedule_id];
    for (auto& comm_break: comm_breaks){
        qDebug() << "Comm Break: "<< comm_break.filepath;
        std::string cached_audio_path =
            m_audio_cache_manager->get_cached_audio_path(comm_break.audio_id);

        // point the scheduled item filepath to audio cache filepath
        if (!cached_audio_path.empty())
            comm_break.filepath = QString::fromStdString(cached_audio_path);
    }

}
*/

void MainWindow::set_scheduled_item_filepath(OATS::ScheduleItem* schedule_item)
{
    std::string cache_audio_filepath =
            m_audio_cache_manager->get_cached_audio_path(schedule_item->audio()->id());

    // point the scheduled item filepath to audio cache filepath
    if (!cache_audio_filepath.empty())
        schedule_item->audio()->set_file_path(cache_audio_filepath);
}
void MainWindow::set_cache_last_play_dtime(OATS::ScheduleItem* schedule_item)
{
    m_audio_cache_manager->set_cache_audio_last_play_dtime(schedule_item->audio()->id());
}


int MainWindow::calculate_yield_contribution(OATS::ScheduleItem* item)
{
    return 0;
}

void MainWindow::calculate_trigger_times()
{
    assert(m_outputA != nullptr);
    assert(m_outputB != nullptr);

    if (m_current_playing_item.item == nullptr)
        return;

    long long current_play_start_time{0};

    if (m_current_playing_item.item->item_status() == OATS::ItemStatus::PLAYING)
    {
        if (m_current_playing_item.item->play_channel() == "A" ) {
            current_play_start_time = m_outputA->start_tick_stamp();
        }else{
            current_play_start_time = m_outputB->start_tick_stamp();
        }

        // Check if current playing item need to contribute to the Time Mark Yield
        int time_yield = calculate_yield_contribution(m_current_playing_item.item);

        if (time_yield > 0){

            if (time_yield > 500) {
                m_current_playing_item.item->audio()->set_duration(
                            (m_current_playing_item.item->audio()->duration()->value() - time_yield) +
                            (YIELD_FADE_DELAY - m_current_playing_item.item->audio()->fade_delay_marker()->value())
                            );
                m_current_playing_item.item->audio()->set_fade_delay_marker(YIELD_FADE_DELAY);
                m_current_playing_item.item->audio()->set_fade_out_marker(YIELD_FADE_OUT);
            }else{
                m_current_playing_item.item->audio()->set_duration(
                            m_current_playing_item.item->audio()->duration()->value() - time_yield
                            );

            }

        } // time_yield > 0

        compute_schedule_time();

        auto next_schedule_item = find_next_schedule_item(m_current_playing_item.item);

        long long drop_start_mark{-1};
        long long fade_start_mark{-1};
        long long track_start_mark{-1};
        int segue_interval{-1};

        if (next_schedule_item != nullptr){

            if (m_current_playing_item.item->transition_type() == OATS::TransitionType::STOP){
                drop_start_mark = -1;
                fade_start_mark = -1;
                track_start_mark= -1;

            }else if(m_current_playing_item.item->transition_type() == OATS::TransitionType::CUT){
                segue_interval = m_current_playing_item.item->audio()->extro_marker()->value() -
                                                             next_schedule_item->audio()->intro_marker()->value();

                track_start_mark = (current_play_start_time + m_current_playing_item.item->audio()->duration()->value());  // - 500;
                fade_start_mark = -1; // Don't fade

            } else if (m_current_playing_item.item->transition_type() == OATS::TransitionType::MIX){
                segue_interval = (m_current_playing_item.item->audio()->extro_marker()->value() +
                                  next_schedule_item->audio()->intro_marker()->value()) -
                                  m_current_playing_item.item->audio()->fade_delay_marker()->value();

                if (m_current_playing_item.item->audio()->fade_delay_marker()->value() >
                                                       (next_schedule_item->audio()->duration()->value() -
                                                        next_schedule_item->audio()->fade_delay_marker()->value() ) ){

                        m_current_playing_item.item->audio()->set_fade_delay_marker((next_schedule_item->audio()->duration()->value() -
                                                                     next_schedule_item->audio()->fade_delay_marker()->value()) ); // - 500);
                }

                track_start_mark = (current_play_start_time + m_current_playing_item.item->audio()->duration()->value()) -
                                   m_current_playing_item.item->audio()->fade_delay_marker()->value();

                fade_start_mark = (current_play_start_time + m_current_playing_item.item->audio()->duration()->value())-
                                   m_current_playing_item.item->audio()->fade_out_marker()->value();

              }

             // Do some drop item calculations
             drop_start_mark = -1;

            }


        if (m_current_playing_item.item->play_channel() == ChannelA){
            m_outputB->set_start_trigger_tick_stamp(track_start_mark);
            m_outputB->set_fade_trigger_tick_stamp(fade_start_mark);
        }else{
            m_outputA->set_start_trigger_tick_stamp(track_start_mark);
            m_outputA->set_fade_trigger_tick_stamp(fade_start_mark);
        }

    }
}

void MainWindow::calculate_time_stats()
{
    int min = QTime::currentTime().minute();
    int sec = QTime::currentTime().second();
    int msec = QTime::currentTime().msec();

    long hour_remaining = 3600000 - ((min * 60000) + (sec * 1000) + msec);

    long time_counter;

    if (m_current_playing_item.item == nullptr)
        return;

    if (m_current_playing_item.item->play_channel() == ChannelA){
        if (m_outputA->panel_status() == OATS::PanelStatus::PLAYING){
            time_counter = m_outputA->time_remaining();
        }else if (m_outputA->panel_status() == OATS::PanelStatus::PLAYED){
            time_counter = 0;
        } else{
            time_counter = m_current_playing_item.item->audio()->duration()->value();
        }
    }

    if (m_current_playing_item.item->play_channel() == ChannelB){
        if (m_outputB->panel_status() == OATS::PanelStatus::PLAYING){
            time_counter = m_outputB->time_remaining();
        }else if (m_outputB->panel_status() == OATS::PanelStatus::PLAYED) {
            time_counter = 0;
        } else {
            time_counter = m_current_playing_item.item->audio()->duration()->value();
        }
    }


    if (m_current_playing_item.item->schedule_type() != OATS::ScheduleType::HOUR_HEADER) {
        if (m_current_playing_item.item->transition_type() == OATS::TransitionType::MIX){
            time_counter -= m_current_playing_item.item->audio()->fade_delay_marker()->value();
        }
    }


    for (int i=m_current_playing_item.schedule_index+1; i <= m_schedule_items.size(); ++i)
    {
        if (m_schedule_items[i]->schedule_type()== OATS::ScheduleType::HOUR_HEADER)
            break;
        time_counter += m_schedule_items[i]->audio()->duration()->value();

        if (m_schedule_items[i]->transition_type() == OATS::TransitionType::MIX){
        time_counter -= m_schedule_items[i]->audio()->fade_delay_marker()->value();
        }

    }


    long music_remaining = time_counter;
    long talk_remaining = hour_remaining - music_remaining;

    music_remaining = (int)music_remaining / 1000;

    int m_r1 = (int)music_remaining / 60;
    int m_r2 = music_remaining % 60;

    hour_remaining = (int)hour_remaining / 1000;
    int h_r1 = (int)hour_remaining / 60;
    int h_r2 = hour_remaining % 60;

    talk_remaining = (int)talk_remaining / 1000;
    int t_r1 = (int)talk_remaining / 60;
    int t_r2 = talk_remaining % 60;


    QString music_remain_txt = QString("%1:%2")
                                   .arg( m_r1, 2, 10, QChar('0'))
                                   .arg(m_r2, 2, 10, QChar('0'));

    QString hour_remain_txt = QString("%1:%2")
                                  .arg(h_r1, 2, 10, QChar('0'))
                                  .arg(h_r2, 2, 10, QChar('0'));

    QString talk_remain_txt = QString("%1:%2")
                                  .arg(t_r1, 2, 10, QChar('0'))
                                  .arg(t_r2, 2, 10, QChar('0'));



    auto time_stats = std::make_tuple(music_remain_txt, hour_remain_txt, talk_remain_txt);
    m_tta_widget->update_stats(time_stats);


}

void MainWindow::play_audio(OATS::OutputPanel* op)
{
    if (op->panel_status() == OATS::PanelStatus::CUED)
    {
        op->set_panel_status(OATS::PanelStatus::PLAYING);
        op->schedule_item()->set_item_status(OATS::ItemStatus::PLAYING);
        op->schedule_item()->set_play_start_time(QTime::currentTime());

        op->set_start_tick_stamp(m_audio_tool.get_tick_count());

        op->schedule_item()->notify();

        if (op->schedule_item()->schedule_type() == OATS::ScheduleType::COMM)
        {

            auto break_items = m_comm_breaks[op->schedule_item()->id()];

            for(auto& item : break_items){
                auto audio_file = item.filepath+
                                  QString::fromStdString(m_audio_tool.make_audio_filename(item.audio_id))+
                                  "."+
                                  item.file_extension;

                qInfo() << "Play Comm: OP: "<< op->panel_name() << " :Audio: "<< audio_file;

                m_audio_player->append_playlist(op->panel_name(), audio_file);
            }

            m_audio_player->play_audio();

        }else{
            auto audio = op->schedule_item()->audio();

            //FIXME: full_audio_filename to read from cache
            std::string audio_filename = audio->full_audio_filename().toStdString();
            QString audio_file = QString::fromStdString(audio_filename);

            qInfo() << "Play Audio: OP: "<< op->panel_name() << " :Audio: " << audio_file;

            m_audio_player->play_audio(op->panel_name(), audio_file);
        }

        int grid_index = index_of(op->schedule_item()->schedule_ref());
        m_current_playing_item.item = op->schedule_item();
        m_current_playing_item.schedule_index = op->schedule_item()->index();
        m_current_playing_item.grid_index = grid_index;

        int next_id = next_output_panel_id(op->id());

        auto next_output_panel = find_output_panel(next_id);
        auto next_schedule_item = find_next_schedule_item(op->schedule_item());

        if (m_current_playing_item.item != next_schedule_item){

            if (next_schedule_item != nullptr){

                if (next_schedule_item->schedule_type() == OATS::ScheduleType::COMM){
                        cue_commercial_item(next_schedule_item, next_output_panel);
                }else{
                        cue_schedule_item(next_schedule_item, next_output_panel);
                }
            }

        }

        display_schedule();

    }

}

void MainWindow::stop_audio(OATS::OutputPanel* op)
{
    if (op->schedule_item()->item_status() == OATS::ItemStatus::PLAYING)
    {
        op->set_panel_status(OATS::PanelStatus::PLAYED);
        op->set_start_trigger_tick_stamp(-1);
        op->set_fade_trigger_tick_stamp(-1);

        op->schedule_item()->set_item_status(OATS::ItemStatus::PLAYED);
        op->schedule_item()->notify();

        set_cache_last_play_dtime(op->schedule_item());

        if (op->schedule_item()->schedule_type() == OATS::ScheduleType::COMM)
            mark_comm_as_played(op->schedule_item()->id());
    }

    m_audio_player->stop_play();

    //display_schedule(ui->gridScroll->value()+1);
    display_schedule();

    op->reset_play_button();
    op->reset_stop_button();

    op->update_progress_bar(100);
}

void MainWindow::mark_comm_as_played(int schedule_id)
{
    m_updatedb_mutex->try_lock();

    for (auto& comm_break : m_comm_breaks[schedule_id]){
        comm_break.break_played = true;
    }

    m_updatedb_mutex->unlock();

}

void MainWindow::db_update_comm_break_play_status()
{
    //print_comm_breaks();

    m_updatedb_mutex->try_lock();

    std::vector<int> break_ids;
    for(auto& [break_id, commercials]: m_comm_breaks){
        for(auto& comm: commercials){
            if (comm.break_played && !comm.db_persisted){
                break_ids.push_back(comm.booking_id);
            }
        }
    }

    if (break_ids.size() == 0)
        return;

    std::string ids = join(break_ids, ",");
    std::string where_filter = std::format(" Where id in ({})", ids);
    std::string booking_status =" booking_status = 'PLAYED', ";
    std::string play_date = std::format(" play_date = '{}', ",
                                        QDate::currentDate().toString("yyyy-MM-dd").toStdString());
    std::string play_time = std::format(" play_time = '{}' ",
                                        QTime::currentTime().toString("HH:mm").toStdString());

    std::ostringstream sql;

    sql << " Update rave_orderbooking set "
        << booking_status
        << play_date
        << play_time
        << where_filter;

    EntityDataModel edm;
    edm.executeRawSQL(sql.str());

    for (auto& b_id: break_ids){
        for(auto& comm : m_comm_breaks[b_id]){
            comm.db_persisted = true;
        }
    }

    m_updatedb_mutex->unlock();

    //print_comm_breaks();

}

void MainWindow::print_comm_breaks()
{
    for(auto&[break_id, commercials]: m_comm_breaks){
        if (commercials.size() == 0)
            continue;
        for(auto& comm : commercials){
            if (comm.break_played){
                qDebug() << "Break ID: " << break_id;
                qDebug() << "-------------------------------------------------";
                qDebug() << "Booking ID: "<< comm.booking_id;
                qDebug() << "Schedule ID: "<< comm.schedule_id;
                qDebug() << "Book Hour: "<< comm.book_hour;
                qDebug() << "Book Time: "<< comm.book_time.toString("HH:mm");
                qDebug() << "Spot ID: "<< comm.spot_id;
                qDebug() << "Book HR: "<< comm.book_hour;
                qDebug() << "Break Played: "<< QVariant(comm.break_played).toString();
                qDebug() << "DB Persisted: "<< QVariant(comm.db_persisted).toString();
                qDebug() << "\n";
            }
        }
    }

}



void MainWindow::play_outputC(OATS::OutputPanel* op)
{
    op->set_panel_status(OATS::PanelStatus::CUED);
    op->schedule_item()->set_item_status(OATS::ItemStatus::CUED);
    op->schedule_item()->set_play_start_time(QTime::currentTime());
    op->set_start_tick_stamp(m_audio_tool.get_tick_count());

//    op->schedule_item()->notify();

    play_audio(op);

//    m_jingle_player->play_audio(JinglePlayoutChannel,
//                                QString::fromStdString(audio_file));


}

void MainWindow::item_move_up(int schedule_ref, int grid_pos)
{
    if (grid_pos == 0)
        return;

    int index = index_of(schedule_ref);

    auto si = schedule_item(index);
    auto prev_si = schedule_item(index-1);

    if ((si->item_status() == OATS::ItemStatus::CUED) &&
        (prev_si->item_status() == OATS::ItemStatus::PLAYING)){

        QMessageBox::information(this, tr("OATS"),
                                 tr("Cannot move CUED item past a playing item."),
                                 QMessageBox::Ok);
        return;
    }

    std::iter_swap(m_schedule_items.begin()+index, m_schedule_items.begin()+(index-1));
    m_schedule_grid[grid_pos]->set_subject(schedule_item(index));
    m_schedule_grid[grid_pos-1]->set_subject(schedule_item(index-1));

    recompute_time(grid_pos);
}

void MainWindow::item_move_down(int schedule_ref, int grid_pos)
{
    if (grid_pos+1 > MAX_GRID_ITEMS-1)
        return;

    int index = index_of(schedule_ref);
    std::iter_swap(m_schedule_items.begin()+index, m_schedule_items.begin()+(index+1));

    m_schedule_grid[grid_pos]->set_subject(schedule_item(index));
    m_schedule_grid[grid_pos+1]->set_subject(schedule_item(index+1));
}

void MainWindow::make_item_current(int schedule_ref, int grid_pos)
{
    assert(m_outputA != nullptr);
    assert(m_outputB != nullptr);

    int index = index_of(schedule_ref);
    //auto si = schedule_item(index);

    auto si = m_schedule_grid[grid_pos]->schedule_item();

    if (si->item_status() == OATS::ItemStatus::PLAYING)
        return;

    if (si->item_status() == OATS::ItemStatus::ERROR_01)
        return;

    if ( grid_pos < m_current_playing_item.grid_index)
    {
         if (m_current_playing_item.item->item_status()==OATS::ItemStatus::PLAYING) {
            QMessageBox::information(this, tr("OATS"),
                         tr("Cannot schedule past playing item"),
                         QMessageBox::Ok);
            return;
         }
    }

    if ((m_current_cued_item.item != nullptr) &&
        (m_current_cued_item.item->item_status() != OATS::ItemStatus::PLAYING) )
    {
        m_current_cued_item.item->set_item_status(OATS::ItemStatus::WAITING);
        m_current_cued_item.item->notify();
    }

    set_scheduled_item_filepath(si);

    m_current_cued_item.item = si;

    QTime curr_time = QTime::currentTime();
    int item_duration = si->audio()->duration()->value();

    si->set_item_status(OATS::ItemStatus::CUED);
    si->set_schedule_time(curr_time);
    si->set_schedule_time(curr_time.addMSecs(item_duration));
    si->notify();
    m_schedule_grid[grid_pos]->set_subject(si);

    QString cue_string = output_string(si);

    if (si->play_channel() == ChannelA){
        m_outputA->set_panel_status(OATS::PanelStatus::CUED);
        m_outputA->cue_item(si);
        m_outputA->set_cue_time_string(cue_string);
    }else{
        m_outputB->set_panel_status(OATS::PanelStatus::CUED);
        m_outputB->cue_item(si);
        m_outputB->set_cue_time_string(cue_string);
    }

    //int gs_value = ui->gridScroll->value();
    //display_schedule();

}

void MainWindow::grid_clicked(int schedule_ref, int grid_pos)
{
    switch(m_control_page)
    {
      case ControlPage::Home:
        break;
      case ControlPage::Commercial:
        qDebug() << schedule_ref;
        show_commercial(schedule_ref);
        break;
      case ControlPage::Segue:
        qDebug() << "Segue page ...";
        break;
      case ControlPage::Cart:
        qDebug() << "Cart page ... ";
        break;
      case ControlPage::Jingle:
        qDebug() << "Jingle page ...";
        break;
      case ControlPage::TrackInfo:
        show_track_info(schedule_ref);
        break;
      case ControlPage::Load:
        load_item(schedule_ref, grid_pos);
        break;
     default:
        qDebug() << "None-existance page";
    }
}

void MainWindow::delete_schedule_item(int schedule_ref, int grid_pos)
{
    int index = index_of(schedule_ref);
    auto si = schedule_item(index);

    // We don't delete a played item!
    if (si->item_status()== OATS::ItemStatus::PLAYED)
        return;

    // We don't delete commercials and hour headers
    if (si->schedule_type()==OATS::ScheduleType::COMM ||
        si->schedule_type()==OATS::ScheduleType::HOUR_HEADER)
        return;

    auto it = std::find_if(m_schedule_items.begin(), m_schedule_items.end(),
                           [&](std::unique_ptr<OATS::ScheduleItem> const& item){return item->schedule_ref()==si->schedule_ref() ;});

    if (it == m_schedule_items.end())
        return;

    if (si->item_status()==OATS::ItemStatus::CUED){
        if (si->play_channel() == ChannelA)
            m_outputA->delete_cued_item();
        else
            m_outputB->delete_cued_item();
    }

    m_schedule_items.erase(it);

    display_schedule();

    ui->gridScroll->setSliderPosition(m_scrollbar_current_value);
    ui->gridScroll->setMaximum(m_schedule_items.size() - MAX_GRID_ITEMS);
}

void MainWindow::reload_schedule(int schedule_ref, int grid_pos)
{
//    int index = index_of(schedule_ref);
    qDebug() << "Items Size: "<< m_schedule_items.size();
    qDebug() << "Schedule Ref: "<< schedule_ref;
    qDebug() << "Grid Max: "<<ui->gridScroll->maximum();
    qDebug() << "Grid Scroll Value: "<< ui->gridScroll->value();

    auto si = schedule_item(schedule_ref);

    auto erased = std::erase_if(m_schedule_items, [&](const auto& item){ return (item->hour() >= si->hour()); });

    fetch_data_from_db(QDate::currentDate(), si->hour());

    //ui->gridScroll->setValue(ui->gridScroll->value());
    display_schedule(grid_pos, schedule_ref);

    for (int i=schedule_ref; i <= m_schedule_items.size()-1; ++i){
        qDebug() << "SREF: " << m_schedule_items[i]->schedule_ref();
    }

    qDebug() << "Schedule Ref: "<< schedule_ref;
    qDebug() << "Grid Pos: "<< grid_pos;

}


void MainWindow::load_item(int schedule_ref, int grid_pos)
{
    std::shared_ptr<AUDIO::Audio> audio = m_track_browser->current_selected_audio();

    if (audio == nullptr)
        return;

    qDebug() << ">>> Audio: " << audio->title()->to_qstring();

    //int index = index_of(schedule_ref);
    //auto item_at_cursor = schedule_item(grid_pos);
    auto item_at_cursor = m_schedule_grid[grid_pos]->schedule_item();

    auto new_item = std::make_unique<OATS::ScheduleItem>();

    new_item->set_hour(item_at_cursor->hour());
    new_item->set_schedule_date(item_at_cursor->schedule_date());

    new_item->set_schedule_type(audio->audio_type()->value());
    new_item->set_transition_type(OATS::TransitionType::MIX);
    new_item->set_id(-1);
    new_item->set_schedule_ref(s_sched_ref++);

    QTime new_time;
    if (item_at_cursor->schedule_type() == OATS::ScheduleType::HOUR_HEADER){
        new_time = QTime::fromString(QTime::currentTime().toString(), "hh:mm:ss");
    } else {
        new_time = item_at_cursor->schedule_time().addMSecs(item_at_cursor->audio()->duration()->value());
    }

    new_item->set_schedule_time(new_time);

    new_item->set_comment("MANUAL-INSERT");

    new_item->set_audio(audio);

    // check if audio exist
    QString filename = audio->full_audio_filename();

    if (!m_audio_tool.audio_exist(filename)){
        new_item->set_item_status(OATS::ItemStatus::ERROR_01);
        new_item->set_transition_type(OATS::TransitionType::SKIP);
    }

    auto play_channel = [&](){
        return ChannelA;

        auto prev_si = schedule_item(grid_pos);
        if (prev_si == nullptr)
            return ChannelA;

        return (prev_si->play_channel() == ChannelA) ? ChannelB : ChannelA;

        return ChannelA;
    };

    new_item->set_play_channel(play_channel());

    if (m_current_playing_item.item == nullptr)
        m_current_playing_item.item = new_item.get();

    auto insert_schedule_item = [&](OATS::ScheduleType s_type){
        int insert_pos = -1;
        if (s_type == OATS::ScheduleType::HOUR_HEADER){
            insert_pos = index_of(schedule_ref)+1;
        }else{
            insert_pos = index_of(schedule_ref);
        }


        auto it = m_schedule_items.begin()+insert_pos;
        m_schedule_items.insert(it,  std::move(new_item));

        ui->gridScroll->setSliderPosition(ui->gridScroll->value()+1);
    };

   if (new_item->transition_type() != OATS::TransitionType::SKIP){
     //  queue_for_caching(schedule_audio);
     //  queue_for_caching(audio);
   }

    insert_schedule_item(item_at_cursor->schedule_type());

    recompute_time(grid_pos);

    ui->gridScroll->setMinimum(0);
    ui->gridScroll->setMaximum(m_schedule_items.size() - MAX_GRID_ITEMS);

}

void MainWindow::queue_for_caching(std::shared_ptr<AUDIO::Audio> audio)
{
   auto audio_cache = std::make_shared<AUDIO::AudioCache>(audio);

   /*
   audio_cache->set_audio_id(audio->id());
   audio_cache->set_title(audio->title()->value());
   audio_cache->set_artist_name(audio->artist_fullname());
   audio_cache->set_orig_filepath(audio->audio_lib_path()->value());
   audio_cache->set_cache_filepath(AUDIO_CACHE_LOCATION.toStdString());
   audio_cache->set_audio_type(audio->audio_type()->value());
   audio_cache->set_cache_datetime(QDateTime::currentDateTime());
   audio_cache->set_file_extension(audio->file_extension()->value());
   */

   /*
   audio_cache->set_audio_id(audio.id());
   audio_cache->set_title(audio.title());
   audio_cache->set_artist_name(audio.artist());
   audio_cache->set_orig_filepath(audio.file_path());
   audio_cache->set_audio_type(audio.audio_type());
   audio_cache->set_file_extension(audio.file_extension());
   */

   audio_cache->set_cache_filepath(AUDIO_CACHE_LOCATION.toStdString());
   audio_cache->set_cache_datetime(QDateTime::currentDateTime());
   audio_cache->set_audio_id(audio->id());

   //std::cout << audio_cache;

    for (auto const& cq_thread : m_cache_queue_threads){
        if (cq_thread->thread_status() == AUDIO::ThreadStatus::READY){
            cq_thread->queue_add(std::move(audio_cache));
            if (!cq_thread->isRunning()){
                cq_thread->start();
                break;
            }
        }
    }

}


void MainWindow::show_commercial(int schedule_ref)
{
    int index = index_of(schedule_ref);
    auto schedule = schedule_item(index);

    m_comm_viewer->clear();
    m_comm_viewer->create_view_headers();

    fetch_commercial_from_db(schedule->id());

    m_comm_viewer->set_title("Commercial Break: "+schedule->schedule_time().toString("HH:mm"));

}

void MainWindow::show_track_info(int schedule_ref)
{
    int index = index_of(schedule_ref);
    auto schedule = schedule_item(index);

    if (schedule == nullptr)
        return;

    if ((schedule->schedule_type() == OATS::ScheduleType::HOUR_HEADER) ||
       (schedule->schedule_type() == OATS::ScheduleType::COMM))
        return;

    auto history =  make_history(schedule->audio()->id());

    m_track_info->show_audio_info(schedule->audio(), history);

}

History MainWindow::make_history(int audio_id)
{
    History hist;

    if (audio_id < 0)
        return hist;

    Schedule schedule;
    auto edm = std::make_unique<EntityDataModel>(std::make_shared<Schedule>());

    QString date_format = "yyyy-MM-dd";
    QString d_range1 = QDate::currentDate().addDays(-7).toString(date_format);
    QString d_range2 = QDate::currentDate().toString(date_format);

    QString date_range = "'"+d_range1+"' and '"+d_range2+ "'";

    auto date_filter = std::make_tuple(schedule.schedule_date()->dbColumnName(),
                                       " between ",
                                       date_range.toStdString());
    auto audio_filter = std::make_tuple(schedule.audio()->dbColumnName(),
                                        " = ",
                                        audio_id);

    try{
        edm->search(edm->prepareFilter(date_filter, audio_filter));
    } catch(DatabaseException& de) {
        showMessage(de.errorMessage());
    }

    for (auto& [name, entity] : edm->modelEntities()){
        Schedule* sched = dynamic_cast<Schedule*>(entity.get());
        hist[sched->schedule_date()->value().dayOfWeek()].push_back(sched->schedule_hour()->value());
    }

    return hist;

}


QString MainWindow::output_string(OATS::ScheduleItem* si)
{
    auto duration_seconds = (int)si->audio()->duration()->value()/1000;
    auto intro_seconds = (int)si->audio()->intro_marker()->value()/1000;
    auto fade_seconds = (int)si->audio()->fade_delay_marker()->value()/1000;

    int is_u1 = (int)intro_seconds/60;
    int is_u2 = intro_seconds % 60;

    int ds_u1 = (int)duration_seconds/60;
    int ds_u2 = duration_seconds % 60;

    int fs_u1 = (int)fade_seconds/60;
    int fs_u2 = fade_seconds % 60;

    QString cue_string = QString("%1:%2 / %3:%4 / %5:%6").arg(is_u1, 2, 10, QChar('0'))
                                                         .arg(is_u2, 2, 10, QChar('0'))
                                                         .arg(ds_u1, 2, 10, QChar('0'))
                                                         .arg(ds_u2, 2, 10, QChar('0'))
                                                         .arg(fs_u1, 2, 10, QChar('0'))
                                                         .arg(fs_u2, 2, 10, QChar('0'));

    return cue_string;
}

void MainWindow::transition_stop(int item_ref, int grid_index)
{
    int index = index_of(item_ref);

    auto si = schedule_item(index);
    si->set_transition_type(OATS::TransitionType::STOP);
    m_schedule_grid[grid_index]->set_subject(si);

    calculate_trigger_times();
}

void MainWindow::transition_mix(int item_ref, int grid_index)
{
    int index = index_of(item_ref);
    auto si = schedule_item(index);
    si->set_transition_type(OATS::TransitionType::MIX);
    m_schedule_grid[grid_index]->set_subject(si);

    calculate_trigger_times();
}

void MainWindow::transition_cut(int item_ref, int grid_index)
{
    int index = index_of(item_ref);
    auto si = schedule_item(index);
    si->set_transition_type(OATS::TransitionType::CUT);
    m_schedule_grid[grid_index]->set_subject(si);
}


void MainWindow::print_schedule_items()
{
    for (const auto& item : m_schedule_items){
        qDebug() << stoq(item->audio()->title()->value());
    }
}

void MainWindow::print_grid()
{
    int i = 0;
    qDebug() << " -------------------- ";
    for(const auto& item: m_schedule_grid){
        qDebug() << i++ << ":" << "Audio: " << item->schedule_item()->audio()->title()->to_qstring();
    }
    qDebug() << " -------------------- ";
}

void MainWindow::fetch_commercial_from_db(int schedule_id)
{
    std::stringstream sql;

    sql <<"select a.id, a.schedule_date, a.schedule_time, a.schedule_hour, "
        <<" b.schedule_id, b.spot_id,   "
        <<" c.name spot_title, c.spot_duration, c.real_duration, c.client_id,   "
        <<" d.audio_id, "
        <<" e.name client_name   "
        <<" from rave_schedule a "
        <<" left outer join rave_orderbooking b on a.id = b.schedule_id "
        <<" left outer join rave_spot c on b.spot_id = c.id "
        <<" left outer join rave_spotaudio d on c.id = d.spot_id "
        <<" left outer join rave_client e on c.client_id = e.id ";

   std::string where_filter = " where a.id = "+ std::to_string(schedule_id);

   sql << where_filter;

   EntityDataModel edm;
   edm.readRaw(sql.str());

   auto provider = edm.getDBManager()->provider();

   if (provider->cacheSize() == 0)
       return;

   QList<QStandardItem*> columns;

   provider->cache()->first();

   std::string spot_title;
   std::string client_name;
   std::string spot_duration;

   do{
       auto it_begin = provider->cache()->currentElement()->begin();
       auto it_end = provider->cache()->currentElement()->end();

       for(; it_begin != it_end; ++it_begin)
       {
           std::string field_name = (*it_begin).first;
           std::string field_value = (*it_begin).second;

           if (field_name == "spot_title"){
               spot_title = field_value;
           }

           if (field_name == "spot_duration"){
               spot_duration = field_value;
           }

           if (field_name == "client_name"){
               client_name = field_value;
           }
       }

       auto title = new QStandardItem(QString::fromStdString(spot_title));
       auto client = new QStandardItem(QString::fromStdString(client_name));
       auto duration = new QStandardItem(QString::fromStdString(spot_duration));
       columns.append(title);
       columns.append(client);
       columns.append(duration);

       m_comm_viewer->model()->appendRow(columns);
       columns.clear();

       provider->cache()->next();
   } while (!provider->cache()->isLast());

}

void MainWindow::end_of_play()
{
    qDebug() << "Audio play finished.";
    m_audio_player->stop_play();
}

void MainWindow::play_next()
{
    qDebug() << "Playing next audio...";
}

void MainWindow::jingle_end_of_play()
{
    m_jingle_player->stop_play();
    qDebug() << "Stopped jingle player";
}

void MainWindow::close_window()
{
    QMessageBox msg_box;
    msg_box.setIcon(QMessageBox::Warning);
    msg_box.setText("Closing OATS");
    msg_box.setInformativeText("Are you sure you want to close the application?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg_box.setDefaultButton(QMessageBox::No);

    int result = msg_box.exec();

    if (result == QMessageBox::Yes){
       close();
    }

}

void MainWindow::to_lower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){
      return std::tolower(c) ;
    });
}

void MainWindow::print(QString msg)
{
    qDebug() << msg;
}

void MainWindow::print_output_status()
{
    for (auto const& panel: m_output_panels){
        qDebug() << "PANEL STATUS: "<< panel->panel_name() << ": "<< panel->panel_status_text();
    }

}


