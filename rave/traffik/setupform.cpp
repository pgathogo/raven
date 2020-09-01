#include "setupform.h"
#include "ui_setupform.h"
#include "ui_baseentitydetaildlg.h"

#include "traffiksetup.h"
#include "../framework/choicefield.h"

SetupForm::SetupForm(TraffikSetup* setup,
                     QDialog* parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::SetupForm),
    mSetup{ setup }
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    populateFormWidgets();

    hideSaveNewBtn();
}

SetupForm::~SetupForm()
{
    delete ui;
}

ActionResult SetupForm::saveRecord()
{
    return mSetup->validate();
}

std::string SetupForm::windowTitle()
{
    return "Traffik Setup";
}

void SetupForm::populateEntityFields()
{

}

void SetupForm::populateFormWidgets()
{
    ui->edtStationName->setText(stoq(mSetup->stationName()->value()));
    ui->edtAddress1->setText(stoq(mSetup->address1()->value()));
    ui->edtAddress2->setText(stoq(mSetup->address2()->value()));

    ui->sbAgencyComm->setValue(mSetup->agencyComm()->value());
    populateChoiceCombo(ui->cbAgencyCommType, mSetup->agencyCommType());

    ui->sbSaleRepComm->setValue(mSetup->saleRepComm()->value());
    populateChoiceCombo(ui->cbSaleRepCommType, mSetup->saleRepCommType());

    populateChoiceCombo(ui->cbBillCycle, mSetup->billingCycle());

    ui->edtLateFee->setValue(mSetup->lateFee()->value());
    ui->edtIntRate->setValue(mSetup->interestRate()->value());
    ui->edtGracePeriod->setValue(mSetup->gracePeriod()->value());
    ui->edtAprvLevels->setValue(mSetup->orderApprovalLevels()->value());
    ui->edtMaxSpots->setValue(mSetup->maxBreakSpots()->value());

}

void SetupForm::populateChoiceCombo(QComboBox* cbox, const ChoiceField<std::string>* cf)
{
    for (const auto& c : cf->choices())
        cbox->addItem(stoq(std::get<1>(c)), stoq(std::get<0>(c)));

    cbox->setCurrentIndex( cbox->findData(QVariant(stoq(cf->value()))) );
}
