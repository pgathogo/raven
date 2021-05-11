#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include "../../audio/audiofile.h"

class AudioThread;

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    AudioPlayer(AudioFile& audio_file);
    ~AudioPlayer();

    void play_audio();
    void stop_play();
private:
    AudioThread* m_audio_thread;
    AudioFile& m_audio_file;

};

#endif // AUDIOPLAYER_H
