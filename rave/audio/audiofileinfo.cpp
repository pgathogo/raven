#include <chrono>

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
        QFile af(m_audio_file);
        if (!af.open(QIODevice::ReadOnly)){
            qWarning("Couldn't open audio file! Probe failed.");
            return;
        }

        QFileInfo fi(m_audio_file);
        m_info_file = fi.absolutePath()+"/"+fi.baseName()+".prb";
        if (QFile::exists(m_info_file)){
            set_data(m_info_file);
            return;
        }

        QString batch_runner = QDir::currentPath()+"/batch_runner.bat";
        if (QFile::exists(batch_runner))
            QFile::remove(batch_runner);

        QFile temp_batch_file(batch_runner);
        if (!temp_batch_file.open(QIODevice::WriteOnly)){
            qWarning("Couldn't open temp batch file. Probe failed");
            return;
        }

        QTextStream out_stream(&temp_batch_file);
        QString probe_tool = QDir::currentPath()+"/"+PROBE_TOOL;

        out_stream << probe_tool
                   << " -v error -show_format -of json "
                   << m_audio_file
                   << " > "
                   << m_info_file;

        temp_batch_file.close();

        std::this_thread::sleep_for(std::chrono::seconds(1));
        m_probe_process->start("cmd.exe", QStringList() << "/c" << batch_runner);
        m_probe_process->waitForFinished();
    }

    void AudioFileInfo::file_probe_finished(int code, QProcess::ExitStatus status)
    {
        set_audio_file(m_info_file);
        m_probed = true;
    }

    void AudioFileInfo::set_data(QString info_file)
    {
        if (!QFile::exists(info_file)){
            qDebug() << "Missing info file!";
            return;
        }

        QFile pf(info_file);
        if (!pf.open(QIODevice::ReadOnly)){
            qWarning("Unable to read info file!");
            return;
        }

        QByteArray data = pf.readAll();

        if (data.size() > 0){
            QJsonDocument doc_data(QJsonDocument::fromJson(data));
            m_data = doc_data.object()["format"].toObject();
        }
    }


    QString AudioFileInfo::file_format()
    {
        if (!m_probed)
            probe_file_info();

        QString fn = "format_name";

        if (m_data.size() == 0)
            return "";

        if (!m_data.contains(fn))
            return "";

        return m_data[fn].toString();
    }

    void AudioFileInfo::set_audio_file(const QString& audio_file)
    {
        m_probed = false;
        m_audio_file = audio_file;
        probe_file_info();
    }
}
