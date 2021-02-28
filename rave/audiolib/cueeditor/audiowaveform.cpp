#include <math.h>
#include <QDebug>
#include "audiowaveform.h"
#include "ui_audiowaveform.h"
#include "audiothread.h"


namespace AUDIO {

    AudioWaveForm::AudioWaveForm(AudioFile audio_file, QDialog *parent) :
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

        m_scene = new AudioWaveScene(ui->lblCurrTime);
        ui->gvWave->setScene(m_scene);
        ui->gvWave->setMaximumWidth(800);
        ui->gvWave->setAlignment(Qt::AlignTop|Qt::AlignLeft);

        // Audiothread
        m_audio_thread = new AudioThread(this);
        connect(m_audio_thread, SIGNAL(current_position(double, double)), this, SLOT(audio_current_position(double, double)));
        connect(m_audio_thread, SIGNAL(current_peak(float[1024])), this, SLOT(audio_current_peak(float[1024])));

        m_player_timer = new QTimer(this);
        connect(m_player_timer, &QTimer::timeout, this, &AudioWaveForm::update_vumeter);

        const int audio_sample_msec = 100;
        m_indicator_timer = new QTimer(this);
        m_indicator_timer->setInterval(audio_sample_msec);
        connect(m_indicator_timer, &QTimer::timeout, this, &AudioWaveForm::update_indicator);

        connect(ui->btnPlay, &QPushButton::clicked, this, &AudioWaveForm::on_play);
        connect(ui->btnStop, &QPushButton::clicked, this, &AudioWaveForm::stop_play);

        connect(ui->btnSave, &QPushButton::clicked, this, &AudioWaveForm::on_save);
        connect(ui->btnCancel, &QPushButton::clicked, this, &AudioWaveForm::on_cancel);

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
        QPixmap pixmap(QString::fromStdString(m_audio_file.wave_file()));
        m_scene->addPixmap(pixmap);
        QRectF scene_bounds = m_scene->itemsBoundingRect();

        //qreal top = scene_bounds.top();
        //qreal bottom = scene_bounds.bottomLeft().ry();

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

    void AudioWaveForm::on_save()
    {
        done(1);
    }

    void AudioWaveForm::on_cancel()
    {
        done(0);
    }

    void AudioWaveForm::on_play()
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

    void AudioWaveForm::showEvent(QShowEvent *event)
    {
        show_wave_file();
        m_scene->draw_indicator_line();
    }

    void AudioWaveForm::move_indicator_line(double new_position)
    {
        m_scene->move_indicator_line(new_position);
    }

}
