#include "audioplayer.h"
#include "audiothread.h"

namespace AUDIO
{
    using OPChannel = AudioPlayer::OutputChannel;

    AudioPlayer::OutputChannel current_output_channel = AudioPlayer::OutputChannel::ChannelA;

    AudioPlayer::AudioPlayer()
        :m_audio_thread{nullptr}
    {
        m_audio_thread = std::make_unique<AudioThread>(this);
        connect(m_audio_thread.get(), &AudioThread::end_of_playback, this, &AudioPlayer::end_of_playback);
    }

    AudioPlayer::AudioPlayer(AudioFile& audio_file)
          :m_audio_thread{nullptr}
           ,m_audio_file{audio_file}
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

        qDebug() << "Item added: Output=" << output << " Item="<< play_item;
    }

    void AudioPlayer::play_audio(QString /*channel*/, QString audio_file)
    {
//        OPChannel op_ch = str_to_channel(channel);
        m_audio_thread->play(audio_file);
        //m_audio_thread->play(m_output_channel[op_ch]);
    }

    void AudioPlayer::stop_play()
    {
        if (m_audio_thread != nullptr)
            m_audio_thread->stop();
    }

    void AudioPlayer::end_of_playback()
    {
        m_audio_thread->stop();
        emit end_of_play();

//        if (current_channel < m_output_channel.size()){
//            play_audio();
//            emit play_next();
//        }else {
//            emit end_of_play();
//        }

    }

    OPChannel AudioPlayer::str_to_channel(QString channel)
    {
        return (channel == "A" ? OPChannel::ChannelA : OPChannel::ChannelB);
    }
}
