#include "mainwindow.h"

#include <QApplication>
#include <QDate>
#include <QDir>

#include "../../../rave/security/loginform.h"
#include "../../../rave/security/authentication.h"
#include "../../../rave/security/user.h"
#include "../../../rave/security/loginform.h"

#include "../../../rave/framework/logger.h"


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

    LoginForm lf("nbohr", "abc123", 4);
    if (lf.exec() > 0)
    {
        auto station_info = lf.get_station_info();
        auto conn_info = lf.get_connection_info();

        QString msg = QObject::tr("User connected to server `%1`").
                      arg(station_info.server_name);
        Logger::info(module, msg);

        MainWindow w(&a, station_info, conn_info);
        w.resize(1300, 700);
        w.show();
        return a.exec();
    }

    return 1;

    //}
    //return 0;
}
