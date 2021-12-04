#include "trashcanform.h"
#include "ui_trashcanform.h"

#include "../audio/audiolibrary.h"
#include "../audio/audiolibitem.h"
#include "../audio/audiotool.h"
#include "../audiolib/headers/cueeditor.h"
#include "../audiolib/headers/audioplayer.h"

#include "../framework/baseentity.h"
#include "../framework/ravenexception.h"
#include "../framework/choicefield.h"

TrashCanForm::TrashCanForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrashCanForm)
{
    ui->setupUi(this);

    setWindowTitle("Audio Trash Can");

    m_audio_edm = std::make_unique<EntityDataModel>(std::make_unique<AUDIO::Audio>());

    m_tracks_model = new QStandardItemModel(this);
    create_track_view_headers();
    set_track_view_column_width();

    m_audio_lib_item = std::make_unique<AUDIO::AudioLibItem>(m_tracks_model);
    ui->tvTracks->horizontalHeader()->setStretchLastSection(true);
    ui->tvTracks->setModel(m_tracks_model);
    ui->tvTracks->verticalHeader()->setVisible(false);

    fetch_del_audio();
}

TrashCanForm::~TrashCanForm()
{
    delete ui;
}

void TrashCanForm::create_track_view_headers()
{
    m_tracks_model->setHorizontalHeaderItem(0, new QStandardItem("Track Title"));
    m_tracks_model->setHorizontalHeaderItem(1, new QStandardItem("Folder"));
}

void TrashCanForm::set_track_view_column_width()
{
    enum Column{Title, Folder};
    ui->tvTracks->setColumnWidth(Column::Title, 300);
    ui->tvTracks->setColumnWidth(Column::Folder, 250);
}

void TrashCanForm::fetch_deleted_audio(FRAMEWORK::RelationMapper* r_mapper)
{
    m_audio_edm->clearEntities();
    auto const& audio = dynamic_cast<AUDIO::Audio*>(m_audio_edm->get_entity().get());

    try{
        m_audio_edm->readRaw(r_mapper->query());
    } catch (DatabaseException& de) {
        showMessage(de.errorMessage());
    }

    r_mapper->map_data();

    for(auto const& [record_id, record] : r_mapper->mapped_entities()){
        auto audio_uPtr = std::make_unique<AUDIO::Audio>();
        bool audio_is_constructed = false;

        for (auto const& [table_name, entities] : record) {

            for (auto const& entity : entities) {
                if (audio_uPtr->tableName() == entity->tableName() &&
                        !audio_is_constructed){
                    if (entity->id() > -1 ){
                        auto audio_ptr = dynamic_cast<AUDIO::Audio*>(entity.get());
                        *audio_uPtr.get() = *audio_ptr;
                        audio_is_constructed = true;
                    }

                 }

                auto const& artist = audio_uPtr->artist()->data_model_entity();
                if(artist == nullptr){
                    continue;
                }

                if (artist->tableName() == entity->tableName()){
                    auto artist_ptr = dynamic_cast<AUDIO::Artist*>(entity.get());

                    auto artist_uPtr = std::make_unique<AUDIO::Artist>();
                    *artist_uPtr.get() = *artist_ptr;
                    audio->artist()->set_fk_entity(std::move(artist_uPtr));
                }

                auto const& folder = audio_uPtr->folder()->data_model_entity();
                if (folder == nullptr){
                    continue;
                }

                if (folder->tableName() == entity->tableName()){
                    auto folder_ptr = dynamic_cast<AUDIO::Folder*>(entity.get());
                    auto folder_uPtr = std::make_unique<AUDIO::Folder>();
                    *folder_uPtr.get() = *folder_ptr;
                    audio->folder()->set_fk_entity(std::move(folder_uPtr));
                }

            }
        }

        if (audio_uPtr->audio_type()->value() != "") {
            m_audio_edm->add_entity(std::move(audio_uPtr));
        }

    }

    show_audio_data();
}


void TrashCanForm::show_audio_data()
{
    set_tracks_view();
    for (auto& [name, entity] : m_audio_edm->modelEntities()){

        AUDIO::Audio* audio = dynamic_cast<AUDIO::Audio*>(entity.get());

        audio->artist()->setVisible(false);
        audio->duration()->setVisible(false);
        audio->audio_type()->setVisible(false);
        audio->file_path()->setVisible(false);

        if (audio->audio_type()->displayName() == "Song")
            m_audio_lib_item->create_row_item<AUDIO::SongAudioLibItem>(audio);

        if (audio->audio_type()->displayName() == "Jingle")
            m_audio_lib_item->create_row_item<AUDIO::JingleAudioLibItem>(audio);

        if (audio->audio_type()->displayName() == "Commercial")
            m_audio_lib_item->create_row_item<AUDIO::CommercialAudioLibItem>(audio);

    }
}

void TrashCanForm::set_tracks_view()
{
    m_tracks_model->clear();
    create_track_view_headers();
    set_track_view_column_width();
}
