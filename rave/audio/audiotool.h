#ifndef AUDIOTOOL_H
#define AUDIOTOOL_H

#include <QObject>
#include <QProcess>

#include "../audio/audiofile.h"

class QProcess;
class QProgressDialog;

namespace AUDIO{
class Audio;
}


class AudioTool : public QObject
{
    Q_OBJECT
public:
    QString AUDIO_WAVE_FILE_EXE = "audiowaveform.exe";
    QString FFMPEG_EXE = "ffmpeg.exe";
    QString BATCH_EXEC = "test.bat";
    QString TEMP_BATCH = "mp3_to_ogg.bat";

    AudioTool();
    ~AudioTool();

    bool generate_wave_file(const std::string src_audio_file, const std::string dest_wave_file);
    std::string mp3_to_ogg(AudioFile&);
    std::string generate_ogg_filename(int audio_file_id);
    bool copy_ogg_to_audiolib(std::string src_ogg, std::string dest_ogg);

//    std::string full_audio_name(AUDIO::Audio*);


private slots:
    void wave_gen_finished();
    void mp3_to_ogg_finished(int, QProcess::ExitStatus);
    void mp3_ready();

private:
    QString m_wave_form_exe;
    QProcess* m_wave_gen_proc;
    QProcess* m_mp3_to_ogg_proc;
    QProgressDialog* m_progress_dialog;
};


class ADFRepository
{
    public:
        ADFRepository();
        bool write(const AudioFile&);
        bool read_markers(AudioFile&);
    protected:
        void object_to_json(const AudioFile&, QJsonObject&);
        Marker json_to_markers(const QJsonObject&);
};

#endif // AUDIOTOOL_H
