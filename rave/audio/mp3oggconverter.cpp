#include <sstream>
#include <chrono>
#include <thread>

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QProgressDialog>
#include <QProcess>

#include "../framework/ravenexception.h"
#include "mp3oggconverter.h"
#include "audiotool.h"
#include "audio.h"

namespace AUDIO
{
    Mp3ToOggConverter::Mp3ToOggConverter(const QString audio_filename,
                                     std::shared_ptr<QProcess> process_runner)
        :AudioConverter(audio_filename)
        ,m_mp3_filename{audio_filename}
        ,m_converter_process{process_runner.get()}
    {
        // m_converter_process = new QProcess(this);

        if (m_converter_process != nullptr)
        {
            //connect(m_converter_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &Mp3ToOggConverter::conversion_finished);
            connect(m_converter_process, SIGNAL(finished(int, QProcess::ExitStatus)),this, SLOT(conversion_finished(int, QProcess::ExitStatus)));
            //connect(m_converter_process, &QProcess::errorOccurred, this, &Mp3ToOggConverter::conversion_error);
            connect(m_converter_process, SIGNAL(errorOccured(QProcess::ProcessError)),this, SLOT(conversion_error(QProcess::ProcessError)));
        }
    }

    Mp3ToOggConverter::~Mp3ToOggConverter()
    {
        //delete m_converter_process;
    }


    bool Mp3ToOggConverter::convert()
    {
        qDebug() << "MP3 to OGG conversion started ...";

        QString ffmpeg = QDir::currentPath()+"/"+FFMPEG_EXE;
        //QString batch_exec = QDir::currentPath()+"/"+BATCH_EXEC;

        auto in_file = QDir::toNativeSeparators(m_mp3_filename);
        auto out_file = QDir::toNativeSeparators(ogg_filename());

        qDebug() << "In File: "<< in_file;
        qDebug() << "Out File: "<< out_file;

        QString batch_file = QDir::currentPath()+"/"+TEMP_BATCH;

        qDebug() << "Batch File: "<< batch_file;

        if (QFile::exists(batch_file)){
            if (!QFile::remove(batch_file)){
                throw AudioImportException("MP3-to-OGG", "Error: Unable to remove batch file `"+batch_file.toStdString()+"`");
            }
        }

        QFile temp_batch(batch_file);
        if (!temp_batch.open(QIODevice::WriteOnly)){
            throw AudioImportException("MP3-to-OGG", "Error: Could not open temp batch file `"+batch_file.toStdString()+"`");
        }

        QTextStream out_stream(&temp_batch);

        out_stream << ffmpeg << " -y -i "
                   << "\""+in_file+"\""
                   << " -c:a libvorbis -q:a 4 -vsync 2 "
                   << "\""+out_file+"\"";

        temp_batch.close();

        //std::this_thread::sleep_for(std::chrono::seconds(2));

        m_converter_process->start("cmd.exe", QStringList() << "/c" << batch_file);

        //m_progress_dialog->exec();

        //m_converter_process->waitForReadyRead();
        m_converter_process->waitForFinished();

        return true;
    }

    QString Mp3ToOggConverter::mp3_filename()
    {
        return m_mp3_filename;
    }

    void Mp3ToOggConverter::set_mp3_filename(QString mp3_file)
    {
        m_mp3_filename = mp3_file;
    }


    void Mp3ToOggConverter::conversion_finished(int code, QProcess::ExitStatus exit)
    {
        m_progress_dialog->close();
        qDebug() << "MP3 to OGG conversion finished.";
    }

    void Mp3ToOggConverter::conversion_error(QProcess::ProcessError error)
    {
        switch(error){

        case QProcess::FailedToStart:
            throw AudioImportException("WAVE-GENERATOR", "Fail Start: Wave generator failed to start, missing file or insufficient rights");
            break;
        case QProcess::Crashed:
            throw AudioImportException("WAVE-GENERATOR", "Crashed: Wave generator crashed!");
            break;
        case QProcess::Timedout:
            throw AudioImportException("WAVE-GENERATOR", "Timeout: Wave generator took so long to finish.");
            break;
        case QProcess::WriteError:
            throw AudioImportException("WAVE-GENERATOR", "Write Error: Unable to write to proces.");
            break;
        case QProcess::ReadError:
            throw AudioImportException("WAVE-GENERATOR", "Read Error: Unable to read from the process.");
            break;
        case QProcess::UnknownError:
            throw AudioImportException("WAVE-GENERATOR", "Unknown Error: Unknown error occured.");
            break;
        }

    }

}
