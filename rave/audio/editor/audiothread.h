#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QMetaType>
#include <QVector>
#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include "../../lib/BassLib/Include/bass.h"

// bass.h pulls in <windows.h>, whose min/max macros break std::min / std::max / std::clamp
// ("error C2589: '(': illegal token on right side of '::'").
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include "../audiofile.h"

void __stdcall syncFunc(HSYNC handle, DWORD channel, DWORD data, void* user);

// ---------------------------------------------------------------------------
// Waveform summary produced by AudioThread::decode_waveform().
// One WavePeak per `blockFrames` audio frames and per channel (0 = left,
// 1 = right; mono files are duplicated into both, files with more than two
// channels use the first two).
// ---------------------------------------------------------------------------
struct WavePeak
{
    float min = 0.0f;
    float max = 0.0f;
    float rms = 0.0f;
};

struct WaveformData
{
    int sampleRate = 0;
    int channels = 0;               // channel count of the source file
    int blockFrames = 64;           // frames summarised per WavePeak
    qint64 totalFrames = 0;
    std::vector<WavePeak> peaks[2];

    double duration() const { return sampleRate > 0 ? double(totalFrames) / sampleRate : 0.0; }
};

using WaveformDataPtr = std::shared_ptr<WaveformData>;
Q_DECLARE_METATYPE(WaveformDataPtr)


class AudioThread : public QThread
{
    Q_OBJECT
    public:

        explicit AudioThread(QObject* parent=nullptr);
        ~AudioThread() override;

        bool playing = false;
        void run();
        float audio_sample_rate(QString);
        float audio_bitrate(QString audio_file);
        std::tuple<float, float> channel_levels();
        std::tuple<float, float> get_channel_levels();

        // ---- additions used by WaveformWidget -----------------------------
        /// Decodes the whole file (any format BASS can read) and builds the
        /// min/max/RMS summary. Thread-safe and static, so it can run on any
        /// worker thread. `progress` receives 0..100, `cancel` aborts the job
        /// (a null pointer is returned). On failure `error` is filled.
        static WaveformDataPtr decode_waveform(const QString& filename,
                                               int block_frames,
                                               const std::atomic_bool& cancel,
                                               const std::function<void(int)>& progress = {},
                                               QString* error = nullptr);

        /// Starts playback at `seconds` (sample accurate for most formats).
        void play_from_seconds(QString filename, double seconds);
        /// Moves the play position of the current channel. False if nothing is loaded.
        bool seek_seconds(double seconds);
        /// Live play position / length of the current channel in seconds.
        double position_seconds() const;
        double length_seconds() const;
        bool is_active() const;                 // playing or paused
        /// Linear volume 0..1; also applied to channels created later.
        void set_volume(float volume);
        float volume() const { return m_volume; }

    private:
        friend void __stdcall syncFunc(HSYNC, DWORD, DWORD, void*);

        bool start_channel(const QString& filename, bool prescan = false);
        void free_channel();

        unsigned long m_channel = 0;
        bool m_is_music = false;                // m_channel came from BASS_MusicLoad
        QTimer* timer = nullptr;
        std::atomic<bool> m_end_of_music{true}; // set from BASS' sync thread
        float m_volume = 1.0f;
    signals:
        void end_of_playback();
        void current_position(double position, double total);
        void current_peak(float fft[1024]);
        void timer_tick();
    public slots:
        void play(QString filepath);
        void play_from_position(QString filename, int position);
        void pause();
        void resume();
        void fade_audio();
        void stop();
        void signal_update();
        void change_position(int position);
        void read_FFT(QString filename);
        float* get_channel_data();

        DWORD audio_len(QString audio);

        void fetch_audio_energy(const std::string&, std::vector<float>&);
        DWORD get_audio_data(HSTREAM&, std::vector<float>&, QVector<float>&);

        AudioInfo get_audio_info(std::string&);

        void set_timer_interval(int);


};


#endif // AUDIOTHREAD_H
