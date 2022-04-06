#ifndef CUEEDITOR_H
#define CUEEDITOR_H

#include <QApplication>

#include <memory>
#include "cueeditor_global.h"

#include "../../audio/audiofile.h"

class AudioPlayer;

namespace AUDIO{
    class AudioWaveForm;
}

class CUEEDITOR_EXPORT CueEditor
{
    QApplication* app;
public:
    enum DialogCloseResult{Cancel, Save};

    CueEditor(AudioFile& audio_file, const std::string name);
    CueEditor(AudioFile& audio_file, QApplication* q_app);
    //CueEditor(AudioFile& audio_file, int argc=0, char** argv = NULL);
    ~CueEditor();
    int editor();
    void play_audio();
    void stop_audio();
    void set_audio(AudioFile& audio_file);
    CueMarker cue_marker() const;
private:
    AudioFile& m_audio_file;
    void* m_audio_wave_form;
    CueMarker m_cue_marker;
    std::unique_ptr<AudioPlayer> m_audio_player;
    AUDIO::AudioWaveForm* m_awf;
    const std::string m_player_only;
};

#endif // CUEEDITOR_H
