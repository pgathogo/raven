#ifndef AUDIOWAVEFORM_H
#define AUDIOWAVEFORM_H

#include <memory>
#include <QDialog>

#include "audiographic.h"
#include "../../audio/audiofile.h"

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
        explicit AudioWaveForm(AudioFile& audio_file, QDialog* parent = nullptr);
        ~AudioWaveForm() override;

        void show_wave_file();
        unsigned int audio_len(QString);
        float audio_sample_rate(QString);
        float audio_bitrate(QString audio_file);

    public slots:
        void save();
        void on_cancel();

        void start_play();
        void stop_play();
        void pause_play();

        void update_vumeter();
        void update_indicator();

        void audio_current_position(double, double);
        void audio_current_peak(float fft[1024]);
        double seconds_per_pixel();

        std::string base_filename(const std::string filepath);
        QString format_time(double time);

        Marker marker() const;

        void mark_start();
        void mark_fade_in();
        void mark_intro();
        void mark_fade_out();
        void mark_extro();
        void mark_end();

        void play_mark(QPointF);
        void play_start_mark();
        void play_fade_in();
        void play_intro();
        void play_fade_out();
        void play_extro();
        void play_end_marker();

    protected:
        void showEvent(QShowEvent* event) override;
        void move_indicator_line(double new_position);
        AUDIO::MarkerDisplayUnit* find_display_unit(MarkerType marker_type);
        void create_marker_display_unit(MarkerIndicator* marker, QLineF line);
        void create_marker_line(AUDIO::MarkerType marker_type);
        void create_marker_indicator(MarkerType, QLineF);
        void show_markers(Marker markers);
        void show_marker_value(Marker marker);
        void show_mark(double mark, MarkerType marker_type);

        void create_marker_line(AUDIO::MarkerType marker_type, QLineF line);

    private:
        Ui::AudioWaveForm *ui;
        AudioFile& m_audio_file;
        double m_seconds_per_pixel;
        AudioWave* m_audio_wave;
        AudioWaveScene* m_scene;
        AudioThread* m_audio_thread;
        QTimer* m_player_timer;
        QTimer* m_indicator_timer;

        void init_widgets();

        MarkerIndicator* m_start_marker;
        Marker m_marker;

        void save_markers();

        AUDIO::MarkerIndicator*	m_start_indicator;
        AUDIO::MarkerDisplayUnit* m_start_display_unit;

        AUDIO::MarkerIndicator*	m_fade_in_indicator;
        AUDIO::MarkerDisplayUnit* m_fade_in_display_unit;

        std::map<AUDIO::MarkerType, AUDIO::MarkerDisplayUnit*> m_display_units;

    };

} // AUDIO

#endif // AUDIOWAVEFORM_H
