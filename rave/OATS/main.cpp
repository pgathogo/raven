#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include "../security/authentication.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qss("stylesheet.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());

    auto auth = std::make_unique<Authentication>();
    auth->connect("postgres", "abc123");
    MainWindow w;
    w.show();
    return a.exec();
}
