#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include <QtWidgets/QAbstractButton>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtGui/QBrush>
#include <QtGui/QColor>

class QPropertyAnimation;
class QPainter;

class ToggleButton : public QAbstractButton
{
    Q_OBJECT

    Q_PROPERTY(qreal mOffset READ offset WRITE setOffset)
    Q_PROPERTY(qreal glowStrength READ glowStrength WRITE setGlowStrength)

    public:
        explicit ToggleButton(int trackRadius, int thumbRadius, QWidget* parent = nullptr);
        ~ToggleButton();

        QSize sizeHint() const override;
        void setChecked(bool checked);

        // ---- Glow -------------------------------------------------------
        // Setting a valid color enables the glow; an invalid QColor() disables it.
        void setGlowColor(const QColor& color);
        QColor glowColor() const;               // falls back to palette highlight if unset

        void setGlowEnabled(bool enabled);
        bool isGlowEnabled() const;

        void setGlowRadius(int radius);         // glow thickness in pixels (default 10)
        int glowRadius() const;

        // true  (default): glow only while the button is ON (fades in/out)
        // false          : glow is always visible
        void setGlowOnlyWhenChecked(bool only);
        bool glowOnlyWhenChecked() const;

        // ---- State text ("Off" / "On") ---------------------------------
        void setStateTextVisible(bool visible);
        bool isStateTextVisible() const;
        void setStateTexts(const QString& onText, const QString& offText);

        // ---- Track / thumb colors ---------------------------------------
        // 'checked' selects which state the color applies to (true = ON, false = OFF).
        void setTrackColor(bool checked, const QColor& color);
        void setThumbColor(bool checked, const QColor& color);
        // Convenience: set both states at once.
        void setTrackColors(const QColor& onColor, const QColor& offColor);
        void setThumbColors(const QColor& onColor, const QColor& offColor);

    protected:
        void paintEvent(QPaintEvent *) override;
        void resizeEvent(QResizeEvent*) override;
        void enterEvent(QEnterEvent *event) override;
        bool hitButton(const QPoint& pos) const override;

        qreal offset() const;
        void setOffset(qreal value);

        qreal glowStrength() const;
        void setGlowStrength(qreal value);

    private slots:
        void onToggled(bool checked);

    private:
        int glowPadding() const;
        qreal glowTarget() const;
        void syncGlowImmediately();
        void paintGlow(QPainter& p, const QRectF& trackRect) const;
        void paintStateText(QPainter& p, const QBrush& trackBrush, qreal trackOpacity,
                            qreal logicalWidth, qreal logicalHeight, bool check) const;

        qreal mOffset;
        qreal mBaseOffset;
        qreal mMargin;
        qreal mTrackRadius;
        qreal mThumbRadius;
        qreal mOpacity;
        QPropertyAnimation* mAnimation;

        QHash<bool, qreal> mEndOffset;
        QHash<bool, QBrush> mTrackColor;
        QHash<bool, QBrush> mThumbColor;
        QHash<bool, QColor> mTextColor;
        QHash<bool, QString> mThumbText;

        // glow
        QColor mGlowColor;
        int mGlowRadius;
        bool mGlowEnabled;
        bool mGlowOnlyWhenChecked;
        qreal mGlowStrength;                    // 0..1, animated
        QPropertyAnimation* mGlowAnimation;

        // state text
        bool mStateTextVisible;
        QHash<bool, QString> mStateText;
    };

#endif // TOGGLEBUTTON_H
