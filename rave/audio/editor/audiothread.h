#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QThread>
#include <QDebug>
#include <QTimer>
#include "../../lib/BassLib/Include/bass.h"
#include "../audiofile.h"

void __stdcall syncFunc(HSYNC handle, DWORD channel, DWORD data, void* user);


class AudioThread : public QThread
{
    Q_OBJECT
    public:
        explicit AudioThread(QObject* parent=nullptr);
        bool playing;
        void run();
        float audio_sample_rate(QString);
        float audio_bitrate(QString audio_file);
        std::tuple<float, float> channel_levels();
        std::tuple<float, float> get_channel_levels();

    private:
        unsigned long m_channel;
        QTimer* timer;
    signals:
        void end_of_playback();
        void current_position(double position, double total);
        void current_peak(float fft[1024]);
    public slots:
        void play(QString filepath);
        void play_from_position(QString filename, int position);
        void pause();
        void resume();
        void stop();
        void signal_update();
        void change_position(int position);
        void read_FFT(QString filename);
        float* get_channel_data();

        DWORD audio_len(QString audio);

        void fetch_audio_energy(const std::string&, std::vector<float>&);
        DWORD get_audio_data(HSTREAM&, std::vector<float>&, QVector<float>&);

        AudioInfo get_audio_info(std::string&);

};


#endif // AUDIOTHREAD_H
