#include "roleform.h"
#include "ui_roleform.h"
#include "../framework/ui_baseentitydetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/entitydatamodel.h"
#include "../framework/manytomanybrowser.h"
#include "../utils/tools.h"

#include "role.h"
#include "user.h"
#include "rolemember.h"

RoleForm::RoleForm(SECURITY::Role* role, QDialog* parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::RoleForm),
    mRole{role}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());

    connect(ui->cbExpiry, &QCheckBox::stateChanged, this, &RoleForm::updateValidity);

    populateFormWidgets();

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
    mRole->set_role_name(ui->edtRoleName->text().toStdString());
    mRole->set_valid_until(ui->edtValidUntil->date().toString("yyyy-MM-dd").toStdString());

   // mRole->setComment(ui->edtComment->text().toStdString());
    //bool canExpire = [&](){ return (ui->cbExpiry->checkState()==Qt::Checked) ? true : false; }();
    //mRole->setRoleExpire(canExpire);
}

void RoleForm::populateFormWidgets()
{
    ui->edtRoleName->setText(stoq(mRole->role_name()->value()));
    ui->edtRoleName->setEnabled(mRole->role_name()->value().empty());

    //ui->edtComment->setText(stoq(mRole->comment()->value()));

    if (!mRole->valid_until()->value().empty() &&
            mRole->valid_until()->value() != "infinity"){

        auto [y, m, d] = ymd(mRole->valid_until()->value());

        QDate qdate = QDate(y, m, d);

        ui->edtValidUntil->setDate(qdate);
        ui->edtValidUntil->setEnabled(true);
    }

    if (mRole->valid_until()->value() == "infinity" ||
                mRole->valid_until()->value().empty()){
            ui->cbExpiry->setCheckState(Qt::Checked);
        }
}

ActionResult RoleForm::saveRecord()
{
    populateEntityFields();

    ActionResult ar = std::make_tuple(ActionResultType::arSUCCESS, "");

    qDebug() << "SAVE RECORD: "<< stoq(mRole->role_name()->valueToString());

    ar = mRole->validate();
    auto [result_type, msg] = ar;
    if (result_type != ActionResultType::arSUCCESS)
            return ar;

    if (ui->edtRoleName->isEnabled()){
        try{
            EntityDataModel edm = EntityDataModel(std::make_unique<SECURITY::Role>());
            auto role_filter = std::make_tuple(mRole->role_name()->dbColumnName(),
                               "like",
                               mRole->role_name()->value());
            std::string filter = edm.prepareFilter(role_filter);
            edm.search(filter);

            if (edm.entitiesCount() > 0){
                ar = std::make_tuple(ActionResultType::arERROR,
                         "The role name is already in use!");
                return ar;
             }
        } catch(DatabaseException& de) {
            showMessage(de.errorMessage());
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

std::vector<EntityRecord> const& RoleForm::role_members() const
{
    return  mMtoMUserBrowser->entityDataModel().modelEntities();
}
