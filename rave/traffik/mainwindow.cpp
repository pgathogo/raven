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
#include "userbrowser.h"
#include "rolebrowser.h"
#include "contentbrowser.h"
#include "contentauthbrowser.h"

#include "../framework/entityregister.h"


MainWindow::MainWindow(QWidget *parent)
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
{
    ui->setupUi(this);

    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);

    createActions();

    mPGManager = new PostgresDatabaseManager;

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
    //connect(commlogAction, &QAction::triggered, this, &MainWindow::printCommLog);

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

    plainFormAction = new QAction(tr("&Test"), setupMenu);
    plainFormAction = new QAction(tr("&Test"), setupMenu);
    setupMenu->addAction(plainFormAction);
    plainFormAction->setStatusTip(tr("Maintain Gender value list"));
    connect(plainFormAction, &QAction::triggered, this, &MainWindow::plainForm);

    QToolBar* mainToolBar = addToolBar(tr("Traffik"));
    mainToolBar->setStyleSheet("QToolButton{padding: 10px }");
    mainToolBar->addAction(clientAction);
    mainToolBar->addAction(spotAction);
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
    delete mPGManager;

}

