#ifndef CUEEDITOR_H
#define CUEEDITOR_H

#include <memory>
#include "cueeditor_global.h"

#include "../../audio/audiofile.h"

class CUEEDITOR_EXPORT CueEditor
{
public:
    enum DialogCloseResult{Cancel, Save};

    CueEditor(AudioFile& audio_file);
    ~CueEditor();
    int editor();
    Marker marker() const;
private:
    AudioFile& m_audio_file;
    void* m_audio_wave_form;
    Marker m_marker;
};

#endif // CUEEDITOR_H
