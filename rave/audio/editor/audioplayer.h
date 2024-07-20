#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include "../../audio/audiofile.h"

class AudioThread;


namespace AUDIO
{
    struct PlayList{
        QString output_channel{""};
        QString audio_filename{""};
    };

    class AudioPlayer : public QObject
    {
        Q_OBJECT
    public:

        enum class OutputChannel {ChannelA=1, ChannelB=2};

        AudioPlayer();
        explicit AudioPlayer(AudioFile& audio_file);
//        AudioPlayer(std::vector<QString> play_list);
        ~AudioPlayer();

        void play_audio();
        void play_audio(QString);
        void stop_play();
        float* audio_current_peak();

        void update_output_channel(QString, QString);
        OutputChannel str_to_channel(QString);

        void append_playlist(QString, QString);
        std::tuple<float, float> audio_levels();

        void set_audio_file(const AudioFile);
    signals:
        void play_next();
        void end_of_play();
        void audio_played(QString);
        void current_peak(float fft[1024]);
    private slots:
        void end_of_playback();
    private:
        std::unique_ptr<AudioThread> m_audio_thread;
        AudioFile m_audio_file;
        std::map<OutputChannel, QString> m_output_channel;
        std::vector<PlayList> m_playlist;
        static OutputChannel current_output_channel;
        static int play_list_index;
        QString m_played_audio;
    };
}

#endif // AUDIOPLAYER_H
