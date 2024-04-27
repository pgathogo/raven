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

// #define LOG_TO_FILE

QString module = "Main";

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

    /*
    QString msg = "MsgHeader; MsgBody";
    QStringList msg_list = msg.split(';');
    QString module = msg_list[0];
    QString msg_body = msg_list[1];

    qDebug() << "Module: "<< module;
    qDebug() << "Body:  "<< msg_body;
    */


    Logger::info(module, "Opening login form...");

    LoginForm lf("nbohr", "abc123", 4);

    Logger::info(module, "User logged in...");

    lf.express_login();
    {
        Logger::info(module, "Starting MainWindow...");

        MainWindow w;
        w.show();
        bool state = a.exec();

    #ifdef LOG_TO_FILE
        Logger::clean();
    #endif

    return state;

    }

}
