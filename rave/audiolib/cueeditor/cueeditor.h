#ifndef CUEEDITOR_H
#define CUEEDITOR_H

#include <memory>
#include "cueeditor_global.h"

#include "../headers/audiofile.h"

class CUEEDITOR_EXPORT CueEditor
{
public:
    enum DialogCloseResult{Cancel, Save};

    CueEditor(AudioFile audio_file);
    Marker editor();
private:
    AudioFile m_audio_file;
    void* m_audio_wave_form;
};

#endif // CUEEDITOR_H
