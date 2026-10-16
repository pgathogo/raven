#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QPointer>
#include <atomic>
#include <map>
#include <memory>
#include <vector>
#include "../../audio/audiofile.h"

#include "audiothread.h"

//class AudioThread;

namespace AUDIO
{
    struct PlayList{
        int grid_index=-1;
        int schedule_id=-1;
        QString output_channel{""};
        QString audio_filename{""};
    };

    class AudioPlayer : public QObject
    {
        Q_OBJECT
    public:

        enum class OutputChannel {ChannelA=1, ChannelB=2};

        enum class PlaybackState
        {
            StoppedState,
            PlayingState,
            PausedState
        };

        AudioPlayer();
        explicit AudioPlayer(AudioFile& audio_file);
//        AudioPlayer(std::vector<QString> play_list);
        ~AudioPlayer();

        // TODO:: Harmonize the "play" functions!!!!!
        void play_audio();
        void play_audio(QString);
        void play();
        void fade_audio();
        void pause_audio();
        void resume_audio();

        void stop_play();
        float* audio_current_peak();

        void update_output_channel(QString, QString);
        OutputChannel str_to_channel(QString);

        void append_playlist(int, int, QString, QString);
        std::tuple<float, float> audio_levels();

        void set_audio_file(const AudioFile);
        double audio_duration_sec(QString audio_file="");
        double audio_duration_msec(QString);

        AudioFile audio_file();
        unsigned int audio_length();
        float audio_bitrate();
        float audio_sample_rate();
        void change_position(int);
        void play_from_position(int);
        AudioInfo get_audio_info(std::string&);
        void fetch_audio_energy(const std::string&, std::vector<float>&);

        void set_timer_interval(int);
        double current_position();
        void set_source(std::string);
        AudioPlayer::PlaybackState playback_state();

        // ---- additions used by WaveformWidget ------------------------------
        /// Starts playing the current source at `seconds`.
        void play_from_seconds(double seconds);
        /// Moves the play position (playing or paused). False if nothing is loaded.
        bool seek_seconds(double seconds);
        /// Live play position of the current channel in seconds (not tied to the timer tick).
        double position_seconds();
        double length_seconds();
        void set_volume(float volume01);
        float volume() const;

        /// Decodes `filename` (any format BASS supports) on a worker thread and
        /// emits waveform_progress() / waveform_ready() / waveform_failed().
        /// A new request cancels the previous one.
        void load_waveform(const QString& filename, int block_frames = 64);
        void cancel_waveform();
    signals:
        void play_next();
        void end_of_play();
        void audio_played(QString);
        void current_peak(float fft[1024]);
        void audio_current_position(double, double);

        void sig_start_play();
        void sig_stop_play();
        void sig_play_item_index(int, int, int);

        void player_timer_tick();
        void playback_state_changed(PlaybackState);

        void waveform_progress(int percent);
        void waveform_ready(WaveformDataPtr data);
        void waveform_failed(QString message);

    private slots:
        void end_of_playback();
        void audio_position(double, double);

    private:
        void init_connections();
        void set_state(PlaybackState state);

        std::unique_ptr<AudioThread> m_audio_thread;
        AudioFile m_audio_file;
        std::map<OutputChannel, QString> m_output_channel;
        std::vector<PlayList> m_playlist;
        static OutputChannel current_output_channel;
        static int play_list_index;
        static int play_counter;
        QString m_played_audio;
        double m_current_position = 0.0;
        PlaybackState m_playback_state = PlaybackState::StoppedState;

        // background waveform decoding
        QPointer<QThread> m_wave_thread;
        std::shared_ptr<std::atomic_bool> m_wave_cancel;
        quint64 m_wave_job = 0;
    };
}

#endif // AUDIOPLAYER_H
