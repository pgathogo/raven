#pragma once

#include <QWidget>

#include "WaveFormWidget.h"

class WaveformWidget;

namespace AUDIO {

class WaveToolBar : public QWidget
{
    Q_OBJECT
public:
    WaveToolBar();
signals:
    void play_pause_audio();
    void stop_audio();
    void play_sel();
    void set_loop_play(bool);
    void zoom_in();
    void zoom_out();
    void zoom_fit();
    void zoom_sel();
    void highlight_wave();
    void clear_highlight();
    void scale_wave(int);
    void arm_marker(int);
private:
    void setupui();
};

class AudioWaveWidget : public QWidget
{
    Q_OBJECT
public:
    enum class MarkerType { Start = 0, FadeIn, Intro, Extro, FadeOut, End };

    AudioWaveWidget(QString);
    double audio_duration_sec();
    double audio_duration_ms();
    QMap<MarkerType, double> markers() const;
    void set_marker(MarkerType, double);
signals:
    void wave_is_loaded();
private:
    void setupui();
    QFrame* hline(QWidget*);

    QString m_audio_filepath;

    double m_audio_duration{0.0};
    WaveformWidget* m_wf;

};


}