#ifndef CUEEDITOR_H
#define CUEEDITOR_H

#include <memory>
#include "cueeditor_global.h"

#include "../../audio/audiofile.h"

class AudioPlayer;

class CUEEDITOR_EXPORT CueEditor
{
public:
    enum DialogCloseResult{Cancel, Save};

    CueEditor(AudioFile& audio_file);
    ~CueEditor();
    int editor();
    void play_audio();
    void stop_audio();
    Marker marker() const;
private:
    AudioFile& m_audio_file;
    void* m_audio_wave_form;
    Marker m_marker;
    AudioPlayer* m_audio_player;
};

#endif // CUEEDITOR_H
