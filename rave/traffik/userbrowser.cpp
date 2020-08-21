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

    /*
    std::unique_ptr<User> user = std::make_unique<User>();
    std::unique_ptr<UserForm> userForm =
            std::make_unique<UserForm>(user.get());

    if (userForm->exec() > 0)
    {
        std::string create_stmt = user->make_create_stmt();

        try{

            entityDataModel().executeRawSQL( create_stmt );
            entityDataModel().cacheEntity(std::move(user));

        }catch (DatabaseException& de){
            showMessage(de.errorMessage());
        }

    }
    */

}

void UserBrowser::updateRecord()
{
    rawUpdate<User, UserForm>();

    /*
   BaseEntity* entity = findSelectedEntity();
   User* user = dynamic_cast<User*>(entity);
    std::unique_ptr<UserForm> userForm =
            std::make_unique<UserForm>(user);
    if (userForm->exec() > 0)
    {
        std::string alter_stmt = user->make_alter_stmt(user->userName()->value());

        try{
            entityDataModel().executeRawSQL( alter_stmt );
            updateTableViewRecord(user->tableViewValues());
            entityDataModel().updateEntity(*user);

        }catch (DatabaseException& de){
            showMessage(de.errorMessage());
        }

    }
    */

}

void UserBrowser::deleteRecord()
{
    rawDelete<User>();

    /*
   BaseEntity* entity = findSelectedEntity();
   User* user = dynamic_cast<User*>(entity);

   std::string drop_stmt = user->make_drop_stmt(user->userName()->value());
    try{

        entityDataModel().executeRawSQL( drop_stmt );
        removeSelectedRow();

    }catch (DatabaseException& de){
        showMessage(de.errorMessage());
    }
    */
}
