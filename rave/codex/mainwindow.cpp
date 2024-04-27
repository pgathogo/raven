#include <memory>

#include "mainwindow.h"

#ifndef QT_NO_SYSTEMTRAYICON

#include <QAction>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QMenu>



MainWindow::MainWindow()
{
    create_logger_groupbox();
    create_actions();
    create_tray_icon();

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(m_logger_groupbox);
    setLayout(main_layout);

    m_client = std::make_unique<Client>(this);
    connect(m_client.get(), &Client::log_message, this, &MainWindow::log_message);

    //TODO::We need a thread to initiate a connectiont the server

    setIcon();
    m_tray_icon->show();

}

void MainWindow::create_logger_groupbox()
{
    m_logger_groupbox = new QGroupBox(tr("Log Messages"));
    m_edt_logger = new QTextEdit;

    QVBoxLayout* logger_layout = new QVBoxLayout();
    logger_layout->addWidget(m_edt_logger);

    QSpacerItem* h_spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding);

    m_btn_cmd = new QPushButton(tr("Test"));
    m_btn_hide = new QPushButton(tr("Hide"));
    m_btn_close = new QPushButton(tr("Close"));


    connect(m_btn_hide, &QPushButton::clicked, this, &MainWindow::soc_connect);
    connect(m_btn_close, &QPushButton::clicked, this, &MainWindow::soc_disconnect);
    connect(m_btn_cmd, &QPushButton::clicked, this, &MainWindow::send_request);

    QHBoxLayout* buttons_layout = new QHBoxLayout();
    buttons_layout->addItem(h_spacer);
    buttons_layout->addWidget(m_btn_cmd);
    buttons_layout->addWidget(m_btn_hide);
    buttons_layout->addWidget(m_btn_close);

    QVBoxLayout* main_logger_layout = new QVBoxLayout();
    main_logger_layout->addLayout(logger_layout);
    main_logger_layout->addLayout(buttons_layout);

    m_logger_groupbox->setLayout(main_logger_layout);

}

void MainWindow::create_actions()
{
    m_act_min = new QAction(tr("Mi&ninmize"), this);
    connect(m_act_min, &QAction::triggered, this, &QWidget::hide);

    m_act_max = new QAction(tr("Ma&ximize"));
    connect(m_act_max, &QAction::triggered, this, &QWidget::showMaximized);

    m_act_restore = new QAction(tr("&Restore"));
    connect(m_act_restore, &QAction::triggered, this, &QWidget::showNormal);

    m_act_quit = new QAction(tr("&Quit"));
    connect(m_act_quit, &QAction::triggered, this, &QCoreApplication::quit);
}

void MainWindow::create_tray_icon()
{
    m_tray_icon_menu = new QMenu(this);
    m_tray_icon_menu->addAction(m_act_min);
    m_tray_icon_menu->addAction(m_act_max);
    m_tray_icon_menu->addAction(m_act_restore);
    m_tray_icon_menu->addAction(m_act_restore);
    m_tray_icon_menu->addAction(m_act_quit);

    m_tray_icon = new QSystemTrayIcon(this);
    m_tray_icon->setContextMenu(m_tray_icon_menu);

}

void MainWindow::setVisible(bool visible)
{
    m_act_min->setEnabled(visible);
    m_act_max->setEnabled(visible);
    m_act_restore->setEnabled(isMaximized() || !visible);
    QDialog::setVisible(visible);
}

void MainWindow::setIcon()
{
    QIcon icon = QIcon(":/images/heart.png");
    m_tray_icon->setIcon(icon);
    setWindowIcon(icon);
}

void MainWindow::log_message(const QString msg)
{
    m_edt_logger->append(msg);
}

void MainWindow::soc_connect()
{
    m_client->soc_connect();
}

void MainWindow::soc_disconnect()
{
    m_client->soc_disconnect();
}

void MainWindow::send_request()
{
    m_edt_logger->append("Sending request");
    m_client->request("DISK-LIST");
}


#endif
