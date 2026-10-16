#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>

#include "audio.h"
#include "audiopropertywidget.h"
#include "audiofile.h"
#include "audiotool.h"
#include "audiometer.h"
#include "editor/audioplayer.h"

namespace AUDIO
{
    // AudioToolBar class
    AudioToolBar::AudioToolBar()
    {
        setupui();
    }

    void AudioToolBar::setupui()
    {
        QHBoxLayout* btnLayout = new QHBoxLayout();

        m_btn_play = new QPushButton("Play");
        m_btn_stop = new QPushButton("Stop");
        m_btn_cue_edit = new QPushButton("Cue Edit");

        QFrame* vline = new QFrame(this);
        vline->setFrameShape(QFrame::VLine);
        vline->setFrameShadow(QFrame::Sunken);

        // Signals
        connect(m_btn_play, &QPushButton::clicked, this, [&](){emit play_audio(); });
        connect(m_btn_stop, &QPushButton::clicked, this, [&](){emit stop_audio(); });
        connect(m_btn_cue_edit, &QPushButton::clicked, this, [&](){emit cue_edit(); });

        btnLayout->addStretch(1);
        btnLayout->addWidget(m_btn_play);
        btnLayout->addWidget(m_btn_stop);
        btnLayout->addSpacing(10);
        btnLayout->addWidget(vline);
        btnLayout->addWidget(m_btn_cue_edit);

        setLayout(btnLayout);
    }

    /* --- AudioProperty class ---- */

    AudioProperty::AudioProperty(std::shared_ptr<Audio> audio)
        :m_audio{audio}
    {
        setupui();
    }

    void AudioProperty::setupui()
    {
        // Audio widget
        QHBoxLayout* main_layout = new QHBoxLayout;

        QGridLayout* audio_layout = new QGridLayout();

        QLabel* lblAudioRef = new QLabel("Audio Id:");
        m_lblAudioRefValue = new QLabel(QString::number(m_audio->id()));

        QLabel* lblTitle = new QLabel("Audio Title:");
        m_lblTitleValue = new QLabel(QString::fromStdString(m_audio->title()->value()));

        QLabel* lblFilePath = new QLabel("Audio File Path:");
        m_lblFilePathValue = new QLabel(QString::fromStdString(m_audio->file_path()->value()));

        AudioTool at;
        QLabel* lblDuration = new QLabel("Duration:");
        m_lblDurationValue = new QLabel(at.format_time_hr_to_ms(m_audio->duration()->value()));

        audio_layout->addWidget(lblAudioRef, 0, 0);
        audio_layout->addWidget(m_lblAudioRefValue, 0, 1);
        audio_layout->addWidget(lblTitle, 1, 0);
        audio_layout->addWidget(m_lblTitleValue, 1, 1);
        audio_layout->addWidget(lblFilePath, 2, 0);
        audio_layout->addWidget(m_lblFilePathValue, 2, 1);
        audio_layout->addWidget(lblDuration, 3, 0);
        audio_layout->addWidget(m_lblDurationValue, 3, 1);

        main_layout->addLayout(audio_layout);
        main_layout->addStretch();

        setLayout(main_layout);

    }

    void AudioProperty::update_properties(std::shared_ptr<Audio> audio)
    {
        m_lblAudioRefValue->setText(QString::number(audio->id()));
        m_lblTitleValue->setText(QString::fromStdString(audio->title()->value()));
        m_lblFilePathValue->setText(QString::fromStdString(audio->file_path()->value()));

        AudioTool at;
        m_lblDurationValue->setText(at.format_time_hr_to_ms(audio->duration()->value()));
    }


    AudioPropertyWidget::AudioPropertyWidget(std::shared_ptr<Audio> audio)
       :m_audio{audio}
       ,m_audio_player{nullptr}
       ,m_audio_meter{nullptr}
       ,m_audio_prop{nullptr}
    {
        setupui();

    }

    void AudioPropertyWidget::setupui()
    {
        QVBoxLayout* main_layout = new QVBoxLayout();

        AudioToolBar* toolbar = new AudioToolBar();
        connect(toolbar, &AudioToolBar::play_audio, this, &AudioPropertyWidget::on_play_audio);
        connect(toolbar, &AudioToolBar::stop_audio, this, &AudioPropertyWidget::on_stop_audio);
        connect(toolbar, &AudioToolBar::cue_edit, this, [this]{emit cue_edit_audio(); });

        m_audio_prop = new AudioProperty(m_audio);

        QGroupBox* grp_box = new QGroupBox("Audio:");
        QVBoxLayout* grp_layout = new QVBoxLayout;

        AudioFile af;
        m_audio_player = std::make_shared<AudioPlayer>(af);
        m_audio_meter = std::make_shared<AudioMeter>(m_audio_player) ;

        QFrame* hline = new QFrame(this);
        hline->setFrameShape(QFrame::HLine);
        hline->setFrameShadow(QFrame::Sunken);

        grp_layout->addWidget(toolbar);
        grp_layout->addWidget(m_audio_prop);
        grp_layout->addWidget(hline);
        grp_layout->addWidget(m_audio_meter.get());
        grp_box->setLayout(grp_layout);

        main_layout->addWidget(grp_box);

        setLayout(main_layout);

    }

    void AudioPropertyWidget::on_play_audio()
    {
        if (m_audio->id() == -1) {
            m_audio_player->play_audio(QString::fromStdString(m_audio->source_filepath()->value()));
        } else {
            m_audio_player->play_audio(m_audio->full_audio_filename());
        }
    }

    void AudioPropertyWidget::on_stop_audio()
    {
        m_audio_player->stop_play();

    }

    void AudioPropertyWidget::update_widget(std::shared_ptr<Audio> audio)
    {
        m_audio = audio;
        m_audio_prop->update_properties(audio);

    }

}
