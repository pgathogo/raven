#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDate>
#include <QDateTime>
#include <QDir>
#include <QIODevice>
#include <QSettings>

#include "../framework/Logger.h"

#include "../security/authentication.h"
#include "../security/loginform.h"
#include "../security/selectstationform.h"
#include "../framework/ravenexception.h"

// #define LOG_TO_FILE

QString module = "Main";

const std::unique_ptr<Authentication> cluster_server_authentication(QString username, QString password)
{
    auto app_auth = std::make_unique<Authentication>();

    try{
        app_auth->connect_to_cluster_server(username.toStdString(),password.toStdString() );
        return std::move(app_auth);
    }catch(DatabaseException& de){
        std::cout << de.errorMessage();
        //mNoticeBar->errorNotification(de.errorMessage());
        return nullptr;
    }

}

Credentials get_credentials_locally()
{
    Credentials cred;

    QSettings registry(R"(HKEY_LOCAL_MACHINE\SOFTWARE\Baryonic\Raven\OATS)", QSettings::NativeFormat);

    if (registry.childKeys().contains("username", Qt::CaseInsensitive))
        cred.username = registry.value("username").toString();

    if (registry.childKeys().contains("password", Qt::CaseInsensitive))
        cred.password = registry.value("password").toString();

    if (registry.childKeys().contains("station_code", Qt::CaseInsensitive))
        cred.station_code = registry.value("station_code").toString();


    return cred;

}


int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

#ifdef LOG_TO_FILE

    QString log_path = QCoreApplication::applicationDirPath()+"/log";

    QDir log_dir(log_path);
    if (!log_dir.exists()){
        log_dir.mkpath(log_path);
    }

    QString cur_dtime = QDateTime::currentDateTime().toString("ddMMyyyy");
    QString log_filepath = log_path+"/oatslog_"+cur_dtime+".log";

    Logger::init(log_filepath);

#endif


    QFile qss("stylesheet.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());

    Credentials cred = get_credentials_locally();

    const auto auth = cluster_server_authentication(cred.username, cred.password);

    if (auth == nullptr) {
        QString msg = QString("User %1 failed to authenticate in the cluster server.").arg(cred.username);
        Logger::error(module, msg);
        return 0;
    }

    QString msg = QString("Cluster server authentication for user %1... Successful.").arg(cred.username);
    Logger::info(module, msg);

    StationInfo station_info = auth->station_by_station_code(cred.station_code);

    if (station_info.db_name == "") {
        QString msg = QString("No station with code `%1` found").arg(cred.station_code);
        Logger::info(module, msg);
        return 0;
    }

    ConnInfo conn_info;
    conn_info.host = station_info.ip_address.toStdString();
    conn_info.port = station_info.port_no;
    conn_info.db_name = station_info.db_name.toStdString();
    conn_info.username = cred.username.toStdString();
    conn_info.password = cred.password.toStdString();

    Authentication* station_auth = new Authentication(conn_info);
    station_auth->connect_to_server();

    QString auth_msg = QString("Connected to host: %1, port: %2  database: %3 ")
                           .arg(QString::fromStdString(conn_info.host))
                           .arg(QString::number(conn_info.port))
                           .arg(QString::fromStdString(conn_info.db_name));
    Logger::info(module, auth_msg);

    Logger::info(module, "Opening main OATS window...");

    MainWindow w;
    w.show();
    bool state = a.exec();

    #ifdef LOG_TO_FILE
        Logger::clean();
    #endif

    return state;

    //}

}
