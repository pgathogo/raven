#include "userbrowser.h"
#include "ui_userbrowser.h"

#include "user.h"
#include "userform.h"

UserBrowser::UserBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent,
                         std::make_unique<User>()),
    ui(new Ui::UserBrowser)

{
    ui->setupUi(this);
    setDialogTitle("System Users");
}


UserBrowser::~UserBrowser()
{
    delete ui;
}

void UserBrowser::addRecord()
{
    rawAdd<User, UserForm>();

}

void UserBrowser::updateRecord()
{
    rawUpdate<User, UserForm>();

}

void UserBrowser::deleteRecord()
{
    rawDelete<User>();

}
