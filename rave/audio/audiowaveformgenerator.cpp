#include <thread>

#include <QFileInfo>
#include <QFile>

#include <QDir>
#include <QProgressDialog>

#include <QDebug>

#include "../framework/ravenexception.h"
#include "audiowaveformgenerator.h"

namespace AUDIO{

    AudioWaveFormGenerator::AudioWaveFormGenerator(const QString audio_file)
    {
        set_audio_filename(audio_file);
        m_generator_process = new QProcess(this);

        connect(m_generator_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),this, &AudioWaveFormGenerator::wave_generation_finished);
        connect(m_generator_process, &QProcess::errorOccurred, this, &AudioWaveFormGenerator::generator_error);
    }

    AudioWaveFormGenerator::~AudioWaveFormGenerator()
    {
 //       delete m_progress_dialog;
        delete m_generator_process;
    }

    bool AudioWaveFormGenerator::generate()
    {
        auto gen_engine = QDir::currentPath()+"/"+AUDIO_WAVE_FILE_EXE;

        QFileInfo fi(gen_engine);
        if (!fi.exists()){
            throw AudioImportException("GEN-ENGINE",
                                        "Wave file generator engine does NOT exists!");
        }

        // m_progress_dialog = new QProgressDialog("Reading audio file...please wait.","", 0, 100);
        // m_progress_dialog->setAttribute(Qt::WA_DeleteOnClose);
        // m_progress_dialog->setRange(0,0);
        // m_progress_dialog->setCancelButton(nullptr);

        QString input_file  = m_audio_filename;
        QString output_file = m_wave_filename;

        QStringList args;
        args << "-i" << input_file
             << "-o" << output_file
             << "--pixels-per-second" << "100"
             << "-b" << "16"
             << "-w" << "800";

             // << "-h" << "200";
             //<< "--split-channels"
             //<< "-z" << "44100"
             //<< "-z" << "auto";

        qDebug() << "INPUT File: "<< input_file;
        qDebug() << "OUTPUT File: "<< output_file;

        m_generator_process->start(gen_engine, args);
        // m_progress_dialog->exec();
        m_generator_process->waitForFinished();

        return true;
    }

    QString AudioWaveFormGenerator::audio_filename()
    {
        return m_audio_filename;
    }

    void AudioWaveFormGenerator::set_audio_filename(QString audio_file)
    {
        m_audio_filename = audio_file;
        set_wave_filename(audio_file);
    }

    QString AudioWaveFormGenerator::wave_filename()
    {
        return m_wave_filename;
    }

    void AudioWaveFormGenerator::set_wave_filename(QString audio_file)
    {
        QFileInfo fi(audio_file);
        auto abs_path = fi.absolutePath();
        auto base_name = fi.baseName();

        m_wave_filename = abs_path+"/"+base_name+".png";
    }

    void AudioWaveFormGenerator::wave_generation_finished(int code, QProcess::ExitStatus status)
    {
        // m_progress_dialog->close();
    }

    void AudioWaveFormGenerator::generator_error(QProcess::ProcessError error)
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

    void AudioWaveFormGenerator::remove_wave_file()
    {
        if (QFileInfo::exists(m_wave_filename)){
            QFile::remove(m_wave_filename);
        }
    }
}
