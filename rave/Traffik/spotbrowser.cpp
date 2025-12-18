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

SpotBrowser::SpotBrowser(std::shared_ptr<Client> client, QWidget* parent)
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
    auto spot_form = std::make_unique<SpotForm>(m_client, spot, this);

    log_info("Opening SpotForm...");

    if (spot_form->exec() > 0){
        try{

            int id = entityDataModel().createEntity(std::move(spot));

            log_info(QString("Saved spot ID: %1").arg(QString::number(id)));

            log_info("Saving voice overs...");
            save_voice_overs(*spot_form);

            log_info("Saving type exclusions...");
            save_type_exclusions(*spot_form);

            log_info("Saving spot audio...");
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

        std::shared_ptr<TRAFFIK::Spot> spot = std::dynamic_pointer_cast<TRAFFIK::Spot>(be);

        //FIXME: Refactor code - remove this and find a way to filter brands lookup
        // based on the selected client.
        //--------

        std::tuple filter = std::make_tuple("client_id", "=", m_client->id());
        spot->brand()->dataModel()->searchByInt(filter);

        //----

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

void SpotBrowser::search_by_client(std::shared_ptr<Client> client)
{
    search_related<TRAFFIK::Spot, Client>(client);
}

void SpotBrowser::set_client(std::shared_ptr<Client> client)
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

        if (mtom->dbAction() == DBAction::dbaCREATE) {
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
    log_info("Start save spot audio function...");

    auto edm = std::make_unique<EntityDataModel>();

    auto& spot_audios = sf.spot_audios();

    QString audio_count = QString("Spot audio count: %1").arg(QString::number(spot_audios.size()));
    log_info(audio_count);

    auto audio_creation_mode = sf.get_audio_creation_mode();

    for(auto& sa : spot_audios) {

        TRAFFIK::SpotAudio* s_audio = static_cast<TRAFFIK::SpotAudio*>(std::get<1>(sa).get());

        if (s_audio->dbAction() == DBAction::dbaCREATE)
        {
            log_info("Create spot audio");

            switch(audio_creation_mode) {

            case AudioCreationMode::Attach:
            {
                auto audio = s_audio->audio();
                s_audio->setDetailId(audio->id());
                s_audio->setParentId(sf.parentId());
                edm->createEntityDB(*s_audio);
                break;
            } // 'Attach' case

            case AudioCreationMode::Import:
            {
                log_info("Action mode: Import.");

                AUDIO::AudioTool at;

                auto& audio = s_audio->get_paudio();

                log_info(QString("Create audio entity: `%1`").arg(audio.title()->to_qstring()));

                int id = edm->createEntityDB(audio);

                s_audio->setDetailId(id); // Audio
                s_audio->setParentId(sf.parentId()); // SpotAudio

                log_info("Creating SpotAudio...");

                edm->createEntityDB(*s_audio);

                const std::string OGG_EXT = ".ogg";
                const std::string ADF_EXT = ".adf";
                const std::string WAVE_EXT = ".png";

                // Format audio name from Id
                std::string ogg_file = at.make_audio_filename(id);
                std::string lib_path = audio.audio_lib_path()->value();

                log_info(QString("OGG File: `%1`").arg(QString::fromStdString(ogg_file)));
                log_info(QString("Lib Path: %1").arg(QString::fromStdString(lib_path)));

                //std::string old_filename = lib_path+audio.audio_file().short_filename()+OGG_EXT;
                std::string old_filename = audio.audio_file().ogg_filename();
                std::string new_filename = lib_path+ogg_file+OGG_EXT;

                log_info(QString("Old filename: %1").arg(QString::fromStdString(old_filename)));
                log_info(QString("New filename: %1").arg(QString::fromStdString(new_filename)));


                fs::path old_f{old_filename};
                fs::path new_f{new_filename};

                try{

                    log_info(QString("Copy file SRC: %1 DST: %1")
                                 .arg(QString::fromStdString(old_filename))
                                 .arg(QString::fromStdString(new_filename)));

                    fs::copy(old_f, new_f);

                } catch (fs::filesystem_error& fe) {
                        QString q_what = QString::fromStdString(fe.what());
                        QString q_src_path = QString::fromStdString(fe.path1().string());
                        QString q_dst_path = QString::fromStdString(fe.path2().string());
                        QString q_message = QString::fromStdString(fe.what());

                        log_error("----- Copy Audio File Failed -----");

                        log_error(QString("What: %1").arg(q_what));
                        log_error(QString("src Path:  %1").arg(q_src_path));
                        log_error(QString("dst Path:  %1").arg(q_dst_path));
                        log_error(QString("Message:   %1").arg(q_message));

                        log_error("-------------------------------------");

                    // TODO: Clean-up the database entries done earlier in the process!
                    return;
                }

                // Write ADF file
                AUDIO::ADFRepository adf_repo;
                auto audio_file = audio.audio_file();
                std::string adf_filepath =lib_path+ogg_file+ADF_EXT;

                log_info(QString("Creating ADF file: %1").arg(QString::fromStdString(adf_filepath)));

                audio_file.set_adf_file(adf_filepath);
                audio_file.set_ogg_filename(ogg_file);

                adf_repo.write(audio_file);

                QString q_old_wave_file = QString::fromStdString(audio.audio_file().wave_file());

                // Copy Wave File to AudioLib directory
                if (fs::exists(audio.audio_file().wave_file()) )
                {

                    log_info(QString("Wave file: %1 exists").arg(q_old_wave_file));

                    fs::path old_wave_file{audio.audio_file().wave_file()};

                    // auto wave_file = old_wave_file.filename();

                    std::string wave_filepath = lib_path+ogg_file+WAVE_EXT;

                    QString q_new_wave_file = QString::fromStdString(wave_filepath);

                    fs::path new_wave_file{wave_filepath};

                    log_info(QString("Copy wave file... OLD: [ %1 ] NEW: [ %2 ]")
                                 .arg(q_old_wave_file)
                                 .arg(q_new_wave_file));

                    try {
                        fs::copy(old_wave_file, new_wave_file);
                    } catch (std::filesystem::filesystem_error const& ex) {
                        QString q_what = QString::fromStdString(ex.what());
                        QString q_src_path = QString::fromStdString(ex.path1().string());
                        QString q_dst_path = QString::fromStdString(ex.path2().string());
                        QString q_message = QString::fromStdString(ex.what());

                        log_error("----- Copy Wave File Failed -----");

                        log_error(QString("What: %1").arg(q_what));
                        log_error(QString("src Path:  %1").arg(q_src_path));
                        log_error(QString("dst Path:  %1").arg(q_dst_path));
                        log_error(QString("Message:   %1").arg(q_message));

                        log_error("-------------------------------------");

                    }

                    log_info(QString("Removing old wave file: %1")
                                 .arg(q_old_wave_file));

                    fs::remove(old_wave_file);

                } else {
                    log_error(QString("Wave file: %1 NOT found.")
                                  .arg(q_old_wave_file));
                }

                break;

            } // 'Import' case

            default:
                Logger::info("SpotBrowser", "Unknown Type");
                break;

        } // switch


    } // if create.


    }

}

