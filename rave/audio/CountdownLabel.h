#ifndef COUNTDOWNLABEL_H
#define COUNTDOWNLABEL_H

#include <QLabel>
#include <QPointer>

class WaveformWidget;

/**
 * CountdownLabel - a QLabel that shows the time remaining of the audio
 * loaded in a WaveformWidget, formatted as hh:mm:ss.
 *
 *   auto *countdown = new CountdownLabel;
 *   countdown->setWaveform(waveform);
 *
 * - Shows the full duration after a file is loaded / the playback is stopped.
 * - Counts down while playing; it also follows seeks and paused positions.
 * - Remaining time is rounded UP to whole seconds, so it only reads
 *   00:00:00 when the audio has really finished.
 * - Optionally counts down to the "End" marker instead of the end of the file.
 *
 * It is a normal QLabel, so setFont(), setStyleSheet(), setAlignment() etc.
 * can be used to restyle it.
 */
class CountdownLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CountdownLabel(QWidget *parent = nullptr);

    void setWaveform(WaveformWidget *waveform);
    WaveformWidget *waveform() const { return m_wf; }

    /// When true and an End marker is set, count down to that marker.
    void setUseEndMarker(bool on);
    bool useEndMarker() const { return m_useEndMarker; }

private slots:
    void refresh();

private:
    QPointer<WaveformWidget> m_wf;
    bool m_useEndMarker = false;
};

#endif // COUNTDOWNLABEL_H
