#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include "../security/authentication.h"

#include "../framework/Logger.h"

//#define LOG_TO_FILE

int main(int argc, char *argv[])
{

#ifdef LOG_TO_FILE
    Logger::init();
#endif

    QApplication a(argc, argv);

    QFile qss("stylesheet.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());

    auto auth = std::make_unique<Authentication>();
    auth->connect("postgres", "abc123");

    MainWindow w;
    w.show();
    bool state = a.exec();

#ifdef LOG_TO_FILE
    Logger::clean();
#endif

    return state;

}
