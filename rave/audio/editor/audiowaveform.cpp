#include <filesystem>
#include <math.h>
#include <chrono>
#include <thread>

#include <QDebug>
#include <QDir>
#include <QProgressBar>
#include <QTimeLine>
#include <QProcess>
#include <QFileInfo>

#include "../audio/audiometer.h"
#include "../audio/audiowaveformgenerator.h"
#include "../audio/audio.h"
#include "../audio/audiofile.h"
#include "../audio/audioconverter.h"
#include "../audio/mp3oggconverter.h"
#include "../audio/mtstomp3converter.h"
#include "../audio/oggtooggconverter.h"

#include "audioplayer.h"

#include "audiowaveform.h"
#include "ui_audiowaveform.h"

#include "audiothread.h"
#include "../audiotool.h"
//#include "vumeter.h"
#include "../audiometer.h"


namespace fs = std::filesystem;
using namespace std::literals;

namespace AUDIO {

    AudioWaveForm::AudioWaveForm(AudioFile& audio_file,
                             FormMode form_mode,
                             QDialog *parent)
        :QDialog(parent)
        ,ui(new Ui::AudioWaveForm)
        ,m_audio_file{audio_file}
        ,m_audio_filename{QString::fromStdString(audio_file.audio_file())}
        ,m_seconds_per_pixel{0.0}
        ,m_audio_wave{nullptr}
        ,m_scene{nullptr}
        ,m_form_mode{form_mode}
        //,m_audio_thread{nullptr}
        //,m_player_timer{nullptr}
        //,m_indicator_timer{nullptr}
        ,m_new_pos{5.5}

    {
        ui->setupUi(this);

        // double audio_duration_secs = m_audio_player->audio_duration();
        //audio_file.set_duration(audio_duration_secs*1000); //msec
        //audio_file.set_duration(m_audio_file.duration());
        //m_audio_file.set_duration(m_audio_file.duration());

        connect(ui->btnPlay, &QPushButton::clicked, this, &AudioWaveForm::start_play);
        connect(ui->btnStop, &QPushButton::clicked, this, &AudioWaveForm::stop_play);
        // connect(ui->btnPause, &QPushButton::clicked, this, &AudioWaveForm::pause_play);

        connect(ui->btnSave, &QPushButton::clicked, this, &AudioWaveForm::save);
        connect(ui->btnCancel, &QPushButton::clicked, this, &AudioWaveForm::on_cancel);

        connect(ui->btnMarkStartMarker, &QPushButton::clicked, this, &AudioWaveForm::mark_start);
        connect(ui->btnMarkFadeIn, &QPushButton::clicked, this, &AudioWaveForm::mark_fade_in);
        connect(ui->btnMarkIntro, &QPushButton::clicked, this, &AudioWaveForm::mark_intro);
        connect(ui->btnMarkFadeOut, &QPushButton::clicked, this, &AudioWaveForm::mark_fade_out);
        connect(ui->btnMarkExtro, &QPushButton::clicked, this, &AudioWaveForm::mark_extro);
        connect(ui->btnMarkEndMarker, &QPushButton::clicked, this, &AudioWaveForm::mark_end);

        connect(ui->btnPlayStartMarker, &QPushButton::clicked, this, &AudioWaveForm::play_start_mark);
        connect(ui->btnPlayFadeIn, &QPushButton::clicked, this, &AudioWaveForm::play_fade_in);
        connect(ui->btnPlayIntro, &QPushButton::clicked, this, &AudioWaveForm::play_intro);
        connect(ui->btnPlayFadeOut, &QPushButton::clicked, this, &AudioWaveForm::play_fade_out);
        connect(ui->btnPlayExtro, &QPushButton::clicked, this, &AudioWaveForm::play_extro);
        connect(ui->btnPlayEndMarker, &QPushButton::clicked, this, &AudioWaveForm::play_end_marker);

        QString audio_filename = QString::fromStdString(m_audio_file.audio_file());

        ui->btnSave->setEnabled(false);

        if (form_mode == FormMode::New) {
            convert_audio_to_ogg(audio_filename);
            create_audio_wave_file(audio_filename);
        } else {
            m_audio_import_status.audio_converted_to_ogg = true;
            create_audio_wave_file(audio_filename);
        }

        m_audio_player = std::make_shared<AUDIO::AudioPlayer>(audio_file);
        m_audio_meter = std::make_unique<AUDIO::AudioMeter>(m_audio_player);
        connect(m_audio_player.get(), &AUDIO::AudioPlayer::audio_current_position, this, &AudioWaveForm::audio_current_position);
        ui->hlVumeter->addWidget(m_audio_meter.get());

        auto duration = m_audio_player->audio_duration_msec(audio_filename);

        m_audio_file.set_duration(duration);
        audio_file.set_duration(duration);

        m_scene = new AudioWaveScene(ui->lblCurrTime, duration);

        ui->gvWave->setScene(m_scene);
        ui->gvWave->setMinimumWidth(650);
        ui->gvWave->setMinimumHeight(200);
        ui->gvWave->setAlignment(Qt::AlignTop|Qt::AlignLeft);

        setMaximumWidth(this->width());

        init_widgets();
    }


    AudioWaveForm::~AudioWaveForm()
    {
        delete ui;

        //delete m_audio_thread;
        //delete m_player_timer;
        //delete m_indicator_timer;

        delete m_start_indicator;
        delete m_start_display_unit;
        delete m_fade_in_indicator;
        delete m_fade_in_display_unit;

        delete m_scene;

    }

    void AudioWaveForm::load_audio_wave_file(const QString audio_filename)
    {

    }

    void AudioWaveForm::remove_temp_wave_file()
    {
        if (QFileInfo::exists(m_wave_filename)){
            QFile::remove(m_wave_filename);
        }
    }

    void AudioWaveForm::remove_temp_ogg_file()
    {
        if (QFileInfo::exists(m_ogg_filename)) {
            QFile::remove(m_ogg_filename);
        }

    }

    void AudioWaveForm::start_progress_indicator(const QString audio_filename)
    {
        if (m_conversion_timer)
            m_conversion_timer->stop();

        if (m_conversion_pb)
            m_conversion_pb->setValue(0);

        const int KB = 1024;

        QFileInfo fi(audio_filename);
        int byte_size = fi.size();

        if(byte_size >= KB) {
            m_file_toconvert_size = (int)byte_size/KB;
        }

        if (m_file_toconvert_size == 0)
            return;

        std::chrono::milliseconds timer_interval = 70ms;

        m_conversion_pb =  std::make_unique<QProgressBar>(this);
        m_conversion_pb->setRange(0, 100);

        m_conversion_timer = std::make_unique<QTimer>(this);
        m_conversion_timer->setInterval(timer_interval);

        ui->vlWave->addWidget(m_conversion_pb.get());

        connect(m_conversion_timer.get(), &QTimer::timeout, this, &AudioWaveForm::update_conversion_pb);

        m_conversion_timer->start();

    }


    void AudioWaveForm::update_conversion_pb()
    {
        int value =  100 - (((float)(m_file_toconvert_size - m_progress_counter)/ m_file_toconvert_size)*100);

        m_conversion_pb->setValue(value);

        m_progress_counter+=30;

    }

    bool AudioWaveForm::convert_audio_to_ogg(const QString audio_filename)
    {
        start_progress_indicator(audio_filename);

        m_conversion_thread = new AudioConversionThread(audio_filename,
                                                        ProcessOption::FormatConversion,
                                                        this);

        m_conversion_thread->setPriority(QThread::HighPriority);

        connect(m_conversion_thread, &AudioConversionThread::resultReady, this, &AudioWaveForm::audio_conversion_done);
        connect(m_conversion_thread, &AudioConversionThread::finished, m_conversion_thread, &QObject::deleteLater);

        ui->lblStatus->setText("Audio conversion started (mp3 to ogg)...");
        m_conversion_thread->start();

    }

    bool AudioWaveForm::create_audio_wave_file(const QString audio_filename)
    {
        start_progress_indicator(audio_filename);

        m_ct_wave_generator = new AudioConversionThread(audio_filename,
                                                        ProcessOption::GenerateWaveFile,
                                                        this);

        m_ct_wave_generator->setPriority(QThread::LowPriority);
        connect(m_ct_wave_generator, &AudioConversionThread::resultReady, this, &AudioWaveForm::wave_file_generated);
        connect(m_ct_wave_generator, &AudioConversionThread::finished, m_ct_wave_generator, &QObject::deleteLater);

        ui->lblStatus->setText("Generating audio wave...");
        m_ct_wave_generator->start();
    }

    void AudioWaveForm::show_wave_file(const QString wave_filename)
    {
        QPixmap pixmap(wave_filename);
        m_scene->addPixmap(pixmap);
        QRectF scene_bounds = m_scene->itemsBoundingRect();

        scene_bounds.setWidth(scene_bounds.width()*0.9);
        scene_bounds.setHeight(scene_bounds.height()*0.9);
        ui->gvWave->setSceneRect(scene_bounds);

        ui->gvWave->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatioByExpanding);

        double scene_width = scene_bounds.width()+scene_bounds.width()*0.11;

        m_seconds_per_pixel = ((m_audio_file.duration()/1000) *100)/scene_width;

        m_scene->set_sec_per_px(m_seconds_per_pixel);

        ui->gvWave->centerOn(0, 0);
        ui->gvWave->show();

        m_scene->draw_indicator_line();

        m_audio_file.marker().dump_markers();

        show_markers(m_audio_file.marker());
        show_marker_value(m_audio_file.marker());

    }

    unsigned int AudioWaveForm::audio_len(QString audio_file)
    {
        return m_audio_player->audio_length();
        //return m_audio_thread->audio_len(audio_file);
    }

    float AudioWaveForm::audio_bitrate(QString audio_file)
    {
        return m_audio_player->audio_bitrate();
        //return m_audio_thread->audio_bitrate(audio_file);
    }

    // AudioThread *AudioWaveForm::audio_thread() const
    // {
    //     return m_audio_thread;
    // }

    float AudioWaveForm::audio_sample_rate(QString audio_file)
    {
        auto sr = m_audio_player->audio_sample_rate();

        if (sr > 0)
            sr = sr / 1000;

        return sr;
    }

    void AudioWaveForm::save()
    {
        save_cue_markers();
        stop_play();
        done(1);
    }

    void AudioWaveForm::on_cancel()
    {
        done(0);
    }

    void AudioWaveForm::start_play()
    {
        play_mark(m_scene->indicator_position());
    }

    void AudioWaveForm::stop_play()
    {
        m_audio_player->stop_play();

    }

    /*
    void AudioWaveForm::pause_play()
    {
        m_scene->display_marker_position_sec();
    }
   */


    void AudioWaveForm::audio_current_position(double position, double total)
    {
        // move progressbar
        // ui->pbAudio->setValue(value);
        move_indicator_line(position);

    }

    void AudioWaveForm::audio_current_peak(float fft[])
    {
        float peak = 0;
        int i = 0;
        int x = 0;
        int max_peak = 0;

        for (; i < 201; ++i) {
            x = 20*log10(sqrt(fft[i+1])*3*201);
            if (x>201) x=201;
            //m_vumeter->setPeak(x*6);
        }

    }

    double AudioWaveForm::seconds_per_pixel()
    {
        return m_seconds_per_pixel;
    }

    std::string AudioWaveForm::base_filename(const std::string filepath)
    {
        return {std::find_if(filepath.rbegin(), filepath.rend(),
                             [](char c){ return c == '\\'; }).base(),
                    filepath.end()};
    }

    QString AudioWaveForm::format_time(double time)
    {
        // hh:mm:ss:ms
        const int msec_per_second = 1000;
        QString time_format = "%1:%2:%3:%4";
        QString time_str{};

        int msec_dbl = 0;
        int msec = 0;
        int mins = 0;
        int secs = 0;

        msec_dbl = time * msec_per_second;
        msec = msec_dbl % msec_per_second;

        mins = time / 60;
        secs = (int)time % 60;

        time_str = time_format.arg(0)
                .arg(mins, 2, 10, QChar('0'))
                .arg(secs, 2, 10, QChar('0'))
                .arg(msec, 3, 10, QChar('0'));

        return time_str;
    }

    CueMarker AudioWaveForm::cue_marker() const
    {
        return m_cue_marker;
    }

    AUDIO::MarkerIndicator* AudioWaveForm::create_marker(AUDIO::MarkerType marker_type)
    {
        auto line = *m_scene->indicator_line();
        auto marker = m_scene->find_marker(marker_type);

        if (marker != nullptr)
        {
            m_scene->update_marker(marker, line);
            return marker;

        }else{
            auto m = create_marker_line(marker_type, line);
            return m;
        }
    }

    AUDIO::MarkerIndicator* AudioWaveForm::create_marker_line(AUDIO::MarkerType marker_type, QLineF line)
    {
        auto marker = new AUDIO::MarkerIndicator(marker_type);
        create_marker_display_unit(marker, line);
        m_scene->add_marker_line(marker, line);
        return marker;
    }

    void AudioWaveForm::create_marker_display_unit(MarkerIndicator* marker, QLineF line)
    {
        QWidget* display_widget;

        switch(marker->marker_type())
        {
          case MarkerType::StartMarker:
             display_widget = ui->lblStartMarkTime;
             break;
          case MarkerType::FadeInMarker:
             display_widget = ui->lblFadeInMarkTime;
             break;
          case MarkerType::IntroMarker:
             display_widget = ui->lblIntroMarkTime;
             break;
          case MarkerType::FadeOutMarker:
             display_widget = ui->lblFadeOutMarkTime;
             break;
          case MarkerType::ExtroMarker:
             display_widget = ui->lblExtroMarkTime;
             break;
          case MarkerType::EndMarker:
             display_widget = ui->lblEndMarkTime;
             break;
        }

        auto mdu = find_display_unit(marker->marker_type());

        if ( mdu == nullptr)
        {
            mdu = new MarkerDisplayUnit(marker);
            mdu->set_display_unit(display_widget);
            m_display_units[marker->marker_type()] = mdu;
        }
    }

    AUDIO::MarkerDisplayUnit* AudioWaveForm::find_display_unit(MarkerType marker_type)
    {
        std::map<AUDIO::MarkerType, AUDIO::MarkerDisplayUnit*>::iterator it;
        it = m_display_units.find(marker_type);
        return (it != m_display_units.end()) ? (*it).second : nullptr;
    }

    void AudioWaveForm::show_markers(CueMarker cue_markers)
    {
        show_mark(m_scene->msec_to_pixel(cue_markers.start_marker), MarkerType::StartMarker);
        show_mark(m_scene->msec_to_pixel(cue_markers.fade_in), MarkerType::FadeInMarker);
        show_mark(m_scene->msec_to_pixel(cue_markers.intro), MarkerType::IntroMarker);
        show_mark(m_scene->msec_to_pixel(cue_markers.fade_out), MarkerType::FadeOutMarker);
        show_mark(m_scene->msec_to_pixel(cue_markers.extro), MarkerType::ExtroMarker);
        show_mark(m_scene->msec_to_pixel(cue_markers.end_marker), MarkerType::EndMarker);
    }

    void AudioWaveForm::show_mark(double mark, MarkerType marker_type)
    {
        auto y1 = m_scene->indicator_line()->y1();
        auto y2 = m_scene->indicator_line()->y2();
        double x1, x2;
        x1 = x2 = mark;
        auto line = QLineF(x1, y1, x2, y2);

        create_marker_line(marker_type, line);
    }

    void AudioWaveForm::show_marker_value(CueMarker cue_marker)
    {

       AUDIO::AudioTool at;

       ui->lblStartMarkTime->setText(at.format_time(cue_marker.start_marker));
       ui->lblFadeInMarkTime->setText(at.format_time(cue_marker.fade_in));
       ui->lblIntroMarkTime->setText(at.format_time(cue_marker.intro));
       ui->lblFadeOutMarkTime->setText(at.format_time(cue_marker.fade_out));
       ui->lblExtroMarkTime->setText(at.format_time(cue_marker.extro));
       ui->lblEndMarkTime->setText(at.format_time(cue_marker.end_marker));
    }

    void AudioWaveForm::mark_start()
    {
        create_marker(MarkerType::StartMarker);
    }


    void AudioWaveForm::mark_fade_in()
    {
        create_marker(MarkerType::FadeInMarker);
    }

    void AudioWaveForm::mark_intro()
    {
        create_marker(MarkerType::IntroMarker);
    }

    void AudioWaveForm::mark_fade_out()
    {
        create_marker(MarkerType::FadeOutMarker);
    }

    void AudioWaveForm::mark_extro()
    {
        create_marker(MarkerType::ExtroMarker);
    }

    void AudioWaveForm::mark_end()
    {
        constexpr int MSEC = 1000;
        auto marker = create_marker(MarkerType::EndMarker);

        // Constraint EndMarker to the track duration
         if (marker->current_position_msec() > m_audio_file.duration()) {
            marker->set_current_position_msec(m_audio_file.duration());
            marker->set_current_position_sec(m_audio_file.duration()/ 100);
         }

    }

    void AudioWaveForm::play_start_mark()
    {
        play_mark(m_scene->marker_position(MarkerType::StartMarker));
    }

    void AudioWaveForm::play_fade_in()
    {
        play_mark(m_scene->marker_position(MarkerType::FadeInMarker));
    }

    void AudioWaveForm::play_intro()
    {
        play_mark(m_scene->marker_position(MarkerType::IntroMarker));
    }

    void AudioWaveForm::play_fade_out()
    {
        play_mark(m_scene->marker_position(MarkerType::FadeOutMarker));
    }

    void AudioWaveForm::play_extro()
    {
        play_mark(m_scene->marker_position(MarkerType::ExtroMarker));
    }

    void AudioWaveForm::play_end_marker()
    {
        play_mark(m_scene->marker_position(MarkerType::EndMarker));
    }


    void AudioWaveForm::play_mark(QPointF mark_position)
    {
        m_scene->change_indicator_position(mark_position);

        int new_position = mark_position.x();
        m_audio_player->change_position(new_position);
        m_audio_player->play_from_position(new_position);

        //m_audio_thread->change_position(mark_position.x());
        //m_audio_thread->play_from_position(QString::fromStdString(m_audio_file.audio_file()),
        //                                  mark_position.x());
    }

    void AudioWaveForm::showEvent(QShowEvent *event)
    {
        /*
        show_wave_file();
        m_scene->draw_indicator_line();
        show_markers(m_audio_file.marker());
        show_marker_value(m_audio_file.marker());
       */

    }

    void AudioWaveForm::move_indicator_line(double new_position)
    {
        m_scene->move_indicator_line(new_position);
    }

    void AudioWaveForm::init_widgets()
    {
        AUDIO::AudioTool at;
        setWindowTitle("Cue Editor: "+QString::fromStdString(base_filename(m_audio_file.audio_file())));

        ui->lblTitle->setText(QString::fromStdString(m_audio_file.audio_title()));
        ui->lblArtist->setText(QString::fromStdString(m_audio_file.artist_name()));

        ui->lblDuration->setText(at.format_time(m_audio_file.duration()));

        // status bar
        ui->lblFilepath->setText(QString::fromStdString(m_audio_file.audio_file()));
        ui->lblFileDuration->setText(at.format_time(m_audio_file.duration())+" Secs");

        int b_rate = audio_bitrate(QString::fromStdString(m_audio_file.audio_file()));

        ui->lblBitRate->setText(QString::number(b_rate)+" bits/s");

        float sample_rate = audio_sample_rate(QString::fromStdString(m_audio_file.audio_file()));

        ui->lblSampleRate->setText(QString::number(sample_rate)+" KHz");

        ui->lblFileSize->setText(QString::number(m_audio_file.file_size()/1000)+" Kb");

        m_start_indicator = new AUDIO::MarkerIndicator(MarkerType::StartMarker);
        m_start_display_unit = new MarkerDisplayUnit(m_start_indicator);
        m_start_display_unit->set_display_unit(ui->lblStartMarkTime);

        m_fade_in_indicator = new AUDIO::MarkerIndicator(MarkerType::FadeInMarker);
        m_fade_in_display_unit = new MarkerDisplayUnit(m_fade_in_indicator);
        m_fade_in_display_unit->set_display_unit(ui->lblFadeInMarkTime);

        // m_vumeter = std::make_unique<VuMeter>(VuMeter::meter_dir::DIR_HORIZONTAL, this);
        // ui->hlVumeter->addWidget(m_vumeter.get());

        set_button_icons();

    }

    void AudioWaveForm::set_button_icons()
    {
        ui->btnPlay->setIcon(QIcon(":/media/editor/media/editor/play_green.png"));
        // ui->btnPause->setIcon(QIcon(":/media/editor/media/editor/pause_play.png"));
        ui->btnStop->setIcon(QIcon(":/media/editor/media/editor/stop_play.png"));
        ui->btnMarkStartMarker->setIcon(QIcon(":/media/editor/media/editor/mark_start.png"));
        ui->btnPlayStartMarker->setIcon(QIcon(":/media/editor/media/editor/play_mark_start.png"));
        ui->btnMarkFadeIn->setIcon(QIcon(":/media/editor/media/editor/mark_fade_in.png"));
        ui->btnPlayFadeIn->setIcon(QIcon(":/media/editor/media/editor/play_fade_in.png"));
        ui->btnMarkIntro->setIcon(QIcon(":/media/editor/media/editor/mark_intro.png"));
        ui->btnPlayIntro->setIcon(QIcon(":/media/editor/media/editor/play_intro.png"));
        ui->btnMarkFadeOut->setIcon(QIcon(":/media/editor/media/editor/mark_fade_out.png"));
        ui->btnPlayFadeOut->setIcon(QIcon(":/media/editor/media/editor/play_fade_out.png"));
        ui->btnMarkExtro->setIcon(QIcon(":/media/editor/media/editor/mark_extro.png"));
        ui->btnPlayExtro->setIcon(QIcon(":/media/editor/media/editor/play_extro.png"));
        ui->btnMarkEndMarker->setIcon(QIcon(":/media/editor/media/editor/mark_end.png"));
        ui->btnPlayEndMarker->setIcon(QIcon(":/media/editor/media/editor/play_end.png"));

    }

    void AudioWaveForm::save_cue_markers()
    {
        qDebug() << "m_scene->markers(): "<< m_scene->markers().size();

        for (auto& [type, marker] : m_scene->markers()){
            switch (type){
             case AUDIO::MarkerType::StartMarker:
                m_cue_marker.start_marker  = marker->current_position_msec();
                break;
             case AUDIO::MarkerType::FadeInMarker:
                m_cue_marker.fade_in  = marker->current_position_msec();
                break;
             case AUDIO::MarkerType::IntroMarker:
                m_cue_marker.intro  = marker->current_position_msec();
                break;
             case AUDIO::MarkerType::ExtroMarker:
                m_cue_marker.extro  = marker->current_position_msec();
                break;
             case AUDIO::MarkerType::FadeOutMarker:
                m_cue_marker.fade_out  = marker->current_position_msec();
                break;
             case AUDIO::MarkerType::EndMarker:
                m_cue_marker.end_marker  = marker->current_position_msec();
                break;
            }
        }

    }

    void AudioWaveForm::closeEvent(QCloseEvent*)
    {
        m_audio_player->stop_play();

        /*
        if (m_audio_thread != nullptr){
            m_audio_thread->stop();
            m_player_timer->stop();
        }
        */

    }

    bool AudioWaveForm::convert_audio()
    {

        QString m_audio_filename = QString::fromStdString(m_audio_file.audio_file());

        QFileInfo fi(m_audio_filename);
        auto file_ext = fi.suffix(); //m_audio->file_extension()->value();

        auto proc = std::make_shared<QProcess>(this);

        if (file_ext == "mp3"){
        // TODO: Do we really need to convert MP3 to OGG, yet we can play MP3 audio just fine?

            auto ac = std::make_unique<AUDIO::Mp3ToOggConverter>(m_audio_filename, proc);
            if (!ac->convert()){
                //clean-up
                return false;
            }

        }

        if (file_ext == "ogg"){
            auto ac = std::make_unique<AUDIO::OggToOggConverter>(m_audio_filename);
            if (!ac->convert()){
                // clean-up
                return false;
            }
        }

        if (file_ext == "mts"){
            auto ac = std::make_unique<AUDIO::MtsToMp3Converter>(m_audio_filename);
            ac->convert();
            auto mp3_file = dynamic_cast<AUDIO::MtsToMp3Converter*>(ac.get());
            auto audio_file = mp3_file->dest_mp3_filename();

            // TODO: Refactor this line since we no longer have Audio object
            // m_audio->audio_file().set_audio_filename(audio_file.toStdString());
        }

        return true;

    }

    void AudioWaveForm::enable_save_button()
    {
        ui->btnSave->setEnabled(
            (m_audio_import_status.audio_converted_to_ogg && m_audio_import_status.wavefile_is_generated)
            );
    }

    void AudioWaveForm::audio_conversion_done(const QString& ogg_filename)
    {
        dbgln("audio_conversion_done:: >> "+ogg_filename);
        m_ogg_filename = ogg_filename;

        ui->lblStatus->setText("Audio conversion done.");
        m_conversion_timer->stop();
        m_conversion_pb->setValue(100);

        m_audio_import_status.audio_converted_to_ogg = true;
        enable_save_button();
    }

    void AudioWaveForm::wave_file_generated(const QString& wave_filename)
    {
        m_wave_filename = wave_filename;

        ui->lblStatus->setText("Audio wave generated.");
        m_conversion_timer->stop();
        m_conversion_pb->setValue(100);

        show_wave_file(wave_filename);

        m_audio_import_status.wavefile_is_generated = true;
        enable_save_button();

    }

    QString AudioWaveForm::wave_filename()
    {
        return m_wave_filename;
    }


    AudioConversionThread::AudioConversionThread(const QString audio_filename,
                                                 ProcessOption process_option,
                                         QObject* parent)
        :QThread(parent)
        ,m_audio_filename{audio_filename}
        ,m_process_option{process_option}
        ,m_parent(parent)
    {}

    QString AudioConversionThread::convert_audio(const QString audio_filename)
    {
        QString converted_audio = audio_filename;

        QFileInfo fi(audio_filename);
        auto file_ext = fi.suffix(); //m_audio->file_extension()->value();

        // TODO: Find a parent for this process
        auto proc = std::make_shared<QProcess>();

        if (file_ext == "mp3"){
        // TODO: Do we really need to convert MP3 to OGG, yet we can play MP3 audio just fine?

            auto ac = std::make_unique<AUDIO::Mp3ToOggConverter>(audio_filename, proc);

            if (ac->convert()){
                converted_audio =  ac->ogg_filename();
            }

        }

        if (file_ext == "ogg"){
            auto ac = std::make_unique<AUDIO::OggToOggConverter>(audio_filename);
            if (ac->convert()){
                converted_audio = ac->ogg_filename();
            }
        }

        if (file_ext == "mts"){
            auto ac = std::make_unique<AUDIO::MtsToMp3Converter>(audio_filename);
            ac->convert();
            auto mp3_file = dynamic_cast<AUDIO::MtsToMp3Converter*>(ac.get());
            auto audio_file = mp3_file->dest_mp3_filename();

            converted_audio = ac->ogg_filename();

            // TODO: Refactor this line since we no longer have Audio object
            // m_audio->audio_file().set_audio_filename(audio_file.toStdString());
        }


        return converted_audio;

    }

    QString AudioConversionThread::generate_audio_wave_file(const QString audio_filename)
    {
        qDebug() << "generate_audio_wave_file...";

        auto wave_gen = std::make_unique<AUDIO::AudioWaveFormGenerator>(audio_filename);
        wave_gen->generate();

        qDebug() << "* File Generated *";

        return wave_gen->wave_filename();
    }

    // WorkerThread
    void AudioConversionThread::run()
    {
        switch (m_process_option)
        {
        case ProcessOption::FormatConversion:
        {
           QString ogg_filename = convert_audio(m_audio_filename);
            dbgln("THREAD --> 01");
            emit resultReady(ogg_filename);
           break;
        }

        case ProcessOption::GenerateWaveFile:
        {
            qDebug() << "GenerateWaveFile" ;
           // Generate wave file
            QString wave_filename = generate_audio_wave_file(m_audio_filename);
            // std::this_thread::sleep_for(std::chrono::seconds(5));
            dbgln("THREAD --> 02");
            emit resultReady(wave_filename);
            break;
        }

        }
    }


}
