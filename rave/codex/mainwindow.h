#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSystemTrayIcon>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QDialog>
#include "client.h"

QT_BEGIN_NAMESPACE

class QAction;
class QGroupBox;
class QMenu;
class QPushButton;
class QTextEdit;

QT_END_NAMESPACE


class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow();

    void setVisible(bool) override;

// protected:
//     void closeEvent(QCloseEvent*) override;

private slots:
    void setIcon();
    void log_message(const QString);
//     void iconActivated(QSystemTrayIcon::ActivationReason);
//     void show_message();
//     void message_clicked();

private:
    void create_logger_groupbox();
    void create_actions();
    void create_tray_icon();

    void soc_connect();
    void soc_disconnect();
    void send_request();

    QGroupBox* m_logger_groupbox;
    QTextEdit* m_edt_logger;

    QPushButton* m_btn_hide;
    QPushButton* m_btn_close;
    QPushButton* m_btn_cmd;

    QAction* m_act_min;
    QAction* m_act_max;
    QAction* m_act_restore;
    QAction* m_act_quit;

    QSystemTrayIcon* m_tray_icon;
    QMenu* m_tray_icon_menu;

    std::unique_ptr<Client> m_client;


};

#endif

#endif // MAINWINDOW_H
