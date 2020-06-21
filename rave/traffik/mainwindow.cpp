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

    // Spots
    spotAction = traffikMenu->addAction("&Client Spots");
    spotAction->setStatusTip("Client Spots details");
    //connect(spotAction, &QAction::triggered, this, &MainWindow::openClientSpotBrowser);

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

