#pragma once

#include <memory>

#include <QObject>
#include <QString>

#include "audioconverter.h"

class QProcess;
class QProgressDialog;

namespace AUDIO{
  class Audio;
}

namespace AUDIO
{
    class Mp3ToOggConverter : public AUDIO::AudioConverter
    {
    public:
        QString FFMPEG_EXE = "ffmpeg.exe";
        QString BATCH_EXEC = "test.bat";
        QString TEMP_BATCH = "mp3_to_ogg.bat";

        //Mp3ToOggConverter(const QString mp3_file);
        Mp3ToOggConverter(const QString, std::shared_ptr<QProcess> process_runner=nullptr);
        ~Mp3ToOggConverter() override;

        bool convert() override;
        QString mp3_filename();
        void set_mp3_filename(QString);

    private slots:
        void conversion_finished(int, QProcess::ExitStatus);
        void conversion_error(QProcess::ProcessError);

    private:
        QString m_mp3_filename;
        QString m_ogg_filename;
        QProcess* m_converter_process;
        QProgressDialog* m_progress_dialog;
    };

}
