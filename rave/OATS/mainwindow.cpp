#include <map>
#include <chrono>
#include <math.h>
#include <algorithm>
#include <concepts>
#include <iostream>

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

//#include "timeanalyzerwidget.h"
#include "datetimewidget.h"
#include "playlistcontrolwidget.h"
#include "scheduleitem.h"
#include "schedulegriditem.h"
#include "outputpanel.h"
#include "playmodepanel.h"
#include "../data/commercialviewer.h"
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
    m_current_cued_item.item = nullptr;

    ui->setupUi(this);

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
    load_schedule(QDate::currentDate(), QTime::currentTime().hour()-1);

    set_playout_widgets();

    compute_schedule_time();

    setup_timers();

    ui->swMain->setCurrentIndex(0);
//    ui->twLoad->setCurrentIndex(0);

    connect(ui->gridScroll, &QScrollBar::valueChanged, this, &MainWindow::scroll_changed);
    connect(m_play_mode_panel.get(), &OATS::PlayModePanel::go_current, this, &MainWindow::go_current);
    connect(m_play_mode_panel.get(), &OATS::PlayModePanel::keep_current, this, &MainWindow::keep_current);
    connect(m_dtw.get(), &OATS::DateTimeWidget::time_updated, this, &MainWindow::time_updated);
    connect(m_jingle_grid.get(), &OATS::JingleGrid::play_jingle, this, &MainWindow::play_jingle);
    connect(m_jingle_grid.get(), &OATS::JingleGrid::stop_all_jingles, this, &MainWindow::stop_all_jingles);

    connect(ui->btnHome, &QPushButton::clicked, this, [&](){ ui->swMain->setCurrentIndex(0); m_control_page = ControlPage::Home; });
    connect(ui->btnComm, &QPushButton::clicked, this, [&](){ ui->swMain->setCurrentIndex(1); m_control_page = ControlPage::Commercial; });
    connect(ui->btnSegue, &QPushButton::clicked, this, [&](){ ui->swMain->setCurrentIndex(2); m_control_page = ControlPage::Segue; });
    connect(ui->btnCarts, &QPushButton::clicked, this, [&](){ui->swMain->setCurrentIndex(3); m_control_page = ControlPage::Cart; });
    connect(ui->btnJingles, &QPushButton::clicked, this, [&](){ui->swMain->setCurrentIndex(4); m_control_page = ControlPage::Jingle; });
    connect(ui->btnTrackInfo, &QPushButton::clicked, this, [&](){ui->swMain->setCurrentIndex(5); m_control_page = ControlPage::TrackInfo; });
    connect(ui->btnLoad, &QPushButton::clicked, this, [&](){ui->swMain->setCurrentIndex(7); m_control_page = ControlPage::Load; });

    connect(ui->btnPrint1, &QPushButton::clicked, this, [&](){m_audio_cache_manager->print_queue();});
    connect(ui->btnPrint2, &QPushButton::clicked, this, [&](){m_audio_cache_manager->print_cache();});
    connect(ui->btnPrint3, &QPushButton::clicked, this, [&](){m_audio_cache_manager->cache_audio();});
    connect(ui->btnClearQue, &QPushButton::clicked, this, [&](){m_audio_cache_manager->clear_queue();});
    connect(ui->btnClearCache, &QPushButton::clicked, this, [&](){m_audio_cache_manager->clear_cache();});

    connect(ui->btnPrintCache, &QPushButton::clicked, this, [&](){m_audio_cache_manager->print_cache();});

    // Audio Library Page

//    connect(ui->btnSearch, &QPushButton::clicked, this, &MainWindow::search_audio);

    //AUDIO::TrackBrowser* tb = new AUDIO::TrackBrowser();
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

    //QMainWindow::showFullScreen();
}

void MainWindow::test_concept()
{
    std::vector ints{1, 2, 3,4,4,3,6,7};
    std::cout << Average(ints);

}


MainWindow::~MainWindow()
{
    delete ui;
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
}

void MainWindow::start_timers()
{
    auto SLOW_FLASH_INTERVAL = 400ms;
    auto FAST_FLASH_INTERVAL = 250ms;
    auto MAIN_TIMER_INTERVAL = 25ms;
    auto COUNT_DOWN_INTERVAL = 50ms;

    m_slow_flash_timer->start(SLOW_FLASH_INTERVAL);
    m_fast_flash_timer->start(FAST_FLASH_INTERVAL);
    m_countdown_timer->start(COUNT_DOWN_INTERVAL);
    m_main_player_timer->start(MAIN_TIMER_INTERVAL);
}

void MainWindow::set_playout_widgets()
{
    set_datetime_widget();
    set_playlist_control_widget();
    //set_time_analytics_widget();

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
	/*
    m_taw = std::make_unique<TimeAnalyzerWidget>();
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
    //ui->hlPlayMode->addWidget(m_pcw.get());
}

void MainWindow::compute_schedule_time()
{
    if (m_current_playing_item.item == nullptr)
        return;

    int time_counter = m_current_playing_item.item->audio().duration();

    m_current_playing_item.item->set_current_time(time_counter);
    m_current_playing_item.item->notify();

    time_counter = time_counter + m_current_playing_item.item->audio().duration();

    for (int i=m_current_playing_item.schedule_index+1; i < m_schedule_items.size()-1; ++i){
        auto si = schedule_item(i);
        si->set_current_time(time_counter);
        time_counter = time_counter + si->audio().duration();

        //si->notify();
    }
}


void MainWindow::load_schedule(QDate date, int hr)
{
//    fetch_cached_data(hr);

#ifdef DB_SCHEDULE
    fetch_db_data(date, hr);
#endif

#ifdef TEMP_SCHEDULE
    fetch_temp_data(hr);
#endif
    make_playlist_grid();

    set_current_play_item();
}

void MainWindow::fill_schedule_headers(QDate date, int hr)
{
    for (int i=0; i<= MAX_GRID_ITEMS; ++i){
        auto header_item = std::make_unique<OATS::ScheduleItem>();

        if (hr > HOURS_IN_A_DAY){
            date = date.addDays(1);
            hr = 0;
        }

        set_header_item(header_item.get(), hr++, date);
        m_schedule_items.push_back(std::move(header_item));
    }
}

void MainWindow::fetch_db_data(QDate date, int hr)
{
    std::stringstream sql;

    sql << " SELECT a.id, a.schedule_date, a.schedule_hour, a.schedule_time, "
        << " a.auto_transition, a.play_date, a.play_time, "
        << " a.schedule_item_type, a.comment, a.booked_spots, "
        << " a.audio_id, b.title, b.filepath, b.duration, b.file_extension,"
        << " b.audio_type, c.fullname "
        << " FROM rave_schedule a  "
        << " left outer join rave_audio b on a.audio_id = b.id "
        << " left outer join rave_artist c on b.artist_id = c.id ";

    std::string where_filter = " WHERE a.schedule_date = '"+date.toString("yyyy/MM/dd").toStdString()+"'";
    std::string and_filter   = " AND a.schedule_hour >= "+std::to_string(hr);
    std::string order_by     = " ORDER BY a.schedule_hour, a.schedule_time, a.id ";

    sql << where_filter << and_filter << order_by;

    EntityDataModel edm;
    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();

    if (provider->cacheSize() == 0){
        fill_schedule_headers(date, hr);
        return;
    }

    cache_commercial_break_data(date, hr);

    int last_hour = hr;
    bool is_hour_header_created = false;

    provider->cache()->first();

    int item_duration = 0;
    int total_duration = 0;

    do{
        auto sched_item = std::make_unique<OATS::ScheduleItem>();

        set_schedule_fields(provider, sched_item.get());

        if (sched_item->hour() != last_hour){
            last_hour = sched_item->hour();
            is_hour_header_created = false;
            item_duration = 0;
        }

        if (!is_hour_header_created){
            auto header_item = std::make_unique<OATS::ScheduleItem>();
            set_header_item(header_item.get(), last_hour, date);
            m_schedule_items.push_back(std::move(header_item));
            is_hour_header_created = true;
        }

        total_duration = (item_duration == 0) ?  0 : sched_item->current_time()+item_duration;

        QTime item_time = sched_item->schedule_time().addMSecs(total_duration);
        sched_item->set_current_time(total_duration);

        sched_item->set_schedule_time(item_time);

        item_duration = item_duration + sched_item->audio().duration();

        if (sched_item->schedule_type() == OATS::ScheduleType::COMM)
        {
            auto comm_duration = get_comm_duration(sched_item->id());

            sched_item->audio().set_duration(comm_duration);
            sched_item->audio().set_title(
                        sched_item->schedule_time().toString("HH:mm").toStdString()+
                       " Commercial Break ("+std::to_string(sched_item->booked_spots())+" items)");

            item_duration += comm_duration;
        }

        sched_item->set_transition_type(OATS::TransitionType::CUT);

        m_schedule_items.push_back(std::move(sched_item));

        //m_display_items.push_back(schedule);

        provider->cache()->next();

    } while (!provider->cache()->isLast());


    if (m_schedule_items.size() < MAX_GRID_ITEMS){

        int extra_items = MAX_GRID_ITEMS-m_schedule_items.size();

        for(int i=0; i < extra_items; ++i){
            auto header_item = std::make_unique<OATS::ScheduleItem>();
            set_header_item(header_item.get(), ++last_hour, date);
            m_schedule_items.push_back(std::move(header_item));
        }
    }

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


void MainWindow::cache_commercial_break_data(QDate date, int hr)
{
    std::stringstream sql;

    sql << " select a.id, a.schedule_id, a.spot_id,  a.booking_status, "
           " a.book_date, a.book_hour, a.book_time,"
           " d.title, d.duration, d.file_extension "
           " From rave_orderbooking a "
           " left outer join rave_spot b on b.id = a.spot_id "
           " left outer join rave_spotaudio c on c.spot_id = b.id "
           " left outer join rave_audio d on d.id = c.audio_id ";

    std::string where_filter = " WHERE a.book_date = '"+date.toString("yyyy/MM/dd").toStdString()+"'";
    std::string and_filter   = " AND a.book_hour >= "+std::to_string(hr);
    std::string order_by     = " ORDER BY a.book_time ";

    sql << where_filter << and_filter << order_by;

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

        for (; it_begin != it_end; ++it_begin){
            std::string field_name = (*it_begin).first;
            std::string field_value = (*it_begin).second;

            if (field_name == "id"){ cb.booking_id = str_to_int(field_value); }
            if (field_name == "schedule_id"){ cb.schedule_id = str_to_int(field_value); }
            if (field_name == "spot_id"){ cb.spot_id = str_to_int(field_value); }
            if (field_name == "booking_status") {cb.booking_status = QString::fromStdString(field_value); }
            if (field_name == "book_hour"){cb.book_hour = str_to_int(field_value);}
            if (field_name == "book_time") {cb.book_time = QTime::fromString(QString::fromStdString(field_value), "hh:mm:ss"); }
            if (field_name == "title") {cb.comm_title = QString::fromStdString(field_value);}
            if (field_name == "duration"){ cb.duration = str_to_int(field_value); }
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
    OATS::Audio audio;
    audio.set_title("HOUR: "+std::to_string(hr)+" Header "+dt.toString("dd/MM/yyyy").toStdString());
    audio.set_duration(0);
    header->set_transition_type(OATS::TransitionType::STOP);
    header->set_schedule_ref(s_sched_ref++);
    header->set_schedule_date(dt);
    header->set_hour(hr);
    header->set_audio(audio);
    header->set_play_channel("X");
}

void MainWindow::set_schedule_fields(BaseDataProvider* provider,
                                     OATS::ScheduleItem* schedule)
{
    OATS::Audio audio;

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

        if (field_name == "play_date")
            schedule->set_play_date(QDate::fromString(QString::fromStdString(field_value), "yyyy/MM/dd"));

        if (field_name == "play_time")
            schedule->set_play_time(QTime::fromString(QString::fromStdString(field_value), "hh:mm:ss"));

        if (field_name == "comment")
            schedule->set_comment(field_value);

        if (field_name == "booked_spots")
            schedule->set_booked_spots(str_to_int(field_value));

        if (field_name == "duration"){
            audio.set_duration(str_to_int(field_value));
            //schedule->set_break_duration(str_to_int(field_value));
        }

        if (field_name == "audio_id"){
            //schedule->set_audio(str_to_int(field_value));
            audio.set_id(str_to_int(field_value));
        }

        if (field_name == "title")
            audio.set_title(field_value);

        if (field_name ==  "filepath")
            audio.set_file_path(field_value);

        if (field_name == "fullname")
            audio.set_artist(field_value);

        if (field_name == "file_extension")
            audio.set_file_extension(field_value);

        if (field_name == "audio_type")
            audio.set_audio_type(field_value);
    }

    schedule->set_audio(audio);
}

void MainWindow::fetch_temp_data(int hr)
{
    OATS::Audio audio;
    audio.set_title("Let It Go");
    audio.set_artist("Frozen");
    audio.set_duration(15000); // 15 seconds
    audio.set_intro(3000);
    //audio.set_duration(150000); // 2.5 minutes

    auto sched_item = std::make_unique<OATS::ScheduleItem>();
    sched_item->set_schedule_ref(s_sched_ref++);
    sched_item->set_item_status(OATS::ItemStatus::WAITING);
    sched_item->set_play_channel(play_channel());
    sched_item->set_schedule_type("SONG");
    sched_item->set_hour(hr);
    sched_item->set_schedule_time((QTime::fromString("08:08:09", "HH:mm:ss")));
    sched_item->set_audio(audio);
    sched_item->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item));

    OATS::Audio audio1;
    audio1.set_title("Waiting on the World to Change");
    audio1.set_artist("John Mayer");
    audio1.set_duration(15000);
    audio1.set_intro(5000);
    //audio1.set_duration(204000); // 3.4 minutes
    auto sched_item1 = std::make_unique<OATS::ScheduleItem>();
    sched_item1->set_schedule_ref(s_sched_ref++);
    sched_item1->set_item_status(OATS::ItemStatus::WAITING);
    sched_item1->set_play_channel("B");
    sched_item1->set_schedule_type("SONG");
    sched_item1->set_hour(hr);
    sched_item1->set_schedule_time(QTime::fromString("08:12:02", "HH:mm:ss"));
    sched_item1->set_audio(audio1);
    sched_item1->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item1));

    OATS::Audio audio2;
    audio2.set_title("Tuonela");
    audio2.set_artist("Therion");
    //audio2.set_duration(192000); // 3.2
    audio2.set_duration(15000);
    audio2.set_intro(2000);
    auto sched_item2 = std::make_unique<OATS::ScheduleItem>();
    sched_item2->set_schedule_ref(s_sched_ref++);
    sched_item2->set_item_status(OATS::ItemStatus::WAITING);
    sched_item2->set_play_channel("A");
    sched_item2->set_schedule_type("SONG");
    sched_item2->set_hour(hr);
    sched_item2->set_schedule_time(QTime::fromString("08:15:02", "HH:mm:ss"));
    sched_item2->set_audio(audio2);
    sched_item2->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item2));

    OATS::Audio audio3;
    audio3.set_title("Ever Dream");
    audio3.set_artist("Nightwish");
    audio3.set_duration(15000);
    audio3.set_intro(3000);
    //audio3.set_duration(120000); //2 minutes
    auto sched_item3 = std::make_unique<OATS::ScheduleItem>();
    sched_item3->set_schedule_ref(s_sched_ref++);
    sched_item3->set_item_status(OATS::ItemStatus::WAITING);
    sched_item3->set_play_channel("B");
    sched_item3->set_schedule_type("SONG");
    sched_item3->set_hour(hr);
    sched_item3->set_schedule_time(QTime::fromString("08:17:05", "HH:mm:ss"));
    sched_item3->set_audio(audio3);
    sched_item3->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item3));

    OATS::Audio audio4;
    audio4.set_title("The 13th Floor");
    audio4.set_artist("Sirenia");
    audio4.set_duration(15000); //2.6
    //audio4.set_duration(156000); //2.6
    auto sched_item4 = std::make_unique<OATS::ScheduleItem>();
    sched_item4->set_schedule_ref(s_sched_ref++);
    sched_item4->set_item_status(OATS::ItemStatus::WAITING);
    sched_item4->set_play_channel("A");
    sched_item4->set_schedule_type("SONG");
    sched_item4->set_hour(hr);
    sched_item4->set_schedule_time(QTime::fromString("08:21:11", "HH:mm:ss"));
    sched_item4->set_audio(audio4);
    sched_item4->set_index(m_schedule_items.size());
    sched_item4->set_schedule_type("COMM");
    m_schedule_items.push_back(std::move(sched_item4));

    OATS::Audio audio5;
    audio5.set_title("Christina");
    audio5.set_artist("Theatres Des Vampires");
    //audio5.set_duration(204000); //3.4
    audio5.set_duration(15000); //3.4
    auto sched_item5 = std::make_unique<OATS::ScheduleItem>();
    sched_item5->set_schedule_ref(s_sched_ref++);
    sched_item5->set_item_status(OATS::ItemStatus::WAITING);
    sched_item5->set_play_channel("B");
    sched_item5->set_schedule_type("SONG");
    sched_item5->set_hour(hr);
    sched_item5->set_schedule_time(QTime::fromString("08:25:32", "HH:mm:ss"));
    sched_item5->set_audio(audio5);
    sched_item5->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item5));

    OATS::Audio audio6;
    audio6.set_title("No Way Out");
    audio6.set_artist("Dust in Mind");
    //audio6.set_duration(186000); // 3.1
    audio6.set_duration(15000); // 3.1
    auto sched_item6 = std::make_unique<OATS::ScheduleItem>();
    sched_item6->set_schedule_ref(s_sched_ref++);
    sched_item6->set_item_status(OATS::ItemStatus::WAITING);
    sched_item6->set_play_channel("A");
    sched_item6->set_schedule_type("SONG");
    sched_item6->set_hour(hr);
    sched_item6->set_schedule_time(QTime::fromString("08:27:37", "HH:mm:ss"));
    sched_item6->set_audio(audio6);
    sched_item6->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item6));

    OATS::Audio audio7;
    audio7.set_title("Lying From You");
    audio7.set_artist("Linkin Park");
    //audio7.set_duration(174000); // 2.9
    audio7.set_duration(15000); // 2.9
    auto sched_item7 = std::make_unique<OATS::ScheduleItem>();
    sched_item7->set_schedule_ref(s_sched_ref++);
    sched_item7->set_item_status(OATS::ItemStatus::WAITING);
    sched_item7->set_play_channel("B");
    sched_item7->set_schedule_type("SONG");
    sched_item7->set_hour(hr);
    sched_item7->set_schedule_time(QTime::fromString("08:31:30", "HH:mm:ss"));
    sched_item7->set_audio(audio7);
    sched_item7->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item7));

    OATS::Audio audio8;
    audio8.set_title("Float On");
    audio8.set_artist("Modest Mouse");
    //audio8.set_duration(210000); //3.5
    audio8.set_duration(15000); //3.5
    auto sched_item8 = std::make_unique<OATS::ScheduleItem>();
    sched_item8->set_schedule_ref(s_sched_ref++);
    sched_item8->set_item_status(OATS::ItemStatus::WAITING);
    sched_item8->set_play_channel("A");
    sched_item8->set_schedule_type("SONG");
    sched_item8->set_hour(hr);
    sched_item8->set_schedule_time(QTime::fromString("08:35:40", "HH:mm:ss"));
    sched_item8->set_audio(audio8);
    sched_item8->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item8));

    OATS::Audio audio9;
    audio9.set_title("Maniac - From Flashdance");
    audio9.set_artist("Micheal Sembello");
    //audio9.set_duration(180000); //3
    audio9.set_duration(15000); //3
    auto sched_item9 = std::make_unique<OATS::ScheduleItem>();
    sched_item9->set_schedule_ref(s_sched_ref++);
    sched_item9->set_item_status(OATS::ItemStatus::WAITING);
    sched_item9->set_play_channel("B");
    sched_item9->set_schedule_type("SONG");
    sched_item9->set_hour(hr);
    sched_item9->set_schedule_time(QTime::fromString("08:38:22", "HH:mm:ss"));
    sched_item9->set_audio(audio9);
    sched_item9->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item9));

    OATS::Audio audio10;
    audio10.set_title("Sugar, We're Going Down");
    audio10.set_artist("Fall Out Boy");
    //audio10.set_duration(132000); //2.2
    audio10.set_duration(15000); //2.2
    auto sched_item10 = std::make_unique<OATS::ScheduleItem>();
    sched_item10->set_schedule_ref(s_sched_ref++);
    sched_item10->set_item_status(OATS::ItemStatus::WAITING);
    sched_item10->set_play_channel("A");
    sched_item10->set_schedule_type("SONG");
    sched_item10->set_hour(hr);
    sched_item10->set_schedule_time(QTime::fromString("08:42:55", "HH:mm:ss"));
    sched_item10->set_audio(audio10);
    sched_item10->set_index(m_schedule_items.size());
    m_schedule_items.push_back(std::move(sched_item10));

    ui->gridScroll->setMaximum(m_schedule_items.size() - MAX_GRID_ITEMS );
}

void MainWindow::make_playlist_grid()
{
    ui->vlPlaylist->setSpacing(1);
    ui->vlPlaylist->setContentsMargins(0,0,0,2);

    std::string ch = ChannelA;

    for (int i=0; i < MAX_GRID_ITEMS; ++i){

        auto si = schedule_item(i);

        if (si->schedule_type() != OATS::ScheduleType::HOUR_HEADER){
            si->set_play_channel(ch);
            ch = (ch == ChannelA) ? ChannelB : ChannelA;
        }

        auto grid_item = std::make_unique<OATS::ScheduleGridItem>(si);
        si->notify();

        connect(grid_item.get(), &OATS::ScheduleGridItem::move_up, this, &MainWindow::item_move_up);
        connect(grid_item.get(), &OATS::ScheduleGridItem::move_down, this, &MainWindow::item_move_down);
        connect(grid_item.get(), &OATS::ScheduleGridItem::make_current, this, &MainWindow::make_item_current);
        connect(grid_item.get(), &OATS::ScheduleGridItem::insert_item, this, &MainWindow::grid_clicked);
        connect(grid_item.get(), &OATS::ScheduleGridItem::delete_item, this, &MainWindow::delete_schedule_item);

        connect(grid_item.get(), &OATS::ScheduleGridItem::transition_stop, this, &MainWindow::transition_stop);
        connect(grid_item.get(), &OATS::ScheduleGridItem::transition_mix, this, &MainWindow::transition_mix);
        connect(grid_item.get(), &OATS::ScheduleGridItem::transition_cut, this, &MainWindow::transition_cut);

        ui->vlPlaylist->addWidget(grid_item.get());
        m_schedule_grid.push_back(std::move(grid_item));

    }

    ui->vlPlaylist->insertStretch(-1,1);

    if (m_schedule_items.size() > 0){
        ui->gridScroll->setMaximum(m_schedule_items.size() - MAX_GRID_ITEMS );
    } else {
        ui->gridScroll->setMaximum(MAX_GRID_ITEMS);
    }

    ui->gridScroll->setPageStep(MAX_GRID_ITEMS/2);

}

OATS::ScheduleItem* MainWindow::schedule_item(int index)
{
    if (index <= m_schedule_items.size()-1){
        return m_schedule_items[index].get();
    } else {
        return nullptr;
    }
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

void MainWindow::display_schedule(int start_index)
{
    auto schedule = schedule_item(start_index);
    auto ch = schedule->play_channel();

   // for (int i=start_index; i < MAX_GRID_ITEMS; ++i){

    for (int schedule_index=start_index; schedule_index < MAX_GRID_ITEMS; ++schedule_index){
//        int schedule_index = i+start_index;
        if (schedule_index >= m_schedule_items.size())
            break;

        auto schedule = schedule_item(schedule_index);
        QTime schedule_time = schedule_time_at(schedule_index);

        //schedule->set_play_channel(play_channel(schedule_index));
        schedule->set_play_channel(ch);
        schedule->set_schedule_time(schedule_time);
        ch = (ch == ChannelA) ? ChannelB : ChannelA;

        m_schedule_grid[schedule_index]->set_subject(schedule);
    }
}

void MainWindow::display_schedule2(int start_index)
{

    auto schedule = schedule_item(start_index);

    if (schedule == nullptr)
        return;


    auto ch = schedule->play_channel();

    int idx = 0;

    for (int schedule_index=0; schedule_index < m_schedule_items.size(); ++schedule_index){

        if (schedule_index >= MAX_GRID_ITEMS)
            break;

        if (start_index > MAX_GRID_ITEMS)
            break;

        auto schedule = schedule_item(start_index);

        QTime schedule_time = schedule_time_at(start_index);
        ++start_index;

        //schedule->set_play_channel(play_channel(schedule_index));
        //schedule->set_play_channel(ch);
        schedule->set_schedule_time(schedule_time);

        //ch = (ch == ChannelA) ? ChannelB : ChannelA;

        m_schedule_grid[idx++]->set_subject(schedule);
        //schedule->set_play_channel(play_channel(schedule_index));
    }
}


void MainWindow::push_items_down(int from_pos)
{
    auto play_channel = [&](){
        auto prev_si = schedule_item(from_pos);
        if (prev_si == nullptr)
            return "A";

        if (prev_si->play_channel() == "A")
            return "B";

        if (prev_si->play_channel() == "B")
            return "A";

        return "A";
    };

    auto ch = (play_channel() == ChannelA) ? ChannelB : ChannelA;

    for (int i=from_pos+1; i< MAX_GRID_ITEMS; ++i){
        auto schedule = schedule_item(i-1);
        if (schedule != nullptr){
           schedule->set_play_channel(ch);
           m_schedule_grid[i]->set_subject(schedule);
           ch = (ch == ChannelA) ? ChannelB : ChannelA;
        }
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

    for(int i=from_pos; i<MAX_GRID_ITEMS; ++i){

        if (i > 0){
            auto prev_schedule_item = schedule_item(i-1);
            prev_schedule_time  = prev_schedule_item->schedule_time();

            auto current_schedule = schedule_item(i);

            if (prev_schedule_item->schedule_type() != OATS::ScheduleType::HOUR_HEADER){
                new_time = prev_schedule_time.addMSecs(prev_schedule_item->audio().duration());
            }else{
                new_time = current_schedule->schedule_time();
            }

            current_schedule->set_schedule_time(new_time);
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
        new_time = prev_schedule_item->schedule_time().addMSecs(prev_schedule_item->audio().duration());
    }else{
        new_time = curr_schedule_item->schedule_time();
    }

    return new_time;

}


void MainWindow::scroll_changed(int new_pos)
{
   display_schedule2(new_pos);
}

void MainWindow::go_current()
{
    ui->gridScroll->setValue(m_current_playing_item.grid_index);
}

void MainWindow::keep_current(bool checked)
{
    if (checked)
        go_current();
}

void MainWindow::play_jingle(OATS::Jingle* jingle)
{

    std::string audio_filename = jingle->audio()->audio_lib_path()->value()+
        m_audio_tool.make_audio_filename(jingle->audio()->id())+"."+jingle->audio()->file_extension()->value();

    m_current_jingle_item->set_schedule_date(QDate::currentDate());
    m_current_jingle_item->set_hour(QTime::currentTime().hour());
    m_current_jingle_item->set_schedule_type("JINGLE");
    m_current_jingle_item->set_transition_type(OATS::TransitionType::STOP);
    m_current_jingle_item->set_id(-1);
    m_current_jingle_item->set_schedule_ref(999);
    m_current_jingle_item->set_comment("JINGLE-PLAY");
    m_current_jingle_item->set_schedule_time(QTime::currentTime());

    OATS::Audio audio;
    audio.set_id(jingle->audio()->id());
    audio.set_title(jingle->audio()->title()->value());
    audio.set_duration(jingle->audio()->duration()->value());
    audio.set_file_path(jingle->audio()->file_path()->value());
    audio.set_artist(jingle->audio()->artist()->displayName());
    audio.set_file_path(jingle->audio()->audio_lib_path()->value());
    audio.set_file_extension(jingle->audio()->file_extension()->value_tolower());

    m_current_jingle_item->set_audio(audio);
    m_current_jingle_item->set_play_channel("C");

    cue_schedule_item(m_current_jingle_item.get(), m_outputC.get());

    play_outputC(m_outputC.get());
//    m_jingle_player->play_audio(JinglePlayoutChannel, QString::fromStdString(audio_filename));

}

void MainWindow::stop_all_jingles()
{
    m_jingle_player->stop_play();
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
        long long remaining = m_outputA->schedule_item()->audio().duration() - elapsed;

        if ( elapsed < m_outputA->schedule_item()->audio().intro() ){
            // intro is still in progress
            auto duration_seconds = (int)(m_outputA->schedule_item()->audio().intro() - elapsed)/100;
            int ds_u1 = (int)duration_seconds/10;
            int ds_u2 = duration_seconds % 10;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputA->set_cue_time_string(cue_string);
            m_outputA->set_progress_bar_background(OATS::ProgressBarBGColor::RED);

            m_outputA->update_progress_bar( 100 - round(( ((float)(m_outputA->schedule_item()->audio().intro() - elapsed ) /
                                     m_outputA->schedule_item()->audio().intro()) * 100 )
                    ) );
        } else if ( remaining < m_outputA->schedule_item()->audio().fade_out()) {
            int duration_seconds = (int)remaining / 1000;
            int ds_u1 = (int)duration_seconds / 60;
            int ds_u2 = duration_seconds % 60;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputA->set_cue_time_string(cue_string);
            m_outputA->set_progress_bar_background(OATS::ProgressBarBGColor::GREEN);

            if (m_outputA->schedule_item()->audio().fade_out() > 0) {
                m_outputA->update_progress_bar(
                            100 - round( ((float)m_outputA->time_remaining() /
                                          m_outputA->schedule_item()->audio().fade_out()
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
                                 (m_outputA->schedule_item()->audio().duration() -
                                  m_outputA->schedule_item()->audio().intro()) ) * 100 )
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
        long long remaining = m_outputB->schedule_item()->audio().duration() - elapsed;

        if ( elapsed < m_outputB->schedule_item()->audio().intro() ){
            // intro is still in progress
            auto duration_seconds = (int)(m_outputB->schedule_item()->audio().intro() - elapsed)/100;
            int ds_u1 = (int)duration_seconds/10;
            int ds_u2 = duration_seconds % 10;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputB->set_cue_time_string(cue_string);
            m_outputB->set_progress_bar_background(OATS::ProgressBarBGColor::RED);

            m_outputB->update_progress_bar(
                        100 - round(((float)(m_outputB->schedule_item()->audio().intro() - elapsed ) /
                                     m_outputB->schedule_item()->audio().intro()) * 100 )
                    );
        } else if ( remaining < m_outputB->schedule_item()->audio().fade_out()) {
            int duration_seconds = (int)remaining / 1000;
            int ds_u1 = (int)duration_seconds / 60;
            int ds_u2 = duration_seconds % 60;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputB->set_cue_time_string(cue_string);
            m_outputB->set_progress_bar_background(OATS::ProgressBarBGColor::GREEN);

            if (m_outputB->schedule_item()->audio().fade_out() > 0) {
                m_outputB->update_progress_bar(
                            100 - round( ((float)m_outputB->time_remaining() /
                                          m_outputB->schedule_item()->audio().fade_out()
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
                                      (m_outputB->schedule_item()->audio().duration() -
                                       m_outputB->schedule_item()->audio().intro())
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
        long long remaining = m_outputC->schedule_item()->audio().duration() - elapsed;

        if (elapsed < m_outputC->schedule_item()->audio().intro() ) {
            // intro is still in progress
            auto duration_seconds = (int)(m_outputC->schedule_item()->audio().intro() - elapsed)/100;
            int ds_u1 = (int)duration_seconds / 10;
            int ds_u2 = duration_seconds % 10;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                   .arg(ds_u2, 2, 10, QChar('0'));
            m_outputC->set_cue_time_string(cue_string);
            m_outputC->set_progress_bar_background(OATS::ProgressBarBGColor::RED);

            m_outputC->update_progress_bar( 100 - round(( ((float)(m_outputC->schedule_item()->audio().intro() - elapsed ) /
                                     m_outputC->schedule_item()->audio().intro()) * 100 )
                    ) );
        } else if ( remaining < m_outputC->schedule_item()->audio().fade_out()) {
            int duration_seconds = (int)remaining / 1000;
            int ds_u1 = (int)duration_seconds / 60;
            int ds_u2 = duration_seconds % 60;
            QString cue_string = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                                                 .arg(ds_u2, 2, 10, QChar('0'));
            m_outputC->set_cue_time_string(cue_string);
            m_outputC->set_progress_bar_background(OATS::ProgressBarBGColor::GREEN);

            if (m_outputC->schedule_item()->audio().fade_out() > 0) {
                m_outputC->update_progress_bar(
                            100 - round( ((float)m_outputC->time_remaining() /
                                          m_outputC->schedule_item()->audio().fade_out()
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
                                 (m_outputC->schedule_item()->audio().duration() -
                                  m_outputC->schedule_item()->audio().intro()) ) * 100 )
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

            remaining = m_outputA->schedule_item()->audio().duration() - elapsed;
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
            (m_outputB->start_trigger_tick_stamp() <= (trigger_ticker+0)))
        {
            if (m_outputA->panel_status() == OATS::PanelStatus::PLAYING)
                stop_audio(m_outputA);

            play_audio(m_outputB);
        }
        break;

     case OATS::PanelStatus::PLAYING:
         if (m_outputB->start_tick_stamp() > 0) {

            elapsed = trigger_ticker - m_outputB->start_tick_stamp();
            remaining = m_outputB->schedule_item()->audio().duration()-elapsed;
            m_outputB->set_time_remaining(remaining);

            if ((remaining <= 0) || (m_outputB->start_tick_stamp() < 0)){
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
            remaining = m_outputC->schedule_item()->audio().duration()- elapsed;
            m_outputC->set_time_remaining(remaining);

            if ((remaining <= 0 ) || (m_outputC->start_tick_stamp() < 0)){
                stop_audio(m_outputC.get());
            }

        }

    }

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

void MainWindow::cue_schedule_item(OATS::ScheduleItem* next_schedule_item, OATS::OutputPanel* next_output_panel)
{
    set_scheduled_item_filepath(next_schedule_item);

    next_schedule_item->set_item_status(OATS::ItemStatus::CUED);
    next_output_panel->cue_item(next_schedule_item);
    next_output_panel->set_panel_status(OATS::PanelStatus::CUED);

    m_current_cued_item.item = next_schedule_item;

    auto duration_seconds = (int)next_schedule_item->audio().duration()/1000;
    auto intro_seconds = (int)next_schedule_item->audio().intro()/1000;
    auto fade_seconds = (int)next_schedule_item->audio().fade_delay()/1000;

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

    if (next_schedule_item->audio().intro() > 0) {
        next_output_panel->set_progress_bar_background(OATS::ProgressBarBGColor::RED);
    } else {
        next_output_panel->set_progress_bar_background(OATS::ProgressBarBGColor::BLUE);
    }

    calculate_trigger_times();

    queue_for_caching(next_schedule_item->audio());

}

void MainWindow::set_scheduled_item_filepath(OATS::ScheduleItem* schedule_item)
{
    std::string cache_audio_filepath =
            m_audio_cache_manager->get_audio_cache_path(schedule_item->audio().id());

    // point the scheduled item filepath to audio cache filepath
    if (!cache_audio_filepath.empty())
        schedule_item->audio().set_file_path(cache_audio_filepath);
}

void MainWindow::set_cache_last_play_dtime(OATS::ScheduleItem* schedule_item)
{
    m_audio_cache_manager->set_cache_audio_last_play_dtime(schedule_item->audio().id());
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
                m_current_playing_item.item->audio().set_duration(
                            (m_current_playing_item.item->audio().duration() - time_yield) +
                            (YIELD_FADE_DELAY - m_current_playing_item.item->audio().fade_delay())
                            );
                m_current_playing_item.item->audio().set_fade_delay(YIELD_FADE_DELAY);
                m_current_playing_item.item->audio().set_fade_out(YIELD_FADE_OUT);
            }else{
                m_current_playing_item.item->audio().set_duration(
                            m_current_playing_item.item->audio().duration() - time_yield
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
                segue_interval = m_current_playing_item.item->audio().extro() - next_schedule_item->audio().intro();

                track_start_mark = (current_play_start_time + m_current_playing_item.item->audio().duration());  // - 500;
                fade_start_mark = -1; // Don't fade

            } else if (m_current_playing_item.item->transition_type() == OATS::TransitionType::MIX){
                segue_interval = (m_current_playing_item.item->audio().extro() + next_schedule_item->audio().intro()) -
                                  m_current_playing_item.item->audio().fade_delay();

                if (m_current_playing_item.item->audio().fade_delay() > (next_schedule_item->audio().duration()-
                                                                         next_schedule_item->audio().fade_delay()) ){

                        m_current_playing_item.item->audio().set_fade_delay((next_schedule_item->audio().duration() -
                                                                     next_schedule_item->audio().fade_delay()) ); // - 500);
                }

                track_start_mark = (current_play_start_time + m_current_playing_item.item->audio().duration()) -
                                   m_current_playing_item.item->audio().fade_delay();

                fade_start_mark = (current_play_start_time + m_current_playing_item.item->audio().duration())-
                                   m_current_playing_item.item->audio().fade_out();

              }

             // Do some drop item calculations
             drop_start_mark = -1;

            }


        if (m_current_playing_item.item->play_channel() == "A"){
            m_outputB->set_start_trigger_tick_stamp(track_start_mark);
            m_outputB->set_fade_trigger_tick_stamp(fade_start_mark);
        }else{
            m_outputA->set_start_trigger_tick_stamp(track_start_mark);
            m_outputA->set_fade_trigger_tick_stamp(fade_start_mark);
        }

    }
}

void MainWindow::play_audio(OATS::OutputPanel* op)
{
    if (op->panel_status() == OATS::PanelStatus::CUED){

        op->set_panel_status(OATS::PanelStatus::PLAYING);
        op->schedule_item()->set_item_status(OATS::ItemStatus::PLAYING);
        op->schedule_item()->set_play_start_time(QTime::currentTime());

        op->set_start_tick_stamp(m_audio_tool.get_tick_count());

        op->schedule_item()->notify();

        auto audio = op->schedule_item()->audio();

        std::string file_ext = audio.file_extension();
        to_lower(file_ext);
        std::string audio_filename = audio.file_path()+
                m_audio_tool.make_audio_filename(audio.id())+"."+file_ext;

        QString audio_file = QString::fromStdString(audio_filename);

        qDebug() << "Playing Audio: "<< audio_file;
        m_audio_player->play_audio(op->panel_name(), audio_file);

        int grid_index = index_of(op->schedule_item()->schedule_ref());

        m_current_playing_item.item = op->schedule_item();
        m_current_playing_item.schedule_index = op->schedule_item()->index();
        m_current_playing_item.grid_index = grid_index;

        int next_id = next_output_panel_id(op->id());

        auto next_output_panel = find_output_panel(next_id);
        auto next_schedule_item = find_next_schedule_item(op->schedule_item());

        if (m_current_playing_item.item != next_schedule_item){

            if (next_schedule_item != nullptr){
                cue_schedule_item(next_schedule_item, next_output_panel);
            }

        }

        display_schedule2(ui->gridScroll->value());

    }

}

void MainWindow::stop_audio(OATS::OutputPanel* op)
{
    if (op->schedule_item()->item_status() == OATS::ItemStatus::PLAYING){
        op->set_panel_status(OATS::PanelStatus::PLAYED);
        op->set_start_trigger_tick_stamp(-1);
        op->set_fade_trigger_tick_stamp(-1);

        op->schedule_item()->set_item_status(OATS::ItemStatus::PLAYED);
        op->schedule_item()->notify();

        set_cache_last_play_dtime(op->schedule_item());


//        m_current_playing_item.item->set_item_status(OATS::ItemStatus::PLAYED);
    }

    m_audio_player->stop_play();

    display_schedule(ui->gridScroll->value()+1);

    op->reset_play_button();
    op->reset_stop_button();

    op->update_progress_bar(100);
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
    auto si = schedule_item(index);

    if (si->item_status() == OATS::ItemStatus::PLAYING)
        return;

    if (si->item_status() == OATS::ItemStatus::ERROR_01)
        return;


    if ((m_current_cued_item.item != nullptr) &&
        (m_current_cued_item.item->item_status() != OATS::ItemStatus::PLAYING) ){
        m_current_cued_item.item->set_item_status(OATS::ItemStatus::WAITING);
        m_current_cued_item.item->notify();
    }


    if ( grid_pos < m_current_playing_item.grid_index){

         if (m_current_playing_item.item->item_status()==OATS::ItemStatus::PLAYING) {
            QMessageBox::information(this, tr("OATS"),
                         tr("Cannot schedule past playing item"),
                         QMessageBox::Ok);
            return;
         }
    }

    set_scheduled_item_filepath(si);

    m_current_cued_item.item = si;

    QTime curr_time = QTime::currentTime();
    int item_duration = si->audio().duration();

    si->set_item_status(OATS::ItemStatus::CUED);
    si->set_schedule_time(curr_time);
    si->set_schedule_time(curr_time.addMSecs(item_duration));
    si->notify();

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

    for(int i=index+1; i < m_schedule_items.size()-1; ++i){
        auto sched_item = schedule_item(i);
        sched_item->set_schedule_time(curr_time.addMSecs(item_duration));

        if (sched_item->item_status()== OATS::ItemStatus::ERROR_01)
            continue;

        sched_item->set_item_status(OATS::ItemStatus::WAITING);
        curr_time = sched_item->schedule_time();
        item_duration = sched_item->audio().duration();

        sched_item->notify();
    }

    display_schedule(ui->gridScroll->value()+1);
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

    if (si->item_status()== OATS::ItemStatus::PLAYED)
        return;

    if (si->schedule_type()==OATS::ScheduleType::COMM || si->schedule_type()==OATS::ScheduleType::HOUR_HEADER)
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

    ui->gridScroll->setMaximum(ui->gridScroll->maximum()-1);

    display_schedule2(grid_pos+1);
    ui->gridScroll->setSliderPosition(MAX_GRID_ITEMS);
    ui->gridScroll->setSliderPosition(0);

}


void MainWindow::load_item(int schedule_ref, int grid_pos)
{
    AUDIO::Audio* audio = m_track_browser->current_selected_audio();

    if (audio == nullptr)
        return;

    int index = index_of(schedule_ref);
    auto item_at_cursor = schedule_item(index);

    auto new_item = std::make_unique<OATS::ScheduleItem>();

    new_item->set_hour(item_at_cursor->hour());
    new_item->set_schedule_date(item_at_cursor->schedule_date());
    //new_item->set_schedule_type(new_item->str_to_schedule_type(audio->audio_type()->value()));
    new_item->set_schedule_type(audio->audio_type()->value());
    new_item->set_transition_type(OATS::TransitionType::MIX);
    new_item->set_id(-1);
    new_item->set_schedule_ref(s_sched_ref++);
    //new_item->set_schedule_date(item_at_cursor->schedule_date());
    new_item->set_comment("MANUAL-INSERT");

    QTime new_time;
    if (item_at_cursor->schedule_type() == OATS::ScheduleType::HOUR_HEADER){
        new_time = QTime::fromString(QTime::currentTime().toString(), "hh:mm:ss");
    } else {
        new_time = item_at_cursor->schedule_time().addMSecs(item_at_cursor->audio().duration());
    }

    new_item->set_schedule_time(new_time);

    OATS::Audio new_audio;
    new_audio.set_id(audio->id());
    new_audio.set_title(audio->title()->value());
    new_audio.set_duration(audio->duration()->value());
    new_audio.set_file_path(audio->file_path()->value());
    new_audio.set_artist(audio->artist()->displayName());
    new_audio.set_file_path(audio->audio_lib_path()->value());

    std::string file_ext = audio->file_extension()->value_tolower();
    new_audio.set_file_extension(file_ext);

    new_item->set_audio(new_audio);

    // check if audio exist
    std::string filename = audio->audio_lib_path()->value()+m_audio_tool.make_audio_filename(audio->id())+"."+file_ext;

    if (!m_audio_tool.audio_exist(QString::fromStdString(filename))){
        new_item->set_item_status(OATS::ItemStatus::ERROR_01);
        new_item->set_transition_type(OATS::TransitionType::SKIP);
    }

    auto play_channel = [&](){
        auto prev_si = schedule_item(grid_pos);
        if (prev_si == nullptr)
            return "A";

        if (prev_si->play_channel() == "A")
            return "B";

        if (prev_si->play_channel() == "B")
            return "A";

        return "A";
    };

    new_item->set_play_channel(play_channel());

    if (m_current_playing_item.item == nullptr)
        m_current_playing_item.item = new_item.get();

    auto insert_schedule_item = [&](int next_slot=0){
        m_schedule_grid[grid_pos]->set_subject(new_item.get());
        std::vector<std::unique_ptr<OATS::ScheduleItem>>::iterator it;
        it = m_schedule_items.begin()+(grid_pos+next_slot);
        m_schedule_items.insert(it,  std::move(new_item));
    };

   if (new_item->transition_type() != OATS::TransitionType::SKIP){
       OATS::Audio schedule_audio = make_schedule_audio(audio);
       queue_for_caching(schedule_audio);
   }

    if (item_at_cursor->schedule_type() == OATS::ScheduleType::HOUR_HEADER){
        push_items_down(grid_pos+1);
        insert_schedule_item(1);
        reprint_schedule(grid_pos-1);
    } else {
        push_items_down(grid_pos);
        insert_schedule_item();
        recompute_time(grid_pos);
    }

   ui->gridScroll->setMaximum(ui->gridScroll->maximum()+1);

   //print_schedule_items();  // debugging purposes only
}

OATS::Audio MainWindow::make_schedule_audio(AUDIO::Audio* audio)
{
    OATS::Audio schedule_audio;

    schedule_audio.set_id(audio->id());
    schedule_audio.set_title(audio->title()->value());
    schedule_audio.set_file_path(audio->audio_lib_path()->value());
    schedule_audio.set_artist(audio->artist_fullname());
    schedule_audio.set_file_extension(audio->file_extension()->value());
    schedule_audio.set_audio_type(audio->audio_type()->value());

    return schedule_audio;
}

void MainWindow::queue_for_caching(OATS::Audio audio)
{
   auto audio_cache = std::make_unique<AUDIO::AudioCache>();

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

   audio_cache->set_audio_id(audio.id());
   audio_cache->set_title(audio.title());
   audio_cache->set_artist_name(audio.artist());
   audio_cache->set_orig_filepath(audio.file_path());
   audio_cache->set_cache_filepath(AUDIO_CACHE_LOCATION.toStdString());
   audio_cache->set_audio_type(audio.audio_type());
   audio_cache->set_cache_datetime(QDateTime::currentDateTime());
   audio_cache->set_file_extension(audio.file_extension());

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

    auto history =  make_history(schedule->audio().id());
    AUDIO::Audio* audio = m_track_browser->find_audio_by_id(schedule->audio().id());
    m_track_info->show_audio_info(audio, history);
}

History MainWindow::make_history(int audio_id)
{
    History hist;

    if (audio_id < 0)
        return hist;

    Schedule schedule;
    auto edm = std::make_unique<EntityDataModel>(std::make_unique<Schedule>());

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
    auto duration_seconds = (int)si->audio().duration()/1000;
    auto intro_seconds = (int)si->audio().intro()/1000;
    auto fade_seconds = (int)si->audio().fade_delay()/1000;

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
        qDebug() << stoq(item->audio().title());
    }
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

       for(; it_begin != it_end; ++it_begin) {

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


