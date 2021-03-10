#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QProgressDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

#include "audiotool.h"


AudioTool::AudioTool(AudioFile& audio_file)
    :m_audio_file{audio_file}
{
    m_wave_form_exe = QDir::currentPath()+"/"+AUDIO_WAVE_FILE_EXE;

    m_wave_gen_proc = new QProcess(this);
    connect(m_wave_gen_proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &AudioTool::wave_gen_finished);
}

AudioTool::~AudioTool()
{
    delete m_wave_gen_proc;
}

AudioFile AudioTool::audio_file() const
{
    return m_audio_file;
}

bool AudioTool::generate_wave_file()
{
    m_progress_dialog = new QProgressDialog("Reading audio file...please wait.","", 0, 100);
    m_progress_dialog->setAttribute(Qt::WA_DeleteOnClose);
    m_progress_dialog->setRange(0,0);
    m_progress_dialog->setCancelButton(nullptr);

    QStringList args;

    args << "-i" << QString::fromStdString(m_audio_file.audio_file())
         << "-o" << QString::fromStdString(m_audio_file.wave_file())
         << "--pixels-per-second" << "100"
         << "-b" << "16"
         << "-w" << "800";
         // << "-h" << "200";
         //<< "--split-channels"
         //<< "-z" << "44100"
         //<< "-z" << "auto";

    m_wave_gen_proc->start(m_wave_form_exe, args);
    m_progress_dialog->exec();
    m_wave_gen_proc->waitForFinished();

    return true;
}

bool AudioTool::mp3_to_ogg()
{

}

void AudioTool::wave_gen_finished()
{
    m_progress_dialog->close();
}

ADFRepository::ADFRepository(AudioFile &audio_file)
    :m_audio_file{audio_file}
{

}

bool ADFRepository::write()
{
    QFile adf_file(QString::fromStdString(m_audio_file.adf_file()));
    if (!adf_file.open(QIODevice::WriteOnly)){
        qWarning("Couldn't open adf file.");
        return false;
    }

    QJsonObject adf_data;
    update_json_object(adf_data);

    QJsonDocument save_adf(adf_data);
    adf_file.write(save_adf.toJson());

    qDebug() << "Data is saved.";

    return true;
}

bool ADFRepository::read()
{
    QFile adf_file(QString::fromStdString(m_audio_file.adf_file()));
    if (!adf_file.open(QIODevice::ReadOnly)){
        qWarning("Couldn't open adf file.");
        return false;
    }

    QByteArray data = adf_file.readAll();

    QJsonDocument doc_data(QJsonDocument::fromJson(data));

    read_audio_marker(doc_data.object()["markers"].toObject());

    return true;
}

void ADFRepository::update_json_object(QJsonObject& json)
{
    json["audio_title"] = QString::fromStdString(m_audio_file.audio_title());
    json["artist_name"] = QString::fromStdString(m_audio_file.artist_name());
    json["audio_path"] = QString::fromStdString(m_audio_file.audio_path());
    json["audio_file"] = QString::fromStdString(m_audio_file.audio_file());
    json["filename"] = QString::fromStdString(m_audio_file.filename());
    json["file_ext"] = QString::fromStdString(m_audio_file.file_ext());
    json["short_filename"] = QString::fromStdString(m_audio_file.short_filename());
    json["duration"] = m_audio_file.duration();

    QJsonObject marker;

    marker["start"] = m_audio_file.marker().start_marker;
    marker["fade_in"] = m_audio_file.marker().fade_in;
    marker["intro"] = m_audio_file.marker().intro;
    marker["extro"] = m_audio_file.marker().extro;
    marker["fade_out"] = m_audio_file.marker().fade_out;
    marker["end"] = m_audio_file.marker().end_marker;

    json["markers"] = marker;
}

void ADFRepository::read_audio_marker(const QJsonObject& json)
{
    Marker m;

    m.start_marker = json["start"].toDouble();
    m.fade_in = json["fade_in"].toDouble();
    m.intro = json["intro"].toDouble();
    m.fade_out = json["fade_out"].toDouble();
    m.extro = json["extro"].toDouble();
    m.end_marker = json["end"].toDouble();

    m_audio_file.set_marker(m);
}
