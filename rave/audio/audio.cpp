#include <cstring>
#include <filesystem>

#include <QFileInfo>

#include "audio.h"
#include "artist.h"
#include "../framework/choicefield.h"
#include "../framework/entitydatamodel.h"
#include "../audio/audiotool.h"

namespace fs = std::filesystem;

namespace AUDIO {

    Genre::Genre()
        :m_genre_name{},
         m_table_name{},
         m_header{}
    {
        m_genre_name = createField<StringField>("genre", "Genre");

        m_header << QString::fromStdString(m_genre_name->fieldLabel());
        setTableName("rave_genre");
    }

    StringField *Genre::genre_name() const
    {
        return m_genre_name;
    }

    void Genre::set_genre_name(const std::string genre_name)
    {
        m_genre_name->setValue(genre_name);
    }

    std::string Genre::tableName() const
    {
        return m_table_name;
    }

    void Genre::setTableName(const std::string table_name)
    {
        m_table_name = table_name;
    }

    std::unique_ptr<BaseEntity> Genre::mapFields(StringMap *raw_entity)
    {

        return nullptr;
    }

    std::vector<std::string> Genre::tableViewColumns() const
    {
        return tableViewCols<std::string>(genre_name()->displayName());
    }

    std::vector<std::string> Genre::tableViewValues()
    {
        return {genre_name()->displayName()};
    }

    QStringList Genre::tableHeaders() const
    {
        return m_header;
    }

    std::string Genre::searchColumn() const
    {
        return genre_name()->valueToString();
    }

    void Genre::populateEntity()
    {

    }

    std::shared_ptr<BaseEntity> Genre::cloneAsShared()
    {
        return std::make_shared<Genre>();
    }

    void Genre::afterMapping(BaseEntity &entity)
    {

    }

    ActionResult Genre::validate()
    {
        return std::make_tuple(ActionResultType::arSUCCESS,"");
    }

    /* --- Audio --- */

    Audio::Audio()
        :m_title{}
         ,m_short_desc{}
         ,m_artist{}
         ,m_add_dtime{}
         ,m_audio_lib_path{}
         ,m_is_deleted{}
         ,m_play_count{}
         ,m_duration{}
         ,m_start_marker{}
         ,m_fade_in_marker{}
         ,m_intro_marker{}
         ,m_extro_marker{}
         ,m_fade_out_marker{}
         ,m_fade_delay_marker{}
         ,m_end_marker{}
         ,m_file_path{}
         ,m_folder{}
         ,m_genre{}
         ,m_mood{}
         ,m_creation_date{}
         ,m_audio_type{}
         ,m_notes{}
         ,m_audio_filename{}
         ,m_file_info{}
    {

        m_title = createField<StringField>("title", "Title");
        m_short_desc = createField<StringField>("short_desc", "Short Desc");

        m_artist = createField<ForeignKeyField>("artist_id", "Artist",
                                                std::make_unique<Artist>(), "fullname");

        m_add_dtime = createField<DateTimeField>("add_dtime", "Add Date Time");

        m_audio_lib_path = createField<StringField>("filepath","Audio Lib");
        m_audio_lib_path->setFormOnly(true);

        m_is_deleted = createField<BooleanField>("deleted", "Is Deleted?");
        m_play_count= createField<IntegerField>("play_count", "Play Count");
        m_duration = createField<DecimalField>("duration","Duration");
        m_start_marker = createField<DecimalField>("start_marker", "Start Marker");
        m_fade_in_marker = createField<DecimalField>("fade_in_marker", "Fade in Marker");
        m_fade_delay_marker = createField<DecimalField>("fade_delay_marker", "Fade Delay Marker");
        m_intro_marker= createField<DecimalField>("intro_marker", "Intro Marker");
        m_extro_marker = createField<DecimalField>("extro_marker", "Extro Marker");
        m_fade_out_marker = createField<DecimalField>("fade_out_marker", "Fade Out Marker");
        m_fade_delay_marker = createField<DecimalField>("fade_delay_marker", "Fade Delay Marker");
        m_end_marker = createField<DecimalField>("end_marker", "End Marker");

        m_file_path = createField<StringField>("filepath", "File Path");

        m_folder = createField<ForeignKeyField>("folder_id", "Folder",
                                                std::make_unique<Folder>(), "folder_name");
        m_genre = createField<ForeignKeyField>("genre_id", "Genre",
                                               std::make_unique<Genre>(), "genre");
        m_mood = createField<ForeignKeyField>("mood_id", "Mood",
                                              std::make_unique<Mood>(), "mood");
        m_creation_date = createField<DateField>("creation_date", "Creation Date");

        m_audio_type = createField<ChoiceField<std::string>>("audio_type", "Audio Type");
        m_audio_type->addChoice({"SONG", "Song"});
        m_audio_type->addChoice({"COMM-AUDIO", "Commercial"});
        m_audio_type->addChoice({"JINGLE", "Jingle"});
        m_audio_type->addChoice({"DROP", "Drop"});
        m_audio_type->addChoice({"NBITE", "News Bite"});

        m_audio_year = createField<IntegerField>("audio_year", "Audio Year");

        m_notes = createField<TextField>("notes", "Notes");

        m_header << QString::fromStdString(m_title->fieldLabel());
        setTableName("rave_audio");

        m_play_count->setValue(0);
        m_add_dtime->setValue(QDateTime::currentDateTime());

        m_file_extension = createField<StringField>("file_extension", "File Extension");
        m_file_extension->setValue("");

        m_creation_date->setValue(QDate::currentDate());
        auto dt = QDate(QDate::currentDate());
        m_audio_year->setValue(dt.year());

        m_audio_filename = createField<StringField>("audio_filename", "Audio Filename");
        m_audio_filename->setFormOnly(true);
    }


    Audio::Audio(const std::string audio_file)
        :m_title{}
         ,m_short_desc{}
         ,m_artist{}
         ,m_add_dtime{}
         ,m_audio_lib_path{}
         ,m_is_deleted{}
         ,m_play_count{}
         ,m_duration{}
         ,m_start_marker{}
         ,m_fade_in_marker{}
         ,m_fade_delay_marker{}
         ,m_intro_marker{}
         ,m_extro_marker{}
         ,m_fade_out_marker{}
         ,m_end_marker{}
         ,m_file_path{}
         ,m_folder{}
         ,m_genre{}
         ,m_mood{}
         ,m_creation_date{}
         ,m_audio_type{}
         ,m_audio_filename{}
    {

        m_title = createField<StringField>("title", "Title");

        set_title(fs::path(audio_file).stem().string());

        m_short_desc = createField<StringField>("short_desc", "Short Desc");

        m_artist = createField<ForeignKeyField>("artist_id", "Artist",
                                                std::make_unique<Artist>(), "fullname");
        m_add_dtime = createField<DateTimeField>("add_dtime", "Add Date Time");

        m_audio_lib_path = createField<StringField>("filepath","Audio Lib");
        m_audio_lib_path->setFormOnly(true);

        m_is_deleted = createField<BooleanField>("deleted", "Is Deleted?");
        m_play_count= createField<IntegerField>("play_count", "Play Count");
        m_duration = createField<DecimalField>("duration","Duration");
        m_start_marker = createField<DecimalField>("start_marker", "Start Marker");
        m_fade_in_marker = createField<DecimalField>("fade_in_marker", "Fade in Marker");
        m_intro_marker= createField<DecimalField>("intro_marker", "Intro Marker");
        m_extro_marker = createField<DecimalField>("extro_marker", "Extro Marker");
        m_fade_out_marker = createField<DecimalField>("fade_out_marker", "Fade Out Marker");
        m_fade_delay_marker = createField<DecimalField>("fade_delay_marker", "Fade Delay Marker");
        m_end_marker = createField<DecimalField>("end_marker", "End Marker");

        m_file_path = createField<StringField>("filepath", "File Path");

        m_folder = createField<ForeignKeyField>("folder_id", "Folder",
                                                std::make_unique<Folder>(), "folder_name");
        m_genre = createField<ForeignKeyField>("genre_id", "Genre",
                                               std::make_unique<Genre>(), "genre");
        m_mood = createField<ForeignKeyField>("mood_id", "Mood",
                                              std::make_unique<Mood>(), "mood");
        m_creation_date = createField<DateField>("creation_date", "Creation Date");

        m_audio_type = createField<ChoiceField<std::string>>("audio_type", "Audio Type");
        m_audio_type->addChoice({"SONG", "Song"});
        m_audio_type->addChoice({"COMM-AUDIO", "Commercial"});
        m_audio_type->addChoice({"JINGLE", "Jingle"});
        m_audio_type->addChoice({"DROP", "Drop"});
        m_audio_type->addChoice({"NBITE", "News Bite"});

        m_audio_year = createField<IntegerField>("audio_year", "Audio Year");

        m_notes = createField<TextField>("notes", "Notes");

        m_header << QString::fromStdString(m_title->fieldLabel());
        setTableName("rave_audio");

        m_play_count->setValue(0);
        m_add_dtime->setValue(QDateTime::currentDateTime());

        m_creation_date->setValue(QDate::currentDate());
        auto dt = QDate(QDate::currentDate());
        m_audio_year->setValue(dt.year());

        m_file_extension = createField<StringField>("file_extension", "File Extension");
        m_file_extension->setValue(get_file_extension(audio_file));

        m_audio_filename = createField<StringField>("audio_filename", "Audio Filename");
        m_audio_filename->setFormOnly(true);
        m_audio_filename->setValue(audio_file);
    }

    Audio &Audio::operator=(const Audio &other)
    {
        if (this == &other) return *this;

        this->setId(other.id());
        m_title->setValue(other.title()->value());
        m_short_desc->setValue(other.short_desc()->value());
        m_duration->setValue(other.duration()->value());
        m_artist->setValue(other.m_artist->value());

        m_add_dtime->setValue(other.add_dtime()->value());

        m_audio_lib_path->setValue(other.audio_lib_path()->value());

        m_is_deleted->setValue(other.is_deleted()->value());
        m_play_count->setValue(other.play_count()->value());
        m_duration->setValue(other.duration()->value());
        m_start_marker->setValue(other.start_marker()->value());
        m_fade_in_marker->setValue(other.fade_in_marker()->value());
        m_intro_marker->setValue(other.intro_marker()->value());
        m_extro_marker->setValue(other.extro_marker()->value());
        m_fade_out_marker->setValue(other.fade_out_marker()->value());
        m_fade_delay_marker->setValue(other.fade_delay_marker()->value());
        m_end_marker->setValue(other.end_marker()->value());

        m_file_path->setValue(other.file_path()->value());

        m_folder->setValue(other.folder()->value());
        m_genre->setValue(other.genre()->value());
        m_mood->setValue(other.mood()->value());
        m_creation_date->setValue(other.creation_date()->value());
        m_audio_type->setValue(other.audio_type()->value());
        m_audio_year->setValue(other.audio_year()->value());
        m_notes->setValue(other.notes()->value());
        m_header = other.m_header;

        m_audio_filename->setValue(other.audio_filename()->value());
        m_file_extension->setValue(other.file_extension()->value());

        m_file_info = other.m_file_info;

        setTableName("rave_audio");

        return *this;
    }

    StringField *Audio::title() const
    {
        return m_title;
    }

    StringField *Audio::short_desc() const
    {
        return m_short_desc;
    }

    ForeignKeyField* Audio::artist() const
    {
        return m_artist;
    }

    DateTimeField *Audio::add_dtime() const
    {
        return m_add_dtime;
    }

    StringField *Audio::audio_lib_path() const
    {
        return m_audio_lib_path;
    }

    BooleanField *Audio::is_deleted() const
    {
        return m_is_deleted;
    }

    IntegerField *Audio::play_count() const
    {
        return m_play_count;
    }

    DecimalField *Audio::duration() const
    {
        return m_duration;
    }

    DecimalField *Audio::start_marker() const
    {
        return m_start_marker;
    }

    DecimalField *Audio::fade_in_marker() const
    {
        return m_fade_in_marker;
    }

    DecimalField *Audio::intro_marker() const
    {
        return m_intro_marker;
    }

    DecimalField *Audio::extro_marker() const
    {
        return m_extro_marker;
    }

    DecimalField *Audio::fade_out_marker() const
    {
        return m_fade_out_marker;
    }

    DecimalField* Audio::fade_delay_marker() const
    {
        return m_fade_delay_marker;
    }

    DecimalField *Audio::end_marker() const
    {
        return m_end_marker;
    }

    StringField *Audio::file_path() const
    {
        return m_file_path;
    }

    ForeignKeyField *Audio::folder() const
    {
        return m_folder;
    }

    ForeignKeyField *Audio::genre() const
    {
        return m_genre;
    }

    ForeignKeyField *Audio::mood() const
    {
        return m_mood;
    }

    DateField *Audio::creation_date() const
    {
        return m_creation_date;
    }

    IntegerField *Audio::audio_year() const
    {
        return m_audio_year;
    }

    ChoiceField<std::string> *Audio::audio_type() const
    {
        return m_audio_type;
    }

    TextField *Audio::notes() const
    {
        return m_notes;
    }

    AudioFile& Audio::audio_file()
    {
        return m_file_info;
    }

    StringField* Audio::audio_filename() const
    {
        return m_audio_filename;
    }

    void Audio::set_title(const std::string title)
    {
        m_title->setValue(title);
        m_file_info.set_audio_title(title);
    }

    void Audio::set_short_desc(const std::string s_desc)
    {
        m_short_desc->setValue(s_desc);
        m_file_info.set_short_desc(s_desc);
    }

    void Audio::set_artist(int artist_id)
    {
        m_artist->setValue(artist_id);
    }

    void Audio::set_add_dtime(QDateTime add_dtime)
    {
        m_add_dtime->setValue(add_dtime);
    }

    void Audio::set_audio_lib_path(const std::string lib_path)
    {
        m_audio_lib_path->setValue(lib_path);
        m_file_info.set_audio_lib_path(lib_path);
    }

    void Audio::set_is_deleted(bool deleted)
    {
        m_is_deleted->setValue(deleted);
    }

    void Audio::set_play_count(int play_count)
    {
        m_play_count->setValue(play_count);
    }

    void Audio::set_duration(int duration)
    {
        m_duration->setValue(duration);
        m_file_info.set_duration(duration);
    }

    void Audio::set_start_marker(double start_marker)
    {
        m_start_marker->setValue(start_marker);
    }

    void Audio::set_fade_in_marker(double fade_in_marker)
    {
        m_fade_in_marker->setValue(fade_in_marker);
    }

    void Audio::set_intro_marker(double intro)
    {
        m_intro_marker->setValue(intro);
    }

    void Audio::set_extro_marker(double extro)
    {
        m_extro_marker->setValue(extro);
    }

    void Audio::set_fade_out_marker(double fade_out)
    {
        m_fade_out_marker->setValue(fade_out);
    }

    void Audio::set_fade_delay_marker(double fade_delay)
    {
        m_fade_delay_marker->setValue(fade_delay);
    }

    void Audio::set_end_marker(double end_marker)
    {
        m_end_marker->setValue(end_marker);
    }

    void Audio::set_file_path(const std::string f_path)
    {
        m_file_path->setValue(f_path);
        set_audio_lib_path(f_path);
    }

    void Audio::set_folder(int folder_id)
    {
        m_folder->setValue(folder_id);

    }

    void Audio::set_genre(int genre_id)
    {
        m_genre->setValue(genre_id);
    }

    void Audio::set_mood(int mood_id)
    {
        m_mood->setValue(mood_id);
    }

    void Audio::set_creation_date(QDate dt)
    {
        m_creation_date->setValue(dt);
    }

    void Audio::set_audio_year(int yr)
    {
        m_audio_year->setValue(yr);
    }

    void Audio::set_audio_type(const std::string audio_type)
    {
        m_audio_type->setValue(audio_type);
    }

    void Audio::set_notes(const std::string notes)
    {
        m_notes->setValue(notes);
    }

    void Audio::set_audio_file(AudioFile audio_file)
    {
        m_file_info = audio_file;
    }

    void Audio::set_file_extension(const std::string extension)
    {
        m_file_extension->setValue(extension);
    }

    void Audio::set_cue_marker(CueMarker marker)
    {
        m_marker = marker;
        set_start_marker(marker.start_marker);
        set_fade_in_marker(marker.fade_in);
        set_intro_marker(marker.intro);
        set_extro_marker(marker.extro);
        set_fade_out_marker(marker.fade_out);
        set_end_marker(marker.end_marker);
        m_file_info.set_marker(marker);
    }


    void Audio::set_audio_filename(const std::string audio_filename)
    {
        m_audio_filename->setValue(audio_filename);
    }

    std::string Audio::artist_fullname() const
    {
        std::string fullname{""};

        auto fk_artist = artist()->unique_fk_entity();
        if (fk_artist != nullptr){
            AUDIO::Artist* artist = dynamic_cast<AUDIO::Artist*>(fk_artist);
            fullname = artist->fullName()->value();
        }

        return fullname;
    }

    CueMarker Audio::cue_marker() const
    {
        CueMarker m;
        m.start_marker = start_marker()->value();
        m.fade_in = fade_in_marker()->value();
        m.intro = intro_marker()->value();
        m.extro = extro_marker()->value();
        m.fade_out = fade_out_marker()->value();
        m.end_marker = end_marker()->value();

        return m;
    }

    std::string Audio::tableName() const
    {
        return m_table_name;
    }

    void Audio::setTableName(const std::string table_name)
    {
        m_table_name = table_name;
    }

    std::unique_ptr<BaseEntity> Audio::mapFields(StringMap *raw_entity)
    {
        return nullptr;
    }

    std::vector<std::string> Audio::tableViewColumns() const
    {
        return tableViewCols<std::string>(title()->displayName());
    }

    std::vector<std::string> Audio::tableViewValues()
    {
        return {
            title()->displayName(),
            artist()->displayName(),
            duration()->displayName(),
            audio_type()->valueToString(),
            file_path()->displayName(),
            m_file_extension->displayName()
        };
    }

    QStringList Audio::tableHeaders() const
    {
        return m_header;
    }

    std::string Audio::searchColumn() const
    {
        return title()->valueToString();
    }

    void Audio::populateEntity()
    {

    }

    std::shared_ptr<BaseEntity> Audio::cloneAsShared()
    {
        return std::make_shared<AUDIO::Audio>("");
    }

    void Audio::afterMapping(BaseEntity &entity)
    {
    }


    StringField* Audio::file_extension() const
    {
        return m_file_extension;
    }

    std::string Audio::get_file_extension(std::string file_name)
    {
        QFileInfo afi(QString::fromStdString(file_name));
        QString file_ext = afi.suffix().toLower();
        return file_ext.toStdString();
    }

    QString Audio::full_audio_filename() const
    {
        AUDIO::AudioTool audio_tool;
        QString name = QString::fromStdString(this->audio_lib_path()->value()+
                audio_tool.make_audio_filename(this->id())+"."+this->file_extension()->value_tolower());

        qDebug() << "Path: " << stoq(this->audio_lib_path()->value());
        qDebug() << "Ext: " << stoq(this->file_extension()->value());

        return name;
    }

   bool operator ==(const AUDIO::Audio& lhs, const AUDIO::Audio& rhs)
   {
       return (lhs.id() == rhs.id());
   }

   std::ostream& operator<<(std::ostream& os, const Audio& audio)
   {
       //os << audio.audio_id()->value() << " : "<< audio.title()->value();
       return os << " FIXME:: Operator << not implemented!! ";
   }

    /* ------ Folder ------ */

    Folder::Folder()
        :m_folder_name{},
         m_parent{}
    {
        m_folder_name = createField<StringField>("folder_name", "Folder Name");
        m_parent = createField<IntegerField>("parent", "Parent ID");

        m_header << QString::fromStdString(m_folder_name->fieldLabel());
        setTableName("rave_folder");
    }

    Folder& Folder::operator=(const Folder& other)
    {
        if (this == &other) return *this;

        m_folder_name->setValue(other.folder_name()->value());
        m_parent->setValue(other.parent()->value());

        m_header << QString::fromStdString(m_folder_name->fieldLabel());
        setTableName("rave_folder");

        return *this;
    }

    StringField *Folder::folder_name() const
    {
        return m_folder_name;
    }

    IntegerField *Folder::parent() const
    {
        return m_parent;
    }

    std::string Folder::tableName() const
    {
        return m_table_name;
    }

    void Folder::setTableName(const std::string table_name)
    {
        m_table_name = table_name;
    }

    std::unique_ptr<BaseEntity> Folder::mapFields(StringMap *raw_entity)
    {
        return nullptr;
    }

    std::vector<std::string> Folder::tableViewColumns() const
    {
        return tableViewCols<std::string>(folder_name()->displayName());
    }

    std::vector<std::string> Folder::tableViewValues()
    {
        return {folder_name()->displayName()};
    }

    QStringList Folder::tableHeaders() const
    {
        return m_header;
    }

    std::string Folder::searchColumn() const
    {
        return folder_name()->valueToString();
    }

    void Folder::populateEntity()
    {

    }

    std::shared_ptr<BaseEntity> Folder::cloneAsShared()
    {
        return std::make_shared<Folder>();
    }

    void Folder::afterMapping(BaseEntity &entity)
    {

    }

    ActionResult Folder::validate()
    {
        return ActionResult();

    }

   /* ------ Mood ----------- */

    Mood::Mood()
        :m_mood_name{nullptr},
         m_intensity{nullptr},
         m_timbre{nullptr},
         m_rhythm{nullptr},
         m_pitch{nullptr}
    {
        m_mood_name = createField<StringField>("mood", "Mood");

        m_intensity = createField<ChoiceField<std::string>>("intensity", "Intensity");
        m_intensity->addChoice({"low", "LOW"});
        m_intensity->addChoice({"vlow", "VERY LOW"});
        m_intensity->addChoice({"medium", "MEDIUM"});
        m_intensity->addChoice({"high", "HIGH"});
        m_intensity->addChoice({"vhigh", "VERY HIGH"});

        m_timbre = createField<ChoiceField<std::string>>("timbre", "Timbre");
        m_timbre->addChoice({"low", "LOW"});
        m_timbre->addChoice({"vlow", "VERY LOW"});
        m_timbre->addChoice({"medium", "MEDIUM"});
        m_timbre->addChoice({"high", "HIGH"});
        m_timbre->addChoice({"vhigh", "VERY HIGH"});

        m_pitch = createField<ChoiceField<std::string>>("pitch", "Pitch");
        m_pitch->addChoice({"low", "LOW"});
        m_pitch->addChoice({"vlow", "VERY LOW"});
        m_pitch->addChoice({"medium", "MEDIUM"});
        m_pitch->addChoice({"high", "HIGH"});
        m_pitch->addChoice({"vhigh", "VERY HIGH"});

        m_rhythm = createField<ChoiceField<std::string>>("rhythm", "Rhythm");
        m_rhythm->addChoice({"low", "LOW"});
        m_rhythm->addChoice({"vlow", "VERY LOW"});
        m_rhythm->addChoice({"medium", "MEDIUM"});
        m_rhythm->addChoice({"high", "HIGH"});
        m_rhythm->addChoice({"vhigh", "VERY HIGH"});

        m_header << QString::fromStdString(m_mood_name->fieldLabel());
        setTableName("rave_mood");
    }

    StringField *Mood::mood_name() const
    {
        return m_mood_name;
    }

    ChoiceField<std::string> *Mood::intensity() const
    {
        return m_intensity;
    }

    ChoiceField<std::string> *Mood::timbre() const
    {
        return m_timbre;

    }

    ChoiceField<std::string> *Mood::pitch() const
    {
        return m_pitch;
    }

    /*
    ChoiceField<std::string> *Mood::rythm() const
    {
        return m_rythm;
    }
    */

    void Mood::set_mood_name(const std::string name)
    {
        m_mood_name->setValue(name);
    }

    void Mood::set_intensity(const std::string intensity)
    {
        m_intensity->setValue(intensity);
    }

    void Mood::set_timbre(const std::string timbre)
    {
        m_timbre->setValue(timbre);
    }

    void Mood::set_pitch(const std::string pitch)
    {
        m_pitch->setValue(pitch);
    }

    /*
    void Mood::set_rythm(const std::string rythm)
    {
        m_rythm->setValue(rythm);
    }
    */

    std::string Mood::tableName() const
    {
        return m_table_name;
    }

    void Mood::setTableName(const std::string table_name)
    {
        m_table_name = table_name;
    }

    std::unique_ptr<BaseEntity> Mood::mapFields(StringMap *raw_entity)
    {
        return nullptr;

    }

    std::vector<std::string> Mood::tableViewColumns() const
    {
        return tableViewCols<std::string>(mood_name()->displayName());
    }

    std::vector<std::string> Mood::tableViewValues()
    {
        return {mood_name()->displayName()};
    }

    QStringList Mood::tableHeaders() const
    {
        return m_header;
    }

    std::string Mood::searchColumn() const
    {
        return mood_name()->valueToString();
    }

    void Mood::populateEntity()
    {

    }

    std::shared_ptr<BaseEntity> Mood::cloneAsShared()
    {
        return std::make_shared<Mood>();
    }

    void Mood::afterMapping(BaseEntity &entity)
    {

    }

    ActionResult Mood::validate()
    {
        return ActionResult();

    }



}
