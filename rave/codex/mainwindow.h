#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSystemTrayIcon>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QDialog>

#include "../Network/clientsocket.h"
#include "../Network/serversocket.h"
#include "../Network/message.h"


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
    void recv_processed_message(NETWORK::Message);
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

    QGroupBox* m_logger_groupbox { nullptr};
    QTextEdit* m_edt_logger { nullptr };

    QPushButton* m_btn_hide { nullptr };
    QPushButton* m_btn_close { nullptr };
    QPushButton* m_btn_cmd { nullptr };

    QAction* m_act_min { nullptr };
    QAction* m_act_max { nullptr };
    QAction* m_act_restore { nullptr };
    QAction* m_act_quit { nullptr };

    QSystemTrayIcon* m_tray_icon { nullptr };
    QMenu* m_tray_icon_menu { nullptr };

    std::unique_ptr<NETWORK::ClientSocket> m_client_socket { nullptr };
    std::unique_ptr<NETWORK::ServerSocket> m_server_socket { nullptr };

};

#endif

#endif // MAINWINDOW_H
