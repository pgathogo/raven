#include "cueeditor.h"
#include "audiowaveform.h"
#include "audioplayer.h"

CueEditor::CueEditor(AudioFile& audio_file, const std::string name)
    :m_audio_file{audio_file}
     ,m_audio_wave_form{nullptr}
     ,m_player_only{name}
{
   m_audio_player = std::make_unique<AudioPlayer>(m_audio_file);
}

CueEditor::CueEditor(AudioFile& audio_file, QApplication* q_app)
    :m_audio_file{audio_file}
    ,app(q_app)
{
    m_audio_wave_form = new AUDIO::AudioWaveForm(m_audio_file);
}

CueEditor::~CueEditor()
{
    if (m_audio_wave_form != nullptr){
        delete static_cast<AUDIO::AudioWaveForm*>(m_audio_wave_form);
        //delete app;
     }
}

int CueEditor::editor()
{
    AUDIO::AudioWaveForm* awf = static_cast<AUDIO::AudioWaveForm*>(m_audio_wave_form);
    auto ret_id = awf->exec();

    if (ret_id == DialogCloseResult::Save)
        m_marker = awf->marker();

    return ret_id;
}

void CueEditor::play_audio()
{
   m_audio_player->play_audio();

}

void CueEditor::stop_audio()
{
    m_audio_player->stop_play();
}

Marker CueEditor::marker() const
{
    return m_marker;
}

