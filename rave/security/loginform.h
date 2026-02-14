#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include "accesscontroller.h"
#include "structs.h"

class NotificationBar;
class Authentication;

namespace Ui {
class LoginForm;
}

struct Credentials {
    QString username{};
    QString password{};
    QString station_code{};
};

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget* parent = nullptr);
    explicit LoginForm(const QString username, const QString password, QWidget* parent = nullptr);
    ~LoginForm();

    void closeEvent(QCloseEvent* event);
    bool check_empty_name_pass();

    StationInfo get_station_info();
    ConnInfo get_connection_info();
    void express_login();
    void express_fetch();
    void test_login();

    Credentials credentials();
    bool save_credentials();

    void set_username(QString);
    void set_password(QString);
    void set_auto_save(bool);

    // void eventFilter(QObject *, QEvent *) override;

protected:
    void showEvent(QShowEvent*) override;

private slots:
    void open_station_selector();
    bool forced_to_reset_password();
    bool cluster_server_authentication(std::string, std::string);
    void on_login();
    void cancel();
    void reset_password();
    void cancel_password_reset();

private:
    bool fetch_user_stations(QString);
    int select_station();
    bool validate_password_reset();
    void login_to_station(int);

    Ui::LoginForm* ui;
    Authentication* m_app_auth;
    NotificationBar* mNoticeBar;
    NotificationBar* m_reset_notice_bar;
    bool mOkClose;

    std::map<int, StationInfo> m_stations_info;
    int m_selected_station_id;

    StationInfo m_current_station_info;
    ConnInfo m_current_conn_info;

    Credentials m_credentials;

    bool m_save_credentials;

};

#endif // LOGINFORM_H
