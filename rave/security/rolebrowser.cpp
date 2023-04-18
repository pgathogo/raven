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

}

void RoleBrowser::deleteRecord()
{
    rawDelete<Role>();
}

void RoleBrowser::searchRecord()
{
    if (bui->edtFilter->text().isEmpty()){
        Role* role = dynamic_cast<Role*>(entityDataModel().getEntity().get());
        auto si = searchItem(role->roleCanLogin()->dbColumnName(), "=", false);
        std::string filter = entityDataModel().prepareFilter(si);
        entityDataModel().search(filter);
    }else{
        auto data = bui->cbFilter->itemData(
                            bui->cbFilter->currentIndex()).value<QVariant>();
        std::string columnName = data.toString().toStdString();
        std::string item = bui->edtFilter->text().toStdString();

        auto role_name = std::make_tuple(columnName, "=", item);

        Role* role = dynamic_cast<Role*>(entityDataModel().getEntity().get());
        auto role_filter = searchItem(role->roleCanLogin()->dbColumnName(), "=", false);

        std::string filter = entityDataModel().prepareFilter(role_name, role_filter);
        entityDataModel().search(filter);
    }

}


