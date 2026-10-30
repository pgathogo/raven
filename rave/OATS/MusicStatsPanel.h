#pragma once
#include <QWidget>

class QLabel;
class QGroupBox;

// Panel 2: three group boxes (Music Remaining / Hour Remaining / Talk Time).
// Each box shows only its value; the group box title names it.
class MusicStatsPanel : public QWidget
{
    Q_OBJECT
public:
    explicit MusicStatsPanel(QWidget *parent = nullptr);

public slots:
    void setMusicRemaining(int seconds);
    void setHourRemaining(int seconds);
    void setTalkTime(int seconds);

private:
    QGroupBox *createStatBox(const QString &title, const QString &accent, QLabel *&valueLabel);
    static QString formatTime(int totalSeconds);

    QLabel *m_musicValue = nullptr;
    QLabel *m_hourValue  = nullptr;
    QLabel *m_talkValue  = nullptr;
};
