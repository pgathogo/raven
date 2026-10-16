#ifndef WAVEFORMWIDGET_H
#define WAVEFORMWIDGET_H

#include <QColor>
#include <QJsonObject>
#include <QMap>
#include <QRect>
#include <QString>
#include <QVector>
#include <QWidget>
#include <memory>
#include <vector>

class QScrollBar;
class QTimer;

namespace AUDIO { class AudioPlayer; }
struct WaveformData;                 // see audiothread.h

/**
 * WaveformWidget  (Qt 5.15 / Qt 6, BASS)
 *
 * Shows a stereo (left / right) energy waveform (peak envelope + RMS) of any
 * file BASS can decode (MP3, OGG, WAV, FLAC*, AAC*, MOD ... *with plugins) and
 * plays it with a moving playhead. Decoding and playback go through
 * AUDIO::AudioPlayer / AudioThread; no QtMultimedia is needed.
 *
 * Mouse / keyboard:
 *   Left click            seek
 *   Left drag             select a region (drag the edges to adjust)
 *   Right click           context menu (set / remove markers, clear selection)
 *   Esc                   clear selection (or cancel an armed marker)
 *   Ctrl + wheel          zoom around the cursor
 *   Wheel                 scroll horizontally
 *   H                     turn the selection into a permanent highlight
 *   1 ... 6               arm Start / FadeIn / Intro / Extro / FadeOut / End marker;
 *                         the next click on the waveform places it
 *   Drag a marker line    move that marker
 *   Space                 play / pause
 *   + / -                 zoom in / out
 */
class WaveformWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor waveColor READ leftWaveColor WRITE setWaveColor)
    Q_PROPERTY(QColor playheadColor READ playheadColor WRITE setPlayheadColor)
    Q_PROPERTY(QColor selectionColor READ selectionColor WRITE setSelectionColor)

public:
    enum class AmplitudeScale { Linear, Decibel };

    struct Highlight {
        double start;   // seconds
        double end;     // seconds
        QColor color;   // translucent colour is recommended
    };

    /// The six cue markers. Each type can be placed once.
    enum class MarkerType { Start = 0, FadeIn, Intro, Extro, FadeOut, End };
    static constexpr int MarkerCount = 6;

    explicit WaveformWidget(QWidget *parent = nullptr);
    ~WaveformWidget() override;

    // ---- loading -----------------------------------------------------
    /// Uses `player` for decoding and playback (the widget does not take
    /// ownership). Pass nullptr to go back to a private player. By default the
    /// widget creates and owns its own AudioPlayer.
    void setAudioPlayer(AUDIO::AudioPlayer *player);
    AUDIO::AudioPlayer *audioPlayer() const { return m_player; }

    /// Starts asynchronous decoding (on a worker thread). Returns false if the
    /// file does not exist (errorOccurred() is emitted in that case; files BASS
    /// cannot open are reported through errorOccurred() asynchronously).
    bool loadFile(const QString &path);
    bool hasAudio() const { return m_hasAudio; }
    double duration() const { return m_duration; }

    // ---- playback ----------------------------------------------------
    bool isPlaying() const;
    double playheadPosition() const { return m_playhead; }
    void setVolume(float volume01);

    // ---- zoom / view -------------------------------------------------
    double viewStart() const { return m_viewStart; }
    double visibleDuration() const { return m_viewDur; }

    // ---- selection / highlights --------------------------------------
    bool hasSelection() const { return m_selEnd > m_selStart; }
    double selectionStart() const { return m_selStart; }
    double selectionEnd() const { return m_selEnd; }
    const QVector<Highlight> &highlights() const { return m_highlights; }

    // ---- markers -----------------------------------------------------
    static QString markerName(MarkerType t);      // "Fade In"
    static QString markerKey(MarkerType t);       // "fadeIn"  (JSON key)
    /// Formats seconds as "hh:mm:ss" (or "hh:mm:ss.zzz" with withMillis).
    static QString formatHMS(double seconds, bool withMillis = false);

    bool hasMarker(MarkerType t) const { return m_markerTime[int(t)] >= 0; }
    /// Marker position in seconds, or -1 when the marker is not set.
    double markerTime(MarkerType t) const { return m_markerTime[int(t)]; }
    /// "hh:mm:ss" string for the marker, or an empty string when not set.
    QString markerTimeString(MarkerType t, bool withMillis = false) const;
    /// All markers that are currently set (type -> seconds).
    QMap<MarkerType, double> markers() const;
    /// Serialises the markers, e.g. {"start":{"seconds":1.5,"time":"00:00:01","timeMs":"00:00:01.500"}, ...}
    QJsonObject markersToJson() const;
    /// Restores markers saved by markersToJson(). Call after loaded() was emitted.
    /// Existing markers are replaced. Returns false if audio isn't loaded or a value was invalid.
    bool markersFromJson(const QJsonObject &obj);
    /// True when the markers that are set appear in order Start <= FadeIn <= ... <= End.
    bool isMarkerOrderValid() const;
    bool isMarkerArmed() const { return m_armed; }

    void setMarkerColor(MarkerType t, const QColor &c);
    QColor markerColor(MarkerType t) const { return m_markerColors[int(t)]; }
    void setShowMarkers(bool on);

    // ---- appearance --------------------------------------------------
    void setWaveColor(const QColor &c);                         // both channels
    void setChannelColors(const QColor &left, const QColor &right);
    void setBackgroundColor(const QColor &c);
    void setPlayheadColor(const QColor &c);
    void setSelectionColor(const QColor &c);
    void setRulerColor(const QColor &background, const QColor &text);
    void setGridColor(const QColor &c);
    void setShowRms(bool on);
    void setAmplitudeScale(AmplitudeScale s);                   // Linear or Decibel
    void setMinimumDecibels(double db);                         // e.g. -60 (Decibel scale)
    void setFollowPlayhead(bool on) { m_follow = on; }
    void setLoopSelection(bool on) { m_loopSelection = on; }

    QColor leftWaveColor() const { return m_waveColor[0]; }
    QColor rightWaveColor() const { return m_waveColor[1]; }
    QColor backgroundColor() const { return m_bgColor; }
    QColor playheadColor() const { return m_playheadColor; }
    QColor selectionColor() const { return m_selectionColor; }
    AmplitudeScale amplitudeScale() const { return m_scale; }

public slots:
    void play();
    void pause();
    void stop();
    void togglePlayPause();
    void playSelection();                       // plays (or loops) the selected region
    void setPlayheadPosition(double seconds);

    void zoomIn();
    void zoomOut();
    void zoomToFit();
    void zoomToSelection();
    void zoomAt(double factor, double anchorSeconds);
    void setViewStart(double seconds);

    void setSelection(double startSeconds, double endSeconds);
    void clearSelection();
    void addHighlight(double start, double end, const QColor &color = QColor(255, 200, 0, 70));
    void addSelectionAsHighlight(const QColor &color = QColor(255, 200, 0, 70));
    void clearHighlights();

    /// Sets / moves a marker programmatically.
    void setMarker(MarkerType type, double seconds);
    void clearMarker(MarkerType type);
    void clearMarkers();
    /// The next left click on the waveform places this marker (instead of seeking).
    void armMarker(MarkerType type);
    void disarmMarker();

signals:
    void loadingProgress(int percent);
    void loaded(double durationSeconds);
    void errorOccurred(const QString &message);
    void positionChanged(double seconds);
    void playbackStateChanged(bool playing);
    void selectionChanged(double start, double end);   // start == end -> no selection
    void viewChanged(double start, double visibleDuration);
    void highlightsChanged();
    /// seconds == -1 means the marker was removed.
    void markerChanged(WaveformWidget::MarkerType type, double seconds);
    void armedMarkerChanged(bool armed, WaveformWidget::MarkerType type);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    QSize sizeHint() const override { return {900, 360}; }

private:
    struct Tick { int db; float norm; bool labeled; };
    enum class Drag { None, Pending, Selecting, EditEdge, Marker };

    // decoding
    void clearData();
    void onWaveformReady(const std::shared_ptr<WaveformData> &data);
    void bindPlayer();

    // geometry helpers
    QRect waveRect() const;
    QRect laneRect(int ch) const;
    double timeToX(double t) const;
    double xToTime(double x) const;
    double minVisibleDuration() const;
    double clampStart(double s) const;
    void updateScrollBar();

    // scale helpers
    float dbToNorm(double db) const;
    float ampToNorm(float amplitude) const;     // amplitude >= 0 -> 0..1
    QVector<Tick> computeTicks(double halfHeight) const;

    // painting
    void drawLane(QPainter &p, int ch);
    void drawWave(QPainter &p, int ch);
    void drawDbRuler(QPainter &p, int ch);
    void drawTimeRuler(QPainter &p);
    void drawPlayhead(QPainter &p);
    static QString formatTime(double t, double step);

    // playback
    void onTick();
    void startAt(double seconds);               // play / seek+resume from seconds
    void ensurePlayheadVisible();

    // markers
    int markerAt(double x) const;               // marker index near pixel x, or -1
    void drawMarkers(QPainter &p);
    void showContextMenu(const QPoint &pos, const QPoint &globalPos);

    // decoding / playback objects
    AUDIO::AudioPlayer *m_player = nullptr;
    bool m_ownsPlayer = false;
    QString m_path;                             // currently loaded file
    QScrollBar *m_scrollBar = nullptr;
    QTimer *m_timer = nullptr;

    // audio summary data
    std::shared_ptr<const WaveformData> m_wave;
    int m_sampleRate = 0;
    double m_duration = 0.0;
    bool m_hasAudio = false;
    bool m_decoding = false;
    int m_progress = 0;

    // view state
    double m_viewStart = 0.0;
    double m_viewDur = 1.0;
    double m_playhead = 0.0;
    bool m_follow = true;
    bool m_updatingScrollBar = false;

    // selection / highlights
    double m_selStart = 0.0, m_selEnd = 0.0;
    QVector<Highlight> m_highlights;
    Drag m_drag = Drag::None;
    double m_dragAnchor = 0.0;
    double m_pressX = 0.0;
    bool m_stopAtEnd = false;
    bool m_loopSelection = false;
    double m_regionStart = 0.0, m_regionEnd = 0.0;

    // markers
    double m_markerTime[MarkerCount] = {-1, -1, -1, -1, -1, -1};     // seconds, -1 = unset
    QColor m_markerColors[MarkerCount] = {
        QColor(76, 175, 80),   // Start    green
        QColor(38, 198, 218),  // FadeIn   cyan
        QColor(171, 71, 188),  // Intro    purple
        QColor(236, 64, 122),  // Extro    pink
        QColor(255, 112, 67),  // FadeOut  orange
        QColor(239, 83, 80)    // End      red
    };
    bool m_showMarkers = true;
    bool m_armed = false;
    MarkerType m_armedType = MarkerType::Start;
    int m_dragMarker = -1;

    // appearance
    QColor m_waveColor[2] = {QColor(64, 190, 255), QColor(255, 120, 90)};
    QColor m_bgColor = QColor(24, 26, 30);
    QColor m_rulerBg = QColor(38, 41, 47);
    QColor m_rulerText = QColor(170, 175, 185);
    QColor m_gridColor = QColor(255, 255, 255, 28);
    QColor m_playheadColor = QColor(255, 235, 60);
    QColor m_selectionColor = QColor(120, 160, 255, 80);
    bool m_showRms = true;
    AmplitudeScale m_scale = AmplitudeScale::Linear;
    double m_minDb = -60.0;
};

#endif // WAVEFORMWIDGET_H
