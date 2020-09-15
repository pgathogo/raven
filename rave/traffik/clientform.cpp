#include <QDebug>
#include "clientform.h"
#include "ui_clientform.h"
#include "ui_baseentitydetaildlg.h"
#include "../utils/tools.h"

#include "client.h"
#include "../framework/entitydatamodel.h"
#include "../framework/choicefield.h"

ClientForm::ClientForm(Client* client,
                       QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::ClientForm)
  ,mClient{client}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    ui->twClient->setCurrentIndex(0);
    populateFormWidgets();
    //connect(ui->cbSalute, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
    //        [=](int i){saluteComboChanged(i);});

    connect(ui->cbSalute, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ClientForm::saluteComboChanged);

    connect(ui->cbRevenueType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ClientForm::revenueTypeComboChanged);

    connect(ui->cbBillCycle, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ClientForm::billCycleComboChanged);

    connect(ui->cbAgency, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ClientForm::agentComboChanged);

    connect(ui->cbClientGroup, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ClientForm::clientGroupChanged);

    connect(ui->cbAccountManager, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ClientForm::accountManagerChanged);

}

ClientForm::~ClientForm()
{
    delete ui;
}

ActionResult ClientForm::saveRecord()
{
    populateEntityFields();
    ActionResult ar = mClient->validate();
    return ar;
}

void ClientForm::populateFormWidgets()
{

    ui->edtName->setText(stoq(mClient->name()->value()));
    ui->edtBillName->setText(stoq(mClient->billName()->value()));
    ui->edtAddress2->setText(stoq(mClient->address2()->value()));
    ui->edtPostNo->setText(stoq(mClient->postalNo()->value()));
    ui->edtAddress1->setText(stoq(mClient->address1()->value()));
    ui->edtTown->setText(stoq(mClient->town()->value()));
    ui->edtEmail->setText(stoq(mClient->clientEmail()->value()));
    ui->edtPostCode->setText(stoq(mClient->postalCode()->value()));
    ui->edtTele->setText(stoq(mClient->telephone()->value()));
    ui->edtMobile->setText(stoq(mClient->clientMobile()->value()));

    ui->edtContactName->setText(stoq(mClient->contactName()->value()));
    ui->edtContactEmail->setText(stoq(mClient->contactEmail()->value()));
    ui->edtContactMobile->setText(stoq(mClient->contactMobile()->value()));

    ui->sbAgencyComm->setValue(mClient->agencyComm()->value());
    ui->spSaleRepComm->setValue(mClient->saleRepComm()->value());

    ui->spIntRate->setValue(mClient->interestRate()->value());
    ui->spGracePeriod->setValue(mClient->gracePeriod()->value());
    ui->spLateFee->setValue(mClient->lateFee()->value());
    ui->sbDiscount->setValue(mClient->discountPercent()->value());

    populateChoiceCombo(ui->cbSalute, mClient->contactSalute());
    populateChoiceCombo(ui->cbRevenueType, mClient->revenueType());
    populateChoiceCombo(ui->cbBillCycle, mClient->billCycle());

    ui->cbAgency->setModel(mClient->agency()->dataModel());
    ui->cbAgency->setCurrentIndex(ui->cbAgency->findText(
                                 stoq(mClient->agency()->displayName())));

    ui->cbClientGroup->setModel(mClient->clientGroup()->dataModel());
    ui->cbClientGroup->setCurrentIndex(ui->cbClientGroup->findText(
                                 stoq(mClient->clientGroup()->displayName())));

    ui->cbAccountManager->setModel(mClient->accountManager()->dataModel());
    ui->cbAccountManager->setCurrentIndex(ui->cbAccountManager->findText(
                                 stoq(mClient->accountManager()->displayName())));

}

void ClientForm::populateChoiceCombo(QComboBox* cbox, const ChoiceField<std::string>* cf)
{
    for (const auto& c : cf->choices())
        cbox->addItem(stoq(std::get<1>(c)), stoq(std::get<0>(c)));

    cbox->setCurrentIndex( cbox->findData(QVariant(stoq(cf->value()))) );
}

void ClientForm::populateEntityFields()
{
    mClient->setName(ui->edtName->text().toStdString());
    mClient->setBillName(ui->edtBillName->text().toStdString());
    mClient->setAddress2(ui->edtAddress2->text().toStdString());
    mClient->setPostalNo(ui->edtPostNo->text().toStdString());
    mClient->setAddress1(ui->edtAddress1->text().toStdString());
    mClient->setTown(ui->edtTown->text().toStdString());
    mClient->setClientEmail(ui->edtEmail->text().toStdString());
    mClient->setPostalCode(ui->edtPostCode->text().toStdString());
    mClient->setTelephone(ui->edtTele->text().toStdString());
    mClient->setClientMobile(ui->edtMobile->text().toStdString());
    mClient->setContactName(ui->edtContactName->text().toStdString());
    mClient->setContactEmail(ui->edtContactEmail->text().toStdString());
    mClient->setContactMobile(ui->edtContactMobile->text().toStdString());

    mClient->setAgencyComm(ui->sbAgencyComm->value());
    mClient->setSaleRepComm(ui->spSaleRepComm->value());
    mClient->setInterestRate(ui->spIntRate->value());
    mClient->setGracePeriod(ui->spGracePeriod->value());
    mClient->setLateFee(ui->spLateFee->value());
    mClient->setDiscountPercent(ui->sbDiscount->value());
}

std::string ClientForm::windowTitle()
{
    return "Client Details";
}

void ClientForm::saluteComboChanged(int i)
{
    mClient->setContactSalute(
                ui->cbSalute->itemData(i).toString().toStdString());
}

void ClientForm::revenueTypeComboChanged(int i)
{
    mClient->setRevenueType(
                ui->cbRevenueType->itemData(i).toString().toStdString());
}

void ClientForm::billCycleComboChanged(int i)
{
    mClient->setBillCycle(
                ui->cbBillCycle->itemData(i).toString().toStdString());
}

void ClientForm::agentComboChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbAgency->model());
    mClient->agency()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}

void ClientForm::clientGroupChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbClientGroup->model());
    mClient->clientGroup()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}

void ClientForm::accountManagerChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbAccountManager->model());
    mClient->accountManager()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}
