#include "rolebrowser.h"
#include "ui_rolebrowser.h"

#include "role.h"
#include "roleform.h"
#include "user.h"
#include "userbrowser.h"
#include "../framework/manytomany.h"

RoleBrowser::RoleBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent,
                         std::make_unique<Role>()),
    ui(new Ui::RoleBrowser)
{
    ui->setupUi(this);
    setDialogTitle("System Roles");
}

RoleBrowser::~RoleBrowser()
{
    delete ui;
}

void RoleBrowser::addRecord()
{
    rawAdd<Role, RoleForm>();
}

void RoleBrowser::updateRecord()
{
    rawUpdate<Role, RoleForm, ManyToMany>();

    /*
    BaseEntity* entity = findSelectedEntity();
    Role* role = dynamic_cast<Role*>(entity);

    std::unique_ptr<RoleForm> form =
            std::make_unique<RoleForm>(role, this);

    if (form->exec() > 0){
        std::string alter_stmt =
                role->make_alter_stmt(role->displayName());
        try{
            entityDataModel().executeRawSQL( alter_stmt );
            updateTableViewRecord(role->tableViewValues());
            entityDataModel().updateEntity(*role);

        }catch (DatabaseException& de){
            showMessage(de.errorMessage());
        }
    }
    */
}

void RoleBrowser::deleteRecord()
{
    rawDelete<Role>();
}

void RoleBrowser::searchRecord()
{
    if (bui->edtFilter->text().isEmpty()){
        Role& role = dynamic_cast<Role&>(entityDataModel().getEntity());
        auto si = searchItem(role.roleCanLogin()->dbColumnName(), "=", false);
        std::string filter = entityDataModel().prepareFilter(si);
        entityDataModel().search(filter);
    }else{
        auto data = bui->cbFilter->itemData(
                            bui->cbFilter->currentIndex()).value<QVariant>();
        std::string columnName = data.toString().toStdString();
        std::string item = bui->edtFilter->text().toStdString();

        auto role_name = std::make_tuple(columnName, "=", item);

        Role& role = dynamic_cast<Role&>(entityDataModel().getEntity());
        auto role_filter = searchItem(role.roleCanLogin()->dbColumnName(), "=", false);

        std::string filter = entityDataModel().prepareFilter(role_name, role_filter);
        entityDataModel().search(filter);
    }

}


