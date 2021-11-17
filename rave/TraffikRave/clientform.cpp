#include <QDebug>
#include "clientform.h"
#include "ui_clientform.h"
#include "../framework/ui_baseentitydetaildlg.h"
#include "../utils/tools.h"

#include "client.h"
#include "traffiksetup.h"

#include "../framework/entitydatamodel.h"
#include "../framework/choicefield.h"

ClientForm::ClientForm(Client* client,
                       QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::ClientForm)
  ,m_client{client}
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

    m_edm_setup = std::make_unique<EntityDataModel>(
                std::make_unique<TraffikSetup>());
    m_edm_setup->all();

    if (m_edm_setup->count() > 0)
        m_traffik_setup = dynamic_cast<TraffikSetup*>(m_edm_setup->firstEntity());

    if (m_client->isNew())
        set_defaults();

}

ClientForm::~ClientForm()
{
    delete ui;
}

ActionResult ClientForm::saveRecord()
{
    populateEntityFields();
    ActionResult ar = m_client->validate();
    return ar;
}

void ClientForm::populateFormWidgets()
{

    ui->edtName->setText(stoq(m_client->name()->value()));
    ui->edtBillName->setText(stoq(m_client->bill_name()->value()));
    ui->edtAddress2->setText(stoq(m_client->address2()->value()));
    ui->edtPostNo->setText(stoq(m_client->postalNo()->value()));
    ui->edtAddress1->setText(stoq(m_client->address1()->value()));
    ui->edtTown->setText(stoq(m_client->town()->value()));
    ui->edtEmail->setText(stoq(m_client->client_email()->value()));
    ui->edtPostCode->setText(stoq(m_client->postal_code()->value()));
    ui->edtTele->setText(stoq(m_client->telephone()->value()));
    ui->edtMobile->setText(stoq(m_client->client_mobile()->value()));

    ui->edtContactName->setText(stoq(m_client->contact_name()->value()));
    ui->edtContactEmail->setText(stoq(m_client->contact_email()->value()));
    ui->edtContactMobile->setText(stoq(m_client->contact_mobile()->value()));

    ui->sbAgencyComm->setValue(m_client->agency_comm()->value());
    ui->sbSaleRepComm->setValue(m_client->sale_rep_comm()->value());

    ui->sbIntRate->setValue(m_client->interest_rate()->value());
    ui->sbGracePeriod->setValue(m_client->grace_period()->value());
    ui->sbLateFee->setValue(m_client->late_fee()->value());
    ui->sbDiscount->setValue(m_client->discount_percent()->value());

    populateChoiceCombo(ui->cbSalute, m_client->contact_salute());
    populateChoiceCombo(ui->cbRevenueType, m_client->revenue_type());
    populateChoiceCombo(ui->cbBillCycle, m_client->bill_cycle());

    ui->cbAgency->setModel(m_client->agency()->dataModel());
    ui->cbAgency->setCurrentIndex(ui->cbAgency->findText(
                                 stoq(m_client->agency()->displayName())));

    ui->cbClientGroup->setModel(m_client->group_manager()->dataModel());
    ui->cbClientGroup->setCurrentIndex(ui->cbClientGroup->findText(
                                 stoq(m_client->group_manager()->displayName())));

    ui->cbAccountManager->setModel(m_client->account_manager()->dataModel());
    ui->cbAccountManager->setCurrentIndex(ui->cbAccountManager->findText(
                                 stoq(m_client->account_manager()->displayName())));

}

void ClientForm::populateChoiceCombo(QComboBox* cbox, const ChoiceField<std::string>* cf)
{
    for (const auto& c : cf->choices())
        cbox->addItem(stoq(std::get<1>(c)), stoq(std::get<0>(c)));

    cbox->setCurrentIndex( cbox->findData(QVariant(stoq(cf->value()))) );
}

void ClientForm::populateEntityFields()
{
    m_client->set_name(ui->edtName->text().toStdString());
    m_client->set_bill_name(ui->edtBillName->text().toStdString());
    m_client->set_address2(ui->edtAddress2->text().toStdString());
    m_client->set_postal_no(ui->edtPostNo->text().toStdString());
    m_client->set_address1(ui->edtAddress1->text().toStdString());
    m_client->set_town(ui->edtTown->text().toStdString());
    m_client->set_client_email(ui->edtEmail->text().toStdString());
    m_client->set_postal_code(ui->edtPostCode->text().toStdString());
    m_client->set_telepone(ui->edtTele->text().toStdString());
    m_client->set_client_mobile(ui->edtMobile->text().toStdString());
    m_client->set_contact_name(ui->edtContactName->text().toStdString());
    m_client->set_contact_email(ui->edtContactEmail->text().toStdString());
    m_client->set_contact_mobile(ui->edtContactMobile->text().toStdString());

    m_client->set_agency_comm(ui->sbAgencyComm->value());
    m_client->set_sale_rep_comm(ui->sbSaleRepComm->value());
    m_client->set_interest_rate(ui->sbIntRate->value());
    m_client->set_grace_period(ui->sbGracePeriod->value());
    m_client->set_late_fee(ui->sbLateFee->value());
    m_client->set_discount_percent(ui->sbDiscount->value());
}

std::string ClientForm::windowTitle()
{
    return "Client Details";
}

void ClientForm::saluteComboChanged(int i)
{
    m_client->set_contact_salute(
                ui->cbSalute->itemData(i).toString().toStdString());
}

void ClientForm::revenueTypeComboChanged(int i)
{
    m_client->set_revenue_type(
                ui->cbRevenueType->itemData(i).toString().toStdString());
}

void ClientForm::billCycleComboChanged(int i)
{
    m_client->set_bill_cycle(
                ui->cbBillCycle->itemData(i).toString().toStdString());
}

void ClientForm::agentComboChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbAgency->model());
    m_client->agency()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}

void ClientForm::clientGroupChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbClientGroup->model());
    m_client->group_manager()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}

void ClientForm::accountManagerChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbAccountManager->model());
    m_client->account_manager()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}

void ClientForm::set_defaults()
{
    set_choice_field_default(ui->cbRevenueType, m_traffik_setup->revenueType()->value());
    set_choice_field_default(ui->cbBillCycle, m_traffik_setup->billingCycle()->value());
    ui->sbAgencyComm->setValue(m_traffik_setup->agencyComm()->value());
    ui->sbSaleRepComm->setValue(m_traffik_setup->saleRepComm()->value());
    ui->sbIntRate->setValue(m_traffik_setup->interestRate()->value());
    ui->sbLateFee->setValue(m_traffik_setup->lateFee()->value());
    ui->sbGracePeriod->setValue(m_traffik_setup->gracePeriod()->value());
}

void ClientForm::set_choice_field_default(QComboBox *cbox, std::string value)
{
    cbox->setCurrentIndex( cbox->findData(QVariant(stoq(value))) );
}
