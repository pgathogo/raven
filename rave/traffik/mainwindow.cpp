#include <QMdiArea>
#include <QToolBar>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientgroupdlg.h"
#include "typeexclusionbrowser.h"
#include "../framework/valuelistbrowser.h"

#include "../framework/valuelist.h"
#include "../utils/plainform.h"

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
{
    ui->setupUi(this);

    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);

    createActions();

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
    connect(clientAction, &QAction::triggered, this, &MainWindow::openClientBrowser);


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

    QAction* voAction = new QAction(tr("&Voice Overs"), setupMenu);
    setupMenu->addAction(voAction);
    voAction->setStatusTip(tr("Create new voice over persons"));

    QAction* genderAction = new QAction(tr("&Gender"), setupMenu);
    setupMenu->addAction(genderAction);
    genderAction->setStatusTip(tr("Maintain Gender value list"));
    connect(genderAction, &QAction::triggered, this, &MainWindow::newGender);

    QAction* typeExAction = new QAction(tr("&Exlclusions"), setupMenu);
    setupMenu->addAction(typeExAction);
    typeExAction->setStatusTip(tr("Maintain Type Exclusions"));
    connect(typeExAction, &QAction::triggered, this, &MainWindow::newTypeExclusion);

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
    Gender* gender = new Gender;
    ValueListBrowser* valueList = createSubWindow<ValueListBrowser>(gender);
    //valueList->setEntityDataModel(new Gender());
    valueList->exec();
    //valueList->setTableName("rave_gender");
}

void MainWindow::newTypeExclusion()
{
    TypeExclusionBrowser* typeEx = createSubWindow<TypeExclusionBrowser>();
    typeEx->exec();
}

void MainWindow::plainForm()
{
    pf = new PlainForm();
    pf->exec();
}
void MainWindow::openClientBrowser()
{
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

    /*
    delete clientAction;
    delete spotAction;
    delete orderAction;
    delete timebandAction;
    delete breakAction;
    delete commlogAction;
    delete viewbreakAction;
    delete exitAction;
    delete qreportAction;
    delete clientRptAction;
    delete plainFormAction;
*/


}

