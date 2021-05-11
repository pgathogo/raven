#include <sstream>
#include <chrono>
#include <thread>
#include <filesystem>

#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QProgressDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

#include "audiotool.h"
#include "audiotool.h"
#include "audiotool.h"


namespace fs = std::filesystem;


AudioTool::AudioTool()
{
    m_wave_form_exe = QDir::currentPath()+"/"+AUDIO_WAVE_FILE_EXE;

    m_mp3_to_ogg_proc = new QProcess(this);
    connect(m_mp3_to_ogg_proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &AudioTool::mp3_to_ogg_finished);
//    connect(m_mp3_to_ogg_proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
//            [](int i, QProcess::ExitStatus es){qDebug() << "Lambda call"; });

    connect(m_mp3_to_ogg_proc, &QProcess::readyReadStandardOutput, this, &AudioTool::mp3_ready);
    connect(m_mp3_to_ogg_proc,SIGNAL(readyReadStandardOutput()),this,SLOT(mp3_ready()));
    m_wave_gen_proc = new QProcess(this);
    connect(m_wave_gen_proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &AudioTool::wave_gen_finished);

}

AudioTool::~AudioTool()
{
    delete m_wave_gen_proc;
}

bool AudioTool::generate_wave_file(const std::string src_audio_file, const std::string dest_wave_file)
{
    m_progress_dialog = new QProgressDialog("Reading audio file...please wait.","", 0, 100);
    m_progress_dialog->setAttribute(Qt::WA_DeleteOnClose);
    m_progress_dialog->setRange(0,0);
    m_progress_dialog->setCancelButton(nullptr);

    QStringList args;

    QString input_file  = QString::fromStdString(src_audio_file);
    QString output_file = QString::fromStdString(dest_wave_file);

    args << "-i" << input_file
         << "-o" << output_file
         << "--pixels-per-second" << "100"
         << "-b" << "16"
         << "-w" << "800";

         // << "-h" << "200";
         //<< "--split-channels"
         //<< "-z" << "44100"
         //<< "-z" << "auto";

    qDebug() << m_wave_form_exe ;
    qDebug() << "INPUT: "<< input_file;
    qDebug() << "OUTPUT: "<< output_file;
    qDebug() << m_wave_form_exe ;


    m_wave_gen_proc->start(m_wave_form_exe, args);
    m_progress_dialog->exec();
    m_wave_gen_proc->waitForFinished();

    return true;
}

std::string AudioTool::mp3_to_ogg(AudioFile& audio_file)
{
    qDebug() << "mp3_to_ogg ...";


    //audio_file.set_ogg_filename(audio_file.short_filename()+".ogg");

    QString ffmpeg = QDir::currentPath()+"/"+FFMPEG_EXE;
    QString batch_exec = QDir::currentPath()+"/"+BATCH_EXEC;

    //std::string ogg_file = audio_file.short_filename()+".ogg";
//    QString out_file = QString::fromStdString(audio_file.ogg_filename());
//    out_file = out_file.replace("\\", "/");
    auto out_file = QDir::toNativeSeparators(QString::fromStdString(audio_file.ogg_filename()));


//    QString in_file = QString::fromStdString(audio_file.audio_file());
    auto in_file = QDir::toNativeSeparators(QString::fromStdString(audio_file.audio_file()));


    //in_file = in_file.replace("\\","/");
    //out_file = out_file.replace("\\", "/");

    qDebug() << "SRC: "<< in_file;
    qDebug() << "DEST: "<< out_file;

    m_progress_dialog = new QProgressDialog("Converting and saving MP3 to OGG...please wait.",
                                            out_file, 0, 100);
    m_progress_dialog->setAttribute(Qt::WA_DeleteOnClose);
    m_progress_dialog->setRange(0,0);
    m_progress_dialog->setCancelButton(nullptr);

    QStringList args;

    args << "-i " + in_file
         + " -c:a libvorbis -q:a 4 -vsync 2 "
         + out_file;

    qDebug() << "Converter: "<< ffmpeg;
    qDebug() << "Args: "<< args;

    QString batch_file = QDir::currentPath()+"/"+TEMP_BATCH;
    if (QFile::exists(batch_file))
        QFile::remove(batch_file);

    QFile temp_batch(batch_file);
    if (!temp_batch.open(QIODevice::WriteOnly)){
        qWarning("Couldn't open temp batch file!");
        return "";
    }

    QTextStream out_stream(&temp_batch);

    out_stream << ffmpeg << " -y -i "
               << in_file
               << " -c:a libvorbis -q:a 4 -vsync 2 "
               << out_file;

    temp_batch.close();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    m_mp3_to_ogg_proc->start("cmd.exe", QStringList() << "/c" << batch_file);

    m_progress_dialog->exec();

    m_mp3_to_ogg_proc->waitForReadyRead();

    return audio_file.ogg_short_filename();
}

std::string AudioTool::generate_ogg_filename(int audio_file_id)
{
    std::ostringstream ss;
    int FILENAME_LEN{8};
    ss << std::setw(FILENAME_LEN) << std::setfill('0') << audio_file_id;
    return ss.str();
}

bool AudioTool::copy_ogg_to_audiolib(std::string src_ogg, std::string dest_ogg)
{
//    m_progress_dialog = new QProgressDialog("Converting and saving MP3 to OGG...please wait.","", 0, 100);
//    m_progress_dialog->setAttribute(Qt::WA_DeleteOnClose);
//    m_progress_dialog->setRange(0,0);
//    m_progress_dialog->setCancelButton(nullptr);

//    m_audio_file.set_ogg_filename(generate_ogg_filename(
//                                      m_audio_file.temp_id())+".ogg");

    //QString src_file = QString::fromStdString(m_audio_file.audio_file());
    QString src_file = QString::fromStdString(src_ogg);
    src_file = src_file.replace("\\","/");

    //QString dest_file = QString::fromStdString(m_audio_file.audio_lib_path()+m_audio_file.ogg_filename());
    QString dest_file = QString::fromStdString(dest_ogg);
    dest_file = dest_file.replace("\\", "/");

    try{
        fs::copy(src_file.toStdString(), dest_file.toStdString());
    } catch ( fs::filesystem_error fe) {
        qDebug() << QString::fromUtf8(fe.what());
        return false;
    }

    return true;
}

void AudioTool::wave_gen_finished()
{
    m_progress_dialog->close();
}

void AudioTool::mp3_to_ogg_finished(int code, QProcess::ExitStatus exit)
{
    m_progress_dialog->close();
    qDebug() << "Exit status :" << exit;
    qDebug() << "Finished mp3 conversion...";
}

void AudioTool::mp3_ready()
{
    qDebug() << "MP3_READY: "<< m_mp3_to_ogg_proc->readAllStandardOutput();
}


/*  --- ADFRepository --- */

ADFRepository::ADFRepository()
{
}

bool ADFRepository::write(const AudioFile& audio_file)
{
    QFile adf_file(QString::fromStdString(audio_file.adf_file()));
    if (!adf_file.open(QIODevice::WriteOnly)){
        qWarning("Couldn't open adf file.");
        return false;
    }

    QJsonObject adf_data;
    object_to_json(audio_file, adf_data);

    QJsonDocument save_adf(adf_data);
    adf_file.write(save_adf.toJson());

    qDebug() << "Data is saved.";

    return true;
}

bool ADFRepository::read_markers(AudioFile& audio_file)
{
    QFile adf_file(QString::fromStdString(audio_file.adf_file()));
    if (!adf_file.open(QIODevice::ReadOnly)){
        qWarning("Couldn't open adf file.");
        return false;
    }

    QByteArray data = adf_file.readAll();

    QJsonDocument doc_data(QJsonDocument::fromJson(data));

    audio_file.set_marker(json_to_markers(doc_data.object()["markers"].toObject()));

    return true;
}

void ADFRepository::object_to_json(const AudioFile& audio_file, QJsonObject& json)
{
    json["audio_title"] = QString::fromStdString(audio_file.audio_title());
    json["short_desc"] = QString::fromStdString(audio_file.short_desc());
    json["artist_name"] = QString::fromStdString(audio_file.artist_name());
    json["audio_path"] = QString::fromStdString(audio_file.audio_lib_path());
    json["filename"] = QString::fromStdString(audio_file.ogg_filename()+".ogg");
    json["short_filename"] = QString::fromStdString(audio_file.ogg_filename());
    json["duration"] = audio_file.duration();

    QJsonObject marker;

    marker["start"] = audio_file.marker().start_marker;
    marker["fade_in"] = audio_file.marker().fade_in;
    marker["intro"] = audio_file.marker().intro;
    marker["extro"] = audio_file.marker().extro;
    marker["fade_out"] = audio_file.marker().fade_out;
    marker["end"] = audio_file.marker().end_marker;

    json["markers"] = marker;
}

Marker ADFRepository::json_to_markers(const QJsonObject& json)
{
    Marker m;

    m.start_marker = json["start"].toDouble();
    m.fade_in = json["fade_in"].toDouble();
    m.intro = json["intro"].toDouble();
    m.fade_out = json["fade_out"].toDouble();
    m.extro = json["extro"].toDouble();
    m.end_marker = json["end"].toDouble();

    return m;
}
