#include "CountdownLabel.h"
#include "WaveformWidget.h"

#include <QFontDatabase>
#include <algorithm>
#include <cmath>

CountdownLabel::CountdownLabel(QWidget *parent) : QLabel(parent)
{
    QFont f = QFontDatabase::systemFont(QFontDatabase::FixedFont);   // fixed width: no jitter
    f.setPointSize(20);
    f.setBold(true);
    setFont(f);
    setAlignment(Qt::AlignCenter);
    setMinimumWidth(fontMetrics().horizontalAdvance("00:00:00") + 20);
    setText(WaveformWidget::formatHMS(0));
}

void CountdownLabel::setWaveform(WaveformWidget *wf)
{
    if (m_wf)
        disconnect(m_wf, nullptr, this, nullptr);
    m_wf = wf;
    if (m_wf) {
        connect(m_wf, &WaveformWidget::positionChanged, this, &CountdownLabel::refresh);
        connect(m_wf, &WaveformWidget::loaded, this, &CountdownLabel::refresh);
        connect(m_wf, &WaveformWidget::loadingProgress, this, &CountdownLabel::refresh);
        connect(m_wf, &WaveformWidget::playbackStateChanged, this, &CountdownLabel::refresh);
        connect(m_wf, &WaveformWidget::markerChanged, this, &CountdownLabel::refresh);
    }
    refresh();
}

void CountdownLabel::setUseEndMarker(bool on)
{
    m_useEndMarker = on;
    refresh();
}

void CountdownLabel::refresh()
{
    QString text = WaveformWidget::formatHMS(0);
    if (m_wf && m_wf->hasAudio()) {
        double target = m_wf->duration();
        if (m_useEndMarker && m_wf->hasMarker(WaveformWidget::MarkerType::End))
            target = m_wf->markerTime(WaveformWidget::MarkerType::End);
        const double remaining = std::max(0.0, target - m_wf->playheadPosition());
        text = WaveformWidget::formatHMS(std::ceil(remaining - 1e-6));   // round up to whole seconds
    }
    if (text != this->text())      // avoid needless repaints (positionChanged fires every 25 ms)
        setText(text);
}
