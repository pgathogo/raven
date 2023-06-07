#include <QApplication>
#include <filesystem>

#include <QDebug>

#include "spotbrowser.h"
#include "ui_spotbrowser.h"
#include "../framework/ui_baseentitybrowserdlg.h"

#include "client.h"
#include "spot.h"
#include "spotform.h"
#include "orderbooking.h"
#include "spotaudio.h"
#include "spotaudiobrowser.h"
#include "../audio/audio.h"
#include "../audio/audiotool.h"
#include "../framework/manytomany.h"
#include "../framework/ravenexception.h"

#include "../utils/tools.h"

namespace fs = std::filesystem;

SpotBrowser::SpotBrowser(Client* client, QWidget* parent)
    :BaseEntityBrowserDlg(parent, std::make_shared<TRAFFIK::Spot>(client))
    ,ui{new Ui::SpotBrowser}
    ,m_client{client}
{
    ui->setupUi(this);
    setDialogTitle("Client Spots");

    if (client != nullptr)
        searchRecord();

    show_letter_filter();
}

SpotBrowser::~SpotBrowser()
{
    delete ui;
}


void SpotBrowser::addRecord()
{
    auto spot = std::make_shared<TRAFFIK::Spot>();
    auto spot_form = std::make_unique<SpotForm>(m_client, spot.get(), this);

    if (spot_form->exec() > 0){
        try{

            entityDataModel().createEntity(std::move(spot));

            save_voice_overs(*spot_form);

            save_type_exclusions(*spot_form);

            save_spot_audio(*spot_form);

        }catch(DatabaseException& de){

            showMessage(de.errorMessage());
        }
    }

}

void SpotBrowser::updateRecord()
{
    std::string search_name = selectedRowName().toStdString();

    if (!search_name.empty()){

        std::shared_ptr<BaseEntity> be = entityDataModel().findEntityByName(search_name);

        TRAFFIK::Spot* spot = dynamic_cast<TRAFFIK::Spot*>(be.get());
        spot->client()->setValue(m_client->id());

        std::unique_ptr<SpotForm> spot_form =
                std::make_unique<SpotForm>(m_client, spot, this);

        if (spot_form->exec() > 0){
            try{

                updateTableViewRecord(spot->tableViewValues());

                entityDataModel().updateEntity(*spot);
                entityDataModel().all();

                save_voice_overs(*spot_form);

                save_type_exclusions(*spot_form);

                save_spot_audio(*spot_form);


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

void SpotBrowser::search_by_client(Client* client)
{
    search_related<TRAFFIK::Spot, Client>(client);
}

void SpotBrowser::set_client(Client *client)
{
    m_client = client;
}

bool SpotBrowser::okay_to_delete(std::shared_ptr<BaseEntity> entity)
{
//   BaseEntity* entity = findSelectedEntity();
   TRAFFIK::Spot* spot = dynamic_cast<TRAFFIK::Spot*>(entity.get());
   EntityDataModel edm = EntityDataModel(std::make_shared<OrderBooking>());
   edm.searchByInt({"spot_id", "=", spot->id()});

   if (edm.count() > 0){
        showMessage("Cannot delete spot with existing bookings!");
        return false;
   }

   return true;
}

void SpotBrowser::save_voice_overs(const SpotForm& sf)
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

void SpotBrowser::save_type_exclusions(const SpotForm& sf)
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

void SpotBrowser::save_spot_audio(const SpotForm& sf)
{
    auto edm = std::make_unique<EntityDataModel>();

    auto& spot_audios = sf.spot_audios();

    for(auto& sa : spot_audios){
        TRAFFIK::SpotAudio* s_audio = static_cast<TRAFFIK::SpotAudio*>(std::get<1>(sa).get());

        if (s_audio->dbAction() == DBAction::dbaCREATE){

            AUDIO::AudioTool at;

            auto& audio = s_audio->get_paudio();

            int id = edm->createEntityDB(audio);
            s_audio->setDetailId(id);
            s_audio->setParentId(sf.parentId());
            edm->createEntityDB(*s_audio);

            const std::string OGG_EXT = ".ogg";
            const std::string ADF_EXT = ".adf";
            const std::string WAVE_EXT = ".png";

            // Format audio name from Id
            std::string ogg_file = at.make_audio_filename(id);
            std::string lib_path = audio.audio_lib_path()->value();

            //std::string old_filename = lib_path+audio.audio_file().short_filename()+OGG_EXT;
            std::string old_filename = audio.audio_file().ogg_filename();
            std::string new_filename = lib_path+ogg_file+OGG_EXT;

            fs::path old_f{old_filename};
            fs::path new_f{new_filename};

            try{
                fs::copy(old_f, new_f);
            } catch (fs::filesystem_error& fe) {
                qDebug() << "Unable to copy audio file: "+stoq(fe.what());
                return;
            }

            // Write ADF file
            AUDIO::ADFRepository adf_repo;
            auto audio_file = audio.audio_file();
            audio_file.set_adf_file(lib_path+ogg_file+ADF_EXT);
            audio_file.set_ogg_filename(ogg_file);
            adf_repo.write(audio_file);

            // Copy Wave File to AudioLib directory
            if (fs::exists(audio.audio_file().wave_file()) ){

                fs::path old_wave_file{audio.audio_file().wave_file()};
                auto wave_file = old_wave_file.filename();
                fs::path new_wave_file{lib_path+ogg_file+WAVE_EXT};

                fs::copy(old_wave_file, new_wave_file);
                fs::remove(old_wave_file);

            }
        }
    }

}

