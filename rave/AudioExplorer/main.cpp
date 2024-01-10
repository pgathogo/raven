#include "mainwindow.h"

#include <QApplication>

#include "../security/authentication.h"
#include "../security/loginform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginForm lf("nbohr", "abc123", 4);
    if (lf.exec() > 0)
    {
        auto station_info = lf.get_station_info();
        auto conn_info = lf.get_connection_info();

        MainWindow w(&a, station_info, conn_info);
        w.show();
        return a.exec();
    }

    return 1;
}
