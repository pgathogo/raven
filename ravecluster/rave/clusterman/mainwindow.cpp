#include <QMenu>
#include <QMenuBar>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "../../../rave/framework/ui_baseentitybrowserdlg.h"
#include "../../../rave/framework/entitydatamodel.h"
#include "../../../rave/security/authentication.h"
#include "../../../rave/security/userbrowser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_file_menu{}
    , m_setup_action{}
{
    ui->setupUi(this);

    connect(ui->btnRaveDB, &QPushButton::clicked, this, &MainWindow::connect_rave_db);
    connect(ui->btnClusterDB, &QPushButton::clicked, this, &MainWindow::connect_cluster_db);

    setup_menu();
}

MainWindow::~MainWindow()
{
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
    auth->connect_cluster("postgres", "abc123");


}

void MainWindow::setup_menu()
{
    m_file_menu = menuBar()->addMenu(tr("&File"));
    m_setup_action = m_file_menu->addAction("&User Setup");
    m_setup_action->setStatusTip("User setup");
    connect(m_setup_action, &QAction::triggered, this, &MainWindow::user_browser);
}

void MainWindow::user_browser()
{
    auto user_browser = std::make_unique<UserBrowser>();
    user_browser->exec();
}
