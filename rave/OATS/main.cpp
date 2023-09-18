#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDate>
#include <QDateTime>
#include <QDir>
#include <QIODevice>

#include "../security/authentication.h"

#include "../framework/Logger.h"
#include "../security/loginform.h"

//#define LOG_TO_FILE

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


    LoginForm lf;
    if (lf.exec() > 0 )
    {
//	    auto auth = std::make_unique<Authentication>();
//	    auth->connect("postgres", "abc123");

//        auto station_info = lf.get_station_info();
//        auto conn_info = lf.get_connection_info();

        MainWindow w;
        w.show();
        bool state = a.exec();

    #ifdef LOG_TO_FILE
        Logger::clean();
    #endif
    return state;

    }

}
