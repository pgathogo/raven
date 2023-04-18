#include "audiocache.h"

#include "../framework/choicefield.h"

namespace AUDIO
{
    AudioCache::AudioCache()
        : m_cache_filepath{}
        , m_cache_datetime{}
        , m_last_play_datetime{}
        , m_is_dirty{}
        , m_audio{nullptr}
    {
        m_audio_id = createField<IntegerField>("audio_id", "Audio Id");
        m_cache_filepath = createField<StringField>("cache_filepath", "Cache Filepath");
        m_cache_datetime = createField<DateTimeField>("cache_datetime", "Cache Datetime");
        m_last_play_datetime = createField<DateTimeField>("last_play_datetime", "Last play datetime");

        m_is_cached = createField<BooleanField>("is_cached", "Audio Cached");
        m_is_cached->setFormOnly(true);

        m_is_dirty = createField<BooleanField>("is_dirty", "Audio Dirty");
        m_is_dirty->setFormOnly(true);

        m_header << QString::fromStdString(m_audio->title()->fieldLabel());
        setTableName("audio_cache");

        m_cache_datetime->setValue(QDateTime::currentDateTime());

    }
       AudioCache::AudioCache(std::shared_ptr<AUDIO::Audio> audio)
        : m_cache_filepath{}
        , m_cache_datetime{}
        , m_last_play_datetime{}
        , m_is_dirty{}
        , m_audio{audio}
       {
        m_audio_id = createField<IntegerField>("audio_id", "Audio Id");
        m_cache_filepath = createField<StringField>("cache_filepath", "Cache Filepath");
        m_cache_datetime = createField<DateTimeField>("cache_datetime", "Cache Datetime");
        m_last_play_datetime = createField<DateTimeField>("last_play_datetime", "Last play datetime");

        m_is_cached = createField<BooleanField>("is_cached", "Audio Cached");
        m_is_cached->setFormOnly(true);

        m_is_dirty = createField<BooleanField>("is_dirty", "Audio Dirty");
        m_is_dirty->setFormOnly(true);

        m_header << QString::fromStdString(m_audio->title()->fieldLabel());
        setTableName("audio_cache");

        m_cache_datetime->setValue(QDateTime::currentDateTime());

       }

       bool operator ==(const AudioCache& lhs, const AudioCache& rhs)
       {
           return (lhs.m_audio_id->value() == rhs.m_audio_id->value());
       }

       std::ostream& operator<<(std::ostream& os, AudioCache& ac)
       {
           os << ac.audio_id()->value() << " : "<< ac.audio()->title()->value();
           return os;
       }

       IntegerField* AudioCache::audio_id()
       {
           return m_audio_id;
       }

        StringField* AudioCache::cache_filepath() const
        {
            return m_cache_filepath;
        }

        DateTimeField* AudioCache::cache_datetime() const
        {
            return m_cache_datetime;

        }

        DateTimeField* AudioCache::last_play_datetime() const
        {
            return m_last_play_datetime;

        }

        BooleanField* AudioCache::is_cached() const
        {
            return m_is_cached;
        }

        BooleanField* AudioCache::is_dirty() const
        {
            return m_is_dirty;
        }

        void AudioCache::set_audio_id(int id)
        {
            m_audio_id->setValue(id);
        }

        void AudioCache::set_cache_filepath(const std::string c_filepath)
        {
            m_cache_filepath->setValue(c_filepath);
        }

        void AudioCache::set_cache_datetime(QDateTime c_datetime)
        {
            m_cache_datetime->setValue(c_datetime);
        }

        void AudioCache::set_last_play_datetime(QDateTime lp_datetime)
        {
            m_last_play_datetime->setValue(lp_datetime);
        }

        void AudioCache::set_is_cached(bool cached)
        {
            m_is_cached->setValue(cached);
        }

        void AudioCache::set_is_dirty(bool dirt)
        {
            m_is_dirty->setValue(dirt);
        }

        std::string AudioCache::tableName() const
        {
            return m_table_name;
        }

        void AudioCache::setTableName(const std::string table_name)
        {
            m_table_name = table_name;

        }
        std::unique_ptr<BaseEntity> AudioCache::mapFields(StringMap* raw_entity)
        {
            return nullptr;
        }

        std::vector<std::string> AudioCache::tableViewColumns() const
        {
            return tableViewCols<std::string>("cache");

        }
        std::vector<std::string> AudioCache::tableViewValues()
        {
            return {
                m_audio->title()->displayName(),
                m_audio->artist_fullname(),
                m_audio->audio_type()->valueToString()
            };

        }
        QStringList AudioCache::tableHeaders() const
        {
            return m_header;

        }

        std::string AudioCache::searchColumn() const
        {
            return m_audio->title()->valueToString();

        }
        void AudioCache::populateEntity()
        {

        }

        std::shared_ptr<BaseEntity> AudioCache::cloneAsShared()
        {
            return std::make_shared<AUDIO::AudioCache>();

        }
        void AudioCache::afterMapping(BaseEntity& entity)
        {

        }

        std::shared_ptr<AUDIO::Audio> AudioCache::audio()
        {
            return m_audio;
        }

        void AudioCache::set_audio(std::shared_ptr<AUDIO::Audio> audio)
        {
            m_audio = audio;
        }
}
