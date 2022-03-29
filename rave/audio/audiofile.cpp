#include "audiofile.h"

AudioFile::AudioFile()
    :m_audio_title{""}
     ,m_artist_name{""}
     ,m_audio_path{""}
     ,m_audio_file{""}
     ,m_audio_type{""}
     ,m_wave_file{""}
     ,m_filename{""}
     ,m_file_ext{""}
     ,m_short_filename{""}
     ,m_duration{0}
     ,m_marker{}
     ,m_id{-1}
{
}

AudioFile::AudioFile(const std::string a_file)
    :m_audio_title{""},
     m_artist_name{""},
     m_audio_path{""},
     m_audio_file{a_file},
     m_audio_type{""},
     m_wave_file{""},
     m_filename{""},
     m_file_ext{""},
     m_short_filename{""},
     m_duration{0},
     m_marker{},
     m_id{-1}
{
    std::filesystem::path af{a_file};
    m_file_ext = af.extension().u8string();
    m_filename = af.filename().u8string(); // contains plus the path
    m_audio_path = af.remove_filename().u8string();

    std::filesystem::path f{m_filename};
    m_short_filename = f.replace_extension().u8string();

    m_wave_file = m_audio_path+m_short_filename+".png";

    m_adf_file = m_audio_path+m_short_filename+".adf";

    m_ogg_short_filename = m_short_filename+".ogg";

    m_ogg_filename = m_audio_path+m_ogg_short_filename;

}

std::string AudioFile::audio_title() const
{
    return m_audio_title;
}

std::string AudioFile::short_desc() const
{
    return m_short_desc;
}

std::string AudioFile::artist_name() const
{
    return m_artist_name;
}

std::string AudioFile::audio_path() const
{
    return m_audio_path;
}

std::string AudioFile::audio_file() const
{
    return m_audio_file;
}

std::string AudioFile::audio_type() const
{
    return m_audio_type;
}

std::string AudioFile::wave_file() const
{
    return m_wave_file;
}

std::string AudioFile::filename() const
{
    return m_filename;
}

std::string AudioFile::file_ext() const
{
    return m_file_ext;
}

std::string AudioFile::short_filename() const
{
    return m_short_filename;
}

std::string AudioFile::wave_filename() const
{
    return m_wave_file;
}

std::string AudioFile::adf_file() const
{
    return m_adf_file;
}

std::string AudioFile::audio_lib_path() const
{
    return m_audio_lib_path;
}

std::string AudioFile::ogg_short_filename() const
{
    return m_ogg_short_filename;
}

std::string AudioFile::ogg_filename() const
{
    return m_ogg_filename;
}

std::string AudioFile::creation_date() const
{
    return m_creation_date;
}

std::string AudioFile::audio_class() const
{
    return m_audio_class;
}

std::string AudioFile::genre() const
{
    return m_genre;
}

int AudioFile::id() const
{
    return m_id;
}

int AudioFile::duration() const
{
    return m_duration;
}

int AudioFile::temp_id() const
{
    return m_temp_id;
}

int AudioFile::year() const
{
    return m_year;
}

uintmax_t AudioFile::file_size() const
{
    uintmax_t f_size;
    std::filesystem::path f{audio_file()};

    try{
        f_size = std::filesystem::file_size(f);
    } catch (std::filesystem::filesystem_error& e){  // need to throw
        // log(e.what())
    }

    return f_size;
}

void AudioFile::set_audio_title(const std::string audio_title)
{
    m_audio_title = audio_title;
}

void AudioFile::set_short_desc(const std::string short_desc)
{
    m_short_desc = short_desc;
}

void AudioFile::set_artist_name(const std::string artist_name)
{
    m_artist_name = artist_name;
}

void AudioFile::set_audio_path(const std::string audio_path)
{
    m_audio_path = audio_path;
}

void AudioFile::set_audio_file(const std::string audio_file)
{
    m_audio_file = audio_file;
}

void AudioFile::set_audio_type(const std::string audio_type)
{
    m_audio_type = audio_type;
}

void AudioFile::set_wave_file(const std::string wave_file)
{
    m_wave_file = wave_file;
}

void AudioFile::set_duration(int duration)
{
    m_duration = duration;
}

void AudioFile::set_adf_file(const std::string adf_file)
{
    m_adf_file = adf_file;
}

void AudioFile::set_audio_lib_path(const std::string lib_path)
{
    m_audio_lib_path = lib_path;
}

void AudioFile::set_creation_date(const std::string create_date)
{
    m_creation_date = create_date;
}

void AudioFile::set_audio_class(const std::string a_class)
{
    m_audio_class = a_class;
}
void AudioFile::set_genre(const std::string genre)
{
    m_genre = genre;
}

void AudioFile::set_id(int id)
{
    m_id = id;
}

void AudioFile::set_ogg_short_filename(const std::string name)
{
    m_ogg_short_filename = name;
}

void AudioFile::set_ogg_filename(const std::string name)
{
    m_ogg_filename = name;
}

void AudioFile::set_temp_id(int id)
{
    m_temp_id = id;
}
void AudioFile::set_year(int yr)
{
    m_year = yr;
}

bool AudioFile::is_valid() const
{
    // check if file is valid (exists ... and the likes)
    return true;
}

Marker AudioFile::marker() const
{
    return m_marker;
}

std::string AudioFile::get_audio_lib()
{
    return (audio_lib_path().empty()) ? audio_path() : audio_lib_path();
}

void AudioFile::set_marker(Marker marker)
{
    m_marker = marker;
}
