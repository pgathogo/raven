#include <QMdiArea>
#include <QToolBar>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientgroupdlg.h"
#include "valuelistbrowser.h"

#include "valuelist.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mdiArea{ new QMdiArea }

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

void MainWindow::openClientBrowser()
{
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mdiArea;

    /* know the order of deletion
    delete traffikMenu;
    delete reportMenu;
    delete setupMenu;
    delete helpMenu;

    delete clientAction;
    delete spotAction;
    delete qreportAction;
    delete qreportSubMenu;
    delete clientRptAction;
    delete clientRptSubMenu;
    */
}

