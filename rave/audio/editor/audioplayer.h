#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include "../../audio/audiofile.h"

class AudioThread;

namespace AUDIO
{

    class AudioPlayer : public QObject
    {
        Q_OBJECT
    public:
        enum class OutputChannel {ChannelA=1, ChannelB=2};

        AudioPlayer();
        AudioPlayer(AudioFile& audio_file);
//        AudioPlayer(std::vector<QString> play_list);
        ~AudioPlayer();

        void play_audio(QString, QString);
        void stop_play();

        void update_output_channel(QString, QString);
        OutputChannel str_to_channel(QString);
    signals:
        void play_next();
        void end_of_play();
    private slots:
        void end_of_playback();
    private:
        std::unique_ptr<AudioThread> m_audio_thread;
        AudioFile m_audio_file;
        std::map<OutputChannel, QString> m_output_channel;
        static OutputChannel current_output_channel;

    };
}

#endif // AUDIOPLAYER_H
