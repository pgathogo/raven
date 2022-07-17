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
        AudioPlayer(AudioFile& audio_file);
        AudioPlayer(std::vector<QString> play_list);
        ~AudioPlayer();

        void play_audio();
        void stop_play();

        void add_to_playlist(QString);
    signals:
        void play_next();
        void end_of_play();
    private slots:
        void end_of_playback();
    private:
        std::unique_ptr<AudioThread> m_audio_thread;
        AudioFile m_audio_file;
        std::vector<QString> m_play_list;
        static int play_count;

    };
}

#endif // AUDIOPLAYER_H
