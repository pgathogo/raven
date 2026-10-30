#pragma once
#include <QWidget>

class QLabel;

// Panel 3: play mode, title of the current audio, Go Current / Keep Current.
class NowPlayingPanel : public QWidget
{
    Q_OBJECT
public:
    explicit NowPlayingPanel(QWidget *parent = nullptr);

public slots:
    void setTitle(const QString &title);
    void setPlayMode(const QString &mode);

signals:
    void goCurrentClicked(bool);
    void keepCurrentClicked(bool);

private:
    QLabel *m_modeLabel;
    QLabel *m_titleLabel;
};
