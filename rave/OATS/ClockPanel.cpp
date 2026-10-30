#include "ClockPanel.h"
#include "PanelTheme.h"

#include <QDate>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>

namespace {

const char *kHours[] = {"Twelve", "One", "Two", "Three", "Four", "Five", "Six",
                        "Seven", "Eight", "Nine", "Ten", "Eleven"};

const char *kSmall[] = {"", "one", "two", "three", "four", "five", "six", "seven",
                        "eight", "nine", "ten", "eleven", "twelve", "thirteen",
                        "fourteen", "fifteen", "sixteen", "seventeen", "eighteen",
                        "nineteen"};

QString minuteWords(int n) // 1..29
{
    QString s = (n < 20) ? QString::fromLatin1(kSmall[n])
                         : QStringLiteral("twenty")
                               + (n % 10 ? QStringLiteral(" ") + QString::fromLatin1(kSmall[n % 10])
                                         : QString());
    s[0] = s[0].toUpper();
    return s;
}

} // namespace

ClockPanel::ClockPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(10);

    // ---- Left card: digital time + words -------------------------------
    auto *timeCard = new QFrame;
    timeCard->setObjectName("timeCard");
    timeCard->setStyleSheet(PanelTheme::cardStyle("QFrame#timeCard", "#2f9bd6"));

    m_timeLabel = new QLabel("00:00:00");
    m_timeLabel->setStyleSheet(QString(
        "background:transparent; color:#39ff6a; font-size:52px; font-weight:bold; %1")
        .arg(PanelTheme::monoFamily()));
    PanelTheme::addGlow(m_timeLabel, QColor("#39ff6a"), 30);

    m_wordsLabel = new QLabel;
    m_wordsLabel->setAlignment(Qt::AlignCenter);
    m_wordsLabel->setStyleSheet(
        "background:transparent; color:#f4f8ff; font-size:28px; font-weight:600;");

    auto *timeLay = new QHBoxLayout(timeCard);
    timeLay->setContentsMargins(18, 14, 18, 14);
    timeLay->addWidget(m_timeLabel, 0, Qt::AlignBottom);
    timeLay->addWidget(m_wordsLabel, 1);

    // ---- Right card: date ---------------------------------------------
    auto *dateCard = new QFrame;
    dateCard->setObjectName("dateCard");
    dateCard->setStyleSheet(PanelTheme::cardStyle("QFrame#dateCard", "#2f9bd6"));

    m_dateLabel = new QLabel;
    m_dateLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
    m_dateLabel->setStyleSheet(
        "background:transparent; color:#22d3ee; font-size:16px; font-weight:bold;");
    PanelTheme::addGlow(m_dateLabel, QColor("#22d3ee"), 14);

    auto *dateLay = new QVBoxLayout(dateCard);
    dateLay->setContentsMargins(14, 10, 14, 10);
    dateLay->addWidget(m_dateLabel);
    dateLay->addStretch();

    root->addWidget(timeCard, 4);
    root->addWidget(dateCard, 1);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &ClockPanel::refresh);
    m_timer->start(250);
    refresh();
}

void ClockPanel::refresh()
{
    const QDateTime now = QDateTime::currentDateTime();
    m_timeLabel->setText(now.time().toString("HH:mm:ss"));
    m_wordsLabel->setText(timeInWords(now.time()));
    m_dateLabel->setText(QLocale::c().toString(now.date(), "dddd, d MMMM yyyy"));
}

QString ClockPanel::timeInWords(const QTime &t)
{
    const int m = t.minute();
    int h = t.hour() % 12;
    const QString cur = kHours[h];
    const QString next = kHours[(h + 1) % 12];

    if (m == 0)  return cur + " o'clock";
    if (m == 15) return "Quarter past " + cur;
    if (m == 30) return "Half past " + cur;
    if (m == 45) return "Quarter to " + next;

    if (m < 30)
        return QString("%1 minute%2 past %3").arg(minuteWords(m), m == 1 ? "" : "s", cur);

    const int rem = 60 - m;
    return QString("%1 minute%2 to %3").arg(minuteWords(rem), rem == 1 ? "" : "s", next);
}
