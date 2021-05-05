#include <filesystem>
#include <math.h>

#include <QDebug>
#include <QDir>
#include "audiowaveform.h"
#include "ui_audiowaveform.h"

#include "audiothread.h"
#include "../../audio/audiotool.h"

namespace fs = std::filesystem;

namespace AUDIO {

    AudioWaveForm::AudioWaveForm(AudioFile& audio_file, QDialog *parent) :
        QDialog(parent),
        ui(new Ui::AudioWaveForm),
        m_audio_file{audio_file},
        m_seconds_per_pixel{0.0},
        m_audio_wave{nullptr},
        m_scene{nullptr},
        m_audio_thread{nullptr},
        m_player_timer{nullptr},
        m_indicator_timer{nullptr}
    {
        ui->setupUi(this);

        m_scene = new AudioWaveScene(ui->lblCurrTime, audio_file.duration());
        ui->gvWave->setScene(m_scene);
        ui->gvWave->setMaximumWidth(800);
        ui->gvWave->setAlignment(Qt::AlignTop|Qt::AlignLeft);

        // Audiothread
        m_audio_thread = new AudioThread(this);

        m_audio_file.set_duration(m_audio_thread->audio_len(QString::fromStdString(m_audio_file.audio_file())));

        connect(m_audio_thread, SIGNAL(current_position(double, double)), this, SLOT(audio_current_position(double, double)));
        connect(m_audio_thread, SIGNAL(current_peak(float[1024])), this, SLOT(audio_current_peak(float[1024])));

        m_player_timer = new QTimer(this);
        connect(m_player_timer, &QTimer::timeout, this, &AudioWaveForm::update_vumeter);

        const int audio_sample_msec = 100;
        m_indicator_timer = new QTimer(this);
        m_indicator_timer->setInterval(audio_sample_msec);
        connect(m_indicator_timer, &QTimer::timeout, this, &AudioWaveForm::update_indicator);

        connect(ui->btnPlay, &QPushButton::clicked, this, &AudioWaveForm::start_play);
        connect(ui->btnStop, &QPushButton::clicked, this, &AudioWaveForm::stop_play);
        connect(ui->btnPause, &QPushButton::clicked, this, &AudioWaveForm::pause_play);

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

        init_widgets();

    }

    AudioWaveForm::~AudioWaveForm()
    {
        delete ui;
        delete m_audio_thread;
        delete m_player_timer;
        delete m_indicator_timer;
    }

    void AudioWaveForm::show_wave_file()
    {
        // FIXME:
        QPixmap pixmap(QString::fromStdString(m_audio_file.wave_file()));
        m_scene->addPixmap(pixmap);
        QRectF scene_bounds = m_scene->itemsBoundingRect();

        scene_bounds.setWidth(scene_bounds.width()*0.9);
        scene_bounds.setHeight(scene_bounds.height()*0.9);
        //ui->gvWave->fitInView(scene_bounds, Qt::KeepAspectRatio);
        ui->gvWave->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);

        ui->gvWave->setSceneRect(scene_bounds);

        double scene_width = scene_bounds.width()+scene_bounds.width()*0.11;
        //ui->gvWave->setMaximumWidth(scene_width+20);

        m_seconds_per_pixel = (m_audio_file.duration() *100)/scene_width;

        m_scene->set_sec_per_px(m_seconds_per_pixel);

        ui->gvWave->centerOn(0, 0);
        ui->gvWave->show();

    }

    unsigned int AudioWaveForm::audio_len(QString audio_file)
    {
        return m_audio_thread->audio_len(audio_file);
    }

    float AudioWaveForm::audio_bitrate(QString audio_file)
    {
        return m_audio_thread->audio_bitrate(audio_file);
    }

    float AudioWaveForm::audio_sample_rate(QString audio_file)
    {
        auto sr = m_audio_thread->audio_sample_rate(audio_file);
        if (sr > 0)
            sr = sr / 1000;
        return sr;
    }

    void AudioWaveForm::save()
    {
        save_markers();

        const std::string MP3 = ".mp3";
        const std::string OGG = ".ogg";

        auto q_str = QDir::toNativeSeparators(QString::fromStdString(m_audio_file.ogg_filename()));
//        m_audio_file.set_ogg_filename(q_str.toStdString());
        fs::path p(q_str.toStdString());

        if ((m_audio_file.file_ext() == MP3) && (!fs::exists(p)) ){
            AudioTool audio_tool;
            std::string ogg_file = audio_tool.mp3_to_ogg(m_audio_file);
            if (!ogg_file.empty())
                m_audio_file.set_ogg_filename(ogg_file);
            //	std::string audio_lib = m_audio_file.get_audio_lib();
            //	audio_tool.copy_ogg_to_audiolib(ogg_file, std::string dest_ogg)

        }

//            if (m_audio_file.file_ext() == OGG){
//                AudioTool audio_tool;
//                if (!audio_tool.copy_file_to_audiolib()){
//                    //audio_tool.copy_file_to_audiolib(file_name, audio_lib);
//                }

//            }

        done(1);
    }

    void AudioWaveForm::on_cancel()
    {
        done(0);
    }

    void AudioWaveForm::start_play()
    {
        m_audio_thread->play(QString::fromStdString(m_audio_file.audio_file()));
        //ui->pbAudio->setMaximum(100);
        //on_set_peak();
    }

    void AudioWaveForm::stop_play()
    {

        if (m_audio_thread != nullptr){
            m_audio_thread->stop();
            //stop_indicator_move();
            m_player_timer->stop();
        }

    }

    void AudioWaveForm::pause_play()
    {
        m_scene->display_marker_position_sec();
    }

    void AudioWaveForm::update_vumeter()
    {

    }

    void AudioWaveForm::update_indicator()
    {

    }

    void AudioWaveForm::audio_current_position(double position, double total)
    {
        int value = 100 - round(((total-position)/total)*100);
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
        for (; i < 201; ++i){
            x = 20*log10(sqrt(fft[i+1])*3*201);
            if (x>201) x=201;
            //m_vumeter->set_peak(x*6);
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

    Marker AudioWaveForm::marker() const
    {
        return m_marker;
    }

    void AudioWaveForm::create_marker_line(AUDIO::MarkerType marker_type)
    {
        auto line = *m_scene->indicator_line();
        auto marker = m_scene->find_marker(marker_type);
        if (marker != nullptr)
        {
            m_scene->update_marker(marker, line);

        }else{
            create_marker_line(marker_type, line);
        }
    }

    void AudioWaveForm::create_marker_line(AUDIO::MarkerType marker_type, QLineF line)
    {
        auto marker = new AUDIO::MarkerIndicator(marker_type);
        create_marker_display_unit(marker, line);
        m_scene->add_marker_line(marker, line);
    }

    void AudioWaveForm::create_marker_display_unit(MarkerIndicator* marker, QLineF line)
    {
        QWidget* display_widget;
        switch(marker->marker_type())
        {
          case MarkerType::Start:
             display_widget = ui->lblStartMarkTime;
             break;
          case MarkerType::FadeIn:
             display_widget = ui->lblFadeInMarkTime;
             break;
          case MarkerType::Intro:
             display_widget = ui->lblIntroMarkTime;
             break;
          case MarkerType::FadeOut:
             display_widget = ui->lblFadeOutMarkTime;
             break;
          case MarkerType::Extro:
             display_widget = ui->lblExtroMarkTime;
             break;
          case MarkerType::End:
             display_widget = ui->lblEndMarkTime;
             break;
        }

        auto mdu = find_display_unit(marker->marker_type());
        if ( mdu == nullptr){
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

    void AudioWaveForm::show_markers(Marker markers)
    {

        show_mark(m_scene->seconds_to_pixel(markers.start_marker), MarkerType::Start);
        show_mark(m_scene->seconds_to_pixel(markers.fade_in), MarkerType::FadeIn);
        show_mark(m_scene->seconds_to_pixel(markers.intro), MarkerType::Intro);
        show_mark(m_scene->seconds_to_pixel(markers.fade_out), MarkerType::FadeOut);
        show_mark(m_scene->seconds_to_pixel(markers.extro), MarkerType::Extro);
        show_mark(m_scene->seconds_to_pixel(markers.end_marker), MarkerType::End);
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

    void AudioWaveForm::show_marker_value(Marker marker)
    {
       ui->lblStartMarkTime->setText(QString::fromStdString(std::to_string(marker.start_marker)));
       ui->lblFadeInMarkTime->setText(QString::fromStdString(std::to_string(marker.fade_in)));
       ui->lblIntroMarkTime->setText(QString::fromStdString(std::to_string(marker.intro)));
       ui->lblFadeOutMarkTime->setText(QString::fromStdString(std::to_string(marker.fade_out)));
       ui->lblExtroMarkTime->setText(QString::fromStdString(std::to_string(marker.extro)));
       ui->lblEndMarkTime->setText(QString::fromStdString(std::to_string(marker.end_marker)));
    }

    void AudioWaveForm::mark_start()
    {
        create_marker_line(MarkerType::Start);
    }


    void AudioWaveForm::mark_fade_in()
    {
        create_marker_line(MarkerType::FadeIn);
    }

    void AudioWaveForm::mark_intro()
    {
        create_marker_line(MarkerType::Intro);
    }

    void AudioWaveForm::mark_fade_out()
    {
        create_marker_line(MarkerType::FadeOut);
    }

    void AudioWaveForm::mark_extro()
    {
        create_marker_line(MarkerType::Extro);
    }

    void AudioWaveForm::mark_end()
    {
        create_marker_line(MarkerType::End);
    }

    void AudioWaveForm::play_start_mark()
    {
        play_mark(m_scene->marker_position(MarkerType::Start));
    }

    void AudioWaveForm::play_fade_in()
    {
        play_mark(m_scene->marker_position(MarkerType::FadeIn));
    }

    void AudioWaveForm::play_intro()
    {
        play_mark(m_scene->marker_position(MarkerType::Intro));
    }

    void AudioWaveForm::play_fade_out()
    {
        play_mark(m_scene->marker_position(MarkerType::FadeOut));
    }

    void AudioWaveForm::play_extro()
    {
        play_mark(m_scene->marker_position(MarkerType::Extro));
    }

    void AudioWaveForm::play_end_marker()
    {
        play_mark(m_scene->marker_position(MarkerType::End));
    }


    void AudioWaveForm::play_mark(QPointF mark_position)
    {
        m_scene->change_indicator_position(mark_position);
        m_audio_thread->change_position(mark_position.x());
        m_audio_thread->play_from_position(QString::fromStdString(m_audio_file.audio_file()),
                                           mark_position.x());
    }

    void AudioWaveForm::showEvent(QShowEvent *event)
    {
        show_wave_file();
        m_scene->draw_indicator_line();
        show_markers(m_audio_file.marker());
        show_marker_value(m_audio_file.marker());
    }

    void AudioWaveForm::move_indicator_line(double new_position)
    {
        m_scene->move_indicator_line(new_position);
    }

    void AudioWaveForm::init_widgets()
    {
        setWindowTitle("Cue Editor: "+QString::fromStdString(base_filename(m_audio_file.audio_file())));
        ui->lblTitle->setText(QString::fromStdString(m_audio_file.audio_title()));
        ui->lblArtist->setText(QString::fromStdString(m_audio_file.artist_name()));

        ui->lblDuration->setText(format_time(m_audio_file.duration()));

        // status bar
        ui->lblFilepath->setText(QString::fromStdString(m_audio_file.audio_file()));
        ui->lblFileDuration->setText(format_time(m_audio_file.duration())+" Secs");

        int b_rate = audio_bitrate(QString::fromStdString(m_audio_file.audio_file()));

        ui->lblBitRate->setText(QString::number(b_rate)+" bits/s");

        float sample_rate = audio_sample_rate(QString::fromStdString(m_audio_file.audio_file()));

        ui->lblSampleRate->setText(QString::number(sample_rate)+" KHz");

        ui->lblFileSize->setText(QString::number(m_audio_file.file_size()/1000)+" Kb");

        m_start_indicator = new AUDIO::MarkerIndicator(MarkerType::Start);
        m_start_display_unit = new MarkerDisplayUnit(m_start_indicator);
        m_start_display_unit->set_display_unit(ui->lblStartMarkTime);

        m_fade_in_indicator = new AUDIO::MarkerIndicator(MarkerType::FadeIn);
        m_fade_in_display_unit = new MarkerDisplayUnit(m_fade_in_indicator);
        m_fade_in_display_unit->set_display_unit(ui->lblFadeInMarkTime);

    }

    void AudioWaveForm::save_markers()
    {

        for (auto& [type, marker] : m_scene->markers()){
            switch (type){
             case AUDIO::MarkerType::Start:
                m_marker.start_marker  = marker->current_position_sec();
                break;
             case AUDIO::MarkerType::FadeIn:
                m_marker.fade_in  = marker->current_position_sec();
                break;
             case AUDIO::MarkerType::Intro:
                m_marker.intro  = marker->current_position_sec();
                break;
             case AUDIO::MarkerType::Extro:
                m_marker.extro  = marker->current_position_sec();
                break;
             case AUDIO::MarkerType::FadeOut:
                m_marker.fade_out  = marker->current_position_sec();
                break;
             case AUDIO::MarkerType::End:
                m_marker.end_marker  = marker->current_position_sec();
                break;
            }
        }

    }

}
