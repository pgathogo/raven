#ifndef AUDIOCACHE_H
#define AUDIOCACHE_H

#include <iostream>

#include "../framework/baseentity.h"


template<typename T>
class ChoiceField;


namespace AUDIO
{

    class AudioCache : public BaseEntity
    {
    public:
        AudioCache();

        IntegerField* audio_id() const;
        StringField* title() const;
        StringField* artist_name() const;
        StringField* orig_filepath() const;
        StringField* cache_filepath() const;
        DateTimeField* cache_datetime() const;
        DateTimeField* last_play_datetime() const;
        ChoiceField<std::string>* audio_type() const;
        BooleanField* is_cached() const;
        BooleanField* is_dirty() const;

        void set_audio_id(int);
        void set_title(const std::string);
        void set_artist_name(const std::string);
        void set_orig_filepath(const std::string);
        void set_cache_filepath(const std::string);
        void set_cache_datetime(QDateTime);
        void set_last_play_datetime(QDateTime);
        void set_audio_type(std::string);
        void set_is_cached(bool);
        void set_is_dirty(bool);

        std::string tableName() const override;
        void setTableName(const std::string table_name) override;
        std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

        std::vector<std::string> tableViewColumns() const override;
        std::vector<std::string> tableViewValues() override;
        QStringList tableHeaders() const override;

        std::string searchColumn() const override;
        void populateEntity() override;

        std::unique_ptr<BaseEntity> cloneAsUnique() override;
        void afterMapping(BaseEntity& entity) override;

        friend bool operator ==(const AudioCache& lhs, const AudioCache& rhs);
        friend std::ostream& operator<<(std::ostream& os, const AudioCache& ac);

    private:
        IntegerField* m_audio_id;
        StringField* m_title;
        StringField* m_artist_name;
        StringField* m_orig_filepath;
        StringField* m_cache_filepath;
        DateTimeField* m_cache_datetime;
        DateTimeField* m_last_play_datetime;
        ChoiceField<std::string>* m_audio_type;
        BooleanField* m_is_cached;
        BooleanField* m_is_dirty;

        QStringList m_header;
        std::string m_table_name;
    };

}

#endif // AUDIOCACHE_H
