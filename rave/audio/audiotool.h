#ifndef AUDIOTOOL_H
#define AUDIOTOOL_H

#include <chrono>

#include <QObject>
#include <QProcess>

#include "../audio/audiofile.h"

class QProcess;
class QProgressDialog;

namespace AUDIO
{
    class AudioTool : public QObject
    {
        Q_OBJECT
    public:
        QString AUDIO_WAVE_FILE_EXE = "audiowaveform.exe";
        QString FFMPEG_EXE = "ffmpeg.exe";
        QString BATCH_EXEC = "test.bat";
        QString TEMP_BATCH = "mp3_to_ogg.bat";

        AudioTool();
        ~AudioTool();

        bool generate_wave_file(const std::string src_audio_file, const std::string dest_wave_file);
        std::string mp3_to_ogg(AudioFile&);
        std::string make_audio_filename(int);
        int filename_to_id(QString);
        bool copy_ogg_to_audiolib(std::string, std::string);
        bool copy_wave_to_audiolib(std::string, std::string);
        QString format_time(double);
        void probe_file(const QString&);

        QString probe_filename();
        QJsonObject get_probe_data(QString&);

        long long get_tick_count();
        bool audio_exist(const QString);

        QTime msec_to_time(int, int);
        QTime msec_to_time(qint64);

    private slots:
        void wave_gen_finished();
        void mp3_to_ogg_finished(int, QProcess::ExitStatus);
        void mp3_ready();
        void probe_finished(int, QProcess::ExitStatus);

    private:
        QString m_wave_form_exe;
        QProcess* m_wave_gen_proc;
        QProcess* m_mp3_to_ogg_proc;
        QProgressDialog* m_progress_dialog;
        QProcess* m_probe_process;
        QString m_probe_file;
        QString m_probe_filename;
    };


    class ADFRepository
    {
        public:
        ADFRepository();
        bool write(const AudioFile&);
        CueMarker read_markers(const std::string);
        protected:
        void object_to_json(const AudioFile&, QJsonObject&);
        CueMarker json_to_markers(const QJsonObject&);
    };
}

#endif // AUDIOTOOL_H
