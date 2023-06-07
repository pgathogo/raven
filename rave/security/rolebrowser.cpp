#include "rolebrowser.h"
#include "ui_rolebrowser.h"

#include "role.h"
#include "roleform.h"
#include "user.h"
#include "userbrowser.h"
#include "../framework/manytomany.h"

RoleBrowser::RoleBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent,
                           std::make_unique<SECURITY::Role>()),
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
    rawAdd<SECURITY::Role, RoleForm>();
}

void RoleBrowser::updateRecord()
{
    rawUpdate<SECURITY::Role, RoleForm, ManyToMany>();
}

void RoleBrowser::deleteRecord()
{
    rawDelete<SECURITY::Role>();
}

void RoleBrowser::searchRecord()
{
    if (bui->edtFilter->text().isEmpty()){
        SECURITY::Role* role = dynamic_cast<SECURITY::Role*>(entityDataModel().getEntity().get());
        auto si = searchItem(role->rol_can_login()->dbColumnName(), "=", false);
        std::string filter = entityDataModel().prepareFilter(si);
        entityDataModel().search(filter);
    }else{
        auto data = bui->cbFilter->itemData(
                            bui->cbFilter->currentIndex()).value<QVariant>();
        std::string columnName = data.toString().toStdString();
        std::string item = bui->edtFilter->text().toStdString();

        auto role_name = std::make_tuple(columnName, "=", item);

        SECURITY::Role* role = dynamic_cast<SECURITY::Role*>(entityDataModel().getEntity().get());
        auto role_filter = searchItem(role->rol_can_login()->dbColumnName(), "=", false);

        std::string filter = entityDataModel().prepareFilter(role_name, role_filter);
        entityDataModel().search(filter);
    }

}


