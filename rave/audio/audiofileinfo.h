#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QProcess>

#include "../framework/Logger.h"

namespace AUDIO{

    class AudioFileInfo : public QObject
    {
    public:
        const QString PROBE_TOOL = "ffprobe.exe";

        AudioFileInfo(const QString& audio_file);
        ~AudioFileInfo();
        void probe_file_info();
        QString file_format();
        void set_audio_file(const QString& audio_file);
        void set_data(QString);

        void log_info(const QString msg) { Logger::info("AudioFileInfo", msg); }
        void log_error(const QString msg) { Logger::error("AudioFileInfo", msg); }

    private slots:
        void file_probe_finished(int, QProcess::ExitStatus);

    private:
        QString m_audio_file;
        QString m_info_file;
        QJsonObject m_data;
        QProcess* m_probe_process;
        bool m_probed;
    };
}

