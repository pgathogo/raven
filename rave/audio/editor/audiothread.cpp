#include <math.h>
#include "audiothread.h"

bool endOfMusic;

void __stdcall syncFunc(HSYNC handle, DWORD channel, DWORD data, void* user)
{
    BASS_ChannelRemoveSync(channel, handle);
    BASS_ChannelStop(channel);
    qDebug() << "End of playback!";
    endOfMusic = true;
}

AudioThread::AudioThread(QObject* parent):
    QThread(parent)
{
    if (!BASS_Init(-1, 44100, 0, nullptr, nullptr))
        qDebug() << "Cannot initialize device";
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(signal_update()));
    endOfMusic = true;
}

void AudioThread::play(QString filename)
{
    BASS_ChannelStop(m_channel);

    if (!(m_channel = BASS_StreamCreateFile(false, filename.toLatin1(), 0, 0,
        BASS_SAMPLE_LOOP)) &&
        !(m_channel = BASS_MusicLoad(false, filename.toLatin1(), 0, 0, BASS_MUSIC_RAMP
        | BASS_SAMPLE_LOOP, 1)))
            qDebug() << "Can't play file- ERROR Code: " << BASS_ErrorGetCode();
    else{
        endOfMusic = false;
        BASS_ChannelPlay(m_channel, true);
        timer->start(100);
        BASS_ChannelSetSync(m_channel, BASS_SYNC_END, 0, &syncFunc, nullptr);
        playing = true;
    }

}

void AudioThread::play_from_position(QString filename, int position)
{
    BASS_ChannelStop(m_channel);
    if (!(m_channel = BASS_StreamCreateFile(false, filename.toLatin1(), 0, 0,
        BASS_SAMPLE_LOOP)) &&
        !(m_channel = BASS_MusicLoad(false, filename.toLatin1(), 0, 0, BASS_MUSIC_RAMP
        | BASS_SAMPLE_LOOP, 1)))
            qDebug() << "Can't play file";
    else{
        endOfMusic = false;
        change_position(position);
        BASS_ChannelPlay(m_channel, false);
        timer->start(100);
        BASS_ChannelSetSync(m_channel, BASS_SYNC_END, 0, &syncFunc, nullptr);
        playing = true;
    }

}

void AudioThread::read_FFT(QString filename)
{
    BASS_ChannelStop(m_channel);

    BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 0); // no audio output, therefore no update period is needed
    BASS_Init(0, 44100, 0, 0, NULL);

    DWORD chan = BASS_StreamCreateFile(false, filename.toLatin1(), 0L, 0L, BASS_STREAM_DECODE);

    QWORD pos = BASS_ChannelGetLength(chan, BASS_POS_BYTE);

    DWORD p = (DWORD) BASS_ChannelBytes2Seconds(chan, pos); // length of file in seconds

    float time = 0;

    double fps = 60; // how many samples should be taken per second
    int BANDS = 120; // how many "bins" to divide the spectrum in (also the number of columns on each row of output)

    while(BASS_ChannelIsActive(chan)){
        long byte_pos = BASS_ChannelSeconds2Bytes(chan, time);
        float fft[16384];
        int b0 = 0;
        BASS_ChannelGetData(chan, fft, BASS_DATA_FFT32768);

        for(int i=0; i < BANDS; ++i){
            float peak = 0;
            int b1 = pow(1, i * 10.0 / (BANDS - 1)); // determine the size of bin

            if (b1 > 16383)
                b1 = 16383;

            if ( b1 <= b0)
                b1 = b0 + 1;

            // loop over all bins
            for (; b0<b1; ++b0) {
                if (peak < fft[1+b0])
                    peak = fft[1 + b0];
            }

        }
        qDebug() << "<--- end of row --->";
        pos = BASS_ChannelGetPosition(chan, BASS_POS_BYTE);
        p = (DWORD) BASS_ChannelBytes2Seconds(chan, pos);
        //qDebug() << "TIME: " << p/60 << p % 60;
        time += 1 / fps;
    }

    qDebug() << "Done";
    BASS_Free();

}

DWORD AudioThread::audio_len(QString audio)
{
    DWORD chan = BASS_StreamCreateFile(false, audio.toLatin1(), 0L, 0L, BASS_STREAM_DECODE);
    QWORD len = BASS_ChannelGetLength(chan, BASS_POS_BYTE);
    return (DWORD) BASS_ChannelBytes2Seconds(chan, len); // length of file in seconds
}

float AudioThread::audio_sample_rate(QString audio_file)
{
    float sample_rate;
    DWORD chan = BASS_StreamCreateFile(false, audio_file.toLatin1(), 0L, 0L, BASS_STREAM_DECODE);
    if (BASS_ChannelGetAttribute(chan, BASS_ATTRIB_FREQ, &sample_rate))
        return sample_rate;
    else
        return 0.0;
}

float AudioThread::audio_bitrate(QString audio_file)
{
    float bit_rate;
    DWORD chan = BASS_StreamCreateFile(false, audio_file.toLatin1(), 0L, 0L, BASS_STREAM_DECODE);
    if (BASS_ChannelGetAttribute(chan, BASS_ATTRIB_BITRATE, &bit_rate))
        return bit_rate;
    else
        return 0.0;
}

void AudioThread::pause()
{
    BASS_ChannelPause(m_channel);
    timer->stop();
    playing = false;
}

void AudioThread::resume()
{
    if (!BASS_ChannelPlay(m_channel, true))
        qDebug() << "Error resuming";
    else{
        timer->start(98);
        playing = true;
    }
}

void AudioThread::stop()
{
    BASS_ChannelStop(m_channel);
    timer->stop();
    playing = false;
}

void AudioThread::signal_update()
{
    float fft[1024];

    if (endOfMusic == false){
        emit current_position(BASS_ChannelBytes2Seconds(m_channel, BASS_ChannelGetPosition(m_channel,
            BASS_POS_BYTE)),
                        BASS_ChannelBytes2Seconds(m_channel, BASS_ChannelGetLength(m_channel,
            BASS_POS_BYTE)));

        BASS_ChannelGetData(m_channel, fft, BASS_DATA_FFT2048);
        emit current_peak(fft);
    }else{
        playing = false;
        emit end_of_playback();
    }
}


void AudioThread::change_position(int position)
{
    //BASS_ChannelSetPosition(m_channel, BASS_ChannelSeconds2Bytes(m_channel, position), BASS_POS_BYTE);
    unsigned long bpp = BASS_ChannelGetLength(m_channel, BASS_POS_BYTE)/800;
    BASS_ChannelSetPosition(m_channel, position*bpp, BASS_POS_BYTE);
}

void AudioThread::run()
{
    while(1);
}

