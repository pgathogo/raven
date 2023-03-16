#include "audiocache.h"

#include "../framework/choicefield.h"

namespace AUDIO
{
    AudioCache::AudioCache()
        : m_audio_id{}
        , m_title{}
        , m_artist_name{}
        , m_orig_filepath{}
        , m_cache_filepath{}
        , m_file_extension{}
        , m_cache_datetime{}
        , m_last_play_datetime{}
        , m_audio_type{}
        , m_is_dirty{}
    {
        m_audio_id = createField<IntegerField>("audio_id", "Audio ID");
        m_title = createField<StringField>("title", "Title");
        m_artist_name = createField<StringField>("artist_name", "Artist Name");
        m_orig_filepath = createField<StringField>("orig_filepath", "Orig Filepath");
        m_cache_filepath = createField<StringField>("cache_filepath", "Cache Filepath");
        m_file_extension = createField<StringField>("file_extension", "File Extension");
        m_cache_datetime = createField<DateTimeField>("cache_datetime", "Cache Datetime");
        m_last_play_datetime = createField<DateTimeField>("last_play_datetime", "Last play datetime");

        m_audio_type = createField<ChoiceField<std::string>>("audio_type", "Audio Type");
        m_audio_type->addChoice({"SONG", "Song"});
        m_audio_type->addChoice({"COMM-AUDIO", "Commercial"});
        m_audio_type->addChoice({"JINGLE", "Jingle"});
        m_audio_type->addChoice({"DROP", "Drop"});
        m_audio_type->addChoice({"NBITE", "News Bite"});

        m_is_cached = createField<BooleanField>("is_cached", "Audio Cached");
        m_is_cached->setFormOnly(true);

        m_is_dirty = createField<BooleanField>("is_dirty", "Audio Dirty");
        m_is_dirty->setFormOnly(true);

        m_header << QString::fromStdString(m_title->fieldLabel());
        setTableName("audio_cache");

        m_cache_datetime->setValue(QDateTime::currentDateTime());

    }

   bool operator ==(const AudioCache& lhs, const AudioCache& rhs)
   {
       qDebug() << "comparing ...";
       return (lhs.m_audio_id->value() == rhs.m_audio_id->value());
   }

   std::ostream& operator<<(std::ostream& os, const AudioCache& ac)
   {
       os << ac.audio_id()->value() << " : "<< ac.title()->value();
       return os;
   }

    IntegerField* AudioCache::audio_id() const
    {
        return m_audio_id;
    }
    StringField* AudioCache::title() const
    {
        return m_title;

    }
        StringField* AudioCache::artist_name() const
        {
            return m_artist_name;

        }

        StringField* AudioCache::orig_filepath() const
        {
            return m_orig_filepath;

        }

        StringField* AudioCache::cache_filepath() const
        {
            return m_cache_filepath;
        }

        StringField* AudioCache::file_extension() const
        {
            return m_file_extension;
        }

        DateTimeField* AudioCache::cache_datetime() const
        {
            return m_cache_datetime;

        }

        DateTimeField* AudioCache::last_play_datetime() const
        {
            return m_last_play_datetime;

        }

        ChoiceField<std::string>* AudioCache::audio_type() const
        {
            return m_audio_type;

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

        void AudioCache::set_title(const std::string a_title)
        {
            m_title->setValue(a_title);
        }

        void AudioCache::set_artist_name(const std::string a_name)
        {
            m_artist_name->setValue(a_name);
        }

        void AudioCache::set_orig_filepath(const std::string o_filepath)
        {
            m_orig_filepath->setValue(o_filepath);
        }

        void AudioCache::set_cache_filepath(const std::string c_filepath)
        {
            m_cache_filepath->setValue(c_filepath);
        }

        void AudioCache::set_file_extension(const std::string file_ext)
        {
            m_file_extension->setValue(file_ext);
        }

        void AudioCache::set_cache_datetime(QDateTime c_datetime)
        {
            m_cache_datetime->setValue(c_datetime);
        }

        void AudioCache::set_last_play_datetime(QDateTime lp_datetime)
        {
            m_last_play_datetime->setValue(lp_datetime);
        }

        void AudioCache::set_audio_type(const std::string a_type)
        {
            m_audio_type->setValue(a_type);
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
            return tableViewCols<std::string>(title()->displayName());

        }
        std::vector<std::string> AudioCache::tableViewValues()
        {
            return {
                title()->displayName(),
                artist_name()->displayName(),
                audio_type()->valueToString()
            };

        }
        QStringList AudioCache::tableHeaders() const
        {
            return m_header;

        }

        std::string AudioCache::searchColumn() const
        {
            return title()->valueToString();

        }
        void AudioCache::populateEntity()
        {

        }

        std::unique_ptr<BaseEntity> AudioCache::cloneAsUnique()
        {
            return std::make_unique<AUDIO::AudioCache>();

        }
        void AudioCache::afterMapping(BaseEntity& entity)
        {

        }
}
