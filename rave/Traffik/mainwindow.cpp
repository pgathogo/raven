#include <QDebug>
#include <QToolBar>
#include <QApplication>

#include "../framework/valuelistbrowser.h"
#include "../framework/databasemanager.h"
#include "../framework/valuelist.h"
#include "../framework/entityregister.h"
#include "../framework/entitydatamodel.h"
#include "../framework/ravensetup.h"
#include "../framework/logger.h"

//#include "../framework/applicationcontext.h"
#include <format>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "clientbrowser.h"
#include "setupform.h"
#include "scheduleform.h"
#include "commlogform.h"
#include "mergedbrowser.h"
#include "setupbrowser.h"

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
{
    ui->setupUi(this);

    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);

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

    this->setWindowTitle("Raven - Traffik");

    setFixedSize(1300, 700);
    setWindowIcon(QIcon(":/images/media/icons/raven.bmp"));
    update();

    Logger::info("MainWindow", "Opened MainWindow");
}

void MainWindow::createActions()
{
    traffikMenu = menuBar()->addMenu(tr("&Traffik"));
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

    /*
    */

    QAction* setupAction = new QAction(tr("&Traffik Setup"));
    setupMenu->addAction(setupAction);
    setupAction->setStatusTip(tr("Traffk Default Setup"));
    connect(setupAction, &QAction::triggered, this, &MainWindow::openSetupForm);

    /*
    */

    QAction* scheduleAction = new QAction(tr("&SCHEDULE"));
    scheduleAction->setIcon(QIcon(":/images/media/icons/schedule.png"));
    setupMenu->addAction(scheduleAction);
    scheduleAction->setStatusTip(tr("Schedule"));
    connect(scheduleAction, &QAction::triggered, this, &MainWindow::open_schedule);

    /*
    */

    QAction* merged_browser_act = new QAction(tr("&CLIENTS"));
    merged_browser_act->setIcon(QIcon(":/images/media/icons/clients.png"));
    connect(merged_browser_act, &QAction::triggered, this, &MainWindow::open_merged_browser);

    QAction* setup_browser_act = new QAction(tr("&SETUPS"));
    setupMenu->addAction(setup_browser_act);
    connect(setup_browser_act, &QAction::triggered, this, &MainWindow::open_setup_browser);


    QAction* cue_editor_act = new QAction("Cue Editor");
    connect(cue_editor_act, &QAction::triggered, this, &MainWindow::open_cue_editor);

    QToolBar* mainToolBar = addToolBar(tr("Traffik"));
    mainToolBar->setStyleSheet("QToolButton{padding: 10px }");
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mainToolBar->addAction(merged_browser_act);
    mainToolBar->addSeparator();
    mainToolBar->addAction(scheduleAction);
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


void MainWindow::open_schedule()
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

void MainWindow::open_cue_editor()
{

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


