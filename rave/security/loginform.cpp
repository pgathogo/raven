#include <QCloseEvent>
#include "loginform.h"
#include "ui_loginform.h"

#include "../utils/notificationbar.h"
#include "../framework/ravenexception.h"
#include "user.h"
#include "authentication.h"


LoginForm::LoginForm(Authentication* auth, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginForm),
    mAuth{auth},
    mNoticeBar{},
    mOkClose{true}
{
    ui->setupUi(this);
    connect(ui->btnSignIn, &QPushButton::clicked, this, &LoginForm::signIn);
    connect(ui->btnCancel, &QPushButton::clicked, this, &LoginForm::cancel);
    mNoticeBar = new NotificationBar(ui->vlNotice);
}

LoginForm::~LoginForm()
{
    delete ui;
    delete mNoticeBar;
}

void LoginForm::closeEvent(QCloseEvent *event)
{
    done(0);
    if(!mOkClose)
        event->ignore();
}

bool LoginForm::validNamePass()
{
    if (ui->edtUsername->text().isEmpty())
        return false;

    if (ui->edtPassword->text().isEmpty())
        return false;

    return true;
}

void LoginForm::signIn()
{
    if (!validNamePass()){
        mNoticeBar->errorNotification("Wrong username or password!");
        mOkClose = false;
        return;
    }

    try{
        mAuth->connect(ui->edtUsername->text().toStdString(),
                       ui->edtPassword->text().toStdString());

        // Give user "postgres" all access rights.
        mAuth->access_controller(ui->edtUsername->text().toStdString());
        mOkClose = true;
        done(1);
    }catch(PostgresException& pe){
        mNoticeBar->errorNotification(pe.errorMessage());
        mOkClose = false;
    }

}

void LoginForm::cancel()
{
    done(0);
}

