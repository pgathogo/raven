#include "mainwindow.h"

#include <QApplication>

#include "../../../security/authentication.h"
#include "../../../security/user.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto auth = std::make_unique<Authentication>();
    auth->connect_cluster("postgres", "abc123");
    MainWindow w;
    w.show();
    return a.exec();
}
