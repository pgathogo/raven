#include <memory>
#include "mainwindow.h"

#include <QApplication>

#include "../security/authentication.h"
#include "../security/loginform.h"
#include "../security/selectstationform.h"
#include "../framework/databasemanager.h"
#include "../framework/ravenexception.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginForm lf("postgres", "abc123");
    if (lf.exec() > 0 )
    {

        Credentials cred = lf.credentials();

        auto ssform = std::make_unique<SelectStationForm>(cred.username);
        if (ssform->exec() == 0)
            return 0;


        StationInfo si = ssform->selected_station();
        ConnInfo ci(si, cred.username.toStdString(), cred.password.toStdString());

        try {
        Authentication* auth = new Authentication(ci);
        auth->connect_to_server();
        } catch (DatabaseException& de) {
            QString msg = QString("Failed to connect to DB Server:\nHost: %1 Port: %2 DB: %3 User: %4")
                              .arg(si.ip_address)
                              .arg(QString::number(si.port_no))
                              .arg(si.db_name)
                              .arg(cred.username);


            QMessageBox mb;
            mb.setText(msg);
            mb.exec();
            return 0;
        }


        MainWindow w(&a, si, ci);
        w.resize(1300, 700);
        w.show();
        return a.exec();
    }

    return 1;
}
