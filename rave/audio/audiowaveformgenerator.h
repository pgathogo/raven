#pragma once

#include <memory>
#include <QObject>
#include <QProcess>

#include "../framework/Logger.h"

class QProgressDialog;

namespace AUDIO{

    class AudioWaveFormGenerator : public QObject
    {
    public:
        QString AUDIO_WAVE_FILE_EXE = "audiowaveform.exe";
        QString FFMPEG_EXE = "ffmpeg.exe";
        QString BATCH_EXEC = "test.bat";
        QString TEMP_BATCH = "mp3_to_ogg.bat";

        AudioWaveFormGenerator(const QString);
        ~AudioWaveFormGenerator();
        bool generate();
        QString audio_filename();
        void set_audio_filename(QString);
        QString wave_filename();
        void set_wave_filename(QString);
        void remove_wave_file();

        void log_info(const QString msg) { Logger::info("AudioWaveFromGenerator", msg);}
        void log_error(const QString msg) { Logger::error("AudioWaveFromGenerator", msg);}

    private slots:
        void wave_generation_finished(int, QProcess::ExitStatus);
        void generator_error(QProcess::ProcessError);

    private:
        QString m_audio_filename;
        QString m_wave_filename;

        QProgressDialog* m_progress_dialog;
        QProcess* m_generator_process;
    };

}
