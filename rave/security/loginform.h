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

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget* parent = nullptr);
    ~LoginForm();

    void closeEvent(QCloseEvent* event);
    bool validNamePass();

    StationInfo get_station_info();
    ConnInfo get_connection_info();

private slots:
    void fetch_stations();
    void login();
    void cancel();

private:
    bool populate_station_info(QString);

    Ui::LoginForm* ui;
    Authentication* m_app_auth;
    NotificationBar* mNoticeBar;
    bool mOkClose;

    std::map<int, StationInfo> m_stations_info;
    int m_selected_station_id;

    StationInfo m_current_station_info;
    ConnInfo m_current_conn_info;
};

#endif // LOGINFORM_H
