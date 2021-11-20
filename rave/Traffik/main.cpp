#include "mainwindow.h"

#include <QApplication>

#include "../security/loginform.h"
#include "../security/authentication.h"
#include "../security/user.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //w.setWindowState(w.windowState() ^ Qt::WindowMaximized);
    auto auth = std::make_unique<Authentication>();
    auth->connect("postgres", "abc123");
    //LoginForm lf(auth.get());
    //if (lf.exec() > 0){
        MainWindow w(std::move(auth));
        w.show();
        return a.exec();
    //}
    //return 0;
}
