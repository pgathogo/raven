#include "cueeditor.h"
#include "audiowaveform.h"
#include "audioplayer.h"

CueEditor::CueEditor(AudioFile& audio_file, const std::string name)
    :m_audio_file{audio_file}
    ,m_audio_wave_form{nullptr}
    ,m_player_only{name}
{
   m_audio_wave_form = std::make_unique<AUDIO::AudioWaveForm>(nullptr, m_audio_file);
   m_audio_player = std::make_unique<AudioPlayer>(m_audio_file);
}

CueEditor::CueEditor(AudioFile& audio_file, QApplication* q_app)
    :m_audio_file{audio_file}
{
    //m_audio_wave_form = new AUDIO::AudioWaveForm(m_audio_file);
    m_audio_wave_form = std::make_unique<AUDIO::AudioWaveForm>(q_app, m_audio_file);
    m_audio_player = std::make_unique<AudioPlayer>(m_audio_file);
}

CueEditor::CueEditor(AudioFile& audio_file)
    :m_audio_file{audio_file}
{
    int argc;
    char* argv[1];
    m_app = std::make_unique<QApplication>(argc, argv);
    //m_audio_wave_form = new AUDIO::AudioWaveForm(m_audio_file);
    m_audio_wave_form = std::make_unique<AUDIO::AudioWaveForm>(m_app.get(), m_audio_file);
    m_audio_player = std::make_unique<AudioPlayer>(m_audio_file);
}

CueEditor::~CueEditor()
{
}

int CueEditor::editor()
{
    AUDIO::AudioWaveForm* awf = static_cast<AUDIO::AudioWaveForm*>(m_audio_wave_form.get());
    auto ret_id = awf->exec();

    if (ret_id == DialogCloseResult::Save)
        m_cue_marker = awf->cue_marker();

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

CueMarker CueEditor::cue_marker() const
{
    return m_cue_marker;
}

