#include "mainwindow.h"

#include <QApplication>
#include <QDate>
#include <QDir>

#include "../security/loginform.h"
#include "../security/authentication.h"
#include "../security/user.h"
#include "../security/loginform.h"
#include "../security/selectstationform.h"

#include "../framework/logger.h"


//#define LOG_TO_FILE

QString module = "Main";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef LOG_TO_FILE
    QString log_path = QCoreApplication::applicationDirPath()+"/log";

    qDebug() << "* PATH: "<< log_path;

    QDir log_dir(log_path);
    if (!log_dir.exists()){
        log_dir.mkpath(log_path);
    }

    QString curr_dtime = QDateTime::currentDateTime().toString("ddMMyyyy");
    QString log_filepath = log_path+"/traffik_log_"+curr_dtime+".log";

    Logger::init(log_filepath);

#endif

    //w.setWindowState(w.windowState() ^ Qt::WindowMaximized);
    //auto auth = std::make_unique<Authentication>();
    //auth->connect("postgres", "abc123");
    //LoginForm lf(auth.get());
    //if (lf.exec() > 0){

    LoginForm lf("nbohr", "abc123");
    if (lf.exec() > 0)
    {
        Credentials cred = lf.credentials();
        QString msg = QString("Cluster server authentication for user %1... Successfule.").arg(cred.username);
        Logger::info(module, msg);

        auto ssform = std::make_unique<SelectStationForm>(cred.username);
        if (ssform->exec() == 0) {
            return 0;
        }

        StationInfo station_info = ssform->selected_station();

        ConnInfo conn_info;
        conn_info.host = station_info.ip_address.toStdString();
        conn_info.port = station_info.port_no;
        conn_info.db_name = station_info.db_name.toStdString();
        conn_info.username = cred.username.toStdString();
        conn_info.password = cred.password.toStdString();

        Authentication* auth = new Authentication(conn_info);
        auth->connect_to_server();

        /*
        auto station_info = lf.get_station_info();
        auto conn_info = lf.get_connection_info();
        QString msg = QObject::tr("User connected to server `%1`").
                      arg(station_info.server_name);
        */

        QString msg1 = QString("User connected to server: %1").arg(station_info.server_name);
        Logger::info(module, msg1);

        MainWindow w(&a, station_info, conn_info);
        w.resize(1300, 700);
        w.show();
        return a.exec();
    }

    return 1;

}
