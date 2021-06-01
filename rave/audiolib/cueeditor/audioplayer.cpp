#include "audioplayer.h"
#include "audiothread.h"

AudioPlayer::AudioPlayer(AudioFile& audio_file)
      :m_audio_file{audio_file}
{
    m_audio_thread = std::make_unique<AudioThread>(this);
}

AudioPlayer::~AudioPlayer()
{
}

void AudioPlayer::play_audio()
{
    m_audio_thread->play(QString::fromStdString(m_audio_file.audio_file()));
}

void AudioPlayer::stop_play()
{
    if (m_audio_thread != nullptr)
        m_audio_thread->stop();
}
