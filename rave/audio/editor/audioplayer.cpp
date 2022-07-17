#include "audioplayer.h"
#include "audiothread.h"

namespace AUDIO
{
    int AudioPlayer::play_count = 0;

    AudioPlayer::AudioPlayer(AudioFile& audio_file)
          :m_audio_file{audio_file}
    {
        m_audio_thread = std::make_unique<AudioThread>(this);
        connect(m_audio_thread.get(), &AudioThread::end_of_playback, this, &AudioPlayer::end_of_playback);

        add_to_playlist(QString::fromStdString(m_audio_file.audio_file()));
    }

    AudioPlayer::AudioPlayer(std::vector<QString> play_list)
        :m_audio_file()
    {
        m_audio_thread = std::make_unique<AudioThread>(this);
        connect(m_audio_thread.get(), &AudioThread::end_of_playback, this, &AudioPlayer::end_of_playback);

        for (auto play_item : play_list){
            add_to_playlist(play_item);
        }
    }

    AudioPlayer::~AudioPlayer()
    {
    }

    void AudioPlayer::add_to_playlist(QString play_item)
    {
        m_play_list.push_back(play_item);
    }

    void AudioPlayer::play_audio()
    {
        m_audio_thread->play(m_play_list[play_count++]);
    }

    void AudioPlayer::stop_play()
    {
        if (m_audio_thread != nullptr)
            m_audio_thread->stop();
    }

    void AudioPlayer::end_of_playback()
    {
        m_audio_thread->stop();

        if (play_count < m_play_list.size()){
            play_audio();
            emit play_next();
        }else {
            emit end_of_play();
        }

    }

}
