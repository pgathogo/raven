#include "audioplayer.h"
#include "audiothread.h"


namespace AUDIO
{
    using OPChannel = AudioPlayer::OutputChannel;

    AudioPlayer::OutputChannel current_output_channel = AudioPlayer::OutputChannel::ChannelA;

    int AudioPlayer::play_list_index = 0;

    AudioPlayer::AudioPlayer()
        :m_audio_thread{nullptr}
        ,m_played_audio{""}
    {
        m_audio_thread = std::make_unique<AudioThread>(this);
        connect(m_audio_thread.get(), &AudioThread::end_of_playback, this, &AudioPlayer::end_of_playback);
    }

    AudioPlayer::AudioPlayer(AudioFile& audio_file)
        :m_audio_thread{nullptr}
        ,m_audio_file{audio_file}
        ,m_played_audio{""}
    {
        m_audio_thread = std::make_unique<AudioThread>(this);
        connect(m_audio_thread.get(), &AudioThread::end_of_playback, this, &AudioPlayer::end_of_playback);

        //update_output_channel(QString::fromStdString(m_audio_file.audio_file()));
    }

//    AudioPlayer::AudioPlayer(std::vector<QString> play_list)
//        :m_audio_file()
//    {
//        m_audio_thread = std::make_unique<AudioThread>(this);
//        connect(m_audio_thread.get(), &AudioThread::end_of_playback, this, &AudioPlayer::end_of_playback);

//        for (auto play_item : play_list){
//            add_to_playlist(play_item);
//        }
//    }

    AudioPlayer::~AudioPlayer()
    {
    }

    void AudioPlayer::update_output_channel(QString output, QString play_item)
    {
        OPChannel op_ch = str_to_channel(output);

        m_output_channel[op_ch] = play_item;

    }

    void AudioPlayer::play_audio(QString audio_file)
    {
        m_played_audio = audio_file;
        m_audio_thread->play(audio_file);
    }

    void AudioPlayer::play_audio()
    {
        if (m_playlist.size() > 0) {
            //m_audio_thread->play(m_playlist[0].audio_filename);
            play_audio(m_playlist[0].audio_filename);
            m_playlist.erase(m_playlist.begin());
            --AudioPlayer::play_list_index;
        }
    }

    void AudioPlayer::append_playlist(QString output, QString filename)
    {
        PlayList pl;
        pl.output_channel = output;
        pl.audio_filename = filename;
        qDebug() << "Audio Player: "<<filename;
        m_playlist.push_back(pl);
        ++AudioPlayer::play_list_index;
    }

    void AudioPlayer::stop_play()
    {
        qDebug() << ">>> Audio Player::stop_play >>>";

        emit audio_played(m_played_audio);

        if (m_audio_thread != nullptr)
            m_audio_thread->stop();
    }

    void AudioPlayer::end_of_playback()
    {
        //m_audio_thread->stop();
        qDebug() << "Audio Player::end_of_playback";

        stop_play();

        emit audio_played(m_played_audio);

        if (AudioPlayer::play_list_index > 0){
            play_audio();
        }else {
            emit end_of_play();
        }

    }

    OPChannel AudioPlayer::str_to_channel(QString channel)
    {
        return (channel == "A" ? OPChannel::ChannelA : OPChannel::ChannelB);
    }
}
