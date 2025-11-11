//#include "mainwindow.h"
#include "clustermanagerdlg.h"


#include <QApplication>

#include "../../../security/authentication.h"
#include "../../../security/user.h"
#include "../../../rave/security/loginform.h"

#include "../../../rave/framework/ravenexception.h"


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    LoginForm lf;

    if (lf.exec() >  0)
    {

        Credentials cred = lf.credentials();

        auto auth = std::make_unique<Authentication>();

        try{

            auth->connect_to_cluster_server(cred.username.toStdString(), cred.password.toStdString());


        } catch(DatabaseException& de) {
            showMessage(de.errorMessage());
        }

    }

//    MainWindow w;
//    w.show();
    ClusterManagerDlg cmd;
    cmd.show();
    return a.exec();
}
