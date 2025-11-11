#include "mainwindow.h"

#include <QApplication>
#include <QDate>
#include <QDir>

#include "../../../rave/security/loginform.h"
#include "../../../rave/security/authentication.h"
#include "../../../rave/security/user.h"
#include "../../../rave/security/loginform.h"
#include "../../../rave/security/selectstationform.h"

#include "../../../rave/framework/logger.h"
#include "../../../rave/utils/tools.h"


//#define LOG_TO_FILE

bool cluster_server_authentication(QString username, QString password)
{
    auto app_auth = std::make_unique<Authentication>();
    try{
        app_auth->connect_to_cluster_server(username.toStdString(),password.toStdString() );
        return true;
    }catch(DatabaseException& de){
        std::cout << de.errorMessage();
         //mNoticeBar->errorNotification(de.errorMessage());
        return false;
    }
}




StationInfo select_station(const QString username)
{
    auto ssform = std::make_unique<SelectStationForm>(username);

    if (ssform->exec() > 0)
    {
        return  ssform->selected_station();

    }else{

        return StationInfo();
    }
}



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

    //int station_id = 17;
    //LoginForm lf("nbohr", "abc123", station_id);

    LoginForm lf;

    if (lf.exec() > 0)
    {
        Credentials cred = lf.credentials();

        if (!cluster_server_authentication(cred.username, cred.password)) {
            QString msg = QString("User %1 failed to authenticate in the cluster server.").arg(cred.username);
            showQMessage(msg,  QMessageBox::Critical);
            Logger::error(module, msg);
            return 0;
        }

        QString msg = QString("Cluster server authentication for user %1... Successful.").arg(cred.username);
        Logger::info(module, msg);

        StationInfo station_info = select_station(cred.username);

        ConnInfo conn_info;
        conn_info.host = station_info.ip_address.toStdString();
        conn_info.port = station_info.port_no;
        conn_info.db_name = station_info.db_name.toStdString();
        conn_info.username = cred.username.toStdString();
        conn_info.password = cred.password.toStdString();

        Authentication* auth = new Authentication(conn_info);
        auth->connect_to_server();


        //auto station_info =   lf.get_station_info();
        //auto conn_info = lf.get_connection_info();

        QString msg1 = QObject::tr("User connected to server `%1`").
                      arg(station_info.server_name);

        Logger::info(module, msg1);

        MainWindow w(&a, station_info, conn_info);
        w.resize(1300, 700);
        w.show();
        return a.exec();
    }

    return 1;

    //}
    //return 0;
}
