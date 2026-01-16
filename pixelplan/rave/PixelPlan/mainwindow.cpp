#include <QDebug>
#include <QToolBar>
#include <QApplication>
#include <QMessageBox>

#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>

#include "../../../rave/framework/valuelistbrowser.h"
#include "../../../rave/framework/databasemanager.h"
#include "../../../rave/framework/valuelist.h"
#include "../../../rave/framework/entityregister.h"
#include "../../../rave/framework/entitydatamodel.h"
#include "../../../rave/framework/ravensetup.h"
#include "../../../rave/framework/logger.h"

//#include "../../../rave/framework/applicationcontext.h"
#include <format>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "clientbrowser.h"
#include "setupform.h"
#include "scheduleform.h"
#include "commlogform.h"
#include "mergedbrowser.h"
#include "setupbrowser.h"

#include "order.h"
#include "bookingwizard.h"

#include "tvprogrambrowser.h"
#include "playlistform.h"
#include "commloganalyzer.h"

AccessMap MainWindow::access_map;

MainWindow::MainWindow(QApplication* app,
                       const StationInfo& si,
                       const ConnInfo& ci,
                       QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mdiArea{ new QMdiArea }
    ,traffikMenu{}
    ,reportMenu{}
    ,qreportSubMenu{}
    ,clientRptSubMenu{}
    ,setupMenu{}
    ,helpMenu{}
    ,clientAction{}
    ,spotAction{}
    ,orderAction{}
    ,timebandAction{}
    ,breakAction{}
    ,commlogAction{}
    ,viewbreakAction{}
    ,exitAction{}
    ,qreportAction{}
    ,clientRptAction{}
    ,plainFormAction{}
    ,mPGManager{}
    ,m_station_info{si}
{
    ui->setupUi(this);

    createActions();

    std::string uname = std::format("Username: {}    ", ci.username);
    std::string station = std::format("Station: {}   ", si.station_name.toStdString());
    std::string host = std::format("Host: {}    ", si.ip_address.toStdString());

    QLabel* station_label = new QLabel(QString::fromStdString(station));
    station_label->setStyleSheet("font-weight: bold; color: red");

    QStatusBar* sb = new QStatusBar(this);
    sb->addWidget(new QLabel(QString::fromStdString(uname)));
    sb->addWidget(station_label);
    sb->addWidget(new QLabel(QString::fromStdString(host)));
    setStatusBar(sb);

//    m_report = new LimeReport::ReportEngine(this);
//    FRAMEWORK::ApplicationContext* app_context;
//    app_context = app_context->instance(app);

    this->setWindowTitle("Raven - PixelPlan");

    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);

    int w = 800;
    int h = 700;
    setMinimumSize(w, h);
    setWindowIcon(QIcon(":/images/media/icons/raven.bmp"));

    this->resize(QSize(w,h));

    showRegister();

    Logger::info("MainWindow", "Ctor::opened MainWindow");
}

void MainWindow::createActions()
{
    traffikMenu = menuBar()->addMenu(tr("&PixelPlan"));
    reportMenu = menuBar()->addMenu(tr("&Reports"));
    setupMenu = menuBar()->addMenu(tr("&System Setup"));
    helpMenu = menuBar()->addMenu(tr("&Help"));

    // Client
    clientAction = traffikMenu->addAction("&Client");
    clientAction->setStatusTip("Client details");
    connect(clientAction, &QAction::triggered, this, &MainWindow::browseClients);

    // Quick Reports SubMenu
    qreportAction = reportMenu->addAction("&Quick Reports");
    qreportSubMenu = new QMenu();
    qreportAction->setMenu(qreportSubMenu);

    QAction* commlogAction = new QAction(tr("&Commercial Logs"));
    qreportSubMenu->addAction(commlogAction);
    connect(commlogAction, &QAction::triggered, this, &MainWindow::print_comm_log);

    QAction* viewBreaksAction = new QAction(tr("&View Breaks"));
    qreportSubMenu->addAction(viewBreaksAction);
    //connect(commlogAction, &QAction::triggered, this, &MainWindow::printCommLog);

    // Client Reports SubMenu
    clientRptAction = reportMenu->addAction("&Client Reports");
    clientRptSubMenu = new QMenu();
    clientRptAction->setMenu(clientRptSubMenu);

    QAction* clientListAction = new QAction(tr("&Client Listing"));
    clientRptSubMenu->addAction(clientListAction);
    //connect(commlogAction, &QAction::triggered, this, &MainWindow::printCommLog);

    QAction* setupAction = new QAction(tr("&PixelPlan Setup"));
    setupMenu->addAction(setupAction);
    setupAction->setStatusTip(tr("PixelPlan Default Setup"));
    connect(setupAction, &QAction::triggered, this, &MainWindow::openSetupForm);

    QToolBar* mainToolBar = addToolBar(tr("PixelPlan"));
    mainToolBar->setStyleSheet("QToolButton{padding: 10px }");
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mainToolBar->setIconSize(QSize(60, 60));

    QAction* actTVPrograms  = new QAction("&TV PROGRAMS");
    actTVPrograms->setIcon(QIcon(":/images/media/icons/tvprograms1.png"));
    connect(actTVPrograms, &QAction::triggered, this, &MainWindow::open_tv_programs);
    mainToolBar->addAction(actTVPrograms);

    QAction* scheduleAction = new QAction(tr("&BREAKS"));
    scheduleAction->setIcon(QIcon(":/images/media/icons/clock1.png"));
    setupMenu->addAction(scheduleAction);
    scheduleAction->setStatusTip(tr("Schedule"));
    connect(scheduleAction, &QAction::triggered, this, &MainWindow::on_schedule);
    mainToolBar->addAction(scheduleAction);
    mainToolBar->addSeparator();

    QAction* merged_browser_act = new QAction(tr("&CLIENTS"));
    merged_browser_act->setIcon(QIcon(":/images/media/icons/clients.png"));
    connect(merged_browser_act, &QAction::triggered, this, &MainWindow::open_merged_browser);

    mainToolBar->addAction(merged_browser_act);
    mainToolBar->addSeparator();

    QAction* setup_browser_act = new QAction(tr("&SETUPS"));
    setupMenu->addAction(setup_browser_act);
    connect(setup_browser_act, &QAction::triggered, this, &MainWindow::open_setup_browser);

    QAction* cue_editor_act = new QAction("Cue Editor");
    connect(cue_editor_act, &QAction::triggered, this, &MainWindow::open_cue_editor);

    // QAction* test_wizard_act = new QAction(tr("&Book Order"));
    // test_wizard_act->setIcon(QIcon(":/images/media/icons/booking.bmp"));
    // connect(test_wizard_act, &QAction::triggered, this, &MainWindow::test_new_booking);
    // mainToolBar->addAction(test_wizard_act);

    QAction* playlist_act = new QAction(tr("&PLAYLIST"));
    playlist_act->setIcon(QIcon(":/images/media/icons/playlist.png"));
    connect(playlist_act, &QAction::triggered, this, &MainWindow::open_playlist);

    mainToolBar->addAction(playlist_act);


    QLabel* lbl_station_name = new QLabel(m_station_info.station_name);
    QFont lbl_font = lbl_station_name->font();
    lbl_font.setBold(true);
    lbl_font.setPointSize(28);
    lbl_station_name->setFont(lbl_font);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    mainToolBar->addWidget(spacer);

    mainToolBar->addWidget(lbl_station_name);


    // QAction* loganalyzer_act = new QAction(tr("&CLAS"));
    // loganalyzer_act->setIcon(QIcon(":/images/media/icons/loganalyzer.png"));
    // connect(loganalyzer_act, &QAction::triggered, this, &MainWindow::on_log_analysis);
    // mainToolBar->addAction(loganalyzer_act);

}


void MainWindow::browseClients()
{
    ClientBrowser* clientBrowser = createSubWindow<ClientBrowser>();
    clientBrowser->exec();
}


void MainWindow::showRegister()
{
    for (auto& [k, v] : Factory<BaseEntity, int>::_data()){
        qDebug() << QString::fromStdString(k);
    }
}

void MainWindow::open_tv_programs()
{
    TVProgramBrowser* tv_program_browser = createSubWindow<TVProgramBrowser>();
    tv_program_browser->exec();
}

void MainWindow::openSetupForm()
{
    std::unique_ptr<SetupForm> setupForm;

    EntityDataModel edm(std::make_unique<RavenSetup>());
    edm.all();
    if (edm.count() > 0){
        std::shared_ptr<BaseEntity> be = edm.firstEntity();
        RavenSetup* ts = dynamic_cast<RavenSetup*>(be.get());
        setupForm = std::make_unique<SetupForm>(ts);
        if (setupForm->exec() > 0 ){
            edm.updateEntity(*ts);
            edm.all();
            setupForm->save_approvers();
        }
    }else{
        auto ts = std::make_unique<RavenSetup>();
        setupForm = std::make_unique<SetupForm>(ts.get());
        if (setupForm->exec() > 0 ){
            edm.createEntityDB(*ts);
            setupForm->save_approvers();
        }
    }

}


void MainWindow::on_schedule()
{
    ScheduleForm* schedForm = createSubWindow<ScheduleForm>();
    schedForm->exec();
}

void MainWindow::open_merged_browser()
{
    MergedBrowser* m_browse = createSubWindow<MergedBrowser>();
    m_browse->exec();
}

void MainWindow::open_setup_browser()
{
    SetupBrowser* setup_browser = createSubWindow<SetupBrowser>();
    setup_browser->exec();
}

void MainWindow::open_playlist()
{
    PlaylistForm* playlist_form = createSubWindow<PlaylistForm>();
    playlist_form->exec();

}

void MainWindow::open_log_analysis()
{
    CommLogAnalyzer* cla = createSubWindow<CommLogAnalyzer>();
    cla->exec();
}

void MainWindow::open_cue_editor()
{
}

void MainWindow::test_new_booking()
{

    auto player = new QMediaPlayer;
    player->setSource(QUrl::fromLocalFile("D:/Home/Clients/Baryonic/Data/CitizenTV/BIDCO MSAFI English 39sec_23rd May 2025.mov"));

    auto video_output = new QVideoWidget;
    player->setVideoOutput(video_output);

    QAudioOutput* audio_output = new QAudioOutput;
    player->setAudioOutput(audio_output);

    audio_output->setVolume(0.5);

    video_output->show();
    player->play();


    /*
    int order_id = 49;
    auto order_edm = std::make_unique<EntityDataModel>(std::make_shared<Order>());
    order_edm->getById({"id", "=", order_id});

    Order* order = dynamic_cast<Order*>(order_edm->getEntity().get());
    if (order != nullptr) {
        auto bw = std::make_unique<BookingWizard>(order);
        bw->test_set_start_end_dates();
        bw->exec();
    }
  */


}

void MainWindow::print_comm_log()
{
    CommLogForm* comm_log_form = createSubWindow<CommLogForm>();
    comm_log_form->exec();

    //m_report->loadFromFile( "D:/home/Lab/qtcreator/LimeReport/Reports/Test01.lrxml");
    //m_report->previewReport();
}



MainWindow::~MainWindow()
{
    delete mdiArea;
    //delete pf;
    delete ui;

    delete traffikMenu;
    delete reportMenu;
    delete qreportSubMenu;
    delete clientRptSubMenu;
    delete setupMenu;
    delete helpMenu;
    //delete mPGManager;

}

void MainWindow::showEvent(QShowEvent*)
{
}



