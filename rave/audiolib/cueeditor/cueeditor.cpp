#include "cueeditor.h"
#include "audiowaveform.h"

CueEditor::CueEditor(AudioFile audio_file)
    :m_audio_file{audio_file}
{
    m_audio_wave_form = new AUDIO::AudioWaveForm(m_audio_file);
}

Marker CueEditor::editor()
{
    Marker marks;

    AUDIO::AudioWaveForm* awf = static_cast<AUDIO::AudioWaveForm*>(m_audio_wave_form);

    if (awf->exec() == DialogCloseResult::Save){
        marks.start_marker = 2;
        marks.end_marker = 42;
        marks.is_marked = true;
    }

    return marks;
}
