#include <QMenu>
#include <QMenuBar>
#include <QMdiArea>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "../../../rave/framework/ui_baseentitybrowserdlg.h"
#include "../../../rave/framework/entitydatamodel.h"
#include "../../../rave/security/authentication.h"
#include "../../../rave/security/userbrowser.h"

#include "clusterbrowser.h"
#include "clustermanagerdlg.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_mdi_area{new QMdiArea }
    , m_file_menu{}
    , m_setup_act{}
    , m_coroutine {}
{
    ui->setupUi(this);

    m_mdi_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdi_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(m_mdi_area);

//    connect(ui->btnRaveDB, &QPushButton::clicked, this, &MainWindow::connect_rave_db);
//    connect(ui->btnClusterDB, &QPushButton::clicked, this, &MainWindow::connect_cluster_db);
    setup_menu();

    setWindowTitle("Cluster Manager");

    try{
        connect_rave_db();
    } catch(DatabaseException& de) {
        showMessage(de.errorMessage());
    }

}


MainWindow::~MainWindow()
{
    delete m_mdi_area;
    delete ui;
}

void MainWindow::connect_rave_db()
{
    auto auth =  std::make_unique<Authentication>();
    auth->connect("postgres", "abc123");
}

void MainWindow::connect_cluster_db()
{
    auto auth = std::make_unique<Authentication>();
    auth->connect_to_cluster_server("postgres", "abc123");
}

void MainWindow::setup_menu()
{
    m_file_menu = menuBar()->addMenu(tr("&File"));

    m_setup_act = m_file_menu->addAction("&User Setup");
    m_setup_act->setStatusTip("User setup");

    m_cluster_act = m_file_menu->addAction("&Cluster Setup");
    m_cluster_act->setStatusTip("Cluster setup window");

    m_file_menu->addSeparator();

    m_coroutine = m_file_menu->addAction("Run Coroutine");

    connect(m_setup_act, &QAction::triggered, this, &MainWindow::user_browser);
    connect(m_cluster_act, &QAction::triggered, this, &MainWindow::cluster_manager);
    connect(m_coroutine, &QAction::triggered, this, &MainWindow::run_coroutine);
}

void MainWindow::user_browser()
{
    UserBrowser* user_browser = create_sub_window<UserBrowser>();
    user_browser->exec();
}

void MainWindow::cluster_browser()
{
    ClusterBrowser* cluster_browser = create_sub_window<ClusterBrowser>();
    cluster_browser->exec();
}

void MainWindow::cluster_manager()
{
    ClusterManagerDlg* cluster_manager = create_sub_window<ClusterManagerDlg>();
    cluster_manager->exec();
}



void MainWindow::run_coroutine()
{
}
