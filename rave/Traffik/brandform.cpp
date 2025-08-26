#include <QDebug>

#include "brandform.h"
#include "ui_brandform.h"
#include "../framework/ui_baseentitydetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/entitydatamodel.h"

#include "../framework/picklistbrowser.h"

#include "client.h"

BrandForm::BrandForm(std::shared_ptr<Client> client, std::shared_ptr<TRAFFIK::Brand> brand,
                     QDialog *parent)
    :BaseEntityDetailDlg(parent),
     ui(new Ui::BrandForm),
     mClient{client},
     mBrand{brand},
     mPlb{nullptr}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());

    populateFormWidgets();

    //connect(ui->btnClient, &QPushButton::clicked, this, &BrandForm::selectClient);
    connect(ui->cbBrandManager, QOverload<int>::of(&QComboBox::currentIndexChanged),
           this, &BrandForm::brandManagerComboChanged);

    QPalette* palette = new QPalette();
    palette->setColor(QPalette::Base, Qt::gray);
    ui->edtClient->setPalette(*palette);
}

BrandForm::~BrandForm()
{
    delete ui;
}

ActionResult BrandForm::saveRecord()
{
    populateEntityFields();
    return mBrand->validate();
}

std::string BrandForm::windowTitle()
{
    return "Brand Details";
}
void BrandForm::populateEntityFields()
{
    mBrand->set_brand_name(ui->edtBrandName->text().toStdString());
    mBrand->set_client(mClient->id());
}

void BrandForm::populateFormWidgets()
{
    ui->edtBrandName->setText(QString::fromStdString(mBrand->brand_name()->value()));
    ui->edtClient->setText(QString::fromStdString(mClient->name()->value()));

    ui->cbBrandManager->setModel(mBrand->brand_manager()->dataModel());
    ui->cbBrandManager->setCurrentIndex(ui->cbBrandManager->findText(
                                 stoq(mBrand->brand_manager()->displayName())));
}

void BrandForm::clear_widgets()
{
    ui->edtBrandName->clear();
    ui->edtClient->clear();
    ui->cbBrandManager->setCurrentIndex(-1);
}

void BrandForm::selectClient()
{
    PickListSetting set(std::make_unique<Client>(), 0);
    mPlb = std::make_unique<PickListBrowser>(set);
    mPlb->exec();

    for (const auto& base_entity : set.selectedEntities){
        auto entity = dynamic_cast<Client*>(base_entity);
        mBrand->set_client(entity->id());
        ui->edtClient->setText(QString::fromStdString(entity->name()->displayName()));
    }
}

void BrandForm::brandManagerComboChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbBrandManager->model());
    mBrand->brand_manager()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}
