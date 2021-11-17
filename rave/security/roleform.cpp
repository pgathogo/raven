#include "roleform.h"
#include "ui_roleform.h"
#include "../framework/ui_baseentitydetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/entitydatamodel.h"
#include "../framework/manytomanybrowser.h"
#include "role.h"
#include "../utils/tools.h"

#include "user.h"
#include "rolemember.h"

RoleForm::RoleForm(Role* role, QDialog* parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::RoleForm),
    mRole{role}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());

    connect(ui->cbExpiry, &QCheckBox::stateChanged, this, &RoleForm::updateValidity);

    populateFormWidgets();

    //mMtoMUserBrowser = std::make_unique<ManyToManyBrowser>(&mRole->roleMember(),
    //                                                       ui->vlUsers,
    //                                                       this);

    mMtoMUserBrowser = createMtoM<ManyToManyBrowser>(&mRole->roleMember(),
                                                           ui->vlUsers,
                                                           this);
    ui->twRole->setCurrentIndex(0);
}

RoleForm::~RoleForm()
{
    delete ui;
}

std::string RoleForm::windowTitle()
{
    return "Role Details";
}

void RoleForm::populateEntityFields()
{
    mRole->setRoleName(ui->edtRoleName->text().toStdString());
    mRole->setComment(ui->edtComment->text().toStdString());
    mRole->setValidUntil(ui->edtValidUntil->date().toString("yyyy-MM-dd").toStdString());
    bool canExpire = [&](){ return (ui->cbExpiry->checkState()==Qt::Checked) ? true : false; }();
    mRole->setRoleExpire(canExpire);
}

void RoleForm::populateFormWidgets()
{
    ui->edtRoleName->setText(stoq(mRole->roleName()->value()));
    ui->edtRoleName->setEnabled(mRole->roleName()->value().empty());

    ui->edtComment->setText(stoq(mRole->comment()->value()));

    if (!mRole->validUntil()->value().empty() &&
            mRole->validUntil()->value() != "infinity"){

        auto [y, m, d] = ymd(mRole->validUntil()->value());

        QDate qdate = QDate(y, m, d);

        ui->edtValidUntil->setDate(qdate);
        ui->edtValidUntil->setEnabled(true);
    }

    if (mRole->validUntil()->value() == "infinity" ||
                mRole->validUntil()->value().empty()){
            ui->cbExpiry->setCheckState(Qt::Checked);
        }
}

ActionResult RoleForm::saveRecord()
{
    populateEntityFields();

    ActionResult ar = std::make_tuple(ActionResultType::arSUCCESS, "");
    ar = mRole->validate();
    auto [result_type, msg] = ar;
    if (result_type != ActionResultType::arSUCCESS)
            return ar;

    if (ui->edtRoleName->isEnabled()){
        EntityDataModel edm = EntityDataModel(std::make_unique<Role>());
        auto role_filter = std::make_tuple(mRole->roleName()->dbColumnName(),
                                           "=",
                                           mRole->roleName()->value());
        std::string filter = edm.prepareFilter(role_filter);
        edm.search(filter);

        if (edm.entitiesCount() > 0){
            ar = std::make_tuple(ActionResultType::arERROR,
                                 "The role name is already in use!");
            return ar;
         }
    }

    return ar;

}

int RoleForm::parentId() const
{
    return mRole->id();
}

void RoleForm::updateValidity(int state)
{
    if (state == Qt::Checked)
        ui->edtValidUntil->setEnabled(false);
    else
        ui->edtValidUntil->setEnabled(true);
}
