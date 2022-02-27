#include <map>
#include <chrono>
#include <math.h>

#include <QVBoxLayout>
#include <QPushButton>
#include <QTime>
#include <QDebug>
#include <QScrollBar>
#include <QDebug>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "timeanalyzerwidget.h"
#include "datetimewidget.h"
#include "playlistcontrolwidget.h"
#include "scheduleitem.h"
#include "schedulegriditem.h"
#include "outputpanel.h"
#include "playmodepanel.h"

int MainWindow::s_sched_ref{0};
std::string MainWindow::s_channel{"A"};

using namespace std::chrono_literals;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_taw{nullptr}
    , m_dtw{nullptr}
    , m_outputA{nullptr}
    , m_outputB{nullptr}
{
    ui->setupUi(this);

    load_schedule(8);

    set_current_play_item();

    set_widgets();

    connect(ui->btnTest, &QPushButton::clicked, this, &MainWindow::close_win);
    connect(ui->gridScroll, &QScrollBar::valueChanged, this, &MainWindow::scroll_changed);
    connect(m_play_mode_panel.get(), &OATS::PlayModePanel::go_current, this, &MainWindow::go_current_clicked);

    connect(m_dtw.get(), &OATS::DateTimeWidget::time_updated, this, &MainWindow::time_updated);

    compute_schedule_time();

    m_slow_flash_timer = std::make_unique<QTimer>(this);
    connect(m_slow_flash_timer.get(), &QTimer::timeout, this, &MainWindow::slow_flash);

    m_fast_flash_timer = std::make_unique<QTimer>(this);
    connect(m_fast_flash_timer.get(), &QTimer::timeout, this, &MainWindow::fast_flash);

    m_countdown_timer = std::make_unique<QTimer>(this);
    connect(m_countdown_timer.get(), &QTimer::timeout, this, &MainWindow::count_down);

    m_main_player_timer = std::make_unique<QTimer>(this);
    connect(m_main_player_timer.get(), &QTimer::timeout, this, &MainWindow::status_timer);

    //QMainWindow::showFullScreen();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_widgets()
{
    set_datetime_widget();
    set_playlist_control_widget();
    //set_time_analytics_widget();

    make_playlist_grid();
    make_output_panel();
    make_play_mode_panel();
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
    m_taw = std::make_unique<TimeAnalyzerWidget>();
    std::map<QString, float> time_slots;

    time_slots["music_remain"] = 0.1;
    time_slots["time_remain"] = 0.1;
    time_slots["talk_time"] = 0.1;

    m_taw->set_time_bar(time_slots);
    ui->hlTimeAnalytics->addWidget(m_taw.get());
}

void MainWindow::set_playlist_control_widget()
{
    m_pcw = std::make_unique<OATS::PlayListControlWidget>();
    connect(m_pcw.get(), &OATS::PlayListControlWidget::go_current, this, &MainWindow::go_current);
    //ui->hlPlayMode->addWidget(m_pcw.get());
}

void MainWindow::go_current()
{
    m_schedule_grid[0]->set_subject(m_current_playing_item.item);
    m_current_playing_item.item->notify();

    if (m_current_playing_item.schedule_index > -1 ){
        int idx = m_current_playing_item.schedule_index;

        for (int i=0; i<MAX_GRID_ITEMS; ++i){

            if (i > MAX_GRID_ITEMS)
                break;

            auto si = schedule_item(idx++);
            if (si == nullptr)
                break;

            m_schedule_grid[i]->set_subject(si);
        }
    }

    ui->gridScroll->setValue(m_current_playing_item.grid_index);
}

void MainWindow::compute_schedule_time()
{
    int min = m_current_playing_item.item->schedule_time().minute();
    int sec = m_current_playing_item.item->schedule_time().second();
    int msec = m_current_playing_item.item->schedule_time().msec();

    int time_counter = (min * 60000) + (sec * 1000) + msec;

    m_current_playing_item.item->set_current_time(time_counter);
    m_current_playing_item.item->notify();

    time_counter = time_counter + m_current_playing_item.item->audio().duration();

    for (int i=m_current_playing_item.schedule_index+1; i < m_schedule_items.size()-1; ++i){
        auto si = schedule_item(i);
        si->set_current_time(time_counter);
        time_counter = time_counter + si->audio().duration();
        si->notify();
    }
}


void MainWindow::load_schedule(int hr)
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
    sched_item->set_schedule_type(OATS::ScheduleType::SONG);
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
    sched_item1->set_schedule_type(OATS::ScheduleType::SONG);
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
    sched_item2->set_schedule_type(OATS::ScheduleType::SONG);
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
    sched_item3->set_schedule_type(OATS::ScheduleType::SONG);
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
    sched_item4->set_schedule_type(OATS::ScheduleType::SONG);
    sched_item4->set_hour(hr);
    sched_item4->set_schedule_time(QTime::fromString("08:21:11", "HH:mm:ss"));
    sched_item4->set_audio(audio4);
    sched_item4->set_index(m_schedule_items.size());
    sched_item4->set_schedule_type(OATS::ScheduleType::COMM);
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
    sched_item5->set_schedule_type(OATS::ScheduleType::SONG);
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
    sched_item6->set_schedule_type(OATS::ScheduleType::SONG);
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
    sched_item7->set_schedule_type(OATS::ScheduleType::SONG);
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
    sched_item8->set_schedule_type(OATS::ScheduleType::SONG);
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
    sched_item9->set_schedule_type(OATS::ScheduleType::SONG);
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
    sched_item10->set_schedule_type(OATS::ScheduleType::SONG);
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

    std::string ch = "A";

    for (int i=0; i < MAX_GRID_ITEMS; ++i){

        auto si = schedule_item(i);
        si->set_play_channel(ch);

        auto grid_item = std::make_unique<OATS::ScheduleGridItem>(m_schedule_items[i].get());
        m_schedule_items[i]->notify();

        /*
        connect(grid_item.get(), &OATS::ScheduleGridItem::move_up, this, &MainWindow::item_move_up);
        connect(grid_item.get(), &OATS::ScheduleGridItem::move_down, this, &MainWindow::item_move_down);
        connect(grid_item.get(), &OATS::ScheduleGridItem::delete_item, this, &MainWindow::delete_schedule_item);
        connect(grid_item.get(), &OATS::ScheduleGridItem::insert_item, this, &MainWindow::insert_schedule_item);
        connect(grid_item.get(), &OATS::ScheduleGridItem::make_current, this, &MainWindow::make_item_current);

        connect(grid_item.get(), &OATS::ScheduleGridItem::transition_stop, this, &MainWindow::transition_stop);
        connect(grid_item.get(), &OATS::ScheduleGridItem::transition_mix, this, &MainWindow::transition_mix);
        connect(grid_item.get(), &OATS::ScheduleGridItem::transition_cut, this, &MainWindow::transition_cut);
        */

        ui->vlPlaylist->addWidget(grid_item.get());
        m_schedule_grid.push_back(std::move(grid_item));

        ch = (ch == "A") ? "B" : "A";

    }

    ui->vlPlaylist->insertStretch(-1,1);
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
    return (s_channel == "A" ) ? "B" : "A" ;
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

void MainWindow::play_button()
{
}
void MainWindow::stop_button()
{
}

void MainWindow::make_play_mode_panel()
{
    m_play_mode_panel = std::make_unique<OATS::PlayModePanel>(this);
    ui->hlPlayMode->addWidget(m_play_mode_panel.get());
}

void MainWindow::display_schedule(int start_index)
{
    for (int i=0; i < MAX_GRID_ITEMS; ++i){
        int schedule_index = i+start_index;
        if (schedule_index >= m_schedule_items.size())
            break;
        auto schedule = schedule_item(schedule_index);
        m_schedule_grid[i]->set_subject(schedule);
    }
}

void MainWindow::scroll_changed(int new_pos)
{
    display_schedule(new_pos);
}

void MainWindow::go_current_clicked()
{
    m_schedule_grid[0]->set_subject(m_current_playing_item.item);
    m_current_playing_item.item->notify();

    if (m_current_playing_item.schedule_index > -1){
        int idx = m_current_playing_item.schedule_index;
        for(int i=0; i < MAX_GRID_ITEMS; ++i){

            if (i > MAX_GRID_ITEMS)
                break;

            auto sched_item = schedule_item(idx++);
            if (sched_item == nullptr)
                break;

            m_schedule_grid[i]->set_subject(sched_item);
        }

    }

    ui->gridScroll->setValue(m_current_playing_item.grid_index);
}

void MainWindow::set_current_play_item()
{
    auto s_item = schedule_item(0);
    m_current_playing_item.item = s_item;
    m_current_playing_item.schedule_index = s_item->index();
    m_current_playing_item.grid_index = 0;

    make_item_current(s_item->schedule_ref());
}

void MainWindow::make_item_current(int item_ref)
{
}

void MainWindow::time_updated()
{
    if (m_play_mode_panel->keep_current_checked())
        go_current();
}

long long MainWindow::get_tick_count()
{
    auto time_point = std::chrono::steady_clock::now();
    auto msec = std::chrono::time_point_cast<std::chrono::milliseconds>(time_point);
    return msec.time_since_epoch().count();
}

void MainWindow::slow_flash()
{
    auto TEN_SECONDS  = 10000ms;
    auto FIVE_SECONDS =  5000ms;

    auto current_time = get_tick_count();

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

}

void MainWindow::fast_flash()
{
    auto FIVE_SECONDS = 5000ms;

    auto current_time = get_tick_count();

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
}

void MainWindow::count_down()
{
    auto trigger_ticker = get_tick_count();

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
    }

    // Output B
    switch (m_outputB->panel_status())
    {
     case OATS::PanelStatus::WAITING:
        break;
     case OATS::PanelStatus::CUED:
        break;
     case OATS::PanelStatus::PLAYING:
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

}

void MainWindow::status_timer()
{
    int elapsed{0};
    int remaining{0};
    auto trigger_ticker = get_tick_count();

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
             (m_outputA->start_trigger_tick_stamp() <= (trigger_ticker+0)))
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
                // protect C output
                stop_audio(m_outputA);
            }

          }

    }

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

}

int MainWindow::index_of(int item_ref)
{
    auto it = std::find_if(m_schedule_items.begin(), m_schedule_items.end(), FindByRef(item_ref));
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
    //OATS::ScheduleItem* si{nullptr};
    auto index = index_of(current_item->schedule_ref());
    auto next_item = schedule_item(index+1);
    return next_item;
}

void MainWindow::cue_next_schedule(OATS::ScheduleItem* si, OATS::OutputPanel* op)
{
    si->set_item_status(OATS::ItemStatus::CUED);
    op->cue_item(si);
    op->set_panel_status(OATS::PanelStatus::CUED);

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
    op->set_cue_time_string(cue_string);

    op->update_progress_bar(0);

    if (si->audio().intro() > 0) {
        op->set_progress_bar_background(OATS::ProgressBarBGColor::RED);
    } else {
        op->set_progress_bar_background(OATS::ProgressBarBGColor::BLUE);
    }

    calculate_trigger_times();
}

int MainWindow::calculate_yield_contribution(OATS::ScheduleItem* item)
{
    return 0;
}

void MainWindow::calculate_trigger_times()
{
    assert(m_outputA != nullptr);
    assert(m_outputB != nullptr);

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

        op->set_start_tick_stamp(get_tick_count());

        op->schedule_item()->notify();

        int grid_index = index_of(op->schedule_item()->schedule_ref());

        m_current_playing_item.item = op->schedule_item();
        m_current_playing_item.schedule_index = op->schedule_item()->index();
        m_current_playing_item.grid_index = grid_index;

        int next_id = next_output_panel_id(op->id());

        auto next_output_panel = find_output_panel(next_id);
        auto next_schedule_item = find_next_schedule_item(op->schedule_item());

        if (next_schedule_item != nullptr){
            qDebug() << "cue next schedule";
            cue_next_schedule(next_schedule_item, next_output_panel);
        }

        display_schedule(ui->gridScroll->value());

        //calculate_trigger_times();

    }

}

void MainWindow::stop_audio(OATS::OutputPanel* op)
{
    if (op->schedule_item()->item_status() == OATS::ItemStatus::PLAYING){
        op->set_panel_status(OATS::PanelStatus::STOP);
        op->set_start_trigger_tick_stamp(-1);
        op->set_fade_trigger_tick_stamp(-1);

        op->schedule_item()->set_item_status(OATS::ItemStatus::STOP);
        op->schedule_item()->notify();
    }

    display_schedule(ui->gridScroll->value());

    op->reset_play_button();
    op->reset_stop_button();

    op->update_progress_bar(100);
}





