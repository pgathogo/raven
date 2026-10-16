#include <algorithm>
#include <cmath>
#include <math.h>
#include "audiothread.h"

namespace {

// BASS wants UTF-16 on Windows (BASS_UNICODE) and UTF-8 everywhere else.
// (The old code used toLatin1(), which breaks for any non-Latin-1 file name.)
HSTREAM open_stream(const QString& path, DWORD flags)
{
#ifdef _WIN32
    return BASS_StreamCreateFile(FALSE, reinterpret_cast<const void*>(path.utf16()), 0, 0,
                                 flags | BASS_UNICODE);
#else
    const QByteArray p = path.toUtf8();
    return BASS_StreamCreateFile(FALSE, p.constData(), 0, 0, flags);
#endif
}

HMUSIC open_music(const QString& path, DWORD flags, DWORD freq)
{
#ifdef _WIN32
    return BASS_MusicLoad(FALSE, reinterpret_cast<const void*>(path.utf16()), 0, 0,
                          flags | BASS_UNICODE, freq);
#else
    const QByteArray p = path.toUtf8();
    return BASS_MusicLoad(FALSE, p.constData(), 0, 0, flags, freq);
#endif
}

// Frees a decoding channel on scope exit.
struct DecodeChannel
{
    DWORD handle = 0;
    bool is_music = false;
    ~DecodeChannel()
    {
        if (!handle) return;
        if (is_music) BASS_MusicFree(handle); else BASS_StreamFree(handle);
    }
};

} // namespace

// Called by BASS (from its own thread) when a channel reaches the end.
// `user` is the AudioThread that owns the channel, so several AudioThread /
// AudioPlayer instances (e.g. the main player and the waveform editor) no
// longer share one global "end of music" flag.
void __stdcall syncFunc(HSYNC handle, DWORD channel, DWORD data, void* user)
{
    Q_UNUSED(data);
    BASS_ChannelRemoveSync(channel, handle);
    BASS_ChannelStop(channel);
    qDebug() << "End of playback!";
    if (user)
        static_cast<AudioThread*>(user)->m_end_of_music = true;
}

AudioThread::AudioThread(QObject* parent):
    QThread(parent)
{
    // Another AudioThread may already have initialised the device - that is fine.
    if (!BASS_Init(-1, 44100, 0, nullptr, nullptr) && BASS_ErrorGetCode() != BASS_ERROR_ALREADY)
        qDebug() << "Cannot initialize device";

    qRegisterMetaType<WaveformDataPtr>();

    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, this, &AudioThread::signal_update);
    m_end_of_music = true;
}

AudioThread::~AudioThread()
{
    if (timer)
        timer->stop();
    free_channel();
}

// ---------------------------------------------------------------------------
// channel management
// ---------------------------------------------------------------------------
void AudioThread::free_channel()
{
    if (!m_channel)
        return;

    BASS_ChannelStop(m_channel);
    if (m_is_music)
        BASS_MusicFree(m_channel);
    else
        BASS_StreamFree(m_channel);

    m_channel = 0;
    m_is_music = false;
    m_end_of_music = true;
}

bool AudioThread::start_channel(const QString& filename, bool prescan)
{
    free_channel();

    DWORD stream_flags = BASS_SAMPLE_LOOP;
    if (prescan)
        stream_flags |= BASS_STREAM_PRESCAN;    // pin-point seeking for VBR MP3 etc.

    DWORD ch = open_stream(filename, stream_flags);
    bool is_music = false;
    if (!ch) {
        ch = open_music(filename, BASS_MUSIC_RAMP | BASS_SAMPLE_LOOP, 1);
        is_music = (ch != 0);
    }
    if (!ch) {
        qDebug() << "Can't play file- ERROR Code: " << BASS_ErrorGetCode();
        return false;
    }

    m_channel = ch;
    m_is_music = is_music;
    BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL, m_volume);
    BASS_ChannelSetSync(m_channel, BASS_SYNC_END, 0, &syncFunc, this);
    m_end_of_music = false;
    return true;
}

// ---------------------------------------------------------------------------
// info helpers
// ---------------------------------------------------------------------------
AudioInfo AudioThread::get_audio_info(std::string& audio_file)
{
    AudioInfo audio_info;

    HSTREAM stream_handle = open_stream(QString::fromStdString(audio_file), BASS_STREAM_DECODE);
    if (!stream_handle) {
        // TODO: Inform user that reading the audio file was nto successufull
        return audio_info;
    }

    BASS_CHANNELINFO info;
    BASS_ChannelGetInfo(stream_handle, &info);
    audio_info.sample_rate = info.freq;
    audio_info.num_channels = info.chans;

    audio_info.length_bytes = BASS_ChannelGetLength(stream_handle, BASS_POS_BYTE);
    audio_info.num_samples = BASS_ChannelBytes2Seconds(stream_handle, audio_info.length_bytes) * audio_info.sample_rate * audio_info.num_channels;

    BASS_StreamFree(stream_handle);
    return audio_info;
}

DWORD AudioThread::audio_len(QString audio)
{
    DWORD chan = open_stream(audio, BASS_STREAM_DECODE);
    if (!chan)
        return 0;
    QWORD len = BASS_ChannelGetLength(chan, BASS_POS_BYTE);
    DWORD secs = (DWORD) BASS_ChannelBytes2Seconds(chan, len); // length of file in seconds
    BASS_StreamFree(chan);
    return secs;
}

float AudioThread::audio_sample_rate(QString audio_file)
{
    float sample_rate = 0.0f;
    DWORD chan = open_stream(audio_file, BASS_STREAM_DECODE);
    if (!chan)
        return 0.0;
    const bool ok = BASS_ChannelGetAttribute(chan, BASS_ATTRIB_FREQ, &sample_rate);
    BASS_StreamFree(chan);
    return ok ? sample_rate : 0.0f;
}

float AudioThread::audio_bitrate(QString audio_file)
{
    float bit_rate = 0.0f;
    DWORD chan = open_stream(audio_file, BASS_STREAM_DECODE);
    if (!chan)
        return 0.0;
    const bool ok = BASS_ChannelGetAttribute(chan, BASS_ATTRIB_BITRATE, &bit_rate);
    BASS_StreamFree(chan);
    return ok ? bit_rate : 0.0f;
}

// ---------------------------------------------------------------------------
// playback
// ---------------------------------------------------------------------------
void AudioThread::play(QString filename)
{
    if (!start_channel(filename))
        return;

    BASS_ChannelPlay(m_channel, true);
    timer->start();                 // keeps whatever interval set_timer_interval() chose
    playing = true;
}

void AudioThread::play_from_position(QString filename, int position)
{
    if (!start_channel(filename))
        return;

    change_position(position);
    BASS_ChannelPlay(m_channel, false);
    timer->start();
    playing = true;
}

void AudioThread::play_from_seconds(QString filename, double seconds)
{
    if (!start_channel(filename, /*prescan=*/true))
        return;

    if (seconds > 0.0)
        seek_seconds(seconds);
    BASS_ChannelPlay(m_channel, false);
    timer->start();
    playing = true;
}

bool AudioThread::seek_seconds(double seconds)
{
    if (!m_channel)
        return false;

    seconds = std::max(0.0, seconds);
    const double len = length_seconds();
    if (len > 0.0)
        seconds = std::min(seconds, std::max(0.0, len - 0.001));

    const QWORD bytes = BASS_ChannelSeconds2Bytes(m_channel, seconds);
    return BASS_ChannelSetPosition(m_channel, bytes, BASS_POS_BYTE);
}

double AudioThread::position_seconds() const
{
    if (!m_channel)
        return 0.0;
    const QWORD pos = BASS_ChannelGetPosition(m_channel, BASS_POS_BYTE);
    if (pos == (QWORD)-1)
        return 0.0;
    return BASS_ChannelBytes2Seconds(m_channel, pos);
}

double AudioThread::length_seconds() const
{
    if (!m_channel)
        return 0.0;
    const QWORD len = BASS_ChannelGetLength(m_channel, BASS_POS_BYTE);
    if (len == (QWORD)-1)
        return 0.0;
    return BASS_ChannelBytes2Seconds(m_channel, len);
}

bool AudioThread::is_active() const
{
    if (!m_channel)
        return false;
    const DWORD a = BASS_ChannelIsActive(m_channel);
    return a == BASS_ACTIVE_PLAYING || a == BASS_ACTIVE_PAUSED || a == BASS_ACTIVE_STALLED;
}

void AudioThread::set_volume(float volume)
{
    m_volume = std::clamp(volume, 0.0f, 1.0f);
    if (m_channel)
        BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL, m_volume);
}

void AudioThread::pause()
{
    BASS_ChannelPause(m_channel);
    timer->stop();
    playing = false;
}

void AudioThread::resume()
{
    if (!BASS_ChannelPlay(m_channel, false))
        qDebug() << "Error resuming";
    else{
        timer->start();
        playing = true;
    }
}

void AudioThread::fade_audio()
{
    BASS_ChannelSlideAttribute(m_channel, BASS_ATTRIB_VOL, 0.0f, 5000);
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

    if (m_end_of_music == false)
    {

        emit current_position(BASS_ChannelBytes2Seconds(m_channel, BASS_ChannelGetPosition(m_channel,
            BASS_POS_BYTE)),
                        BASS_ChannelBytes2Seconds(m_channel, BASS_ChannelGetLength(m_channel,
            BASS_POS_BYTE)));

        BASS_ChannelGetData(m_channel, fft, BASS_DATA_FFT2048);

        emit current_peak(fft);
        emit timer_tick();
    }else{
        playing = false;
        emit end_of_playback();
    }
}

float* AudioThread::get_channel_data()
{
    float* fft = new float[2048];

    if (m_end_of_music == false)
    {

        emit current_position(BASS_ChannelBytes2Seconds(m_channel, BASS_ChannelGetPosition(m_channel,
            BASS_POS_BYTE)),
                        BASS_ChannelBytes2Seconds(m_channel, BASS_ChannelGetLength(m_channel,
            BASS_POS_BYTE)));

        BASS_ChannelGetData(m_channel, fft, BASS_DATA_FFT2048);

    }

    return fft;

}

DWORD AudioThread::get_audio_data(HSTREAM& stream_handle,
                                        std::vector<float>& buffer, QVector<float>& energy_data)
{
    Q_UNUSED(energy_data);

    int buffer_size = 1024;

    DWORD byte_read = 0;

    if (m_end_of_music)
        return byte_read;

    byte_read = BASS_ChannelGetData(stream_handle, buffer.data(), buffer_size * sizeof(float) );

    return byte_read;

}

std::tuple<float, float> AudioThread::channel_levels()
{
    auto levels = std::make_tuple<float, float>(0.0, 0.0);

    if (playing)
    {
        BASS_CHANNELINFO info;

        BASS_ChannelGetInfo(m_channel, &info);

        float ch_levels[2];
        BASS_ChannelGetLevelEx(m_channel, ch_levels, 0.02, BASS_LEVEL_STEREO);

        std::get<0>(levels) =  ch_levels[0];
        std::get<1>(levels) =  ch_levels[1];
    }

    return levels;

}

std::tuple<float, float> AudioThread::get_channel_levels()
{
    auto levels = std::make_tuple<float, float>(0.0, 0.0);

    if (playing)
    {
        // One call only: every BASS_ChannelGetLevel() call consumes 20 ms of the
        // measurement window, so calling it twice made the right channel lag.
        const DWORD level = BASS_ChannelGetLevel(m_channel);
        std::get<0>(levels) = float(level & 0xFFFF);
        std::get<1>(levels) = float(level >> 16);
    }

    return levels;

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

void AudioThread::set_timer_interval(int interval)
{
    timer->setInterval(interval);

}

// ---------------------------------------------------------------------------
// Offline analysis
// ---------------------------------------------------------------------------

// NOTE: the original read_FFT() called BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 0),
// BASS_Init(0, ...) and BASS_Free(). Those are *global* calls that would kill all
// playback in the application (including the waveform editor), so they were removed.
void AudioThread::read_FFT(QString filename)
{
    BASS_ChannelStop(m_channel);

    DWORD chan = open_stream(filename, BASS_STREAM_DECODE);
    if (!chan)
        return;

    QWORD pos = BASS_ChannelGetLength(chan, BASS_POS_BYTE);

    DWORD p = (DWORD) BASS_ChannelBytes2Seconds(chan, pos); // length of file in seconds

    float time = 0;

    double fps = 60; // how many samples should be taken per second
    int BANDS = 120; // how many "bins" to divide the spectrum in (also the number of columns on each row of output)

    while(BASS_ChannelIsActive(chan)){
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
        time += 1 / fps;
    }

    Q_UNUSED(p);
    qDebug() << "Done";
    BASS_StreamFree(chan);
}

// Per-block RMS of the (interleaved) sample data. The old version read 32-bit
// floats into a vector<double> from a stream that was not even float, so the
// values were garbage.
void AudioThread::fetch_audio_energy(const std::string& audio_file, std::vector<float>& energy_data)
{
    HSTREAM stream_handle = open_stream(QString::fromStdString(audio_file),
                                        BASS_STREAM_DECODE | BASS_SAMPLE_FLOAT);
    if (!stream_handle) {
        // TODO: Inform the user that reading the audio file was not successfull
        return;
    }

    constexpr int kSamplesPerBlock = 1024;
    std::vector<float> buffer(kSamplesPerBlock);

    for (;;) {
        const DWORD bytes_read = BASS_ChannelGetData(stream_handle, buffer.data(),
                                                     kSamplesPerBlock * sizeof(float) | BASS_DATA_FLOAT);
        if (bytes_read == (DWORD)-1 || bytes_read == 0)
            break;

        const size_t n = bytes_read / sizeof(float);
        double energy = 0.0;
        for (size_t i = 0; i < n; ++i) {
            const double b = buffer[i];
            if (std::isfinite(b))
                energy += b * b;
        }
        energy_data.push_back(float(std::sqrt(energy / double(n))));
    }

    BASS_StreamFree(stream_handle);
}

WaveformDataPtr AudioThread::decode_waveform(const QString& filename,
                                             int block_frames,
                                             const std::atomic_bool& cancel,
                                             const std::function<void(int)>& progress,
                                             QString* error)
{
    auto fail = [error](const QString& msg) {
        if (error) *error = msg;
        return WaveformDataPtr();
    };

    block_frames = std::max(1, block_frames);

    DecodeChannel chan;
    chan.handle = open_stream(filename, BASS_STREAM_DECODE | BASS_SAMPLE_FLOAT);
    if (!chan.handle && BASS_ErrorGetCode() == BASS_ERROR_INIT) {
        // This (worker) thread has no device yet: use the "no sound" device.
        BASS_Init(0, 44100, 0, nullptr, nullptr);
        chan.handle = open_stream(filename, BASS_STREAM_DECODE | BASS_SAMPLE_FLOAT);
    }
    if (!chan.handle) {
        // Tracker modules (MOD/XM/IT/S3M...) are not streams.
        chan.handle = open_music(filename, BASS_MUSIC_DECODE | BASS_MUSIC_PRESCAN | BASS_MUSIC_FLOAT, 1);
        chan.is_music = (chan.handle != 0);
    }
    if (!chan.handle)
        return fail(tr("BASS cannot open '%1' (error %2).").arg(filename).arg(BASS_ErrorGetCode()));

    BASS_CHANNELINFO info;
    if (!BASS_ChannelGetInfo(chan.handle, &info) || info.freq == 0 || info.chans == 0)
        return fail(tr("Cannot read the format of '%1'.").arg(filename));

    const int ch = int(info.chans);
    const qint64 bytesPerFrame = qint64(ch) * qint64(sizeof(float));

    auto data = std::make_shared<WaveformData>();
    data->sampleRate = int(info.freq);
    data->channels = ch;

    // Expected length (for progress + reserve). May be unknown for odd formats.
    const QWORD lenBytes = BASS_ChannelGetLength(chan.handle, BASS_POS_BYTE);
    const qint64 estFrames = (lenBytes == (QWORD)-1) ? 0 : qint64(lenBytes / QWORD(bytesPerFrame));

    // Very long files: coarsen the blocks so the summary stays a sane size.
    constexpr qint64 kMaxBlocks = 4000000;
    while (estFrames / block_frames > kMaxBlocks)
        block_frames *= 2;
    data->blockFrames = block_frames;

    if (estFrames > 0) {
        const size_t blocks = size_t(estFrames / block_frames) + 1;
        data->peaks[0].reserve(blocks);
        data->peaks[1].reserve(blocks);
    }

    struct Acc { float mn = 0, mx = 0; double sumSq = 0; int n = 0; };
    Acc acc[2];

    auto flush = [&](int c) {
        Acc& a = acc[c];
        if (a.n == 0) return;
        data->peaks[c].push_back({a.mn, a.mx, float(std::sqrt(a.sumSq / a.n))});
        a = Acc{};
    };
    auto add = [&](int c, float v) {
        Acc& a = acc[c];
        if (a.n == 0) {
            a.mn = a.mx = v;
        } else {
            a.mn = std::min(a.mn, v);
            a.mx = std::max(a.mx, v);
        }
        a.sumSq += double(v) * v;
        if (++a.n == block_frames)
            flush(c);
    };

    constexpr int kChunkFrames = 4096;
    std::vector<float> buf(size_t(kChunkFrames) * size_t(ch));
    int lastPct = -1;

    while (!cancel.load()) {
        const DWORD got = BASS_ChannelGetData(chan.handle, buf.data(),
                                              DWORD(buf.size() * sizeof(float)) | BASS_DATA_FLOAT);
        if (got == (DWORD)-1 || got == 0)
            break;                                  // end of stream (or error)

        const int frames = int(got / DWORD(bytesPerFrame));
        for (int i = 0; i < frames; ++i) {
            float l = buf[size_t(i) * ch];
            float r = ch > 1 ? buf[size_t(i) * ch + 1] : l;
            if (!std::isfinite(l)) l = 0.0f;
            if (!std::isfinite(r)) r = 0.0f;
            add(0, l);
            add(1, r);
        }
        data->totalFrames += frames;

        if (progress && estFrames > 0) {
            const int pct = int(std::min<qint64>(99, 100 * data->totalFrames / estFrames));
            if (pct != lastPct) {
                lastPct = pct;
                progress(pct);
            }
        }
    }

    if (cancel.load()) {
        if (error) error->clear();
        return {};
    }

    flush(0);
    flush(1);

    if (data->totalFrames <= 0)
        return fail(tr("No audio data could be decoded."));

    return data;
}
