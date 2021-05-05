#include <algorithm>
#include <QFileDialog>
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
    m_setup{ setup }
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

    connect(ui->btnAudioPath, &QPushButton::clicked, this, &SetupForm::set_audio_path);
    connect(ui->btnCommAudioPath, &QPushButton::clicked, this, &SetupForm::set_comm_audio_path);

    ui->tvApprovers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    load_order_approvers();

}

SetupForm::~SetupForm()
{
    delete ui;
}

ActionResult SetupForm::saveRecord()
{
    populateEntityFields();
    return m_setup->validate();
}

std::string SetupForm::windowTitle()
{
    return "Traffik Setup";
}

void SetupForm::populateEntityFields()
{
    m_setup->setStationName(ui->edtStationName->text().toStdString());
    m_setup->setAddress1(ui->edtAddress1->text().toStdString());
    m_setup->setAddress2(ui->edtAddress2->text().toStdString());

    m_setup->setAgencyComm(ui->sbAgencyComm->value());
    m_setup->setSaleRepComm(ui->sbSaleRepComm->value());

    m_setup->setLateFee(ui->edtLateFee->value());
    m_setup->setInterestRate(ui->edtIntRate->value());
    m_setup->setGracePeriod(ui->edtGracePeriod->value());
    m_setup->setOrderApprovalLevels(ui->sbAprvLevels->value());

    m_setup->setOrderNumberSequence(ui->sbOrderSequence->value());

    m_setup->setBreakTimeInterval(ui->sbTimeInterval->value());
    m_setup->setBreakDuration(ui->sbBreakDuration->value());

    m_setup->setBreakMaxSpots(ui->edtMaxSpots->value());

    m_setup->setOrderAprvdBB(ui->cbAprvdBB->isChecked());

    m_setup->set_audio_folder(ui->edtAudioPath->text().toStdString());
    m_setup->set_comm_audio_folder(ui->edtCommAudioPath->text().toStdString());
}

void SetupForm::populateFormWidgets()
{
    ui->edtStationName->setText(stoq(m_setup->stationName()->value()));
    ui->edtAddress1->setText(stoq(m_setup->address1()->value()));
    ui->edtAddress2->setText(stoq(m_setup->address2()->value()));

    ui->sbAgencyComm->setValue(m_setup->agencyComm()->value());
    populate_choice_combo(ui->cbAgencyCommType, m_setup->agencyCommType());

    ui->sbSaleRepComm->setValue(m_setup->saleRepComm()->value());
    populate_choice_combo(ui->cbSaleRepCommType, m_setup->saleRepCommType());

    populate_choice_combo(ui->cbBillCycle, m_setup->billingCycle());
    populate_choice_combo(ui->cbRevenueType, m_setup->revenueType());
    populate_choice_combo(ui->cbBillingType, m_setup->billingType());
    populate_choice_combo(ui->cbBillingBasis, m_setup->billingBasis());

    ui->edtLateFee->setValue(m_setup->lateFee()->value());
    ui->edtIntRate->setValue(m_setup->interestRate()->value());
    ui->edtGracePeriod->setValue(m_setup->gracePeriod()->value());
    ui->sbAprvLevels->setValue(m_setup->orderApprovalLevels()->value());
    ui->sbOrderSequence->setValue(m_setup->orderNumberSequence()->value());

    ui->sbTimeInterval->setValue(m_setup->breakTimeInterval()->value());
    ui->sbBreakDuration->setValue(m_setup->breakDuration()->value());
    ui->edtMaxSpots->setValue(m_setup->breakMaxSpots()->value());

    auto check_state = [](bool state){ return (state) ? Qt::Checked : Qt::Unchecked; };
    ui->cbAprvdBB->setCheckState(check_state(m_setup->orderAprvdBB()->value()));

    ui->edtAudioPath->setText(stoq(m_setup->audio_folder()->value()));
    ui->edtCommAudioPath->setText(stoq(m_setup->comm_audio_folder()->value()));
}

void SetupForm::populate_choice_combo(QComboBox* cbox, const ChoiceField<std::string>* cf)
{
    for (const auto& c : cf->choices())
        cbox->addItem(stoq(std::get<1>(c)), stoq(std::get<0>(c)));

    cbox->setCurrentIndex( cbox->findData(QVariant(stoq(cf->value()))) );
}

QString SetupForm::get_audio_folder(QString default_folder)
{
   auto audio_folder = QFileDialog::getExistingDirectory(this,
                                                   tr("Audio Folder"), default_folder,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
   return audio_folder;
}

void SetupForm::load_order_approvers()
{
    m_edm_approver = std::make_unique<EntityDataModel>(
                std::make_unique<OrderApprover>());
    m_edm_approver->all();
    ui->tvApprovers->setModel(m_edm_approver.get());

}

std::set<int> SetupForm::make_aprv_levels()
{
    std::set<int> levels;
    for(int i=1; i<=ui->sbAprvLevels->value(); ++i)
        levels.insert(i);

    for (auto& [name, entity] : m_edm_approver->modelEntities()){
        OrderApprover* op = dynamic_cast<OrderApprover*>(entity.get());
        if (levels.find(op->level()->value()) != levels.end() ){
            levels.erase(op->level()->value());
        }
    }

    return levels;
}

void SetupForm::save_approvers()
{
    EntityDataModel edm(std::make_unique<OrderApprover>());
    for (auto& [name, entity] : m_edm_approver->modelEntities()){
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
    m_setup->agencyCommType()->setValue(
                ui->cbAgencyCommType->itemData(i).toString().toStdString());
}

void SetupForm::saleRepCommTypeChanged(int i)
{
    m_setup->saleRepCommType()->setValue(
                ui->cbSaleRepCommType->itemData(i).toString().toStdString());
}

void SetupForm::billCycleChanged(int i)
{
    m_setup->billingCycle()->setValue(
                ui->cbBillCycle->itemData(i).toString().toStdString());
}

void SetupForm::revenueTypeChanged(int i)
{
    m_setup->revenueType()->setValue(
                ui->cbRevenueType->itemData(i).toString().toStdString());

}

void SetupForm::billingTypeChanged(int i)
{
    m_setup->billingType()->setValue(
                ui->cbBillingType->itemData(i).toString().toStdString());

}

void SetupForm::billingBasisChanged(int i)
{
    m_setup->billingBasis()->setValue(
                ui->cbBillingBasis->itemData(i).toString().toStdString());
}

void SetupForm::addApprover()
{
    if (ui->sbAprvLevels->value() > 0 &&
            m_edm_approver->count() < ui->sbAprvLevels->value() ) {

        auto approver = std::make_unique<OrderApprover>();

        auto levels = make_aprv_levels();

        auto aprvForm = std::make_unique<ApproverForm>(
                    approver.get(), levels,
                    m_edm_approver->keys());

        if (aprvForm->exec() > 0)
            m_edm_approver->cacheEntity(std::move(approver));
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
   BaseEntity* entity = m_edm_approver->findEntityByName(searchName);
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

void SetupForm::set_audio_path()
{
    auto audio_folder = get_audio_folder(ui->edtAudioPath->text());
    if (!audio_folder.isEmpty())
         ui->edtAudioPath->setText(audio_folder+"/");
}

void SetupForm::set_comm_audio_path()
{
    auto comm_audio_folder = get_audio_folder(ui->edtCommAudioPath->text());
    if (!comm_audio_folder.isEmpty())
         ui->edtCommAudioPath->setText(comm_audio_folder+"/");
}
