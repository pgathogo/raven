#include "audiofile.h"

AudioFile::AudioFile(const std::string audio_file)
    :m_audio_title{""},
     m_artist_name{""},
     m_audio_path{""},
     m_audio_file{audio_file},
     m_audio_type{""},
     m_wave_file{""},
     m_duration{0}
{

}

std::string AudioFile::audio_title() const
{
    return m_audio_title;
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

int AudioFile::duration() const
{
    return m_duration;
}

void AudioFile::set_audio_title(const std::string audio_title)
{
    m_audio_title = audio_title;
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

bool AudioFile::is_valid()
{
    // check if file is valid (exists ... and the likes)
    return true;
}
