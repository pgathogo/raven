#include <sstream>

#include "brandbrowser.h"
#include "ui_brandbrowser.h"
#include "brandform.h"
#include "ui_baseentitybrowserdlg.h"

#include "../utils/tools.h"

#include "client.h"
#include "brand.h"
#include "spot.h"

BrandBrowser::BrandBrowser(Client* client, QWidget *parent) :
    BaseEntityBrowserDlg(parent, std::make_unique<TRAFFIK::Brand>()),
    ui(new Ui::BrandBrowser),
    mClient{client}
{
    ui->setupUi(this);
    setDialogTitle("Client Brands");
    if (client != nullptr)
        searchRecord();
}

BrandBrowser::~BrandBrowser()
{
    delete ui;
}

void BrandBrowser::addRecord()
{
    add_related_record<TRAFFIK::Brand, BrandForm, Client>(mClient);
}

void BrandBrowser::updateRecord()
{
    std::string searchName = selectedRowName().toStdString();

    if (!searchName.empty()){

        BaseEntity* be = entityDataModel().findEntityByName(searchName);
        TRAFFIK::Brand* brand = dynamic_cast<TRAFFIK::Brand*>(be);
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
    search_related<TRAFFIK::Brand, Client>(mClient);
}

void BrandBrowser::search_by_client(Client* client)
{
    search_related<TRAFFIK::Brand, Client>(client);
}

bool BrandBrowser::okay_to_delete(BaseEntity *entity)
{
   TRAFFIK::Brand* brand = dynamic_cast<TRAFFIK::Brand*>(entity);

    std::stringstream sql;
    qDebug() << ">>" << entity->id() << "<<";

    sql << "Select a.id "
        << " From rave_orderbooking a, rave_spot b, rave_brand c"
        << " Where a.spot_id = b.id "
        << " and b.brand_id = c.id "
        << " and c.id = "+std::to_string(brand->id());

    EntityDataModel edm;
    edm.readRaw(sql.str());
    auto provider = edm.getDBManager()->provider();

    qDebug() << "**" << provider->cacheSize() << "***";

    if (provider->cacheSize() > 0 ) {
        showMessage("Cannot delete order with existing bookings!");
        return false;
    }

    return false;

}
