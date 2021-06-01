#ifndef CUEEDITOR_H
#define CUEEDITOR_H

#include <memory>
#include "cueeditor_global.h"

#include "../../audio/audiofile.h"

class AudioPlayer;

namespace AUDIO{
    class AudioWaveForm;
}

class CUEEDITOR_EXPORT CueEditor
{
public:
    enum DialogCloseResult{Cancel, Save};

    CueEditor(AudioFile& audio_file, const std::string name);
    CueEditor(AudioFile& audio_file);
    ~CueEditor();
    int editor();
    void play_audio();
    void stop_audio();
    void set_audio(AudioFile& audio_file);
    Marker marker() const;
private:
    AudioFile& m_audio_file;
    void* m_audio_wave_form;
    Marker m_marker;
    std::unique_ptr<AudioPlayer> m_audio_player;
    AUDIO::AudioWaveForm* m_awf;
    const std::string m_player_only;
};

#endif // CUEEDITOR_H
