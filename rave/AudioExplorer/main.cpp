#include "mainwindow.h"

#include <QApplication>

#include "../security/authentication.h"
#include "../security/loginform.h"
#include "../security/selectstationform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginForm lf("postgres", "abc123");
    if (lf.exec() > 0)
    {
        //auto station_info = lf.get_station_info();
        //auto conn_info = lf.get_connection_info();

        Credentials cred = lf.credentials();

        auto ssform = std::make_unique<SelectStationForm>(cred.username);
        if (ssform->exec() == 0) {
            return 0;
        }

        StationInfo si = ssform->selected_station();

        ConnInfo ci;
        ci.host = si.ip_address.toStdString();
        ci.port = si.port_no;
        ci.db_name = si.db_name.toStdString();
        ci.username = cred.username.toStdString();
        ci.password = cred.password.toStdString();

        Authentication* auth = new Authentication(ci);
        auth->connect_to_server();

        MainWindow w(&a, si, ci);
        w.show();
        return a.exec();
    }

    return 1;
}
