#include "audioplayer.h"

#include <QMetaObject>

namespace AUDIO
{
    using OPChannel = AudioPlayer::OutputChannel;

    AudioPlayer::OutputChannel current_output_channel = AudioPlayer::OutputChannel::ChannelA;

    int AudioPlayer::play_list_index = 0;
    int AudioPlayer::play_counter = 0;

    AudioPlayer::AudioPlayer()
        :m_audio_thread{nullptr}
        ,m_played_audio{""}
    {
        m_audio_thread = std::make_unique<AudioThread>(this);
        init_connections();
    }

    AudioPlayer::AudioPlayer(AudioFile& audio_file)
        :m_audio_thread{nullptr}
        ,m_audio_file{audio_file}
        ,m_played_audio{""}
    {
        m_audio_thread = std::make_unique<AudioThread>(this);
        init_connections();

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
        cancel_waveform();
    }

    // Both constructors used to wire the signals separately (and the default
    // constructor forgot timer_tick) - now they share one place.
    void AudioPlayer::init_connections()
    {
        qRegisterMetaType<WaveformDataPtr>();

        connect(m_audio_thread.get(), &AudioThread::end_of_playback, this, &AudioPlayer::end_of_playback);
        connect(m_audio_thread.get(), &AudioThread::current_position, this, &AudioPlayer::audio_position);
        connect(m_audio_thread.get(), &AudioThread::timer_tick, this, [this]{ emit player_timer_tick(); });
    }

    void AudioPlayer::set_state(PlaybackState state)
    {
        m_playback_state = state;
        emit playback_state_changed(state);
    }

    std::tuple<float, float> AudioPlayer::audio_levels()
    {
        //m_audio_thread->channel_levels();
        return m_audio_thread->get_channel_levels();
    }

    AudioInfo AudioPlayer::get_audio_info(std::string& audio_file)
    {
        return m_audio_thread->get_audio_info(audio_file);
    }

    void AudioPlayer::fetch_audio_energy(const std::string& audio_file, std::vector<float>& energy_data)
    {
        m_audio_thread->fetch_audio_energy(audio_file, energy_data);
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
        m_playback_state = PlaybackState::PlayingState;
        emit sig_start_play();
        emit playback_state_changed(PlaybackState::PlayingState);
    }


    void AudioPlayer::play_audio()
    {
        if (m_playlist.size() > 0) {
            play_audio(m_playlist[0].audio_filename);

            emit sig_play_item_index(m_playlist[0].grid_index,
                                     m_playlist[0].schedule_id,
                                     AudioPlayer::play_counter++);

            m_playlist.erase(m_playlist.begin());
            --AudioPlayer::play_list_index;
        }
    }

    void AudioPlayer::append_playlist(int grid_index, int schedule_id, QString output, QString filename)
    {
        PlayList pl;
        pl.grid_index = grid_index;
        pl.schedule_id = schedule_id;
        pl.output_channel = output;
        pl.audio_filename = filename;
        m_playlist.push_back(pl);
        ++AudioPlayer::play_list_index;
        AudioPlayer::play_counter = 0;
    }

    void AudioPlayer::stop_play()
    {
        qDebug() << ">>> Audio Player::stop_play >>>";

        emit audio_played(m_played_audio);

        if (m_audio_thread != nullptr)
            m_audio_thread->stop();

        m_playback_state = PlaybackState::StoppedState;
        emit sig_stop_play();
        emit playback_state_changed(PlaybackState::StoppedState);
    }

    void AudioPlayer::fade_audio()
    {
        emit audio_played(m_played_audio);

        if (m_audio_thread != nullptr)
            m_audio_thread->fade_audio();
    }

    void AudioPlayer::pause_audio()
    {
        if (m_audio_thread != nullptr) {
            m_audio_thread->pause();
            m_playback_state = PlaybackState::PausedState;
            emit playback_state_changed(PlaybackState::PausedState);
        }
    }

    void AudioPlayer::resume_audio()
    {
        if(m_audio_thread != nullptr) {
            m_audio_thread->resume();
            m_playback_state = PlaybackState::PlayingState;
            emit playback_state_changed(PlaybackState::PlayingState);
        }
    }

    void AudioPlayer::audio_position(double position, double total)
    {
        m_current_position = position;

        emit audio_current_position(position, total);

    }

    double AudioPlayer::current_position()
    {
        return m_current_position;
    }

    void AudioPlayer::end_of_playback()
    {
        //m_audio_thread->stop();
        qDebug() << "Audio Player::end_of_playback";

        stop_play();

        emit audio_played(m_played_audio);

        // Was: "if (AudioPlayer::play_list_index > 0)". That counter is static, i.e.
        // shared by every AudioPlayer, so a second player (e.g. the waveform editor's)
        // could see another player's pending items, never emit end_of_play() and
        // never play anything. Look at this player's own playlist instead.
        if (!m_playlist.empty()){
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

    double AudioPlayer::audio_duration_sec(QString audio_file)
    {
        double audio_duration_secs = 0;

        if (audio_file.isEmpty()) {
            audio_duration_secs = m_audio_thread->audio_len(QString::fromStdString(m_audio_file.audio_file()));
        } else {
            audio_duration_secs = m_audio_thread->audio_len(audio_file);
        }

        return audio_duration_secs;

    }

    double AudioPlayer::audio_duration_msec(QString audio_file)
    {
        auto dur_sec = audio_duration_sec(audio_file);
        return dur_sec * 1000;
    }

    float AudioPlayer::audio_sample_rate()
    {
        return m_audio_thread->audio_sample_rate(QString::fromStdString(m_audio_file.audio_file()));
    }

    void AudioPlayer::play()
    {
        m_audio_thread->play(QString::fromStdString(m_audio_file.audio_file()));
        m_playback_state = PlaybackState::PlayingState;
        emit sig_start_play();
        emit playback_state_changed(PlaybackState::PlayingState);

    }

    void AudioPlayer::change_position(int new_position)
    {
        m_audio_thread->change_position(new_position);

    }

    void AudioPlayer::play_from_position(int position)
    {
        m_audio_thread->play_from_position(QString::fromStdString(m_audio_file.audio_file()),
                                           position);
        m_playback_state = PlaybackState::PlayingState;
        emit sig_start_play();
        emit playback_state_changed(PlaybackState::PlayingState);

    }

    void AudioPlayer::set_timer_interval(int interval)
    {
        m_audio_thread->set_timer_interval(interval);
    }

    void AudioPlayer::set_source(std::string audio_filepath)
    {
        m_audio_file.set_audio_filename(audio_filepath);
    }

    AudioPlayer::PlaybackState AudioPlayer::playback_state()
    {
        return m_playback_state;
    }

    // -----------------------------------------------------------------------
    // Additions for WaveformWidget
    // -----------------------------------------------------------------------
    void AudioPlayer::play_from_seconds(double seconds)
    {
        m_audio_thread->play_from_seconds(QString::fromStdString(m_audio_file.audio_file()), seconds);
        m_playback_state = PlaybackState::PlayingState;
        emit sig_start_play();
        emit playback_state_changed(PlaybackState::PlayingState);
    }

    bool AudioPlayer::seek_seconds(double seconds)
    {
        return m_audio_thread->seek_seconds(seconds);
    }

    double AudioPlayer::position_seconds()
    {
        return m_audio_thread->position_seconds();
    }

    double AudioPlayer::length_seconds()
    {
        return m_audio_thread->length_seconds();
    }

    void AudioPlayer::set_volume(float volume01)
    {
        m_audio_thread->set_volume(volume01);
    }

    float AudioPlayer::volume() const
    {
        return m_audio_thread->volume();
    }

    void AudioPlayer::cancel_waveform()
    {
        ++m_wave_job;                       // results of the old job are ignored
        if (m_wave_cancel)
            m_wave_cancel->store(true);
        if (m_wave_thread) {                // the decode loop checks the flag every chunk
            m_wave_thread->wait();
        }
        m_wave_cancel.reset();
    }

    void AudioPlayer::load_waveform(const QString& filename, int block_frames)
    {
        cancel_waveform();

        auto cancel = std::make_shared<std::atomic_bool>(false);
        m_wave_cancel = cancel;
        const quint64 job = m_wave_job;

        QThread* thread = QThread::create([this, filename, block_frames, cancel, job]() {
            int last_pct = -1;
            auto progress = [this, job, &last_pct](int pct) {
                if (pct == last_pct)
                    return;
                last_pct = pct;
                QMetaObject::invokeMethod(this, [this, job, pct]() {
                    if (job == m_wave_job)
                        emit waveform_progress(pct);
                }, Qt::QueuedConnection);
            };

            QString error;
            WaveformDataPtr data = AudioThread::decode_waveform(filename, block_frames,
                                                                *cancel, progress, &error);
            if (cancel->load())
                return;                                 // superseded / cancelled

            QMetaObject::invokeMethod(this, [this, job, data, error]() {
                if (job != m_wave_job)
                    return;
                if (data)
                    emit waveform_ready(data);
                else
                    emit waveform_failed(error);
            }, Qt::QueuedConnection);
        });

        connect(thread, &QThread::finished, thread, &QObject::deleteLater);
        m_wave_thread = thread;
        thread->start();
    }

}
