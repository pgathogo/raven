#pragma once
// Shared look & feel for the radio automation panels.
#include <QColor>
#include <QGraphicsDropShadowEffect>
#include <QString>
#include <QWidget>

namespace PanelTheme {

// Rounded "glass" card with a dark gradient and a coloured border.
inline QString cardStyle(const QString &selector, const QString &accent)
{
    return QString(
        "%1 {"
        "  background: qlineargradient(x1:0,y1:0,x2:0,y2:1,"
        "              stop:0 #2d394b, stop:1 #171d28);"
        "  border: 1px solid %2;"
        "  border-radius: 12px;"
        "}").arg(selector, accent);
}

// Soft neon glow behind a widget (used for the big digits).
inline void addGlow(QWidget *w, const QColor &c, int blur = 28)
{
    auto *fx = new QGraphicsDropShadowEffect(w);
    fx->setOffset(0, 0);
    fx->setBlurRadius(blur);
    fx->setColor(c);
    w->setGraphicsEffect(fx);
}

inline const char *monoFamily()
{
    return "font-family: 'Consolas','DejaVu Sans Mono','Courier New',monospace;";
}

} // namespace PanelTheme
