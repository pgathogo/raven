#ifndef AUDIOTOOL_H
#define AUDIOTOOL_H

#include <QObject>

#include "../audio/audiofile.h"

class QProcess;
class QProgressDialog;


class AudioTool : public QObject
{
    Q_OBJECT
public:
    QString AUDIO_WAVE_FILE_EXE = "audiowaveform.exe";

    QString FFMPEG_EXE = "ffmpeg.exe";

    AudioTool(AudioFile& audio_file);
    ~AudioTool();
    AudioFile audio_file() const;
    bool generate_wave_file();
    bool mp3_to_ogg();

private slots:
    void wave_gen_finished();

private:
    AudioFile& m_audio_file;
    QString m_wave_form_exe;
    QProcess* m_wave_gen_proc;
    QProgressDialog* m_progress_dialog;
};


class ADFRepository
{
    public:
        ADFRepository(AudioFile& audio_file);
        bool write();
        bool read();
    protected:
        void update_json_object(QJsonObject&);
        void read_audio_marker(const QJsonObject&);
    private:
        AudioFile& m_audio_file;
};

#endif // AUDIOTOOL_H
