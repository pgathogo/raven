#include <memory>
#include "mainwindow.h"

#include <QApplication>
#include "../security/authentication.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto auth = std::make_unique<Authentication>();
    auth->connect("postgres", "abc123");
    MainWindow w(&a);
    w.show();
    return a.exec();
}
