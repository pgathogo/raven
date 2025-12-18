#include <chrono>
#include <format>

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include <QTextStream>
#include <QDebug>
#include <thread>


#include "audiofileinfo.h"

namespace AUDIO{

    AudioFileInfo::AudioFileInfo(const QString& audio_file)
        :m_audio_file{audio_file}
        ,m_probed{false}
    {
        m_probe_process = new QProcess(this);
        //connect(m_probe_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &AudioFileInfo::file_probe_finished);

        connect(m_probe_process, SIGNAL(finished(int, QProcess::ExitStatus)),this, SLOT(file_probe_finished(int, QProcess::ExitStatus)));
    }

    AudioFileInfo::~AudioFileInfo()
    {
    }

    void AudioFileInfo::probe_file_info()
    {
        log_info(QString("Start probe file: `%1`").arg(m_audio_file));

        QFile af(m_audio_file);
        if (!af.open(QIODevice::ReadOnly)){

            QString msg = "Couldn't open audio file! Probe failed.";

            log_error(msg);

            //qWarning("File open error");

            return;
        }

        QFileInfo fi(m_audio_file);
        m_info_file = fi.absolutePath()+"/"+fi.baseName()+".prb";

        log_info(QString("Probe file: `%1`").arg(m_info_file));

        if (QFile::exists(m_info_file)){

            log_info("Probe file found.");

            set_data(m_info_file);
            return;
        } else {

            // TODO: What happens if the probe file is missing.... probably you need to abort the process!
            log_error(QString("Probe file: `%1` NOT found.").arg(m_info_file));

        }

        QString batch_runner = QDir::currentPath()+"/batch_runner.bat";

        log_info(QString("Batch runner file: `%1`").arg(batch_runner));

        if (QFile::exists(batch_runner)) {
            log_info("Batch runner file found... removing it.");
            QFile::remove(batch_runner);
        }

        QFile temp_batch_file(batch_runner);
        if (!temp_batch_file.open(QIODevice::WriteOnly)){

            QString msg = "Couldn't open temp batch file. Probe failed";
            log_error(msg);
            //qWarning(msg);

            return;
        } else {
            log_info("Batch runner file opened for writting.");
        }

        // TODO: Check if probe.exe exist !!

        QTextStream out_stream(&temp_batch_file);
        QString probe_tool = QDir::currentPath()+"/"+PROBE_TOOL;

        log_info(QString("Probe tool: `%1`").arg(probe_tool));

        if (!QFile::exists(probe_tool)) {
            log_error(QString("Probe tool: `%1` NOT found. File probing terminated.").arg(probe_tool));
            return;
        }

        log_info("Writting probe command to batch runner file");

        out_stream << probe_tool
                   << " -v error -show_format -of json "
                   << QString(R"("%1")").arg(m_audio_file)
                   << " > "
                   << QString(R"("%1")").arg(m_info_file);

        temp_batch_file.close();

        log_info("Ready to run probing command");

        std::this_thread::sleep_for(std::chrono::seconds(1));
        m_probe_process->start("cmd.exe", QStringList() << "/c" << batch_runner);
        m_probe_process->waitForFinished();

        log_info(QString("END: probing file `%1` complete.").arg(m_audio_file));

    }

    void AudioFileInfo::file_probe_finished(int code, QProcess::ExitStatus status)
    {
        log_info(QString("Probing complete with code: %1").arg(QString::number(code)));

        set_audio_file(m_info_file);
        m_probed = true;
    }

    void AudioFileInfo::set_data(QString info_file)
    {
        log_info(QString("Setting data from probe info file: `%1`").arg(info_file));

        if (!QFile::exists(info_file)){

            log_error(QString("Probe info file `%1` NOT found. Setting data terminated.").arg(info_file));

            return;
        }

        log_info("Reading audio probe information...");

        QFile pf(info_file);
        if (!pf.open(QIODevice::ReadOnly)){
            QString msg = "Unable to read audio probe info file";

            log_error(QString("%1: `%2`. Process aborted.").arg(msg).arg(info_file));

            //qWarning(msg);
            return;
        }

        log_info("Reading data from audio probe file...");

        QByteArray data = pf.readAll();

        if (data.size() > 0){
            QJsonDocument doc_data(QJsonDocument::fromJson(data));

            m_data = doc_data.object()["format"].toObject();

            log_info(QString("Converting audio probe information to JSON..."));

        } else {
            log_error("NO data found in the audio probe file");
        }

        log_info("END setting probe information data.");

    }


    QString AudioFileInfo::file_format()
    {
        log_info("Start: Find audio file format...");

        if (!m_probed)
            probe_file_info();

        QString json_key = "format_name";

        if (m_data.size() == 0) {
            log_error("NO data in the probe object. Returning empty file format");
            return "";
        } else {
            log_info(QString("Audio probe data file size: %1").arg(QString::number(m_data.size())));
        }


        if (!m_data.contains(json_key))
            return "";


        QString file_fmt =  m_data[json_key].toString();

        log_info(QString("File format found in probe file: %1").arg(file_fmt));

        return file_fmt;

    }

    void AudioFileInfo::set_audio_file(const QString& audio_file)
    {
        m_probed = false;
        m_audio_file = audio_file;
        probe_file_info();
    }
}
