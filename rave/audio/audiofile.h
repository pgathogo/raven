#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include <string>
#include <filesystem>

struct Marker{
    double start_marker{0.0};
    double fade_in{0.0};
    double intro{0.0};
    double extro{0.0};
    double fade_out{0.0};
    double end_marker{0.0};
    bool is_marked{false};

    void set_start_marker(double v){ start_marker = v; }
    void set_fade_in(double v){ fade_in = v; }
    void set_intro(double v){ intro = v; }
    void set_extro(double v){ extro = v; }
    void set_fade_out(double v){ fade_out = v; }
    void set_end_marker(double v){ end_marker = v; }
};

class  AudioFile
{
public:
    AudioFile(const std::string audio_file);

    void set_audio_title(const std::string audio_title);
    void set_artist_name(const std::string artist_name);
    void set_audio_path(const std::string audio_path);
    void set_audio_file(const std::string audio_file);
    void set_audio_type(const std::string audio_type);
    void set_wave_file(const std::string wave_file);
    void set_duration(int duration);
    void set_adf_file(const std::string adf_file);
    void set_audio_lib_path(const std::string lib_path);

    void set_id(int id);
    void set_ogg_filename(const std::string name);
    void set_ogg_short_filename(const std::string name);
    void set_temp_id(int id);
    void set_marker(Marker marker);

    std::string audio_title() const;
    std::string artist_name() const;
    std::string audio_path() const;
    std::string audio_file() const;
    std::string audio_type() const;
    std::string wave_file() const;
    std::string filename() const;
    std::string file_ext() const;
    std::string short_filename() const;
    std::string wave_filename() const;
    std::string adf_file() const;
    std::string audio_lib_path() const;
    std::string ogg_short_filename() const;
    std::string ogg_filename() const;

    int id() const;
    int duration() const;
    int temp_id() const;
    uintmax_t file_size() const;

    bool is_valid() const;
    Marker marker() const;

    std::string get_audio_lib();

private:
    std::string m_audio_title;
    std::string m_artist_name;
    std::string m_audio_path;
    std::string m_audio_file;
    std::string m_audio_type;
    std::string m_wave_file;  		// full path filename with extension ".png"
    std::string m_filename;  		// audio fileanme without the path.
    std::string m_file_ext;  		// audio filename extension (".mp3, ,.ogg")
    std::string m_short_filename;   // filename without extension.
    std::string m_adf_file;  		// full path with filena of Audio Data File (.*adf)
    std::string m_audio_lib_path;	// path where Raven audio files are stored.
    std::string m_ogg_short_filename;
    std::string m_ogg_filename;
    int m_duration;
    int m_id;
    int m_temp_id;
    Marker m_marker;
};

#endif // AUDIOFILE_H
