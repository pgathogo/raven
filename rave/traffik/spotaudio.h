#ifndef SPOTAUDIO_H
#define SPOTAUDIO_H

#include "../audio/audio.h"
#include "../framework/manytomany.h"

//namespace AUDIO{
// class Audio;
//}

class Marker;

namespace TRAFFIK{

    struct AudioStruct{
        std::string m_title{};
        std::string m_short_desc{};
        int m_audio_year;
    };

    class SpotAudio : public ManyToMany
    {
    public:
        SpotAudio();
        SpotAudio(BaseEntity* pEnt, BaseEntity* dEnt);
        ~SpotAudio() override;

        //ForeignKeyField* audio() const;
        DateField* audio_date() const;
        IntegerField* weight() const;
        IntegerField* seqNo() const;

        StringField* short_desc() const;
        TextField* notes() const;
        StringField* title() const;
        DecimalField* duration()const;

        AUDIO::Audio* audio();


//        void set_audio(int val);
        void set_audio_date(QDate);
        void set_weight(int val);
        void set_seq_no(int val);

        void set_title(std::string);
        void set_short_desc(std::string);
        void set_notes(const std::string);

        void set_marker(Marker marker);
        void set_audio_type(std::string);
        void set_audio_lib_path(const std::string);
        void set_duration(int);
        void set_file_path(const std::string);
        void set_folder(int);
        void set_genre(int);
        void set_mood(int);
        void set_artist(int);
        void set_audio_year(int);

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

        BaseEntity* mtomEntity() override;

        std::unique_ptr<ManyToMany> copy(BaseEntity* parent_entity, BaseEntity* detail_entity) const override;
        void setTable(const std::string tablename) override { setTableName(tablename); }
        IntegerField* parentId() const override;
        void setParentId(int id) override;

        IntegerField* detailId() const override;
        void setDetailId(int id) override;

        BaseEntity* parentEntity() const override;
        BaseEntity* detailEntity() const override;
        void set_detail_entity(BaseEntity* other);

        void make_audio();

        AudioStruct get_struct() const;
        void set_struct(AudioStruct as);
        AUDIO::Audio& get_paudio();

    private:
        AUDIO::Audio* m_audio;

        AUDIO::Audio p_audio;

        AudioStruct m_audio_struct;

        DateField* m_audio_date;
        IntegerField* m_weight;
        IntegerField* m_seq_no;

        BaseEntity* m_parent_entity;
        BaseEntity* m_detail_entity;

        IntegerField* m_parent_id;
        IntegerField* m_detail_id;

        StringField* m_short_desc;
        TextField* m_notes;

        QStringList m_header;
        std::string m_table_name;

    };

} // TRAFFIK

#endif // SPOTAUDIO_H
