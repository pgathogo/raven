#ifndef AUDIOCACHE_H
#define AUDIOCACHE_H

#include <iostream>

#include "../framework/baseentity.h"
#include "audio.h"



template<typename T>
class ChoiceField;


namespace AUDIO
{

    class AudioCache : public BaseEntity
    {
    public:
        AudioCache();
        AudioCache(std::shared_ptr<AUDIO::Audio>);

        IntegerField* audio_id();
        StringField* cache_filepath() const;
        DateTimeField* cache_datetime() const;
        DateTimeField* last_play_datetime() const;
        BooleanField* is_cached() const;
        BooleanField* is_dirty() const;

        std::shared_ptr<AUDIO::Audio> audio();

        void set_audio_id(int);
        void set_cache_filepath(const std::string);
        void set_cache_datetime(QDateTime);
        void set_last_play_datetime(QDateTime);
        void set_is_cached(bool);
        void set_is_dirty(bool);

        void set_audio(std::shared_ptr<AUDIO::Audio>);

        std::string tableName() const override;
        void setTableName(const std::string table_name) override;
        std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

        std::vector<std::string> tableViewColumns() const override;
        std::vector<std::string> tableViewValues() override;
        QStringList tableHeaders() const override;

        std::string searchColumn() const override;
        void populateEntity() override;

        std::shared_ptr<BaseEntity> cloneAsShared() override;
        void afterMapping(BaseEntity& entity) override;

        friend bool operator ==(const AudioCache& lhs, const AudioCache& rhs);
        friend std::ostream& operator<<(std::ostream& os, AudioCache& ac);

    private:
        IntegerField* m_audio_id;
        StringField* m_cache_filepath;
        DateTimeField* m_cache_datetime;
        DateTimeField* m_last_play_datetime;
        BooleanField* m_is_cached;
        BooleanField* m_is_dirty;

        std::shared_ptr<Audio> m_audio;

        QStringList m_header;
        std::string m_table_name;
    };

}

#endif // AUDIOCACHE_H
