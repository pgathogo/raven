#include <sstream>

#include "brandbrowser.h"
#include "ui_brandbrowser.h"
#include "brandform.h"
#include "../framework/ui_baseentitybrowserdlg.h"

#include "../utils/tools.h"

#include "client.h"
#include "brand.h"
#include "spot.h"

BrandBrowser::BrandBrowser(Client* client, QWidget *parent) :
    BaseEntityBrowserDlg(parent, std::make_unique<TRAFFIK::Brand>()),
    ui(new Ui::BrandBrowser),
    m_client{client}
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
    add_related_record<TRAFFIK::Brand, BrandForm, Client>(m_client);
}

void BrandBrowser::updateRecord()
{
    std::string searchName = selectedRowName().toStdString();

    if (!searchName.empty()){

        std::shared_ptr<BaseEntity> be = entityDataModel().findEntityByName(searchName);
        TRAFFIK::Brand* brand = dynamic_cast<TRAFFIK::Brand*>(be.get());
        std::unique_ptr<BrandForm> brandForm =
                std::make_unique<BrandForm>(m_client, brand, this);
        if (brandForm->exec() > 0){
            try{
                updateTableViewRecord(brand->tableViewValues());
                entityDataModel().updateEntity(*brand);
                entityDataModel().all();

            }catch(DatabaseException& de){
                showMessage(de.errorMessage());
            }
        }
    }
}

void BrandBrowser::searchRecord()
{
    search_related<TRAFFIK::Brand, Client>(m_client);
}

void BrandBrowser::search_by_client(Client* client)
{
    search_related<TRAFFIK::Brand, Client>(client);
}

bool BrandBrowser::okay_to_delete(std::shared_ptr<BaseEntity> entity)
{
   TRAFFIK::Brand* brand = dynamic_cast<TRAFFIK::Brand*>(entity.get());

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

void BrandBrowser::set_client(Client *client)
{
    m_client = client;
}
