#include "togglebutton.h"

#include <QtCore/QEvent>
#include <QtCore/QCoreApplication>
#include <QtCore/QPropertyAnimation>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QMouseEvent>
#include <QtGui/QPalette>

///<summary>
/// Toggle button has 2 different design. In the first design, if the ball (thumb) radius is
/// larger than the slide (track) radius, a flat ball slides and colors from the slide according to
/// the on / off situation. In the second design, if the ball radius is smaller than the slide radius,
/// the ball moves according to the on / off status inside the slide and includes the check and uncheck marks.
///
/// Additions:
///  - Glow: setGlowColor() / setGlowRadius() / setGlowEnabled() / setGlowOnlyWhenChecked().
///    The glow fades in and out when the button is toggled.
///  - State text: a tiny "Off" / "On" label is drawn in the free part of the track,
///    on the side opposite to the thumb.
///</summary>
ToggleButton::ToggleButton(int trackRadius, int thumbRadius, QWidget* parent)
    : QAbstractButton(parent)
    , mGlowRadius(10)
    , mGlowEnabled(false)
    , mGlowOnlyWhenChecked(true)
    , mGlowStrength(0.0)
    , mStateTextVisible(true)
{
    setCheckable(true);
    setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
    mTrackRadius = trackRadius;
    mThumbRadius = thumbRadius;

    mAnimation = new QPropertyAnimation(this);
    mAnimation->setTargetObject(this);
    mAnimation->setPropertyName("mOffset");
    mAnimation->setDuration(120);

    mGlowAnimation = new QPropertyAnimation(this);
    mGlowAnimation->setTargetObject(this);
    mGlowAnimation->setPropertyName("glowStrength");
    mGlowAnimation->setDuration(200);

    mStateText.insert(true, QStringLiteral("On"));
    mStateText.insert(false, QStringLiteral("Off"));

    mMargin = 0 > (mThumbRadius - mTrackRadius) ? 0 : (mThumbRadius - mTrackRadius);
    mBaseOffset = mThumbRadius > mTrackRadius ? mThumbRadius : mTrackRadius;
    mEndOffset.insert(true, 4 * mTrackRadius + 2 * mMargin - mBaseOffset); // width - offset
    mEndOffset.insert(false, mBaseOffset);
    mOffset = mBaseOffset;
    QPalette palette = this->palette();

    if (mThumbRadius > mTrackRadius)
    {
        mTrackColor.insert(true, palette.highlight());
        mTrackColor.insert(false, palette.dark());
        mThumbColor.insert(true, palette.highlight());
        mThumbColor.insert(false, palette.light());
        mTextColor.insert(true, palette.highlightedText().color());
        mTextColor.insert(false, palette.dark().color());
        mThumbText.insert(true, "");
        mThumbText.insert(false, "");
        mOpacity = 0.5;
    }
    else
    {
        mTrackColor.insert(true, palette.highlight());
        mTrackColor.insert(false, palette.dark());
        mThumbColor.insert(true, palette.highlightedText());
        mThumbColor.insert(false, palette.light());
        mTextColor.insert(true, palette.highlight().color());
        mTextColor.insert(false, palette.dark().color());
        mThumbText.insert(true, QChar(0x2714)); // check character
        mThumbText.insert(false, QChar(0x2715)); // uncheck character
        mOpacity = 1.0;
    }

    // Animate thumb + glow on every state change (mouse, keyboard, toggle(), click()).
    connect(this, &QAbstractButton::toggled, this, &ToggleButton::onToggled);
}


ToggleButton::~ToggleButton()
{
    delete mAnimation;
    delete mGlowAnimation;
}

// ---------------------------------------------------------------------------
// Geometry
// ---------------------------------------------------------------------------

int ToggleButton::glowPadding() const
{
    return mGlowEnabled ? mGlowRadius : 0;
}

QSize ToggleButton::sizeHint() const
{
    const int pad = glowPadding();
    int w = 4 * mTrackRadius + 2 * mMargin + 2 * pad;
    int h = 2 * mTrackRadius + 2 * mMargin + 2 * pad;

    return QSize(w, h);
}

bool ToggleButton::hitButton(const QPoint& pos) const
{
    // The glow padding must not react to clicks.
    const int pad = glowPadding();
    return QRect(pad, pad, width() - 2 * pad, height() - 2 * pad).contains(pos);
}

// ---------------------------------------------------------------------------
// Painting
// ---------------------------------------------------------------------------

void ToggleButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QPainter::RenderHints m_paintFlags = QPainter::RenderHints(QPainter::Antialiasing |
                                                               QPainter::TextAntialiasing);
    p.setRenderHints(m_paintFlags, true);
    p.setPen(Qt::NoPen);

    // Everything below is drawn in "logical" coordinates, i.e. without the glow padding.
    const int pad = glowPadding();
    p.translate(pad, pad);
    const qreal lw = width() - 2 * pad;
    const qreal lh = height() - 2 * pad;

    bool check = isChecked();
    qreal trackOpacity = mOpacity;
    qreal textOpacity = 1.0;
    qreal thumbOpacity = 1.0;
    QBrush trackBrush;
    QBrush thumbBrush;
    QColor textColor;

    if (this->isEnabled())
    {

        trackBrush = mTrackColor[check];
        thumbBrush = mThumbColor[check];
        textColor = mTextColor[check];
    }
    else
    {
        trackOpacity *= 0.8;
        trackBrush = this->palette().shadow();
        thumbBrush = this->palette().mid();
        textColor = this->palette().shadow().color();
    }

    const QRectF trackRect(mMargin, mMargin, lw - 2 * mMargin, lh - 2 * mMargin);

    // Glow goes underneath everything else.
    paintGlow(p, trackRect);

    p.setPen(Qt::NoPen);
    p.setBrush(trackBrush);
    p.setOpacity(trackOpacity);
    p.drawRoundedRect(trackRect, mTrackRadius, mTrackRadius);

    // Tiny "On"/"Off" text in the free part of the track.
    paintStateText(p, trackBrush, trackOpacity, lw, lh, check);

    p.setPen(Qt::NoPen);
    p.setBrush(thumbBrush);
    p.setOpacity(thumbOpacity);
    p.drawEllipse(QRectF(mOffset - mThumbRadius, mBaseOffset - mThumbRadius, 2 * mThumbRadius, 2 * mThumbRadius));

    p.setPen(textColor);
    p.setOpacity(textOpacity);
    QFont font = p.font();
    font.setPixelSize(1.5*mThumbRadius);
    p.setFont(font);


    // Since the antialiasasing provided by the drawText function is incompetent,
    // DrawPath function preferred. But since the drawPath function is not capable of aligment,
    // Pixel offsets calculated to provide aligment.
    QPainterPath textPath;
    qreal pixelOffset = (qreal)mThumbRadius * (1 - 1 / 1.414);
    textPath.addText(mOffset - mThumbRadius + pixelOffset, mBaseOffset + mThumbRadius - pixelOffset, font, mThumbText.value(check));
    p.drawPath(textPath);
}

void ToggleButton::paintGlow(QPainter& p, const QRectF& trackRect) const
{
    if (!mGlowEnabled || mGlowRadius <= 0 || mGlowStrength <= 0.0 || !isEnabled())
        return;

    p.save();
    p.setBrush(Qt::NoBrush);
    p.setOpacity(1.0);

    QColor c = glowColor();
    for (int i = 1; i <= mGlowRadius; ++i)
    {
        // Quadratic falloff: bright next to the track, transparent at the outer edge.
        const qreal t = 1.0 - static_cast<qreal>(i - 1) / mGlowRadius;
        c.setAlphaF(qBound<qreal>(0.0, 0.55 * t * t * mGlowStrength, 1.0));
        p.setPen(QPen(c, 1.2));

        const qreal d = i - 0.5;
        p.drawRoundedRect(trackRect.adjusted(-d, -d, d, d), mTrackRadius + d, mTrackRadius + d);
    }
    p.restore();
}

void ToggleButton::paintStateText(QPainter& p, const QBrush& trackBrush, qreal trackOpacity,
                                  qreal logicalWidth, qreal logicalHeight, bool check) const
{
    if (!mStateTextVisible)
        return;

    const QString text = mStateText.value(check);
    if (text.isEmpty())
        return;

    // Free area of the track = the side the thumb is NOT on.
    const qreal inset = mTrackRadius * 0.45;
    const qreal top = mMargin;
    const qreal h = logicalHeight - 2 * mMargin;
    QRectF r;
    if (check)
    {
        const qreal left = mMargin + inset;
        const qreal right = mEndOffset.value(true) - mThumbRadius - 1;
        r = QRectF(left, top, right - left, h);
    }
    else
    {
        const qreal left = mEndOffset.value(false) + mThumbRadius + 1;
        const qreal right = logicalWidth - mMargin - inset;
        r = QRectF(left, top, right - left, h);
    }
    if (r.width() <= 0)
        return;

    // Pick black/white text depending on what the track looks like on the parent background.
    const QColor base = trackBrush.color();
    const QColor win = palette().window().color();
    const QColor blended(
        qRound(base.red()   * trackOpacity + win.red()   * (1.0 - trackOpacity)),
        qRound(base.green() * trackOpacity + win.green() * (1.0 - trackOpacity)),
        qRound(base.blue()  * trackOpacity + win.blue()  * (1.0 - trackOpacity)));
    const QColor textColor = blended.lightnessF() > 0.55 ? QColor(20, 20, 20) : QColor(Qt::white);

    QFont f = font();
    f.setPixelSize(qMax(6, qRound(mTrackRadius * 0.55)));

    p.save();
    p.setFont(f);
    p.setPen(textColor);
    p.setOpacity(isEnabled() ? 1.0 : 0.6);
    p.drawText(r, Qt::AlignCenter, text);
    p.restore();
}

void ToggleButton::resizeEvent(QResizeEvent* e)
{
    QAbstractButton::resizeEvent(e);
    mOffset = mEndOffset.value(isChecked());
}

void ToggleButton::enterEvent(QEnterEvent * event)
{
    setCursor(Qt::PointingHandCursor);
    QAbstractButton::enterEvent(event);
}

// ---------------------------------------------------------------------------
// State handling / animation
// ---------------------------------------------------------------------------

void ToggleButton::onToggled(bool checked)
{
    mAnimation->stop();
    mAnimation->setStartValue(mOffset);
    mAnimation->setEndValue(mEndOffset.value(checked));
    mAnimation->start();

    mGlowAnimation->stop();
    mGlowAnimation->setStartValue(mGlowStrength);
    mGlowAnimation->setEndValue(glowTarget());
    mGlowAnimation->start();
}

void ToggleButton::setChecked(bool checked)
{
    QAbstractButton::setChecked(checked);

    // Programmatic change: jump straight to the final state (no animation).
    mAnimation->stop();
    mOffset = mEndOffset.value(checked);
    syncGlowImmediately();
}

qreal ToggleButton::offset() const
{
    return mOffset;
}

void ToggleButton::setOffset(qreal value)
{
    mOffset = value;
    update();
}

// ---------------------------------------------------------------------------
// Glow API
// ---------------------------------------------------------------------------

qreal ToggleButton::glowTarget() const
{
    if (!mGlowEnabled)
        return 0.0;
    return (!mGlowOnlyWhenChecked || isChecked()) ? 1.0 : 0.0;
}

void ToggleButton::syncGlowImmediately()
{
    mGlowAnimation->stop();
    mGlowStrength = glowTarget();
    update();
}

qreal ToggleButton::glowStrength() const
{
    return mGlowStrength;
}

void ToggleButton::setGlowStrength(qreal value)
{
    mGlowStrength = value;
    update();
}

void ToggleButton::setGlowColor(const QColor& color)
{
    mGlowColor = color;
    if (color.isValid())
    {
        if (!mGlowEnabled)
            setGlowEnabled(true);   // also repaints
        else
            update();
    }
    else
    {
        setGlowEnabled(false);
    }
}

QColor ToggleButton::glowColor() const
{
    return mGlowColor.isValid() ? mGlowColor : palette().highlight().color();
}

void ToggleButton::setGlowEnabled(bool enabled)
{
    if (mGlowEnabled == enabled)
        return;

    mGlowEnabled = enabled;
    syncGlowImmediately();
    updateGeometry();   // glow padding changes the size hint
    adjustSize();
}

bool ToggleButton::isGlowEnabled() const
{
    return mGlowEnabled;
}

void ToggleButton::setGlowRadius(int radius)
{
    radius = qMax(0, radius);
    if (mGlowRadius == radius)
        return;

    mGlowRadius = radius;
    updateGeometry();
    adjustSize();
    update();
}

int ToggleButton::glowRadius() const
{
    return mGlowRadius;
}

void ToggleButton::setGlowOnlyWhenChecked(bool only)
{
    mGlowOnlyWhenChecked = only;
    syncGlowImmediately();
}

bool ToggleButton::glowOnlyWhenChecked() const
{
    return mGlowOnlyWhenChecked;
}

// ---------------------------------------------------------------------------
// State text API
// ---------------------------------------------------------------------------

void ToggleButton::setStateTextVisible(bool visible)
{
    mStateTextVisible = visible;
    update();
}

bool ToggleButton::isStateTextVisible() const
{
    return mStateTextVisible;
}

void ToggleButton::setStateTexts(const QString& onText, const QString& offText)
{
    mStateText.insert(true, onText);
    mStateText.insert(false, offText);
    update();
}

// ---------------------------------------------------------------------------
// Track / thumb color API
// ---------------------------------------------------------------------------

void ToggleButton::setTrackColor(bool checked, const QColor& color)
{
    if (!color.isValid())
        return;

    mTrackColor.insert(checked, QBrush(color));

    // In the "ball inside the track" design the check/cross mark is drawn on the thumb
    // in the track's color, so keep it in sync.
    if (mThumbRadius <= mTrackRadius)
        mTextColor.insert(checked, color);

    update();
}

void ToggleButton::setThumbColor(bool checked, const QColor& color)
{
    if (!color.isValid())
        return;

    mThumbColor.insert(checked, QBrush(color));
    update();
}

void ToggleButton::setTrackColors(const QColor& onColor, const QColor& offColor)
{
    setTrackColor(true, onColor);
    setTrackColor(false, offColor);
}

void ToggleButton::setThumbColors(const QColor& onColor, const QColor& offColor)
{
    setThumbColor(true, onColor);
    setThumbColor(false, offColor);
}
