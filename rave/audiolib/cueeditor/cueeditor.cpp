#include "cueeditor.h"
#include "audiowaveform.h"

CueEditor::CueEditor(AudioFile audio_file)
    :m_audio_file{audio_file}
{
    m_audio_wave_form = new AUDIO::AudioWaveForm(m_audio_file);
}

CueEditor::~CueEditor()
{
    delete static_cast<AUDIO::AudioWaveForm*>(m_audio_wave_form);
}

int CueEditor::editor()
{
    AUDIO::AudioWaveForm* awf = static_cast<AUDIO::AudioWaveForm*>(m_audio_wave_form);
    auto ret_id = awf->exec();

    if (ret_id == DialogCloseResult::Save)
        m_marker = awf->marker();

    return ret_id;
}

Marker CueEditor::marker() const
{
    return m_marker;
}
