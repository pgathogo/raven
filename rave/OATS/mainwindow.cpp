#include <map>

#include <QVBoxLayout>
#include <QPushButton>
#include <QTime>
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

    set_widgets();

    connect(ui->btnTest, &QPushButton::clicked, this, MainWindow::close_win);

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
    set_time_analytics_widget();
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
    connect(m_pcw.get(), &OATS::PlayListControlWidget::keep_current, this, &MainWindow::keep_current);

    //ui->hlPlayMode->addWidget(m_pcw.get());
}

void MainWindow::go_current()
{
    qDebug() << "Go current clicked ...";
}

void MainWindow::keep_current()
{
    qDebug() << "Keep current clicked ...";
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

