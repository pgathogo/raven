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

    connect(ui->cbAgencyCommType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SetupForm::agencyCommTypeChanged);

    connect(ui->cbSaleRepCommType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SetupForm::saleRepCommTypeChanged);

    connect(ui->cbBillCycle, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SetupForm::billCycleChanged);

    connect(ui->cbRevenueType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SetupForm::revenueTypeChanged);

    connect(ui->cbBillingType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SetupForm::billingTypeChanged);

    connect(ui->cbBillingBasis, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SetupForm::billingBasisChanged);

    hideSaveNewBtn();

    ui->tabWidget->setCurrentIndex(0);
}

SetupForm::~SetupForm()
{
    delete ui;
}

ActionResult SetupForm::saveRecord()
{
    populateEntityFields();
    return mSetup->validate();
}

std::string SetupForm::windowTitle()
{
    return "Traffik Setup";
}

void SetupForm::populateEntityFields()
{
    mSetup->stationName()->setValue(ui->edtStationName->text().toStdString());
    mSetup->address1()->setValue(ui->edtAddress1->text().toStdString());
    mSetup->address2()->setValue(ui->edtAddress2->text().toStdString());

    mSetup->agencyComm()->setValue(ui->sbAgencyComm->value());
    mSetup->saleRepComm()->setValue(ui->sbSaleRepComm->value());

    mSetup->lateFee()->setValue(ui->edtLateFee->value());
    mSetup->interestRate()->setValue(ui->edtIntRate->value());
    mSetup->gracePeriod()->setValue(ui->edtGracePeriod->value());
    mSetup->orderApprovalLevels()->setValue(ui->edtAprvLevels->value());
    mSetup->maxBreakSpots()->setValue(ui->edtMaxSpots->value());

    mSetup->orderNumberSequence()->setValue(ui->sbOrderSequence->value());
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
    populateChoiceCombo(ui->cbRevenueType, mSetup->revenueType());
    populateChoiceCombo(ui->cbBillingType, mSetup->billingType());
    populateChoiceCombo(ui->cbBillingBasis, mSetup->billingBasis());

    ui->edtLateFee->setValue(mSetup->lateFee()->value());
    ui->edtIntRate->setValue(mSetup->interestRate()->value());
    ui->edtGracePeriod->setValue(mSetup->gracePeriod()->value());
    ui->edtAprvLevels->setValue(mSetup->orderApprovalLevels()->value());
    ui->edtMaxSpots->setValue(mSetup->maxBreakSpots()->value());
    ui->sbOrderSequence->setValue(mSetup->orderNumberSequence()->value());
}

void SetupForm::populateChoiceCombo(QComboBox* cbox, const ChoiceField<std::string>* cf)
{
    for (const auto& c : cf->choices())
        cbox->addItem(stoq(std::get<1>(c)), stoq(std::get<0>(c)));

    cbox->setCurrentIndex( cbox->findData(QVariant(stoq(cf->value()))) );
}

void SetupForm::agencyCommTypeChanged(int i)
{
    mSetup->agencyCommType()->setValue(
                ui->cbAgencyCommType->itemData(i).toString().toStdString());
}

void SetupForm::saleRepCommTypeChanged(int i)
{
    mSetup->saleRepCommType()->setValue(
                ui->cbSaleRepCommType->itemData(i).toString().toStdString());
}

void SetupForm::billCycleChanged(int i)
{
    mSetup->billingCycle()->setValue(
                ui->cbBillCycle->itemData(i).toString().toStdString());
}

void SetupForm::revenueTypeChanged(int i)
{
    mSetup->revenueType()->setValue(
                ui->cbRevenueType->itemData(i).toString().toStdString());

}

void SetupForm::billingTypeChanged(int i)
{
    mSetup->billingType()->setValue(
                ui->cbBillingType->itemData(i).toString().toStdString());

}

void SetupForm::billingBasisChanged(int i)
{
    mSetup->billingBasis()->setValue(
                ui->cbBillingBasis->itemData(i).toString().toStdString());
}
