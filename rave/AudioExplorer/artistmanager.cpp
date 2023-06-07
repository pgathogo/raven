#include "artistmanager.h"

#include "../framework/entitydatamodel.h"
#include "../framework/baseentity.h"
#include "../audio/artist.h"
#include "../audio/audio.h"

namespace AUDIOEXP{

    ArtistManager::ArtistManager()
        :m_model{nullptr}
    {
        m_model = std::make_unique<EntityDataModel>(
                    std::make_shared<AUDIO::Artist>());
    }

    void ArtistManager::create(std::unique_ptr<AUDIO::Artist> artist)
    {
        m_model->createEntity(std::move(artist));
    }

    void ArtistManager::update(AUDIO::Artist* artist)
    {
        m_model->updateEntity(*artist);
	m_model->all();
    }

    AUDIO::Artist* ArtistManager::find_artist(std::string search_value)
    {
        if (search_value.empty())
            return nullptr;

        std::shared_ptr<BaseEntity> be = m_model->findEntityByName(search_value);
        AUDIO::Artist* artist = dynamic_cast<AUDIO::Artist*>(be.get());
        return artist;
    }

    void ArtistManager::delete_artist(AUDIO::Artist* artist)
    {
        artist->setDBAction(DBAction::dbaDELETE);
        m_model->deleteEntity(*artist);
    }

    bool ArtistManager::artist_has_audio(AUDIO::Artist* artist)
    {
        auto audio = std::make_shared<AUDIO::Audio>();
        std::string column_name = audio->artist()->dbColumnName();
        std::unique_ptr<EntityDataModel> edm = std::make_unique<EntityDataModel>(
                    std::move(audio));
        auto audio_filter = std::make_tuple(column_name, " = ", artist->id());
        auto filter = edm->prepareFilter(audio_filter);
        edm->search(filter);

        return (edm->count() == 0) ? true : false;
    }

    void ArtistManager::fetch_data(const std::string filter)
    {
        if (filter.empty())	{
            m_model->all();
        }else{
            m_model->search(filter);
        }
    }



    std::unique_ptr<EntityDataModel> const& ArtistManager::model() const
    {
        return m_model;
    }


}
