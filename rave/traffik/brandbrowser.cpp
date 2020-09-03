#include "brandbrowser.h"
#include "ui_brandbrowser.h"
#include "brandform.h"
#include "ui_baseentitybrowserdlg.h"

#include "../utils/tools.h"

#include "client.h"
#include "brand.h"

BrandBrowser::BrandBrowser(Client* client, QWidget *parent) :
    BaseEntityBrowserDlg(parent, std::make_unique<Brand>()),
    ui(new Ui::BrandBrowser),
    mClient{client}
{
    ui->setupUi(this);
    setDialogTitle("Client Brands");
}

BrandBrowser::~BrandBrowser()
{
    delete ui;
}

void BrandBrowser::addRecord()
{
    auto brand = std::make_unique<Brand>();

    std::unique_ptr<BrandForm> brandForm =
            std::make_unique<BrandForm>(mClient, brand.get(), this);

    if (brandForm->exec() > 0){
        try{
            entityDataModel().createEntity(std::move(brand));
        }
        catch(DatabaseException& de){
            showMessage(de.errorMessage());
        }
    }

}

void BrandBrowser::updateRecord()
{
    std::string searchName = selectedRowName().toStdString();

    if (!searchName.empty()){

        BaseEntity* be = entityDataModel().findEntityByName(searchName);
        Brand* brand = dynamic_cast<Brand*>(be);
        std::unique_ptr<BrandForm> brandForm =
                std::make_unique<BrandForm>(mClient, brand, this);
        if (brandForm->exec() > 0){
            try{
                updateTableViewRecord(brand->tableViewValues());
                entityDataModel().updateEntity(*brand);

            }catch(DatabaseException& de){
                showMessage(de.errorMessage());
            }
        }
    }
}

void BrandBrowser::searchRecord()
{
    search_related<Brand, Client>(mClient);
}
