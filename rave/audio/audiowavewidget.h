#pragma once

#include <QWidget>

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
    AudioWaveWidget(QString&);
    double audio_duration();
private:
    void setupui();
    QString m_audio_filepath;

    double m_audio_duration{0.0};

};


}