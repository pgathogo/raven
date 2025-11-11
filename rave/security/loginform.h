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
    bool validNamePass();

    StationInfo get_station_info();
    ConnInfo get_connection_info();
    void express_login();
    void express_fetch();
    void test_login();

    Credentials credentials();

protected:
    void showEvent(QShowEvent*) override;

private slots:
    void open_station_selector();
    void check_forced_password_reset();
    bool cluster_server_authentication(std::string, std::string);
    void login();
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

};

#endif // LOGINFORM_H
