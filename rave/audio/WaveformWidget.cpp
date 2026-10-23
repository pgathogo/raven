#include "WaveformWidget.h"

#include "editor/audioplayer.h"          // AudioPlayer + AudioThread (WaveformData)

#include <QFileInfo>
#include <QKeyEvent>
#include <QJsonValue>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPolygonF>
#include <QScrollBar>
#include <QTimer>
#include <QWheelEvent>

#include <algorithm>
#include <cmath>

namespace {
constexpr int kBlockFrames = 64;          // audio frames summarised per peak entry
constexpr int kLeftMargin = 52;           // dB ruler width
constexpr int kTopMargin = 22;            // time ruler height
constexpr int kLaneGap = 4;
constexpr double kMinSamplesPerPixel = 32.0;
constexpr double kEdgeGrab = 5.0;         // px to grab a selection edge
} // namespace

// ======================================================================
// Construction
// ======================================================================
WaveformWidget::WaveformWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(360, 180);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_scrollBar = new QScrollBar(Qt::Horizontal, this);
    m_scrollBar->setEnabled(false);
    connect(m_scrollBar, &QScrollBar::valueChanged, this, [this](int v) {
        if (!m_updatingScrollBar)
            setViewStart(v / 1000.0);
    });

    m_timer = new QTimer(this);
    m_timer->setInterval(25);
    connect(m_timer, &QTimer::timeout, this, &WaveformWidget::onTick);

    setAudioPlayer(nullptr);                    // private player by default
}

WaveformWidget::~WaveformWidget()
{
    if (m_player) {
        QObject::disconnect(m_player, nullptr, this, nullptr);
        if (!m_ownsPlayer) {
            m_player->cancel_waveform();
            if (m_player->playback_state() != AUDIO::AudioPlayer::PlaybackState::StoppedState)
                m_player->stop_play();
        }
    }
}

// ======================================================================
// Player binding
// ======================================================================
void WaveformWidget::setAudioPlayer(AUDIO::AudioPlayer *player)
{
    using AUDIO::AudioPlayer;

    if (m_player) {
        QObject::disconnect(m_player, nullptr, this, nullptr);
        m_player->cancel_waveform();
        if (m_player->playback_state() != AudioPlayer::PlaybackState::StoppedState)
            m_player->stop_play();
        if (m_ownsPlayer) {
            m_player->deleteLater();
            m_ownsPlayer = false;
        }
    }

    if (player) {
        m_player = player;
    } else {
        m_player = new AudioPlayer();
        m_player->setParent(this);
        m_ownsPlayer = true;
    }
    bindPlayer();

    m_timer->stop();
    if (!m_path.isEmpty())                      // new player must know the loaded file
        m_player->set_source(m_path.toStdString());
}

void WaveformWidget::bindPlayer()
{
    using AUDIO::AudioPlayer;

    connect(m_player, &AudioPlayer::playback_state_changed, this,
            [this](AudioPlayer::PlaybackState s) {
        const bool playing = (s == AudioPlayer::PlaybackState::PlayingState);
        if (playing) m_timer->start(); else m_timer->stop();
        if (s == AudioPlayer::PlaybackState::StoppedState)
            m_stopAtEnd = false;
        emit playbackStateChanged(playing);
        update();
    });

    // The file played to its end (not a user stop): park the playhead at the end.
    connect(m_player, &AudioPlayer::end_of_play, this, [this]() {
        if (!m_hasAudio) return;
        m_playhead = m_duration;
        emit positionChanged(m_playhead);
        update();
    });

    connect(m_player, &AudioPlayer::waveform_progress, this, [this](int pct) {
        if (!m_decoding) return;
        m_progress = std::clamp(pct, 0, 99);
        emit loadingProgress(m_progress);
        update();
    });

    connect(m_player, &AudioPlayer::waveform_ready, this,
            [this](WaveformDataPtr data) { onWaveformReady(data); });

    connect(m_player, &AudioPlayer::waveform_failed, this, [this](QString msg) {
        if (!m_decoding) return;
        m_decoding = false;
        emit errorOccurred(msg);
        update();
    });
}


// ======================================================================
// Loading / decoding
// ======================================================================
bool WaveformWidget::loadFile(const QString &path)
{
    const QFileInfo fi(path);
    if (!fi.exists() || !fi.isFile()) {
        emit errorOccurred(tr("File not found: %1").arg(path));
        return false;
    }

    clearData();
    m_decoding = true;

    const QString abs = fi.absoluteFilePath();
    m_path = abs;
    m_player->set_source(abs.toStdString());    // used for playback
    m_player->load_waveform(abs, kBlockFrames); // BASS decodes on a worker thread
    update();

    return true;
}

void WaveformWidget::clearData()
{
    m_player->cancel_waveform();
    if (m_player->playback_state() != AUDIO::AudioPlayer::PlaybackState::StoppedState)
        m_player->stop_play();

    m_wave.reset();
    m_sampleRate = 0;
    m_duration = 0;
    m_hasAudio = false;
    m_decoding = false;
    m_progress = 0;
    m_viewStart = 0;
    m_viewDur = 1;
    m_playhead = 0;
    m_selStart = m_selEnd = 0;
    m_highlights.clear();
    for (double &t : m_markerTime) t = -1;
    m_armed = false;
    m_stopAtEnd = false;
    m_drag = Drag::None;
    updateScrollBar();
}

void WaveformWidget::onWaveformReady(const std::shared_ptr<WaveformData> &data)
{
    if (!m_decoding || !data)
        return;
    m_decoding = false;

    if (data->sampleRate <= 0 || data->totalFrames <= 0) {
        emit errorOccurred(tr("No audio data could be decoded."));
        update();
        return;
    }

    m_wave = data;
    m_sampleRate = data->sampleRate;
    m_duration = data->duration();
    m_hasAudio = true;
    m_viewStart = 0;
    m_viewDur = m_duration;
    updateScrollBar();
    emit loadingProgress(100);
    emit loaded(m_duration);
    emit viewChanged(m_viewStart, m_viewDur);
    update();
}

// ======================================================================
// Playback
// ======================================================================
bool WaveformWidget::isPlaying() const
{
    return m_player->playback_state() == AUDIO::AudioPlayer::PlaybackState::PlayingState;
}

void WaveformWidget::setVolume(float v) { m_player->set_volume(std::clamp(v, 0.0f, 1.0f)); }

void WaveformWidget::startAt(double s)
{
    using PS = AUDIO::AudioPlayer::PlaybackState;
    m_playhead = std::clamp(s, 0.0, m_duration);
    emit positionChanged(m_playhead);

    switch (m_player->playback_state()) {
    case PS::StoppedState:
        m_player->play_from_seconds(m_playhead);        // (re)creates the BASS channel
        break;
    case PS::PausedState:
        m_player->seek_seconds(m_playhead);
        m_player->resume_audio();
        break;
    case PS::PlayingState:
        m_player->seek_seconds(m_playhead);
        break;
    }
    update();
}

void WaveformWidget::play()
{
    if (!m_hasAudio || isPlaying()) return;
    if (m_playhead >= m_duration - 0.01)
        m_playhead = 0;
    startAt(m_playhead);
}

void WaveformWidget::pause()
{
    if (isPlaying())
        m_player->pause_audio();
}

void WaveformWidget::stop()
{
    if (m_player->playback_state() != AUDIO::AudioPlayer::PlaybackState::StoppedState)
        m_player->stop_play();
    m_stopAtEnd = false;
    setPlayheadPosition(0);
}

void WaveformWidget::togglePlayPause()
{
    if (isPlaying()) pause(); else play();
}

void WaveformWidget::playSelection()
{
    if (!m_hasAudio || !hasSelection()) return;
    m_regionStart = m_selStart;
    m_regionEnd = m_selEnd;
    m_stopAtEnd = true;
    startAt(m_regionStart);
}

void WaveformWidget::setPlayheadPosition(double s)
{
    if (!m_hasAudio) return;
    m_playhead = std::clamp(s, 0.0, m_duration);
    // Playing or paused: move the BASS channel. Stopped: the position is only
    // remembered and used by the next play().
    if (m_player->playback_state() != AUDIO::AudioPlayer::PlaybackState::StoppedState)
        m_player->seek_seconds(m_playhead);
    emit positionChanged(m_playhead);
    update();
}

void WaveformWidget::onTick()
{
    double pos = m_player->position_seconds();
    if (m_stopAtEnd && pos >= m_regionEnd) {
        if (m_loopSelection) {
            m_player->seek_seconds(m_regionStart);
            pos = m_regionStart;
        } else {
            m_player->pause_audio();
            m_stopAtEnd = false;
            pos = m_regionEnd;
        }
    }
    m_playhead = std::min(pos, m_duration);
    if (m_follow && isPlaying())
        ensurePlayheadVisible();
    emit positionChanged(m_playhead);
    update();
}

void WaveformWidget::ensurePlayheadVisible()
{
    if (m_drag != Drag::None) return;           // don't fight the user's drag
    if (m_playhead < m_viewStart || m_playhead > m_viewStart + m_viewDur)
        setViewStart(m_playhead - 0.1 * m_viewDur);   // page flip
}

// ======================================================================
// Zoom / view
// ======================================================================
double WaveformWidget::minVisibleDuration() const
{
    if (m_sampleRate <= 0) return 0.001;
    const double d = waveRect().width() * kMinSamplesPerPixel / m_sampleRate;
    return std::min(m_duration, std::max(0.001, d));
}

double WaveformWidget::clampStart(double s) const
{
    return std::clamp(s, 0.0, std::max(0.0, m_duration - m_viewDur));
}

void WaveformWidget::setViewStart(double s)
{
    if (!m_hasAudio) return;
    s = clampStart(s);
    if (qFuzzyCompare(s + 1.0, m_viewStart + 1.0)) return;
    m_viewStart = s;
    updateScrollBar();
    emit viewChanged(m_viewStart, m_viewDur);
    update();
}

void WaveformWidget::zoomAt(double factor, double anchor)
{
    if (!m_hasAudio || factor <= 0) return;
    anchor = std::clamp(anchor, m_viewStart, m_viewStart + m_viewDur);
    const double ratio = (anchor - m_viewStart) / m_viewDur;
    const double nd = std::clamp(m_viewDur / factor, minVisibleDuration(), m_duration);
    m_viewDur = nd;
    m_viewStart = clampStart(anchor - ratio * nd);
    updateScrollBar();
    emit viewChanged(m_viewStart, m_viewDur);
    update();
}

void WaveformWidget::zoomIn()  { zoomAt(2.0, m_viewStart + m_viewDur / 2); }
void WaveformWidget::zoomOut() { zoomAt(0.5, m_viewStart + m_viewDur / 2); }

void WaveformWidget::zoomToFit()
{
    if (!m_hasAudio) return;
    m_viewStart = 0;
    m_viewDur = m_duration;
    updateScrollBar();
    emit viewChanged(m_viewStart, m_viewDur);
    update();
}

void WaveformWidget::zoomToSelection()
{
    if (!m_hasAudio || !hasSelection()) return;
    const double len = m_selEnd - m_selStart;
    const double margin = len * 0.05;
    m_viewDur = std::clamp(len + 2 * margin, minVisibleDuration(), m_duration);
    m_viewStart = clampStart(m_selStart - margin);
    updateScrollBar();
    emit viewChanged(m_viewStart, m_viewDur);
    update();
}

void WaveformWidget::updateScrollBar()
{
    m_updatingScrollBar = true;
    const int total = int(m_duration * 1000.0);
    const int page = std::max(1, int(m_viewDur * 1000.0));
    m_scrollBar->setRange(0, std::max(0, total - page));
    m_scrollBar->setPageStep(page);
    m_scrollBar->setSingleStep(std::max(1, page / 10));
    m_scrollBar->setValue(int(m_viewStart * 1000.0));
    m_scrollBar->setEnabled(total > page);
    m_updatingScrollBar = false;
}

// ======================================================================
// Selection / highlights
// ======================================================================
void WaveformWidget::setSelection(double a, double b)
{
    if (!m_hasAudio) return;
    a = std::clamp(a, 0.0, m_duration);
    b = std::clamp(b, 0.0, m_duration);
    if (a > b) std::swap(a, b);
    if (b - a < 1e-4) { a = b = 0; }
    if (a == m_selStart && b == m_selEnd) return;
    m_selStart = a;
    m_selEnd = b;
    emit selectionChanged(m_selStart, m_selEnd);
    update();
}

void WaveformWidget::clearSelection() { setSelection(0, 0); }

void WaveformWidget::addHighlight(double start, double end, const QColor &color)
{
    if (start > end) std::swap(start, end);
    start = std::max(0.0, start);
    end = std::min(m_duration, end);
    if (end - start < 1e-4) return;
    m_highlights.push_back({start, end, color});
    emit highlightsChanged();
    update();
}

void WaveformWidget::addSelectionAsHighlight(const QColor &color)
{
    if (!hasSelection()) return;
    addHighlight(m_selStart, m_selEnd, color);
}

void WaveformWidget::clearHighlights()
{
    m_highlights.clear();
    emit highlightsChanged();
    update();
}

// ======================================================================
// Appearance setters
// ======================================================================
void WaveformWidget::setWaveColor(const QColor &c) { m_waveColor[0] = m_waveColor[1] = c; update(); }
void WaveformWidget::setChannelColors(const QColor &l, const QColor &r) { m_waveColor[0] = l; m_waveColor[1] = r; update(); }
void WaveformWidget::setBackgroundColor(const QColor &c) { m_bgColor = c; update(); }
void WaveformWidget::setPlayheadColor(const QColor &c) { m_playheadColor = c; update(); }
void WaveformWidget::setSelectionColor(const QColor &c) { m_selectionColor = c; update(); }
void WaveformWidget::setRulerColor(const QColor &bg, const QColor &text) { m_rulerBg = bg; m_rulerText = text; update(); }
void WaveformWidget::setGridColor(const QColor &c) { m_gridColor = c; update(); }
void WaveformWidget::setShowRms(bool on) { m_showRms = on; update(); }
void WaveformWidget::setAmplitudeScale(AmplitudeScale s) { m_scale = s; update(); }
void WaveformWidget::setMinimumDecibels(double db) { m_minDb = std::clamp(db, -120.0, -6.0); update(); }

// ======================================================================
// Geometry helpers
// ======================================================================
QRect WaveformWidget::waveRect() const
{
    const int sb = m_scrollBar->sizeHint().height();
    return QRect(kLeftMargin, kTopMargin, std::max(1, width() - kLeftMargin - 1),
                 std::max(1, height() - kTopMargin - sb));
}

QRect WaveformWidget::laneRect(int ch) const
{
    const QRect wr = waveRect();
    const int laneH = (wr.height() - kLaneGap) / 2;
    const int top = wr.top() + ch * (laneH + kLaneGap);
    return QRect(wr.left(), top, wr.width(), laneH);
}

double WaveformWidget::timeToX(double t) const
{
    const QRect wr = waveRect();
    return wr.left() + (t - m_viewStart) / m_viewDur * wr.width();
}

double WaveformWidget::xToTime(double x) const
{
    const QRect wr = waveRect();
    return m_viewStart + (x - wr.left()) / wr.width() * m_viewDur;
}

void WaveformWidget::resizeEvent(QResizeEvent *)
{
    const int sbH = m_scrollBar->sizeHint().height();
    m_scrollBar->setGeometry(kLeftMargin, height() - sbH, std::max(1, width() - kLeftMargin), sbH);
}

// ======================================================================
// Scale helpers
// ======================================================================
float WaveformWidget::dbToNorm(double db) const
{
    if (m_scale == AmplitudeScale::Linear)
        return float(std::min(1.0, std::pow(10.0, db / 20.0)));
    return float(std::clamp((db - m_minDb) / (-m_minDb), 0.0, 1.0));
}

float WaveformWidget::ampToNorm(float a) const
{
    a = std::fabs(a);
    if (m_scale == AmplitudeScale::Linear)
        return std::min(a, 1.0f);
    if (a <= 1e-7f) return 0.0f;
    return dbToNorm(20.0 * std::log10(double(a)));
}

QVector<WaveformWidget::Tick> WaveformWidget::computeTicks(double half) const
{
    static const int dbs[] = {0, -3, -6, -12, -18, -24, -36, -48, -60, -72, -84, -96};
    QVector<Tick> out;
    double lastPx = 1e9;
    for (int db : dbs) {
        if (m_scale == AmplitudeScale::Decibel && db <= m_minDb) continue;
        if (m_scale == AmplitudeScale::Linear && db < -60) continue;
        const float norm = dbToNorm(db);
        const double px = norm * half;
        if (px < 4) continue;
        const bool labeled = (px >= 12) && (lastPx - px >= 14);
        if (labeled) lastPx = px;
        out.push_back({db, norm, labeled});
    }
    return out;
}

// ======================================================================
// Painting
// ======================================================================
void WaveformWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(rect(), m_bgColor);

    // ruler backgrounds
    p.fillRect(QRect(0, 0, width(), kTopMargin), m_rulerBg);
    p.fillRect(QRect(0, kTopMargin, kLeftMargin, height() - kTopMargin), m_rulerBg);

    if (!m_hasAudio) {
        p.setPen(m_rulerText);
        QString msg = m_decoding ? tr("Decoding… %1%").arg(m_progress) : tr("No audio loaded");
        p.drawText(waveRect(), Qt::AlignCenter, msg);
        return;
    }

    for (int c = 0; c < 2; ++c) {
        drawLane(p, c);
        drawDbRuler(p, c);
    }
    drawTimeRuler(p);
    drawMarkers(p);
    drawPlayhead(p);
}

void WaveformWidget::drawLane(QPainter &p, int ch)
{
    const QRect lane = laneRect(ch);
    p.save();
    p.setClipRect(lane);
    p.fillRect(lane, m_bgColor.lightness() < 128 ? m_bgColor.lighter(115) : m_bgColor.darker(105));

    // highlights
    for (const Highlight &h : std::as_const(m_highlights)) {
        const double x0 = std::max<double>(lane.left(), timeToX(h.start));
        const double x1 = std::min<double>(lane.right() + 1, timeToX(h.end));
        if (x1 > x0)
            p.fillRect(QRectF(x0, lane.top(), x1 - x0, lane.height()), h.color);
    }
    // selection
    if (hasSelection()) {
        const double x0 = timeToX(m_selStart), x1 = timeToX(m_selEnd);
        QRectF r(std::max<double>(lane.left(), x0), lane.top(),
                 std::min<double>(lane.right() + 1, x1) - std::max<double>(lane.left(), x0), lane.height());
        if (r.width() > 0) p.fillRect(r, m_selectionColor);
        QColor edge = m_selectionColor; edge.setAlpha(255);
        p.setPen(QPen(edge, 1));
        p.drawLine(QPointF(x0, lane.top()), QPointF(x0, lane.bottom()));
        p.drawLine(QPointF(x1, lane.top()), QPointF(x1, lane.bottom()));
    }

    // dB grid + centre line
    const double yc = lane.top() + lane.height() / 2.0;
    const double half = lane.height() / 2.0 - 1;
    p.setPen(m_gridColor);
    for (const Tick &t : computeTicks(half)) {
        p.drawLine(QPointF(lane.left(), yc - t.norm * half), QPointF(lane.right(), yc - t.norm * half));
        p.drawLine(QPointF(lane.left(), yc + t.norm * half), QPointF(lane.right(), yc + t.norm * half));
    }
    p.setPen(QColor(m_gridColor.red(), m_gridColor.green(), m_gridColor.blue(), std::min(255, m_gridColor.alpha() * 2)));
    p.drawLine(QPointF(lane.left(), yc), QPointF(lane.right(), yc));

    drawWave(p, ch);
    p.restore();
}

void WaveformWidget::drawWave(QPainter &p, int ch)
{
    const QRect lane = laneRect(ch);
    const double yc = lane.top() + lane.height() / 2.0;
    const double half = lane.height() / 2.0 - 1;
    const int w = lane.width();
    const double secPerPx = m_viewDur / w;
    if (!m_wave) return;
    const double blocksPerSec = double(m_sampleRate) / m_wave->blockFrames;
    const std::vector<WavePeak> &pk = m_wave->peaks[ch];
    const int nb = int(pk.size());

    QColor peakCol = m_waveColor[ch];
    peakCol.setAlpha(m_showRms ? 110 : 255);
    const QColor rmsCol = m_waveColor[ch];

    for (int x = 0; x < w; ++x) {
        const double t0 = m_viewStart + x * secPerPx;
        const double t1 = t0 + secPerPx;
        if (t0 >= m_duration) break;
        const int b0 = int(t0 * blocksPerSec);
        if (b0 >= nb) break;
        int b1 = int(std::ceil(t1 * blocksPerSec));
        b1 = std::min(nb, std::max(b1, b0 + 1));

        float mn = pk[b0].min, mx = pk[b0].max;
        double sq = 0;
        for (int b = b0; b < b1; ++b) {
            mn = std::min(mn, pk[b].min);
            mx = std::max(mx, pk[b].max);
            sq += double(pk[b].rms) * pk[b].rms;
        }
        const float rms = float(std::sqrt(sq / (b1 - b0)));

        const double px = lane.left() + x + 0.5;
        const double yTop = yc - ampToNorm(std::max(mx, 0.0f)) * half - 0.5;
        const double yBot = yc + ampToNorm(std::max(-mn, 0.0f)) * half + 0.5;
        p.setPen(peakCol);
        p.drawLine(QPointF(px, yTop), QPointF(px, yBot));

        if (m_showRms) {
            const double r = ampToNorm(rms) * half;
            p.setPen(rmsCol);
            p.drawLine(QPointF(px, yc - r), QPointF(px, yc + r));
        }
    }
}

void WaveformWidget::drawDbRuler(QPainter &p, int ch)
{
    const QRect lane = laneRect(ch);
    const double yc = lane.top() + lane.height() / 2.0;
    const double half = lane.height() / 2.0 - 1;

    p.save();
    QFont f = font();
    f.setPointSizeF(std::max(6.5, f.pointSizeF() - 1.5));
    p.setFont(f);
    p.setPen(m_rulerText);

    // channel name
    QFont bold = f; bold.setBold(true);
    p.setFont(bold);
    p.drawText(QRectF(2, lane.top() + 2, 14, 14), Qt::AlignCenter, ch == 0 ? "L" : "R");
    p.setFont(f);

    // centre label
    const QString centre = (m_scale == AmplitudeScale::Linear) ? QStringLiteral("-∞")
                                                               : QString::number(int(m_minDb));
    p.drawText(QRectF(14, yc - 7, kLeftMargin - 21, 14), Qt::AlignRight | Qt::AlignVCenter, centre);
    p.drawLine(kLeftMargin - 5, int(yc), kLeftMargin, int(yc));

    for (const Tick &t : computeTicks(half)) {
        const double yUp = yc - t.norm * half;
        const double yDn = yc + t.norm * half;
        p.drawLine(QPointF(kLeftMargin - 5, yUp), QPointF(kLeftMargin, yUp));
        p.drawLine(QPointF(kLeftMargin - 5, yDn), QPointF(kLeftMargin, yDn));
        if (t.labeled) {
            const double ty = std::max<double>(yUp - 7, lane.top());
            p.drawText(QRectF(14, ty, kLeftMargin - 21, 14), Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(t.db));
        }
    }
    // unit
    p.drawText(QRectF(2, lane.bottom() - 14, kLeftMargin - 8, 14), Qt::AlignRight | Qt::AlignVCenter, "dB");
    p.restore();
}

QString WaveformWidget::formatTime(double t, double step)
{
    const qint64 totalMs = qint64(std::llround(t * 1000.0));
    const int ms = int(totalMs % 1000);
    const qint64 totalSec = totalMs / 1000;
    const int s = int(totalSec % 60);
    const int m = int((totalSec / 60) % 60);
    const int h = int(totalSec / 3600);

    QString base = h > 0 ? QString("%1:%2:%3").arg(h).arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0'))
                         : QString("%1:%2").arg(m).arg(s, 2, 10, QChar('0'));
    if (step < 1.0) {
        const int digits = step < 0.01 ? 3 : (step < 0.1 ? 2 : 1);
        const int div = digits == 3 ? 1 : (digits == 2 ? 10 : 100);
        base += QString(".%1").arg(ms / div, digits, 10, QChar('0'));
    }
    return base;
}

void WaveformWidget::drawTimeRuler(QPainter &p)
{
    const QRect wr = waveRect();
    p.save();
    p.setClipRect(QRect(wr.left(), 0, wr.width(), kTopMargin));
    QFont f = font();
    f.setPointSizeF(std::max(6.5, f.pointSizeF() - 1.5));
    p.setFont(f);
    p.setPen(m_rulerText);

    static const double steps[] = {0.001, 0.002, 0.005, 0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1, 2, 5,
                                   10, 15, 30, 60, 120, 300, 600, 900, 1800, 3600};
    const double pxPerSec = wr.width() / m_viewDur;
    double step = steps[std::size(steps) - 1];
    for (double s : steps)
        if (s * pxPerSec >= 80) { step = s; break; }

    const qint64 first = qint64(std::floor(m_viewStart / step));
    const qint64 last = qint64(std::ceil((m_viewStart + m_viewDur) / step));
    for (qint64 i = first; i <= last; ++i) {
        const double t = i * step;
        const double x = timeToX(t);
        p.drawLine(QPointF(x, kTopMargin - 7), QPointF(x, kTopMargin));
        p.drawText(QPointF(x + 3, kTopMargin - 9), formatTime(std::max(0.0, t), step));
        for (int k = 1; k < 4; ++k) {                       // minor ticks
            const double xm = timeToX(t + k * step / 4);
            p.drawLine(QPointF(xm, kTopMargin - 3), QPointF(xm, kTopMargin));
        }
    }
    p.restore();
}

void WaveformWidget::drawPlayhead(QPainter &p)
{
    const QRect wr = waveRect();
    const double x = timeToX(m_playhead);
    if (x < wr.left() - 1 || x > wr.right() + 1) return;
    p.save();
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(QPen(m_playheadColor, 1.5));
    p.drawLine(QPointF(x, 0), QPointF(x, wr.bottom()));
    p.setBrush(m_playheadColor);
    p.setPen(Qt::NoPen);
    p.drawPolygon(QPolygonF({QPointF(x - 5, 0), QPointF(x + 5, 0), QPointF(x, 8)}));
    p.restore();
}

// ======================================================================
// Input
// ======================================================================
void WaveformWidget::mousePressEvent(QMouseEvent *e)
{
    if (!m_hasAudio) return;
    setFocus();
    if (e->button() == Qt::RightButton) {
        showContextMenu(e->position().toPoint(), e->globalPosition().toPoint());
        return;
    }
    if (e->button() != Qt::LeftButton) return;

    const double x = e->position().x();
    if (x < kLeftMargin) return;
    const double t = std::clamp(xToTime(x), 0.0, m_duration);

    if (m_armed) {                              // drop the armed marker here
        setMarker(m_armedType, t);
        disarmMarker();
        return;
    }
    if (const int mi = markerAt(x); mi >= 0) { // grab an existing marker
        m_drag = Drag::Marker;
        m_dragMarker = mi;
        return;
    }
    m_pressX = x;
    m_dragAnchor = t;
    m_drag = Drag::Pending;

    if (hasSelection()) {                       // grab an edge?
        if (std::abs(x - timeToX(m_selStart)) <= kEdgeGrab) {
            m_drag = Drag::EditEdge;
            m_dragAnchor = m_selEnd;
        } else if (std::abs(x - timeToX(m_selEnd)) <= kEdgeGrab) {
            m_drag = Drag::EditEdge;
            m_dragAnchor = m_selStart;
        }
    }
}

void WaveformWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (!m_hasAudio) return;
    const double x = e->position().x();

    if (m_drag == Drag::None) {                 // hover cursor
        const bool nearEdge = (hasSelection() &&
            (std::abs(x - timeToX(m_selStart)) <= kEdgeGrab || std::abs(x - timeToX(m_selEnd)) <= kEdgeGrab))
            || markerAt(x) >= 0;
        setCursor(m_armed ? Qt::CrossCursor : (nearEdge ? Qt::SizeHorCursor : Qt::ArrowCursor));
        return;
    }

    if (m_drag == Drag::Marker) {               // move a marker
        const QRect wr = waveRect();
        if (x < wr.left())        setViewStart(m_viewStart - 0.02 * m_viewDur);
        else if (x > wr.right())  setViewStart(m_viewStart + 0.02 * m_viewDur);
        setMarker(static_cast<MarkerType>(m_dragMarker), std::clamp(xToTime(x), 0.0, m_duration));
        return;
    }

    if (m_drag == Drag::Pending && std::abs(x - m_pressX) >= 3)
        m_drag = Drag::Selecting;
    if (m_drag == Drag::Pending) return;

    const QRect wr = waveRect();
    if (x < wr.left())        setViewStart(m_viewStart - 0.02 * m_viewDur);   // edge auto-scroll
    else if (x > wr.right())  setViewStart(m_viewStart + 0.02 * m_viewDur);

    setSelection(m_dragAnchor, std::clamp(xToTime(x), 0.0, m_duration));
}

void WaveformWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton) return;
    if (m_drag == Drag::Pending) {              // plain click -> seek
        clearSelection();
        setPlayheadPosition(m_dragAnchor);
    }
    m_drag = Drag::None;
}

void WaveformWidget::wheelEvent(QWheelEvent *e)
{
    if (!m_hasAudio) return;
    const double dy = e->angleDelta().y();
    const double dx = e->angleDelta().x();
    if (e->modifiers() & Qt::ControlModifier) {
        const double anchor = xToTime(e->position().x());
        zoomAt(std::pow(1.0015, dy), anchor);
    } else {
        const double d = (dy != 0 ? dy : dx);
        setViewStart(m_viewStart - d / 120.0 * 0.1 * m_viewDur);
    }
    e->accept();
}

void WaveformWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Space:  togglePlayPause(); break;
    case Qt::Key_Escape: if (m_armed) disarmMarker(); else clearSelection(); break;
    case Qt::Key_1: armMarker(MarkerType::Start);   break;
    case Qt::Key_2: armMarker(MarkerType::FadeIn);  break;
    case Qt::Key_3: armMarker(MarkerType::Intro);   break;
    case Qt::Key_4: armMarker(MarkerType::Extro);   break;
    case Qt::Key_5: armMarker(MarkerType::FadeOut); break;
    case Qt::Key_6: armMarker(MarkerType::End);     break;
    case Qt::Key_H:      addSelectionAsHighlight(); break;
    case Qt::Key_Plus:
    case Qt::Key_Equal:  zoomIn(); break;
    case Qt::Key_Minus:  zoomOut(); break;
    default: QWidget::keyPressEvent(e);
    }
}

// ======================================================================
// Markers
// ======================================================================
QString WaveformWidget::markerName(MarkerType t)
{
    static const char *names[] = {"Start", "Fade In", "Intro", "Extro", "Fade Out", "End"};
    return QString::fromLatin1(names[int(t)]);
}

QString WaveformWidget::markerKey(MarkerType t)
{
    static const char *keys[] = {"start", "fadeIn", "intro", "extro", "fadeOut", "end"};
    return QString::fromLatin1(keys[int(t)]);
}

QString WaveformWidget::formatHMS(double seconds, bool withMillis)
{
    const qint64 totalMs = std::max<qint64>(0, qint64(std::llround(seconds * 1000.0)));
    const qint64 totalSec = totalMs / 1000;       // truncated, so "hh:mm:ss" never rounds up
    QString s = QString("%1:%2:%3")
                    .arg(totalSec / 3600, 2, 10, QChar('0'))
                    .arg((totalSec / 60) % 60, 2, 10, QChar('0'))
                    .arg(totalSec % 60, 2, 10, QChar('0'));
    if (withMillis)
        s += QString(".%1").arg(totalMs % 1000, 3, 10, QChar('0'));
    return s;
}

QString WaveformWidget::markerTimeString(MarkerType t, bool withMillis) const
{
    return hasMarker(t) ? formatHMS(markerTime(t), withMillis) : QString();
}

QMap<WaveformWidget::MarkerType, double> WaveformWidget::markers() const
{
    QMap<MarkerType, double> out;
    for (int i = 0; i < MarkerCount; ++i)
        if (m_markerTime[i] >= 0)
            out.insert(static_cast<MarkerType>(i), m_markerTime[i]);
    return out;
}

QJsonObject WaveformWidget::markersToJson() const
{
    QJsonObject root;
    for (int i = 0; i < MarkerCount; ++i) {
        if (m_markerTime[i] < 0) continue;
        const auto type = static_cast<MarkerType>(i);
        QJsonObject m;
        m["seconds"] = std::round(m_markerTime[i] * 1000.0) / 1000.0;
        m["time"] = formatHMS(m_markerTime[i]);
        m["timeMs"] = formatHMS(m_markerTime[i], true);
        m["msec"] = m_markerTime[i];
        root[markerKey(type)] = m;
    }
    return root;
}

bool WaveformWidget::markersFromJson(const QJsonObject &obj)
{
    if (!m_hasAudio) return false;
    clearMarkers();
    bool ok = true;
    for (int i = 0; i < MarkerCount; ++i) {
        const auto type = static_cast<MarkerType>(i);
        const QString key = markerKey(type);
        if (!obj.contains(key)) continue;
        const QJsonValue v = obj.value(key);
        const double s = v.isObject() ? v.toObject().value("seconds").toDouble(-1.0) : v.toDouble(-1.0);
        if (s < 0) { ok = false; continue; }
        setMarker(type, s);
    }
    return ok;
}

bool WaveformWidget::isMarkerOrderValid() const
{
    double prev = -1;
    for (int i = 0; i < MarkerCount; ++i) {
        if (m_markerTime[i] < 0) continue;
        if (m_markerTime[i] < prev) return false;
        prev = m_markerTime[i];
    }
    return true;
}

void WaveformWidget::setMarker(MarkerType type, double seconds)
{

    if (!m_hasAudio) return;

    seconds = std::clamp(seconds, 0.0, m_duration);
    double &slot = m_markerTime[int(type)];
    if (slot == seconds) return;
    slot = seconds;
    emit markerChanged(type, seconds);
    update();
}

void WaveformWidget::clearMarker(MarkerType type)
{
    double &slot = m_markerTime[int(type)];
    if (slot < 0) return;
    slot = -1;
    emit markerChanged(type, -1.0);
    update();
}

void WaveformWidget::clearMarkers()
{
    for (int i = 0; i < MarkerCount; ++i)
        clearMarker(static_cast<MarkerType>(i));
}

void WaveformWidget::armMarker(MarkerType type)
{
    if (!m_hasAudio) return;
    m_armed = true;
    m_armedType = type;
    setCursor(Qt::CrossCursor);
    setFocus();
    emit armedMarkerChanged(true, type);
}

void WaveformWidget::disarmMarker()
{
    if (!m_armed) return;
    m_armed = false;
    setCursor(Qt::ArrowCursor);
    emit armedMarkerChanged(false, m_armedType);
}

void WaveformWidget::setMarkerColor(MarkerType t, const QColor &c) { m_markerColors[int(t)] = c; update(); }
void WaveformWidget::setShowMarkers(bool on) { m_showMarkers = on; update(); }

int WaveformWidget::markerAt(double x) const
{
    if (!m_showMarkers || !m_hasAudio) return -1;
    int best = -1;
    double bestDist = kEdgeGrab + 0.001;
    for (int i = 0; i < MarkerCount; ++i) {
        if (m_markerTime[i] < 0) continue;
        const double d = std::abs(x - timeToX(m_markerTime[i]));
        if (d < bestDist) { bestDist = d; best = i; }
    }
    return best;
}

void WaveformWidget::drawMarkers(QPainter &p)
{
    if (!m_showMarkers) return;
    const QRect wr = waveRect();
    p.save();
    p.setClipRect(wr);
    QFont f = font();
    f.setPointSizeF(std::max(6.5, f.pointSizeF() - 1.5));
    f.setBold(true);
    p.setFont(f);
    const QFontMetrics fm(f);

    for (int i = 0; i < MarkerCount; ++i) {
        if (m_markerTime[i] < 0) continue;
        const double x = timeToX(m_markerTime[i]);
        if (x < wr.left() - 1 || x > wr.right() + 1) continue;
        const QColor col = m_markerColors[i];
        const auto type = static_cast<MarkerType>(i);

        p.setPen(QPen(col, 1.5, Qt::DashLine));
        p.drawLine(QPointF(x, wr.top()), QPointF(x, wr.bottom()));

        // label flag (staggered vertically so neighbouring markers stay readable)
        const QString text = QString("%1 %2").arg(markerName(type), formatHMS(m_markerTime[i]));
        const double tw = fm.horizontalAdvance(text) + 8;
        const double th = fm.height() + 2;
        const double y = wr.top() + 2 + (i % 3) * (th + 2);
        const bool flipLeft = (x + tw > wr.right());
        const QRectF flag(flipLeft ? x - tw : x, y, tw, th);
        p.fillRect(flag, col);
        p.setPen(col.lightness() > 150 ? Qt::black : Qt::white);
        p.drawText(flag, Qt::AlignCenter, text);
    }
    p.restore();
}

void WaveformWidget::showContextMenu(const QPoint &pos, const QPoint &globalPos)
{
    const double t = std::clamp(xToTime(pos.x()), 0.0, m_duration);
    QMenu menu(this);

    QMenu *setMenu = menu.addMenu(tr("Set marker here (%1)").arg(formatHMS(t)));
    for (int i = 0; i < MarkerCount; ++i) {
        const auto type = static_cast<MarkerType>(i);
        QAction *a = setMenu->addAction(markerName(type));
        connect(a, &QAction::triggered, this, [this, type, t] { setMarker(type, t); });
    }

    QMenu *rmMenu = menu.addMenu(tr("Remove marker"));
    bool any = false;
    for (int i = 0; i < MarkerCount; ++i) {
        if (m_markerTime[i] < 0) continue;
        const auto type = static_cast<MarkerType>(i);
        QAction *a = rmMenu->addAction(QString("%1  (%2)").arg(markerName(type), formatHMS(m_markerTime[i])));
        connect(a, &QAction::triggered, this, [this, type] { clearMarker(type); });
        any = true;
    }
    rmMenu->setEnabled(any);

    menu.addSeparator();
    QAction *clr = menu.addAction(tr("Clear selection"));
    clr->setEnabled(hasSelection());
    connect(clr, &QAction::triggered, this, &WaveformWidget::clearSelection);

    menu.exec(globalPos);
}
