#include "mainwindow.h"

#include <QApplication>

#include "../security/loginform.h"
#include "../security/authentication.h"
#include "../security/user.h"
#include "../security/loginform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //w.setWindowState(w.windowState() ^ Qt::WindowMaximized);
    //auto auth = std::make_unique<Authentication>();
    //auth->connect("postgres", "abc123");
    //LoginForm lf(auth.get());
    //if (lf.exec() > 0){

    LoginForm lf;
    if (lf.exec() > 0)
    {
        auto station_info = lf.get_station_info();
        auto conn_info = lf.get_connection_info();

        MainWindow w(&a, station_info, conn_info);
        w.resize(1300, 700);
        w.show();
        return a.exec();
    }

    return 1;

    //}
    //return 0;
}
