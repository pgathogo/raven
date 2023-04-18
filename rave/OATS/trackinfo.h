#pragma once

#include <memory>

#include <QWidget>

namespace Ui {
class TrackInfo;
}

namespace AUDIO{
  class Audio;
}

class AudioHistoryForm;

using History = std::map<int, std::vector<int>>;

class TrackInfo : public QWidget
{
    Q_OBJECT

public:
    explicit TrackInfo(QWidget *parent = nullptr);
    ~TrackInfo();

    void show_audio_info(const std::shared_ptr<AUDIO::Audio>, History&);
    QTime msec_to_time(int h, int ms);

private:
    Ui::TrackInfo *ui;

    std::unique_ptr<AudioHistoryForm> m_audio_history;

};

