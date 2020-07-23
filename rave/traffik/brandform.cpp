#include <QDebug>

#include "brandform.h"
#include "ui_brandform.h"
#include "ui_baseentitydetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/entitydatamodel.h"

#include "../framework/picklistbrowser.h"

#include "brand.h"
#include "client.h"

BrandForm::BrandForm(Client* client, Brand* brand,
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
    mBrand->setBrandName(ui->edtBrandName->text().toStdString());
    mBrand->setClient(mClient->id());
}

void BrandForm::populateFormWidgets()
{
    ui->edtBrandName->setText(QString::fromStdString(mBrand->brandName()->value()));
    ui->edtClient->setText(QString::fromStdString(mClient->name()->value()));

    ui->cbBrandManager->setModel(mBrand->brandManager()->dataModel());
    ui->cbBrandManager->setCurrentIndex(ui->cbBrandManager->findText(
                                 stoq(mBrand->brandManager()->displayName())));
}

std::string BrandForm::getClientName()
{
    for(auto& base_entity : mBrand->client()->dataModel()->modelEntities()){
        if (mBrand->client()->value() == std::get<1>(base_entity)->id()){
            Client* c = dynamic_cast<Client*>(std::get<1>(base_entity).get());
            return c->name()->displayName();
        }
    }
    return "";
}

void BrandForm::selectClient()
{
    PickListSetting set(std::make_unique<Client>());
    mPlb = std::make_unique<PickListBrowser>(set);
    mPlb->exec();

    for (const auto& base_entity : set.selectedEntities){
        auto entity = dynamic_cast<Client*>(base_entity);
        mBrand->setClient(entity->id());
        ui->edtClient->setText(QString::fromStdString(entity->name()->displayName()));
    }
}

void BrandForm::brandManagerComboChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbBrandManager->model());
    mBrand->brandManager()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}
