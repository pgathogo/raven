#include "brandbrowser.h"
#include "ui_brandbrowser.h"
#include "brandform.h"

#include "../utils/tools.h"

#include "brand.h"

BrandBrowser::BrandBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent, std::make_unique<Brand>()),
    ui(new Ui::BrandBrowser)
{
    ui->setupUi(this);
}

BrandBrowser::~BrandBrowser()
{
    delete ui;
}

void BrandBrowser::addRecord()
{
    std::unique_ptr<Brand> brand =
            std::make_unique<Brand>();

    std::unique_ptr<BrandForm> brandForm =
            std::make_unique<BrandForm>(brand.get(), this);

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
    auto brand = update<Brand, BrandForm>();
}

void BrandBrowser::deleteRecord()
{
}
