#ifndef AUDIOWAVEFORM_H
#define AUDIOWAVEFORM_H

#include <memory>

#include <QDialog>
#include <QThread>

#include "audiographic.h"
#include "../audiofile.h"

class QShowEvent;
class AudioThread;
class QTimer;
class QProgressBar;

namespace AUDIO {

    namespace Ui {
        class AudioWaveForm;
    }

    //class VuMeter;
    class AudioPlayer;
    class AudioMeter;
    class WaveFormWidget;
    class Audio;

    enum class FormMode {New, Edit};
    enum class ProcessOption {FormatConversion, GenerateWaveFile, LoadWaveFile};

    class AudioConversionThread : public QThread
    {
        Q_OBJECT
    public:
        AudioConversionThread(const QString, ProcessOption, QObject* parent=nullptr);
    private:
        void run() override;

        QString convert_audio(const QString);
        QString generate_audio_wave_file(const QString);
        QString m_audio_filename;
        QObject* m_parent;
        ProcessOption m_process_option;
    signals:
        void resultReady(const QString&);

    };


    class AudioWaveForm : public QDialog
    {
        QApplication* q_app;

        Q_OBJECT

    public:
        //explicit AudioWaveForm(AudioFile& audio_file, bool generate_wave=false, QDialog* parent = nullptr);
        explicit AudioWaveForm(AudioFile& audio_file,
                               FormMode form_mode=FormMode::New,
                               QDialog* parent = nullptr);

        ~AudioWaveForm() override;

        void show_wave_file(const QString);
        unsigned int audio_len(QString);
        float audio_sample_rate(QString);
        float audio_bitrate(QString audio_file);

        // AudioThread* audio_thread() const;
        void set_button_icons();
        bool is_testing() { return true; }
        bool convert_audio();
        bool create_audio_wave_file(const QString);
        void load_audio_wave_file(const QString);

        QString wave_filename();
        QString ogg_filename(){ return m_ogg_filename; }
        QString audio_filename() { return m_audio_filename; }
        AudioFile audio_file_info() { return m_audio_file; }

        void remove_temp_wave_file();
        void remove_temp_ogg_file();

        void enable_save_button();

        struct AudioImportStatus {
            bool audio_converted_to_ogg{false};
            bool wavefile_is_generated{false};
        };

    public slots:
        void save();
        void on_cancel();

        void start_play();
        void stop_play();
        //void pause_play();

        void audio_current_position(double, double);
        void audio_current_peak(float fft[1024]);
        double seconds_per_pixel();

        std::string base_filename(const std::string filepath);
        QString format_time(double time);

        CueMarker cue_marker() const;

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

        void audio_conversion_done(const QString&);
        void update_conversion_pb();
        void wave_file_generated(const QString&);

    protected:
        void showEvent(QShowEvent* event) override;
        void move_indicator_line(double new_position);
        AUDIO::MarkerDisplayUnit* find_display_unit(MarkerType marker_type);
        void create_marker_display_unit(MarkerIndicator* marker, QLineF line);
        AUDIO::MarkerIndicator* create_marker(AUDIO::MarkerType marker_type);
        void create_marker_indicator(MarkerType, QLineF);
        void show_markers(CueMarker);
        void show_marker_value(CueMarker);
        void show_mark(double mark, MarkerType marker_type);

        AUDIO::MarkerIndicator* create_marker_line(AUDIO::MarkerType marker_type, QLineF line);
        void closeEvent(QCloseEvent*) override;

    private:
        Ui::AudioWaveForm *ui;

        AudioFile m_audio_file;
        double m_seconds_per_pixel;
        AudioWave* m_audio_wave;
        AudioWaveScene* m_scene;

        QString m_wave_filename;
        QString m_ogg_filename;
        QString m_audio_filename;

        std::unique_ptr<QProgressBar> m_conversion_pb;
        std::unique_ptr<QTimer> m_conversion_timer;
        int m_file_toconvert_size=0;  // KB
        int m_progress_counter=0;  // KB

        FormMode m_form_mode;

        // AudioThread* m_audio_thread;
        //QTimer* m_player_timer;
        //QTimer* m_indicator_timer;

        //std::unique_ptr<VuMeter> m_vumeter;

        void init_widgets();

        MarkerIndicator* m_start_marker;
        CueMarker m_cue_marker;

        void save_cue_markers();
        void start_progress_indicator(const QString);
        bool convert_audio_to_ogg(const QString);
        bool generate_audio_wave_file(const QString);

        AUDIO::MarkerIndicator*	m_start_indicator;
        AUDIO::MarkerDisplayUnit* m_start_display_unit;

        AUDIO::MarkerIndicator*	m_fade_in_indicator;
        AUDIO::MarkerDisplayUnit* m_fade_in_display_unit;

        std::map<AUDIO::MarkerType, AUDIO::MarkerDisplayUnit*> m_display_units;

        std::shared_ptr<AUDIO::AudioPlayer> m_audio_player;
        std::unique_ptr<AUDIO::AudioMeter> m_audio_meter;

        // std::unique_ptr<AUDIO::WaveFormWidget> m_wave_form_widget;

        double m_new_pos;

        AudioConversionThread* m_conversion_thread;
        AudioConversionThread* m_ct_wave_generator;

        AudioImportStatus m_audio_import_status;

    };

} // AUDIO

#endif // AUDIOWAVEFORM_H
