#pragma once

#include <memory>
#include <vector>

class EntityDataModel;

namespace AUDIO{
 class Artist;
}

namespace AUDIOEXP{

    class ArtistManager
    {
    public:
        ArtistManager();

        void create(std::unique_ptr<AUDIO::Artist>);
        void read();
        void update(AUDIO::Artist*);
        void delete_artist(AUDIO::Artist*);
        bool artist_has_audio(AUDIO::Artist*);

        AUDIO::Artist* find_artist(std::string);
        void fetch_data(const std::string);

        std::unique_ptr<EntityDataModel> const& model() const;

    private:
        std::unique_ptr<EntityDataModel> m_model;

    };
}
