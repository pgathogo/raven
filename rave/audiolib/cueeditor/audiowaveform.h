#ifndef AUDIOWAVEFORM_H
#define AUDIOWAVEFORM_H

#include <memory>
#include <QDialog>

#include "audiographic.h"
//#include "cueeditor.h"
#include "../../headers/audiofile.h"

class AudioFile;
class QShowEvent;
class AudioThread;
class QTimer;

namespace AUDIO {

    namespace Ui {
        class AudioWaveForm;
    }

    class AudioWaveForm : public QDialog
    {
        Q_OBJECT

    public:
        explicit AudioWaveForm(AudioFile audio_file, QDialog* parent = nullptr);
        ~AudioWaveForm() override;

        void show_wave_file();

    public slots:
        void on_save();
        void on_cancel();

        void on_play();
        void stop_play();

        void update_vumeter();
        void update_indicator();

        void audio_current_position(double, double);
        void audio_current_peak(float fft[1024]);
        double seconds_per_pixel();

    protected:
        void showEvent(QShowEvent* event) override;
        void move_indicator_line(double new_position);

    private:
        Ui::AudioWaveForm *ui;
        AudioFile m_audio_file;
        double m_seconds_per_pixel;
        AudioWave* m_audio_wave;
        AudioWaveScene* m_scene;
        AudioThread* m_audio_thread;
        QTimer* m_player_timer;
        QTimer* m_indicator_timer;

    };

} // AUDIO

#endif // AUDIOWAVEFORM_H
