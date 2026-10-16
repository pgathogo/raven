#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QMessageBox>

#include "audiowavewidget.h"
#include "WaveFormWidget.h"

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



AudioWaveWidget::AudioWaveWidget(QString& audio_filepath)
{
    m_audio_filepath = audio_filepath;
    setupui();
}

void AudioWaveWidget::setupui()
{
    QVBoxLayout* main_layout = new QVBoxLayout;
    WaveToolBar* tb = new WaveToolBar();

    auto* wf = new WaveformWidget;

    connect(tb, &WaveToolBar::arm_marker, this, [wf](int marker){
        const auto type = static_cast<WaveformWidget::MarkerType>(marker);
        wf->armMarker(type);
    });

    // Marker Information
    // TODO: This need to be inside the WaveToolBar Widdget
    auto* marker_info = new QLabel;
    marker_info->setTextInteractionFlags(Qt::TextSelectableByMouse);

    auto refresh_markers = [wf, marker_info]{
        QStringList parts;
        for(int i = 0; i < WaveformWidget::MarkerCount; ++i) {
            const auto type = static_cast<WaveformWidget::MarkerType>(i);
            parts << QString("%1 : %2").arg(WaveformWidget::markerName(type),
                                            wf->hasMarker(type) ? wf->markerTimeString(type) : QString("--"));
        }
        marker_info->setText(parts.join("   |   "));
    };

    // QHBoxLayout* marker_layout = new QHBoxLayout;
    // marker_layout->addWidget(marker_info);
    // marker_layout->setContentsMargins(50,0,0,0);

    refresh_markers();

    connect(tb, &WaveToolBar::play_pause_audio, wf, &WaveformWidget::togglePlayPause);
    connect(tb, &WaveToolBar::stop_audio, wf, &WaveformWidget::stop);
    connect(tb, &WaveToolBar::play_sel, wf, &WaveformWidget::playSelection);
    connect(tb, &WaveToolBar::set_loop_play, [wf](bool on) { wf->setLoopSelection(on);});
    connect(tb, &WaveToolBar::zoom_in, wf, &WaveformWidget::zoomIn);
    connect(tb, &WaveToolBar::zoom_out, wf, &WaveformWidget::zoomOut);
    connect(tb, &WaveToolBar::zoom_fit, wf, &WaveformWidget::zoomToFit);
    connect(tb, &WaveToolBar::zoom_sel, wf, &WaveformWidget::zoomToSelection);
    connect(tb, &WaveToolBar::highlight_wave, [wf] {wf->addSelectionAsHighlight();});
    connect(tb, &WaveToolBar::clear_highlight, wf, &WaveformWidget::clearHighlights);
    connect(tb, &WaveToolBar::scale_wave, [wf](int i) {wf->setAmplitudeScale( i == 0 ? WaveformWidget::AmplitudeScale::Linear
                                                                                     : WaveformWidget::AmplitudeScale::Decibel);});


    // WaveformWidget connections
    connect(wf, &WaveformWidget::loaded, [this](double duration){
        m_audio_duration = duration;
    });

    connect(wf, &WaveformWidget::errorOccurred, [this](const QString& msg) {
        QMessageBox::warning(this, "Audio error: ", msg);
    });

    main_layout->addWidget(tb);
    main_layout->addWidget(marker_info);
    //main_layout->addLayout(marker_layout);
    main_layout->addWidget(wf);

    // Open Audio;
    wf->loadFile(m_audio_filepath);

    setLayout(main_layout);

}

    double AudioWaveWidget::audio_duration()
    {
        return m_audio_duration;
    }

}