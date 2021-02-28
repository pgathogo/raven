#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include <string>

#include "audiofile_global.h"

class AUDIOFILE_EXPORT AudioFile
{
public:
    AudioFile(const std::string audio_path);
    std::string audio_title() const;
    std::string artist_name() const;
    std::string audio_path() const;
    std::string filename() const;
    std::string audio_type() const;
    std::string wave_file() const;
    int duration() const;

    void set_audio_title(const std::string audio_title);
    void set_artist_name(const std::string artist_name);
    void set_audio_path(const std::string audio_path);
    void set_filename(const std::string filename);
    void set_audio_type(const std::string audio_type);
    void set_wave_file(const std::string wave_file);
    void set_duration(int duration);
private:
    std::string m_audio_title;
    std::string m_artist_name;
    std::string m_audio_path;
    std::string m_filename;
    std::string m_audio_type;
    std::string m_wave_file;  // png file
    int m_duration;
};

#endif // AUDIOFILE_H
