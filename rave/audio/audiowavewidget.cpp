#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QMessageBox>

#include "audiowavewidget.h"
#include "CountdownLabel.h"

namespace AUDIO {

// ToolBar
WaveToolBar::WaveToolBar()
{
    setupui();

}

void WaveToolBar::setupui()
{
    QVBoxLayout* main_layout = new QVBoxLayout();

    auto* btnPlay = new QPushButton("Play/Pause");
    auto* btnStop = new QPushButton("Stop");
    auto* btnPlaySel = new QPushButton("Play Selection");
    auto* cbLoop = new QCheckBox("Loop");
    auto* btnZin  = new QPushButton("Zoom +");
    auto* btnZout = new QPushButton("Zoom -");
    auto* btnFit = new QPushButton("Fit");
    auto* btnZsel = new QPushButton("Zoom to Selection");
    auto* btnHl = new QPushButton("Highlight Selection");
    auto* btnClear = new QPushButton("Clear Hightlights");

    auto* cbScale = new QComboBox;
    cbScale->addItems({"Linear", "Decibel"});

    auto* cbRms = new QCheckBox("RMS");
    cbRms->setChecked(true);

    auto* btnWaveColor = new QPushButton("Wave colour...");
    auto* btnBgColor = new QPushButton("Background...");
    auto* btnHeadColor = new QPushButton("Playhead...");
    auto* btnSelColor = new QPushButton("Selection...");

    connect(btnPlay, &QPushButton::clicked, this, [&] { emit play_pause_audio(); } );
    connect(btnStop, &QPushButton::clicked, this, [&] { emit stop_audio(); });
    connect(btnPlaySel, &QPushButton::clicked, this, [&] { emit play_sel(); });
    connect(cbLoop, &QCheckBox::toggled, this, [&](bool on) { emit set_loop_play(on); });
    connect(btnZin, &QPushButton::clicked, this, [&] { emit zoom_in(); });
    connect(btnZout, &QPushButton::clicked, this, [&] { emit zoom_out(); });
    connect(btnFit, &QPushButton::clicked, this, [&] { emit zoom_fit(); });
    connect(btnZsel, &QPushButton::clicked, this, [&] { emit zoom_sel(); });
    connect(btnHl, &QPushButton::clicked, this, [&] { emit highlight_wave(); });
    connect(btnClear, &QPushButton::clicked, this, [&] { emit clear_highlight(); });
    connect(cbScale, &QComboBox::currentIndexChanged, this, [&](int i){ emit scale_wave(i); });

    QHBoxLayout* hl_row1 = new QHBoxLayout;

    for (QWidget *w : {(QWidget *)btnPlay, (QWidget *)btnStop, (QWidget *)btnPlaySel, (QWidget *)cbLoop,
                       (QWidget *)btnZin , (QWidget *)btnZout, (QWidget *)btnFit, (QWidget *)btnZsel, (QWidget *)btnHl,
                       (QWidget *)btnClear})
        hl_row1->addWidget(w);

    QHBoxLayout* hl_row2 = new QHBoxLayout;
    hl_row2->addWidget(new QLabel("Scale:"));

    for (QWidget* w :{(QWidget *)cbScale, (QWidget *)cbRms, (QWidget *)btnWaveColor, (QWidget *)btnBgColor,
                       (QWidget *)btnHeadColor, (QWidget *)btnSelColor})
        hl_row2->addWidget(w);
    hl_row2->addStretch();

    main_layout->addLayout(hl_row1);
    main_layout->addLayout(hl_row2);

    // ----- Marker controls ---------------
    QHBoxLayout* hl_markers = new QHBoxLayout();
    hl_markers->addWidget(new QLabel("Audio Place Markers:"));

    for (int i = 0; i < WaveformWidget::MarkerCount; ++i) {
        const auto type = static_cast<WaveformWidget::MarkerType>(i);
        auto* btn = new QPushButton(WaveformWidget::markerName(type));
        connect(btn, &QPushButton::clicked, this, [this, i]{ emit arm_marker(i); });
        hl_markers->addWidget(btn);
    }
    main_layout->addLayout(hl_markers);

    setLayout(main_layout);

}



AudioWaveWidget::AudioWaveWidget(QString audio_filepath)
{
    m_audio_filepath = audio_filepath;
    setupui();
}

void AudioWaveWidget::setupui()
{
    QVBoxLayout* main_layout = new QVBoxLayout;
    WaveToolBar* tb = new WaveToolBar();

    m_wf = new WaveformWidget;

    connect(tb, &WaveToolBar::arm_marker, this, [this](int marker){
        const auto type = static_cast<WaveformWidget::MarkerType>(marker);
        m_wf->armMarker(type);
    });

    // Marker Information
    // TODO: This need to be inside the WaveToolBar Widdget
    auto* marker_info = new QLabel;
    marker_info->setTextInteractionFlags(Qt::TextSelectableByMouse);

    auto refresh_markers = [this, marker_info]{
        QStringList parts;
        for(int i = 0; i < WaveformWidget::MarkerCount; ++i) {
            const auto type = static_cast<WaveformWidget::MarkerType>(i);
            parts << QString("%1 : %2").arg(WaveformWidget::markerName(type),
                                            m_wf->hasMarker(type) ? m_wf->markerTimeString(type) : QString("--"));
        }
        marker_info->setText(parts.join("   |   "));
    };

    // QHBoxLayout* marker_layout = new QHBoxLayout;
    // marker_layout->addWidget(marker_info);
    // marker_layout->setContentsMargins(50,0,0,0);

    refresh_markers();

    connect(tb, &WaveToolBar::play_pause_audio, m_wf, &WaveformWidget::togglePlayPause);
    connect(tb, &WaveToolBar::stop_audio, m_wf, &WaveformWidget::stop);
    connect(tb, &WaveToolBar::play_sel, m_wf, &WaveformWidget::playSelection);
    connect(tb, &WaveToolBar::set_loop_play, [this](bool on) { m_wf->setLoopSelection(on);});
    connect(tb, &WaveToolBar::zoom_in, m_wf, &WaveformWidget::zoomIn);
    connect(tb, &WaveToolBar::zoom_out, m_wf, &WaveformWidget::zoomOut);
    connect(tb, &WaveToolBar::zoom_fit, m_wf, &WaveformWidget::zoomToFit);
    connect(tb, &WaveToolBar::zoom_sel, m_wf, &WaveformWidget::zoomToSelection);
    connect(tb, &WaveToolBar::highlight_wave, [this] {m_wf->addSelectionAsHighlight();});
    connect(tb, &WaveToolBar::clear_highlight, m_wf, &WaveformWidget::clearHighlights);
    connect(tb, &WaveToolBar::scale_wave, [this](int i) {m_wf->setAmplitudeScale( i == 0 ? WaveformWidget::AmplitudeScale::Linear
                                                                                     : WaveformWidget::AmplitudeScale::Decibel);});


    // WaveformWidget connections
    connect(m_wf, &WaveformWidget::loaded, [this](double duration){
        m_audio_duration = duration;
        emit wave_is_loaded();
    });

    connect(m_wf, &WaveformWidget::errorOccurred, [this](const QString& msg) {
        QMessageBox::warning(this, "Audio error: ", msg);
    });

    main_layout->addWidget(tb);
    main_layout->addWidget(marker_info);
    //main_layout->addLayout(marker_layout);
    main_layout->addWidget(m_wf);

    // HLine
    auto* line1 = hline(this);
    main_layout->addWidget(line1);

    // Countdown label
    auto* count_down = new CountdownLabel;
    count_down->setWaveform(m_wf);
    auto* count_down_layout = new QHBoxLayout;
    count_down_layout->addStretch();
    count_down_layout->addWidget(count_down);


    main_layout->addLayout(count_down_layout);

    auto* line2 = hline(this);
    main_layout->addWidget(line2);


    // Open Audio;
    if (!m_audio_filepath.isEmpty())
        m_wf->loadFile(m_audio_filepath);

    setLayout(main_layout);

}

    double AudioWaveWidget::audio_duration_sec()
    {
        return m_audio_duration;
    }

    double AudioWaveWidget::audio_duration_ms()
    {
        return m_audio_duration * 1000.0;
    }

    QFrame* AudioWaveWidget::hline(QWidget* parent)
    {
        QFrame* hline = new QFrame(parent);
        hline->setFrameShape(QFrame::HLine);
        hline->setFrameShadow(QFrame::Sunken);

        return hline;
    }


    QMap<AudioWaveWidget::MarkerType, double> AudioWaveWidget::markers() const
    {

        using aam  = AUDIO::AudioWaveWidget::MarkerType;

        auto marks =  m_wf->markers();

        QMap<aam, double> mark2;

        mark2[aam::Start] = marks[WaveformWidget::MarkerType::Start];
        mark2[aam::FadeIn] = marks[WaveformWidget::MarkerType::FadeIn];
        mark2[aam::Intro] = marks[WaveformWidget::MarkerType::Intro];
        mark2[aam::Extro] = marks[WaveformWidget::MarkerType::Extro];
        mark2[aam::FadeOut] = marks[WaveformWidget::MarkerType::FadeOut];
        mark2[aam::End] = marks[WaveformWidget::MarkerType::End];

        return mark2;

    }

    void AudioWaveWidget::set_marker(MarkerType marker_type, double ms)
    {

        if (ms == 0.0)  return;

        double mark_ms = 0.0;

        if (ms > 0 )
            mark_ms = ms / 1000.0;

        switch (marker_type) {
        case MarkerType::Start:
            qDebug() << "Start Marker: " << mark_ms;
            m_wf->setMarker(WaveformWidget::MarkerType::Start, mark_ms);
            break;
        case MarkerType::FadeIn:
            m_wf->setMarker(WaveformWidget::MarkerType::FadeIn, mark_ms);
            break;
        case MarkerType::Intro:
            qDebug() << "Intro Marker: " << mark_ms;
            m_wf->setMarker(WaveformWidget::MarkerType::Intro, mark_ms);
            break;
        case MarkerType::Extro:
            m_wf->setMarker(WaveformWidget::MarkerType::Extro, mark_ms);
            break;
        case MarkerType::FadeOut:
            m_wf->setMarker(WaveformWidget::MarkerType::FadeOut, mark_ms);
            break;
        case MarkerType::End:
            m_wf->setMarker(WaveformWidget::MarkerType::End, mark_ms);
            break;
        }

    }




}