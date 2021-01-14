#include <QMdiArea>
#include <QToolBar>

#include "../framework/valuelistbrowser.h"
#include "../framework/databasemanager.h"
#include "../framework/valuelist.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientgroupdlg.h"
#include "typeexclusionbrowser.h"

#include "../utils/plainform.h"
#include "clientgroup.h"

#include "voiceoverbrowser.h"
#include "salespersonbrowser.h"
#include "agentbrowser.h"
#include "clientbrowser.h"
#include "timebandbrowser.h"
#include "traffiksetup.h"
#include "setupform.h"
#include "breaklayoutbrowser.h"
#include "scheduleform.h"

#include "../security/userbrowser.h"
#include "../security/rolebrowser.h"
#include "../security/contentbrowser.h"
#include "../security/contentauthbrowser.h"

#include "../framework/entityregister.h"
#include "../framework/entitydatamodel.h"

#include "orderbooking.h"

#include "bookingwizard.h"
#include "order.h"

#include "bookingsegment.h"
#include "commlogform.h"
#include "bookingorderbrowser.h"

AccessMap MainWindow::access_map;

MainWindow::MainWindow(std::unique_ptr<Authentication> auth,
                       QWidget *parent)
    : QMainWindow(parent)
    , pf{}
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
    ,mAuth{std::move(auth)}
{
    ui->setupUi(this);

    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);

    createActions();

    m_report = new LimeReport::ReportEngine(this);


    this->setWindowTitle("Raven - Traffik");
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

    // Setup
    QAction* cgAction = new QAction(tr("&Client Group"), setupMenu);
    setupMenu->addAction(cgAction);
    cgAction->setStatusTip(tr("Create a new client group"));
    connect(cgAction, &QAction::triggered, this, &MainWindow::newClientGroup);

    QAction* genderAction = new QAction(tr("&Gender"), setupMenu);
    setupMenu->addAction(genderAction);
    genderAction->setStatusTip(tr("Maintain Gender value list"));
    connect(genderAction, &QAction::triggered, this, &MainWindow::newGender);

    QAction* typeExAction = new QAction(tr("&Exlclusions"), setupMenu);
    setupMenu->addAction(typeExAction);
    typeExAction->setStatusTip(tr("Maintain Type Exclusions"));
    connect(typeExAction, &QAction::triggered, this, &MainWindow::newTypeExclusion);

    // Voice Overs
    QAction* voiceOverAction = new QAction(tr("&Voice Overs"), setupMenu);
    setupMenu->addAction(voiceOverAction);
    voiceOverAction->setStatusTip(tr("Maintain details of voice overs"));
    connect(voiceOverAction, &QAction::triggered, this, &MainWindow::newVoiceOver);

    QAction* salesPersonAction = new QAction(tr("&Sales Person"), setupMenu);
    setupMenu->addAction(salesPersonAction);
    salesPersonAction->setStatusTip(tr("Maintain details of Sales Persons"));
    connect(salesPersonAction, &QAction::triggered, this, &MainWindow::browseSalesPerson);

    QAction* agentAction = new QAction(tr("&Agents"));
    setupMenu->addAction(agentAction);
    agentAction->setStatusTip(tr("Maintain Agents details"));
    connect(agentAction, &QAction::triggered, this, &MainWindow::browseAgent);

    QAction* brandAction = new QAction(tr("&Brands"));
    setupMenu->addAction(brandAction);
    brandAction->setStatusTip(tr("Maintain Client Brands"));
    connect(brandAction, &QAction::triggered, this, &MainWindow::browseBrands);

    setupMenu->addSeparator();

    QAction* tbandAction = new QAction(tr("&Time Bands"));
    setupMenu->addAction(tbandAction);
    tbandAction->setStatusTip(tr("Maintain Time Bands details"));
    connect(tbandAction, &QAction::triggered, this, &MainWindow::browseTimeBands);

    setupMenu->addSeparator();

    QAction* roleAction = new QAction(tr("&System Roles"));
    setupMenu->addAction(roleAction);
    roleAction->setStatusTip(tr("Maintain system roles details"));
    connect(roleAction, &QAction::triggered, this, &MainWindow::browseRoles);

    QAction* userAction = new QAction(tr("&System Users"));
    setupMenu->addAction(userAction);
    userAction->setStatusTip(tr("Maintain system users details"));
    connect(userAction, &QAction::triggered, this, &MainWindow::browseUsers);

    QAction* regAction = new QAction(tr("&Show Register"));
    setupMenu->addAction(regAction);
    regAction->setStatusTip(tr("Show registered entities"));
    connect(regAction, &QAction::triggered, this, &MainWindow::showRegister);

    setupMenu->addSeparator();

    QAction* contentAction = new QAction(tr("&App Content"));
    setupMenu->addAction(contentAction);
    contentAction->setStatusTip(tr("Maintain applicaion content"));
    connect(contentAction, &QAction::triggered, this, &MainWindow::contentBrowser);

    QAction* contentAuthAction = new QAction(tr("&Content Authorization"));
    setupMenu->addAction(contentAuthAction);
    contentAuthAction->setStatusTip(tr("Maintain content authorization"));
    connect(contentAuthAction, &QAction::triggered, this, &MainWindow::contentAuthBrowser);

    setupMenu->addSeparator();

    QAction* setupAction = new QAction(tr("&Traffik Setup"));
    setupMenu->addAction(setupAction);
    setupAction->setStatusTip(tr("Traffki Default Setup"));
    connect(setupAction, &QAction::triggered, this, &MainWindow::openSetupForm);

    QAction* breakAction = new QAction(tr("&Breaks"));
    setupMenu->addAction(breakAction);
    breakAction->setStatusTip(tr("Breaks Setup"));
    connect(breakAction, &QAction::triggered, this, &MainWindow::openBreakBrowser);

    QAction* scheduleAction = new QAction(tr("&Schedule"));
    setupMenu->addAction(scheduleAction);
    scheduleAction->setStatusTip(tr("Schedule Mngmt"));
    connect(scheduleAction, &QAction::triggered, this, &MainWindow::openSchedule);

    setupMenu->addSeparator();

    plainFormAction = new QAction(tr("&Test"), setupMenu);
    plainFormAction = new QAction(tr("&Test"), setupMenu);
    setupMenu->addAction(plainFormAction);
    plainFormAction->setStatusTip(tr("Maintain Gender value list"));
    connect(plainFormAction, &QAction::triggered, this, &MainWindow::plainForm);

    QAction* orderBookingAction = new QAction(tr("&Order Booking"));
    orderBookingAction->setStatusTip(tr("Book Orders"));
    connect(orderBookingAction, &QAction::triggered, this, &MainWindow::openOrderBooking);

    QToolBar* mainToolBar = addToolBar(tr("Traffik"));
    mainToolBar->setStyleSheet("QToolButton{padding: 10px }");
    mainToolBar->addAction(clientAction);
    mainToolBar->addAction(orderBookingAction);
    //mainToolBar->addAction(spotAction);
}

void MainWindow::newClientGroup()
{
    ClientGroupDlg* clientGroup = createSubWindow<ClientGroupDlg>();
    clientGroup->exec();
}

void MainWindow::newGender()
{
    ValueListBrowser* valueList = createSubWindow<ValueListBrowser>("gender");
    valueList->exec();
}

void MainWindow::newTypeExclusion()
{
    TypeExclusionBrowser* typeEx = createSubWindow<TypeExclusionBrowser>();
    typeEx->exec();
}

void MainWindow::newVoiceOver()
{
    VoiceOverBrowser* vob = createSubWindow<VoiceOverBrowser>();
    vob->exec();
}

void MainWindow::browseSalesPerson()
{
    SalesPersonBrowser* spb = createSubWindow<SalesPersonBrowser>();
    spb->exec();
}

void MainWindow::browseAgent()
{
    AgentBrowser* agentBrowser = createSubWindow<AgentBrowser>();
    agentBrowser->exec();
}

void MainWindow::browseBrands()
{
    //BrandBrowser* brandBrowser = createSubWindow<BrandBrowser>();
    //brandBrowser->exec();
}


void MainWindow::plainForm()
{
    pf = new PlainForm();
    pf->exec();
}
void MainWindow::browseClients()
{
    ClientBrowser* clientBrowser = createSubWindow<ClientBrowser>();
    clientBrowser->exec();
}

void MainWindow::browseTimeBands()
{
    TimeBandBrowser* tbBrowser = createSubWindow<TimeBandBrowser>();
    tbBrowser->exec();
}

void MainWindow::browseUsers()
{
    UserBrowser* userBrowser = createSubWindow<UserBrowser>();
    userBrowser->exec();
}

void MainWindow::browseRoles()
{
    RoleBrowser* roleBrowser = createSubWindow<RoleBrowser>();
    roleBrowser->exec();
}

void MainWindow::showRegister()
{
    for (auto& [k, v] : Factory<BaseEntity, int>::_data()){
        qDebug() << QString::fromStdString(k);
    }
}

void MainWindow::contentBrowser()
{
    ContentBrowser* contentBrowser = createSubWindow<ContentBrowser>();
    contentBrowser->exec();
}

void MainWindow::contentAuthBrowser()
{
    ContentAuthBrowser* contentAuthBrowser = createSubWindow<ContentAuthBrowser>();
    contentAuthBrowser->exec();
}

void MainWindow::openSetupForm()
{
    std::unique_ptr<SetupForm> setupForm;

    EntityDataModel edm(std::make_unique<TraffikSetup>());
    edm.all();
    if (edm.count() > 0){
        BaseEntity* be = edm.firstEntity();
        TraffikSetup* ts = dynamic_cast<TraffikSetup*>(be);
        setupForm = std::make_unique<SetupForm>(ts);
        if (setupForm->exec() > 0 ){
            edm.updateEntity(*ts);
            setupForm->save_approvers();
        }
    }else{
        auto ts = std::make_unique<TraffikSetup>();
        setupForm = std::make_unique<SetupForm>(ts.get());
        if (setupForm->exec() > 0 ){
            edm.createEntityDB(*ts);
            setupForm->save_approvers();
        }
    }
}

void MainWindow::openBreakBrowser()
{
    BreakLayoutBrowser* breakBrowser = createSubWindow<BreakLayoutBrowser>();
    breakBrowser->exec();
}

void MainWindow::openSchedule()
{
    ScheduleForm* schedForm = createSubWindow<ScheduleForm>();
    schedForm->exec();
}

void MainWindow::openOrderBooking()
{

    //auto order = new Order();
    //auto bw = std::make_unique<BookingWizard>(order);
    //bw->exec();

//    OrderBookingBrowser* obBrowser = createSubWindow<OrderBookingBrowser>();
//    obBrowser->exec();
    BookingOrderBrowser* book_order_browser = createSubWindow<BookingOrderBrowser>();
    book_order_browser->exec();
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
    delete pf;
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


