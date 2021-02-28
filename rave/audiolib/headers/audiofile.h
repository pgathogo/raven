#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include <string>

#include "audiofile_global.h"

struct Marker{
    double start_marker{0.0};
    double fade_in{0.0};
    double intro{0.0};
    double extro{0.0};
    double fade_out{0.0};
    double end_marker{0.0};
    bool is_marked{false};
};

class AUDIOFILE_EXPORT AudioFile
{
public:
    AudioFile(const std::string audio_file);
    std::string audio_title() const;
    std::string artist_name() const;
    std::string audio_path() const;
    std::string audio_file() const;
    std::string audio_type() const;
    std::string wave_file() const;
    int duration() const;

    void set_audio_title(const std::string audio_title);
    void set_artist_name(const std::string artist_name);
    void set_audio_path(const std::string audio_path);
    void set_audio_file(const std::string audio_file);
    void set_audio_type(const std::string audio_type);
    void set_wave_file(const std::string wave_file);
    void set_duration(int duration);

    bool is_valid();
private:
    std::string m_audio_title;
    std::string m_artist_name;
    std::string m_audio_path;
    std::string m_audio_file;
    std::string m_audio_type;
    std::string m_wave_file;  // png file
    int m_duration;
};

#endif // AUDIOFILE_H
