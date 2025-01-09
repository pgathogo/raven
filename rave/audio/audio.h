#ifndef AUDIO_H
#define AUDIO_H

#include "audiofile.h"
#include "../framework/baseentity.h"

template<typename T>
class ChoiceField;

namespace AUDIO {

    class Genre : public BaseEntity
    {
    public:
        Genre();

        StringField* genre_name()const;
        void set_genre_name(const std::string);

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

        [[nodiscard]] ActionResult validate() override;
    private:
        StringField* m_genre_name;
        std::string m_table_name;
        QStringList m_header;

    };

    class Mood : public BaseEntity
    {
    public:
        Mood();

        StringField* mood_name() const;

        ChoiceField<std::string>* intensity() const;
        ChoiceField<std::string>* timbre() const;
        ChoiceField<std::string>* pitch() const;
        ChoiceField<std::string>* rhythm() const;

        void set_mood_name(const std::string name);
        void set_intensity(const std::string);
        void set_timbre(const std::string);
        void set_pitch(const std::string);
        void set_rhythm(const std::string);

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

        [[nodiscard]] ActionResult validate() override;
    private:
        ChoiceField<std::string>* m_intensity;
        ChoiceField<std::string>* m_timbre;
        ChoiceField<std::string>* m_pitch;
        ChoiceField<std::string>* m_rhythm;
        StringField* m_mood_name;

        std::string m_table_name;
        QStringList m_header;
    };

    class Folder : public BaseEntity
    {
    public:
        Folder();

        Folder& operator=(const Folder& other);

        StringField* folder_name() const;
        IntegerField* parent() const;

        void set_folder_name(const std::string);
        void set_parent(int);

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

        [[nodiscard]] ActionResult validate() override;

    private:
        StringField* m_folder_name;
        IntegerField* m_parent;

        QStringList m_header;
        std::string m_table_name;
    };

    class Audio : public BaseEntity
    {
    public:
        Audio();
        Audio(const std::string audio_file);
        Audio(const Audio& other);
        Audio& operator=(const Audio& other);

        StringField* title() const;
        StringField* short_desc() const;
        ForeignKeyField* artist() const;
        StringField* audio_lib_path() const;
        BooleanField* is_deleted() const;
        IntegerField* play_count() const;

        DecimalField* duration() const;
        double playable_duration() const;

        DecimalField* start_marker() const;
        DecimalField* fade_in_marker() const;
        DecimalField* intro_marker() const;
        DecimalField* extro_marker() const;
        DecimalField* fade_out_marker() const;
        DecimalField* fade_delay_marker() const;
        DecimalField* end_marker() const;

        StringField* file_path() const;
        ForeignKeyField* folder() const;
        ForeignKeyField* genre() const;
        ForeignKeyField* mood() const;
        DateField* creation_date() const;
        IntegerField* audio_year() const;
        ChoiceField<std::string>* audio_type() const;
        TextField* notes() const;
        AudioFile& audio_file();
        CueMarker cue_marker() const;

        StringField* audio_filename() const;
        StringField* file_extension() const;
        std::string artist_fullname() const;

        void set_title(const std::string);
        void set_short_desc(const std::string);
        void set_artist(int);
        void set_audio_lib_path(const std::string);
        void set_is_deleted(bool);
        void set_play_count(int);

        void set_duration(double);

        void set_start_marker(double);
        void set_fade_in_marker(double);
        void set_intro_marker(double);
        void set_extro_marker(double);
        void set_fade_out_marker(double);
        void set_fade_delay_marker(double);
        void set_end_marker(double);
        void set_file_path(const std::string);
        void set_folder(int);
        void set_genre(int);
        void set_mood(int);
        void set_creation_date(QDate);
        void set_audio_year(int);
        void set_audio_type(const std::string);
        void set_notes(const std::string);
        void set_audio_file(AudioFile);
        void set_cue_marker(CueMarker marker);

        void set_audio_filename(const std::string);
        void set_file_extension(const std::string);

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

        QString full_audio_filename() const;

        friend bool operator ==(const Audio& lhs, const Audio& rhs);
        friend std::ostream& operator<<(std::ostream& os, const Audio& ac);

        //[[nodiscard]] ActionResult validate() override;

    private:
        std::string get_file_extension(std::string);

        StringField* m_title;

        StringField* m_short_desc;

        ForeignKeyField* m_artist;

        StringField* m_audio_lib_path;

        BooleanField*  m_is_deleted;
        IntegerField* m_play_count;

        DecimalField* m_duration;

        DecimalField* m_start_marker;
        DecimalField* m_fade_in_marker;
        DecimalField* m_intro_marker;
        DecimalField* m_extro_marker;
        DecimalField* m_fade_out_marker;
        DecimalField* m_fade_delay_marker;
        DecimalField* m_end_marker;

        StringField* m_file_path;

        ForeignKeyField* m_folder;
        ForeignKeyField* m_genre;
        ForeignKeyField* m_mood;

        DateField* m_creation_date;

        IntegerField* m_audio_year;
        ChoiceField<std::string>* m_audio_type;
        TextField* m_notes;

        CueMarker m_marker;

        StringField* m_audio_filename;
        StringField* m_file_extension;

        AudioFile m_file_info;

        QStringList m_header;
        std::string m_table_name;
    };

} // AUDIO


#endif // AUDIO_H
