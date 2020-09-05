#include "spotbrowser.h"
#include "ui_spotbrowser.h"
#include "ui_baseentitybrowserdlg.h"

#include "client.h"
#include "spot.h"
#include "spotform.h"
#include "../framework/manytomany.h"
#include "../framework/ravenexception.h"

SpotBrowser::SpotBrowser(Client* client, QWidget *parent) :
    BaseEntityBrowserDlg(parent,
                         std::make_unique<Spot>(client)),
    ui(new Ui::SpotBrowser),
    mClient{client}
{
    ui->setupUi(this);
    setDialogTitle("Client Spots");
}

SpotBrowser::~SpotBrowser()
{
    delete ui;
}


void SpotBrowser::addRecord()
{
    auto spot = std::make_unique<Spot>();
    auto spotForm = std::make_unique<SpotForm>(mClient, spot.get(), this);

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
    std::string searchName = selectedRowName().toStdString();

    if (!searchName.empty()){

        BaseEntity* be = entityDataModel().findEntityByName(searchName);

        Spot* spot = dynamic_cast<Spot*>(be);
        std::unique_ptr<SpotForm> spotForm =
                std::make_unique<SpotForm>(mClient, spot, this);
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
    search_related<Spot, Client>(mClient);

    /*
    if (bui->edtFilter->text().isEmpty()){
        Spot& spot = dynamic_cast<Spot&>(entityDataModel().getEntity());
        auto si = searchItem(spot.client()->dbColumnName(), mClient->id());
        entityDataModel().searchByInt(si);
    }else{
        auto data = bui->cbFilter->itemData(
                            bui->cbFilter->currentIndex()).value<QVariant>();
        std::string columnName = data.toString().toStdString();
        std::string item = bui->edtFilter->text().toStdString();
        auto brand_filter = std::make_tuple(columnName, item);

        Spot& spot = dynamic_cast<Spot&>(entityDataModel().getEntity());
        auto client_filter = searchItem(spot.client()->dbColumnName(), mClient->id());

        std::string filter = entityDataModel().prepareFilter(brand_filter, client_filter);
        entityDataModel().search(filter);
    }
    */
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

