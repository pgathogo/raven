#include "approverform.h"
#include "ui_approverform.h"
#include "orderapprover.h"
#include "../framework/ui_baseentitydetaildlg.h"
#include "../security/user.h"

namespace TRAFFIK {

    ApproverForm::ApproverForm(OrderApprover* orderAprv,
                              std::set<int>& levels,
                              const std::list<std::string> approvers,
                               QDialog *parent) :
        BaseEntityDetailDlg(parent),
        ui(new Ui::ApproverForm),
        mOrderAprv{ orderAprv },
        mApprovers{ approvers }
    {
        ui->setupUi(bui->baseContainer);
        setTitle(windowTitle());
        populateFormWidgets();

        populate_levels(levels);

        connect(ui->cbUsers, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &ApproverForm::userCBChanged);
    }

    ApproverForm::~ApproverForm()
    {
        delete ui;
    }

    ActionResult ApproverForm::saveRecord()
    {
        populateEntityFields();
        if (approver_exist(ui->cbUsers->currentText().toStdString()) ){
            ActionResult ar =
                    std::make_tuple(ActionResultType::arERROR,
                                    "Approver already exists!");
            return ar;
        }

        return mOrderAprv->validate();
    }

    std::string ApproverForm::windowTitle()
    {
        return "Order Approver Details";
    }

    void ApproverForm::populateEntityFields()
    {
        mOrderAprv->userTitle()->setValue(ui->edtTitle->text().toStdString());
        mOrderAprv->setUserName(ui->cbUsers->currentText().toStdString());
        mOrderAprv->level()->setValue(
                    ui->cbLevels->itemData(
                        ui->cbLevels->currentIndex()).value<QVariant>().toInt());
        mOrderAprv->setDBAction(DBAction::dbaCREATE);
    }

    void ApproverForm::populateFormWidgets()
    {
        ui->cbUsers->setModel(mOrderAprv->userId()->dataModel());
        ui->edtTitle->setText(stoq(mOrderAprv->userTitle()->value()));
        //ui->sbLevel->setValue(mOrderAprv->level()->value());
    }

    void ApproverForm::clear_widgets()
    {
        ui->cbUsers->setCurrentIndex(-1);
        ui->edtTitle->clear();
    }

    void ApproverForm::populate_levels(std::set<int> &levels)
    {
        for ( int l : levels){
            ui->cbLevels->addItem("Level-"+stoq(std::to_string(l)),
                                  QVariant(l));
        }
    }

    bool ApproverForm::approver_exist(std::string user)
    {
        return (std::find(mApprovers.begin(),
                          mApprovers.end(),
                          user) != mApprovers.end() ) ? true : false;
    }

    void ApproverForm::userCBChanged(int i)
    {
        if (i<0)
            return;

        EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbUsers->model());
        SECURITY::User* user = dynamic_cast<SECURITY::User*>(std::get<1>(*(edm->vecBegin()+i)).get());
        mOrderAprv->userId()->setValue(user->oid()->value());
    }

}
