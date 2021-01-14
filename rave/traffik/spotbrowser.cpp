#include "spotbrowser.h"
#include "ui_spotbrowser.h"
#include "ui_baseentitybrowserdlg.h"

#include "client.h"
#include "spot.h"
#include "spotform.h"
#include "orderbooking.h"
#include "../framework/manytomany.h"
#include "../framework/ravenexception.h"

SpotBrowser::SpotBrowser(Client* client, QWidget* parent) :
    BaseEntityBrowserDlg(parent,
                         std::make_unique<TRAFFIK::Spot>(client)),
    ui{new Ui::SpotBrowser},
    m_client{client}
{
    ui->setupUi(this);
    setDialogTitle("Client Spots");
    if (client != nullptr)
        searchRecord();
}

SpotBrowser::~SpotBrowser()
{
    delete ui;
}


void SpotBrowser::addRecord()
{
    auto spot = std::make_unique<TRAFFIK::Spot>();
    auto spotForm = std::make_unique<SpotForm>(m_client, spot.get(), this);

    if (spotForm->exec() > 0){
        try{

            entityDataModel().createEntity(std::move(spot));

            saveVoiceOvers(*spotForm);

            saveTypeExclusions(*spotForm);

        }catch(DatabaseException& de){

            showMessage(de.errorMessage());
        }
    }

}

void SpotBrowser::updateRecord()
{
    std::string search_name = selectedRowName().toStdString();

    if (!search_name.empty()){

        BaseEntity* be = entityDataModel().findEntityByName(search_name);

        TRAFFIK::Spot* spot = dynamic_cast<TRAFFIK::Spot*>(be);
        spot->client()->setValue(m_client->id());

        std::unique_ptr<SpotForm> spotForm =
                std::make_unique<SpotForm>(m_client, spot, this);
        if (spotForm->exec() > 0){
            try{
                updateTableViewRecord(spot->tableViewValues());
                entityDataModel().updateEntity(*spot);

                saveVoiceOvers(*spotForm);

                saveTypeExclusions(*spotForm);

            }catch(DatabaseException& de){
                showMessage(de.errorMessage());
            }
        }
    }
}

void SpotBrowser::searchRecord()
{
    search_related<TRAFFIK::Spot, Client>(m_client);
}


bool SpotBrowser::okay_to_delete(BaseEntity* entity)
{
//   BaseEntity* entity = findSelectedEntity();
   TRAFFIK::Spot* spot = dynamic_cast<TRAFFIK::Spot*>(entity);
   EntityDataModel edm = EntityDataModel(std::make_unique<OrderBooking>());
   edm.searchByInt({"spot_id", "=", spot->id()});

   if (edm.count() > 0){
        showMessage("Cannot delete spot with existing bookings!");
        return false;
   }

   return true;
}

void SpotBrowser::saveVoiceOvers(const SpotForm& sf)
{
    std::unique_ptr<EntityDataModel> edm =
           std::make_unique<EntityDataModel>();

    auto& vos = sf.voiceOvers();
    for(auto& vo : vos){
        ManyToMany* mtom = dynamic_cast<ManyToMany*>(std::get<1>(vo).get());

        if (mtom->dbAction() == DBAction::dbaCREATE){
            mtom->setParentId(sf.parentId());
            edm->createEntityDB(*mtom);
        }

        if (mtom->dbAction() == DBAction::dbaDELETE)
            edm->deleteEntity(*mtom);
    }
}

void SpotBrowser::saveTypeExclusions(const SpotForm& sf)
{
    std::unique_ptr<EntityDataModel> edm =
           std::make_unique<EntityDataModel>();

    auto& types = sf.typeExclusions();
    for(auto& type : types){
        ManyToMany* mtom = dynamic_cast<ManyToMany*>(std::get<1>(type).get());

        if (mtom->dbAction() == DBAction::dbaCREATE){
            mtom->setParentId(sf.parentId());
            edm->createEntityDB(*mtom);
        }

        if (mtom->dbAction() == DBAction::dbaDELETE)
            edm->deleteEntity(*mtom);
    }
}

