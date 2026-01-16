#ifndef MEDIABROWSER_H
#define MEDIABROWSER_H

#include <QWidget>
#include <QMediaPlayer>

namespace Ui {
class MediaBrowser;
}

namespace TRAFFIK {
class Spot;
}

class QVideoWidget;
class QAudioOutput;
class QVBoxLayout;

class RavenSetup;
class EntityDataModel;

namespace PIXELPLAN {
class AdvertMedia;
}

class Client;

class MediaBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit MediaBrowser(std::shared_ptr<TRAFFIK::Spot>,
                          std::shared_ptr<Client>,
                          QWidget *parent = nullptr);
    ~MediaBrowser();

    std::shared_ptr<PIXELPLAN::AdvertMedia> advert_media();

    void show_media_file();

private slots:
    void import_media();
    void play_media();
    void pause_media();
    void stop_media();
    void media_status_changed(QMediaPlayer::MediaStatus);
    void on_properties(bool);

signals:
    void audio_duration(int);


private:
    std::shared_ptr<PIXELPLAN::AdvertMedia> find_spot_advert_media(int);
    void show_media(const QString media_filename);
    void make_property_widget();

    Ui::MediaBrowser *ui;

    std::shared_ptr<TRAFFIK::Spot> m_spot;
    std::shared_ptr<Client> m_client;
    std::shared_ptr<PIXELPLAN::AdvertMedia> m_advert_media;

    std::shared_ptr<RavenSetup> m_setup;
    std::unique_ptr<EntityDataModel> m_setup_edm;

    std::unique_ptr<QMediaPlayer> m_media_player;
    QVideoWidget* m_video_widget;
    std::unique_ptr<QAudioOutput> m_audio_output;

    QString m_media_filename;

    QHBoxLayout* m_hlplayer;


};

#endif // MEDIABROWSER_H
