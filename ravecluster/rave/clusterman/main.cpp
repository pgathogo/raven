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

    LoginForm lf("postgres", "abc123");

    if (lf.exec() >  0)
    {
        ClusterManagerDlg cmd;
        cmd.show();
        return a.exec();

    }

}
