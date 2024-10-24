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
#include <QTime>
#include <QRegularExpression>

#include "audiotool.h"
#include "audio.h"
#include "../utils/tools.h"

namespace fs = std::filesystem;
using namespace std::chrono_literals;

namespace AUDIO
{

    AudioTool::AudioTool()
        :m_probe_file{""}
    {
        m_wave_form_exe = QDir::currentPath()+"/"+AUDIO_WAVE_FILE_EXE;

        m_mp3_to_ogg_proc = new QProcess(this);
        connect(m_mp3_to_ogg_proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &AudioTool::mp3_to_ogg_finished);

        connect(m_mp3_to_ogg_proc, &QProcess::readyReadStandardOutput, this, &AudioTool::mp3_ready);
        connect(m_mp3_to_ogg_proc,SIGNAL(readyReadStandardOutput()),this,SLOT(mp3_ready()));
        m_wave_gen_proc = new QProcess(this);
        connect(m_wave_gen_proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &AudioTool::wave_gen_finished);

        m_probe_process = new QProcess(this);
        connect(m_probe_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &AudioTool::probe_finished);

    }

    AudioTool::~AudioTool()
    {
        delete m_wave_gen_proc;
        delete m_probe_process;
    }

    bool AudioTool::generate_wave_file(const std::string src_audio_file, const std::string dest_wave_file)
    {
        if (!fs::exists(m_wave_form_exe.toStdString()) ){
        auto msg = QString("Wave file generator does NOT exist! `%1`").arg(m_wave_form_exe);
        qDebug() << msg;
        return false;
        }

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

    std::string AudioTool::make_audio_filename(int audio_file_id)
    {
        std::ostringstream ss;
        int FILENAME_LEN{8};
        ss << std::setw(FILENAME_LEN) << std::setfill('0') << audio_file_id;
        return ss.str();
    }

    int AudioTool::filename_to_id(QString filename)
    {
        int index = filename.indexOf(QRegularExpression("[1-9]"));
        QString audio_id_str{""};

        for(auto ib=filename.begin()+index; ib < filename.end(); ++ib) {
            audio_id_str += *ib;
        }

        return audio_id_str.toInt();
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
        //src_file = src_file.replace("\\","/");
        auto src_path = fs::path(src_file.toStdString());

        //QString dest_file = QString::fromStdString(m_audio_file.audio_lib_path()+m_audio_file.ogg_filename());
        QString dest_file = QString::fromStdString(dest_ogg);
        //dest_file = dest_file.replace("\\", "/");
        auto dst_path = fs::path(dest_file.toStdString());

        try{
        fs::copy(src_path, dst_path);
        } catch ( fs::filesystem_error& fe) {
        qDebug() << stoq(fe.what());
        }

        return true;
    }

    bool AudioTool::copy_wave_to_audiolib(std::string src_file, std::string dest_file)
    {
        try{
        fs::copy(src_file, dest_file);
        } catch(fs::filesystem_error& fe) {
        qDebug() << stoq(fe.what());
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

    QString AudioTool::format_time(double time)
    {
        auto tm = QTime::fromMSecsSinceStartOfDay(time);
        auto fmt_str = QTime::fromMSecsSinceStartOfDay(time).toString("hh:mm:ss:zzz");
        return fmt_str;

    }

    void AudioTool::probe_file(const QString& audio_file)
    {
        m_probe_file = "";

        QFile af(audio_file);
        if (!af.open(QIODevice::ReadOnly)){
        qWarning("Couldn't open audio file!");
        return;
        }

        QFileInfo fi(audio_file);
        m_probe_filename = fi.absolutePath()+"/"+fi.baseName()+".prb";
        if (QFile::exists(m_probe_filename)){
        m_probe_file = m_probe_filename;
        return;
        }

        QString batch_runner = QDir::currentPath()+"/batch_runner.bat";
        if (QFile::exists(batch_runner))
        QFile::remove(batch_runner);

        QFile temp_batch_file(batch_runner);
        if (!temp_batch_file.open(QIODevice::WriteOnly)){
        qWarning("Couldn't open temp batch file");
        return;
        }

        QTextStream out_stream(&temp_batch_file);
        QString probe_tool = QDir::currentPath()+"/ffprobe.exe";

        out_stream << probe_tool
               << " -v error -show_format -of json "
               << audio_file
               << " > "
               << m_probe_filename;

        temp_batch_file.close();

        std::this_thread::sleep_for(std::chrono::seconds(1));
        m_probe_process->start("cmd.exe", QStringList() << "/c" << batch_runner);
        m_probe_process->waitForFinished();


    }

    void AudioTool::probe_finished(int code, QProcess::ExitStatus status)
    {
        qDebug() << "Probe finished ...";
        qDebug() << m_probe_filename;
        m_probe_file = m_probe_filename;
        /*
        QByteArray data = af.readAll();
        QJsonDocument audio_data(QJsonDocument::fromJson(data));
        return doc_data.object()["format"].toObject();
        */
    }
    QString AudioTool::probe_filename()
    {
        return m_probe_file;
    }

    QJsonObject AudioTool::get_probe_data(QString& probe_file)
    {
        QFile pf(probe_file);
        if (!pf.open(QIODevice::ReadOnly)){
        qWarning("Unable to read probe file!");
        return QJsonObject();
        }
        QByteArray data = pf.readAll();
        QJsonDocument doc_data(QJsonDocument::fromJson(data));
        return doc_data.object()["format"].toObject();
    }

    long long AudioTool::get_tick_count()
    {
        auto time_point = std::chrono::steady_clock::now();
        auto msec = std::chrono::time_point_cast<std::chrono::milliseconds>(time_point);
        return msec.time_since_epoch().count();
    }

    bool AudioTool::audio_exist(const QString audio_file)
    {
        QFile af(audio_file);
        return (af.exists()) ? true : false;
    }

    QTime AudioTool::msec_to_time(int h, int ms)
    {
        auto msecs = std::chrono::milliseconds(ms);
        auto hr = std::chrono::duration_cast<std::chrono::hours>(msecs);
        msecs -= hr;
        auto mins = std::chrono::duration_cast<std::chrono::minutes>(msecs);
        msecs -= mins;
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(msecs);
        QTime t(h, mins.count(), secs.count());
        return t;
    }

    //std::string AudioTool::full_audio_name(AUDIO::Audio* audio)
    //{
    //    auto ogg_file = generate_ogg_filename(audio->id())+".ogg";
    //    auto full_name = audio->file_path()->value()+ogg_file;

    //    return full_name;
    //}

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

        auto bytes = adf_file.write(save_adf.toJson());

        return (bytes > -1) ? true : false;

    }

    CueMarker ADFRepository::read_markers(const std::string adf_filename)
    {
        QFile adf_file(QString::fromStdString(adf_filename));
        if (!adf_file.open(QIODevice::ReadOnly)){
        qWarning("Couldn't open adf file.");
        return CueMarker();
        }

        QByteArray data = adf_file.readAll();

        QJsonDocument doc_data(QJsonDocument::fromJson(data));

        return json_to_markers(doc_data.object()["markers"].toObject());
    }



    void ADFRepository::object_to_json(const AudioFile& audio_file, QJsonObject& json)
    {
        json["audio_title"] = QString::fromStdString(audio_file.audio_title());
        json["short_desc"] = QString::fromStdString(audio_file.short_desc());
        json["artist_name"] = QString::fromStdString(audio_file.artist_name());
        json["audio_path"] = QString::fromStdString(audio_file.audio_lib_path());
        json["filename"] = QString::fromStdString(audio_file.audio_lib_path()+
                              audio_file.ogg_short_filename());
        json["short_filename"] = QString::fromStdString(audio_file.ogg_short_filename());
        json["duration"] = audio_file.duration();
        json["creation_date"] = QString::fromStdString(audio_file.creation_date());
        json["class"] = QString::fromStdString(audio_file.audio_class());
        json["genre"] = QString::fromStdString(audio_file.genre());
        json["year"] = audio_file.year();

        QJsonObject marker;

        auto m = audio_file.marker();

        marker["start"] = m.round_marker(m.start_marker);
        marker["fade_in"] = m.round_marker(m.fade_in);
        marker["intro"] = m.round_marker(m.intro);
        marker["extro"] = m.round_marker(m.extro);
        marker["fade_out"] = m.round_marker(m.fade_out);
        marker["end"] = m.round_marker(m.end_marker);

        json["markers"] = marker;
    }

    CueMarker ADFRepository::json_to_markers(const QJsonObject& json)
    {
        CueMarker m;

        m.start_marker = json["start"].toDouble();
        m.fade_in = json["fade_in"].toDouble();
        m.intro = json["intro"].toDouble();
        m.fade_out = json["fade_out"].toDouble();
        m.extro = json["extro"].toDouble();
        m.end_marker = json["end"].toDouble();

        return m;
    }
}
