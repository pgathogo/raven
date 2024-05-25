
#include "../framework/ravenexception.h"

#include "audiodatamodel.h"


namespace AUDIO
{

  AudioDataModel::AudioDataModel(AudioTrackViewer* viewer)
       :m_viewer{viewer}
   {
        m_lib_item = std::make_unique<AudioLibItem>(m_viewer->model());
        m_audio_edm = std::make_unique<EntityDataModel>(std::make_unique<AUDIO::Audio>());

        connect(m_viewer, &AudioTrackViewer::track_selected, this, &AudioDataModel::track_selected);
   }

   void AudioDataModel::track_selected(int track_id)
   {

        auto audio_ptr = m_lib_item->find_audio_by_id(track_id);

        std::shared_ptr<Audio> audio_sh = std::make_shared<Audio>();
        *audio_sh = *audio_ptr;

        emit selected_audio(audio_sh);
   }

   std::shared_ptr<Audio> AudioDataModel::find_audio_by_id(int audio_id)
   {
        std::shared_ptr<Audio> shared_ptr(m_lib_item->find_audio_by_id(audio_id));
        return shared_ptr;
   }


  void AudioDataModel::fetch_audio_data(FRAMEWORK::RelationMapper* r_mapper)
  {
    m_audio_edm->clearEntities();

    auto const& audio = dynamic_cast<Audio*>(m_audio_edm->get_entity().get());

    try{
        m_audio_edm->readRaw(r_mapper->query());
    } catch (DatabaseException& de) {
        showMessage(de.errorMessage());
    }

    r_mapper->map_data();

    for (auto const& [record_id, record] : r_mapper->mapped_entities())
    {
        auto audio_Uptr = std::make_unique<AUDIO::Audio>();
        bool audio_is_constructed = false;

        for(auto const& [table_name, entities] : record)
        {
            for (auto const& entity : entities)
            {
                if (audio_Uptr->tableName() == entity->tableName() &&
                    !audio_is_constructed)
                {
                    if (entity->id() > -1){
                    auto audio_ptr = dynamic_cast<Audio*>(entity.get());
                    *audio_Uptr.get() = *audio_ptr;
                    audio_is_constructed = true;
                    break;
                    }
                }

                auto const& artist = audio_Uptr->artist()->data_model_entity();

                if (artist == nullptr){
                    continue;
                }

                if (artist->tableName() == entity->tableName())
                {
                    auto artist_ptr = dynamic_cast<Artist*>(entity.get());
                    auto artist_uptr = std::make_unique<Artist>();
                    *artist_uptr.get() = *artist_ptr;
                    audio->artist()->set_fk_entity(std::move(artist_uptr));
                }
              }
        }

        if (audio_Uptr->audio_type()->value() != ""){
            m_audio_edm->add_entity(std::move(audio_Uptr));
        }
    }

    show_data();

  }


   void AudioDataModel::show_data()
   {
    m_viewer->clear();
    m_lib_item->clear();
    m_viewer->create_track_view_headers();

    for(auto&[name, entity] : m_audio_edm->modelEntities())
    {
        Audio* audio = dynamic_cast<Audio*>(entity.get());

        if (audio->audio_type()->displayName() == "Song")
            m_lib_item->create_row_item<SongAudioLibItem>(audio);

        if (audio->audio_type()->displayName() == "Jingle")
            m_lib_item->create_row_item<JingleAudioLibItem>(audio);

        if (audio->audio_type()->displayName() == "Commercial")
            m_lib_item->create_row_item<CommercialAudioLibItem>(audio);

    }

   }

}
