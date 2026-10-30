#pragma once
#include <QWidget>

class QLabel;
class QTimer;
class QTime;

// Panel 1: digital clock, time spoken in words, and the current date.
class ClockPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ClockPanel(QWidget *parent = nullptr);

    // "Twenty minutes past Eight", "Quarter to Nine", ...
    static QString timeInWords(const QTime &t);

private slots:
    void refresh();

private:
    QLabel *m_timeLabel;
    QLabel *m_wordsLabel;
    QLabel *m_dateLabel;
    QTimer *m_timer;
};
