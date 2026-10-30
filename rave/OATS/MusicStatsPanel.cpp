#include "MusicStatsPanel.h"
#include "PanelTheme.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>

MusicStatsPanel::MusicStatsPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(12);

    root->addWidget(createStatBox(tr("Music Remaining"), "#ff3b5c", m_musicValue));
    root->addWidget(createStatBox(tr("Hour Remaining"),  "#ff8a4c", m_hourValue));
    root->addWidget(createStatBox(tr("Talk Time"),       "#ffd166", m_talkValue));

    setMusicRemaining(0);
    setHourRemaining(0);
    setTalkTime(0);
}

QGroupBox *MusicStatsPanel::createStatBox(const QString &title, const QString &accent,
                                          QLabel *&valueLabel)
{
    auto *box = new QGroupBox(title);
    box->setObjectName("statBox");
    box->setStyleSheet(
        PanelTheme::cardStyle("QGroupBox#statBox", accent) +
        QString("QGroupBox#statBox { margin-top: 12px; padding-top: 6px;"
                "  font-size: 12px; font-weight: bold; color: %1; }"
                "QGroupBox#statBox::title { subcontrol-origin: margin;"
                "  subcontrol-position: top left; left: 14px; padding: 0 6px; }")
            .arg(accent));

    valueLabel = new QLabel("00:00");
    valueLabel->setAlignment(Qt::AlignCenter);
    valueLabel->setStyleSheet(QString(
        "background:transparent; color:%1; font-size:42px; font-weight:bold; %2")
        .arg(accent, PanelTheme::monoFamily()));
    PanelTheme::addGlow(valueLabel, QColor(accent), 22);

    auto *lay = new QHBoxLayout(box);
    lay->setContentsMargins(12, 8, 12, 8);
    lay->addWidget(valueLabel);
    return box;
}

QString MusicStatsPanel::formatTime(int s)
{
    const QString sign = s < 0 ? "-" : "";
    s = qAbs(s);
    return QString("%1%2:%3").arg(sign)
                             .arg(s / 60, 2, 10, QChar('0'))
                             .arg(s % 60, 2, 10, QChar('0'));
}

void MusicStatsPanel::setMusicRemaining(int s) { m_musicValue->setText(formatTime(s)); }
void MusicStatsPanel::setHourRemaining(int s)  { m_hourValue->setText(formatTime(s)); }
void MusicStatsPanel::setTalkTime(int s)       { m_talkValue->setText(formatTime(s)); }
