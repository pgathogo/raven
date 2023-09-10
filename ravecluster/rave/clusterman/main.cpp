#include "mainwindow.h"

#include <QApplication>

#include "../../../security/authentication.h"
#include "../../../security/user.h"

#include "../../../rave/framework/ravenexception.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    auto auth = std::make_unique<Authentication>();

    try{
        auth->connect_to_cluster_server("postgres", "abc123");
    } catch(DatabaseException& de) {
        showMessage(de.errorMessage());
    }

    MainWindow w;
    w.show();
    return a.exec();
}
