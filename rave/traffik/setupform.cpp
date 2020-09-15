#include <algorithm>
#include "setupform.h"
#include "ui_setupform.h"
#include "ui_baseentitydetaildlg.h"

#include "traffiksetup.h"
#include "../framework/choicefield.h"
#include "orderapprover.h"
#include "approverform.h"


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


    connect(ui->btnAdd, &QPushButton::clicked, this, &SetupForm::addApprover);
    connect(ui->btnDelete, &QPushButton::clicked, this, &SetupForm::deleteApprover);

    ui->tvApprovers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    loadOrderApprovers();
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
    mSetup->setStationName(ui->edtStationName->text().toStdString());
    mSetup->setAddress1(ui->edtAddress1->text().toStdString());
    mSetup->setAddress2(ui->edtAddress2->text().toStdString());

    mSetup->setAgencyComm(ui->sbAgencyComm->value());
    mSetup->setSaleRepComm(ui->sbSaleRepComm->value());

    mSetup->setLateFee(ui->edtLateFee->value());
    mSetup->setInterestRate(ui->edtIntRate->value());
    mSetup->setGracePeriod(ui->edtGracePeriod->value());
    mSetup->setOrderApprovalLevels(ui->sbAprvLevels->value());

    mSetup->setOrderNumberSequence(ui->sbOrderSequence->value());

    mSetup->setBreakTimeInterval(ui->sbTimeInterval->value());
    mSetup->setBreakDuration(ui->sbBreakDuration->value());

    mSetup->setBreakMaxSpots(ui->edtMaxSpots->value());
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
    ui->sbAprvLevels->setValue(mSetup->orderApprovalLevels()->value());
    ui->sbOrderSequence->setValue(mSetup->orderNumberSequence()->value());

    ui->sbTimeInterval->setValue(mSetup->breakTimeInterval()->value());
    ui->sbBreakDuration->setValue(mSetup->breakDuration()->value());
    ui->edtMaxSpots->setValue(mSetup->breakMaxSpots()->value());
}

void SetupForm::populateChoiceCombo(QComboBox* cbox, const ChoiceField<std::string>* cf)
{
    for (const auto& c : cf->choices())
        cbox->addItem(stoq(std::get<1>(c)), stoq(std::get<0>(c)));

    cbox->setCurrentIndex( cbox->findData(QVariant(stoq(cf->value()))) );
}

void SetupForm::loadOrderApprovers()
{
    edmApprover = std::make_unique<EntityDataModel>(
                std::make_unique<OrderApprover>());
    edmApprover->all();
    ui->tvApprovers->setModel(edmApprover.get());

}

std::set<int> SetupForm::make_aprv_levels()
{
    std::set<int> levels;
    for(int i=1; i<=ui->sbAprvLevels->value(); ++i)
        levels.insert(i);

        for (auto& [name, entity] : edmApprover->modelEntities()){
            OrderApprover* op = dynamic_cast<OrderApprover*>(entity.get());
            if (levels.find(op->level()->value()) != levels.end() ){
                levels.erase(op->level()->value());
            }
        }

        return levels;
}

void SetupForm::saveApprovers()
{
    EntityDataModel edm(std::make_unique<OrderApprover>());
    for (auto& [name, entity] : edmApprover->modelEntities()){
        if (entity->dbAction() == DBAction::dbaCREATE){
            edm.createEntityDB(*entity);
        }
        if (entity->dbAction() == DBAction::dbaDELETE){
            edm.deleteEntity(*entity);
        }
    }
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

void SetupForm::addApprover()
{
    if (ui->sbAprvLevels->value() > 0 &&
            edmApprover->count() < ui->sbAprvLevels->value() ) {

        auto approver = std::make_unique<OrderApprover>();

        auto levels = make_aprv_levels();

        auto aprvForm = std::make_unique<ApproverForm>(
                    approver.get(), levels,
                    edmApprover->keys());

        if (aprvForm->exec() > 0)
            edmApprover->cacheEntity(std::move(approver));
    }else{
        showMessage("Approval Levels not set!");
    }

}

void SetupForm::deleteApprover()
{
    if (confirmationMessage("Delete Approver?")) {
        auto entity = findSelectedEntity();
        entity->setDBAction(DBAction::dbaDELETE);
        ui->tvApprovers->model()->removeRow(selectedRowId());
    }

}

BaseEntity* SetupForm::findSelectedEntity()
{
   std::string searchName = selectedRowName().toStdString();
   BaseEntity* entity = edmApprover->findEntityByName(searchName);
   return entity;
}

QString SetupForm::selectedRowName()
{

    QVariant col_name = ui->tvApprovers->model()->data(
                            ui->tvApprovers->model()->index(
                                    selectedRowId(), 0));
    return col_name.toString();
}

int SetupForm::selectedRowId() const
{
    return ui->tvApprovers->currentIndex().row();
}
