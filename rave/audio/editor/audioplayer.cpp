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

        connect(m_audio_thread.get(), &AudioThread::current_position, this, &AudioPlayer::audio_position);
    }

    AudioPlayer::AudioPlayer(AudioFile& audio_file)
        :m_audio_thread{nullptr}
        ,m_audio_file{audio_file}
        ,m_played_audio{""}
    {
        m_audio_thread = std::make_unique<AudioThread>(this);
        connect(m_audio_thread.get(), &AudioThread::end_of_playback, this, &AudioPlayer::end_of_playback);
        connect(m_audio_thread.get(), &AudioThread::current_position, this, &AudioPlayer::audio_position);
        // connect(m_audio_thread.get(), &AudioThread::current_peak, this, &AudioPlayer::audio_current_peak);
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

    std::tuple<float, float> AudioPlayer::audio_levels()
    {
        //m_audio_thread->channel_levels();
        return m_audio_thread->get_channel_levels();
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
        emit sig_start_play();
    }


    void AudioPlayer::play_audio()
    {
        if (m_playlist.size() > 0) {
            //m_audio_thread->play(m_playlist[0].audio_filename);
            play_audio(m_playlist[0].audio_filename);
            m_playlist.erase(m_playlist.begin());
            --AudioPlayer::play_list_index;
        emit sig_start_play();
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

        emit sig_stop_play();
    }

    void AudioPlayer::audio_position(double position, double total)
    {
        emit audio_current_position(position, total);

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
            emit sig_stop_play();  // TODO:: What is the difference "end_of_play" and "stop_play" signal
        }

    }

    float* AudioPlayer::audio_current_peak()
    {
        float* fft = m_audio_thread->get_channel_data();
        return fft;

        // qDebug() << "AP: "<< fft;
        // emit current_peak(fft);
    }

    OPChannel AudioPlayer::str_to_channel(QString channel)
    {
        return (channel == "A" ? OPChannel::ChannelA : OPChannel::ChannelB);
    }

    void AudioPlayer::set_audio_file(const AudioFile af)
    {
        m_audio_file = af;

    }

    AudioFile AudioPlayer::audio_file()
    {
        return m_audio_file;
    }

    unsigned int AudioPlayer::audio_length()
    {
        return m_audio_thread->audio_len(QString::fromStdString(m_audio_file.audio_file()));
    }

    float AudioPlayer::audio_bitrate()
    {
        return m_audio_thread->audio_bitrate(QString::fromStdString(m_audio_file.audio_file()));

    }

    double AudioPlayer::audio_duration()
    {

        double audio_duration_secs = m_audio_thread->audio_len(QString::fromStdString(m_audio_file.audio_file()));
        return audio_duration_secs;

    }

    float AudioPlayer::audio_sample_rate()
    {
        return m_audio_thread->audio_sample_rate(QString::fromStdString(m_audio_file.audio_file()));
    }

    void AudioPlayer::play()
    {
        m_audio_thread->play(QString::fromStdString(m_audio_file.audio_file()));
        emit sig_start_play();

    }

    void AudioPlayer::change_position(int new_position)
    {
        m_audio_thread->change_position(new_position);

    }

    void AudioPlayer::play_from_position(int position)
    {
        m_audio_thread->play_from_position(QString::fromStdString(m_audio_file.audio_file()),
                                           position);

    }

}
