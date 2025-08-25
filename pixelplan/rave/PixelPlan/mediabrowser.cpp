#include <QDateTime>
#include <QFileDialog>

#include <QVideoWidget>
#include <QAudioOutput>

#include <QVideoSink>
#include <QVideoFrame>

#include <QTimer>

#include "mediabrowser.h"
#include "ui_mediabrowser.h"

#include "../../../rave/framework/entitydatamodel.h"

#include "advertmedia.h"
#include "advertmediaform.h"
#include "spot.h"
#include "ravensetup.h"

MediaBrowser::MediaBrowser(std::shared_ptr<TRAFFIK::Spot> spot, QWidget *parent) :
     QWidget(parent)
    ,ui(new Ui::MediaBrowser)
    ,m_spot{spot}
    ,m_advert_media{nullptr}
    ,m_media_player{nullptr}
    ,m_video_widget{nullptr}
    ,m_audio_output{nullptr}
{
    ui->setupUi(this);

    connect(ui->btnImport, &QPushButton::clicked, this, &MediaBrowser::import_media);
    connect(ui->btnPlay, &QPushButton::clicked, this, &MediaBrowser::play_media);
    connect(ui->btnPause, &QPushButton::clicked, this, &MediaBrowser::pause_media);
    connect(ui->btnStop, &QPushButton::clicked, this, &MediaBrowser::stop_media);

    m_advert_media = std::move(find_spot_advert_media(spot->id()));

    if (m_advert_media == nullptr) {
        m_advert_media = std::make_shared<PIXELPLAN::AdvertMedia>();
    }else{
        std::string full_filename = m_advert_media->media_path()->value() + m_advert_media->media_file()->value();
        show_media(QString::fromStdString(full_filename));
    }


    m_setup_edm = std::make_unique<EntityDataModel>(std::make_unique<RavenSetup>());
    m_setup_edm->all();
    if (m_setup_edm->count() > 0)
        m_setup = std::dynamic_pointer_cast<RavenSetup>(m_setup_edm->firstEntity());
    else
        m_setup = std::make_shared<RavenSetup>();

}

MediaBrowser::~MediaBrowser()
{
    delete ui;
}

std::shared_ptr<PIXELPLAN::AdvertMedia> MediaBrowser::find_spot_advert_media(int spot_id)
{
    auto edm = EntityDataModel(std::make_shared<PIXELPLAN::AdvertMedia>());

    auto am = std::make_shared<PIXELPLAN::AdvertMedia>();

    auto spot_filter = std::make_tuple(
        am->spot()->dbColumnName(),
        " = ",
        spot_id
        );
    std::string filter = edm.prepareFilter(spot_filter);

    edm.search(filter);

    if (edm.count() == 0)
        return nullptr;

    auto provider = edm.getDBManager()->provider();

    provider->cache()->first();


    do {
        auto itb = provider->cache()->currentElement()->begin();
        auto ite = provider->cache()->currentElement()->end();


        for(; itb != ite; ++itb) {
            std::string field_name = (*itb).first;
            std::string field_value = (*itb).second;

            if(field_name == "title")
                am->set_title(field_value);
            if(field_name == "media_file")
                am->set_media_file(field_value);
            if(field_name == "media_path")
                am->set_media_path(field_value);
            if (field_name == "duration")
                am->set_duration(stoi(field_value));
            if (field_name == "aspect_ratio")
                am->set_aspect_ratio(field_value);
            if (field_name == "resolution")
                am->set_resolution(field_value);
            if (field_name == "rating")
                am->set_rating(field_value);
            if (field_name == "created_at") {
                QDateTime cdate = QDateTime::fromString(QString::fromStdString(field_value));
                am->set_created_at(cdate);
            }
            if (field_name == "client_id")
                am->set_client(stoi(field_value));
            if (field_name == "spot_id")
                am->set_spot(stoi(field_value));
        }

        provider->cache()->next();
    } while(!provider->cache()->isLast());

    return std::move(am);

}

void MediaBrowser::import_media()
{
    auto media_filename = QFileDialog::getOpenFileName(this,
                                                       tr("Import Media"), QDir::currentPath(),
                                                       tr("MOV Files (*.mov);; MP4 Files (*.mp4);;"));

    if (media_filename.isEmpty())
        return;

    QFileInfo fi = QFileInfo(media_filename);

    m_advert_media->set_title(fi.fileName().toStdString());
    m_advert_media->set_media_file(media_filename.toStdString());
    m_advert_media->set_media_path(m_setup->audio_folder()->value());
    m_advert_media->set_created_at(QDateTime().currentDateTime());
    m_advert_media->set_client(m_spot->client()->value());
    m_advert_media->set_spot(m_spot->id());

    auto advert_media_form = std::make_unique<AdvertMediaForm>(m_advert_media);
    if (advert_media_form->exec() > 0)
    {
        qDebug() << "--------- ";
        qDebug() << m_advert_media->duration()->value();
        qDebug() << " -------- ";
        qDebug() << " -------- ";


        m_advert_media->set_media_file(fi.fileName().toStdString());

        EntityDataModel edm;
        int id = edm.createEntityDB(*m_advert_media);
        if (id > 0)
            show_media(media_filename);



    }
}




void MediaBrowser::play_media()
{
    m_audio_output->setVolume(0.5);

    m_media_player->play();

}

void MediaBrowser::pause_media()
{
    m_media_player->pause();
}

void MediaBrowser::stop_media()
{
    m_media_player->stop();
}


void MediaBrowser::show_media(const QString media_filename)
{
    m_media_player = std::make_unique<QMediaPlayer>(this);
    connect(m_media_player.get(), &QMediaPlayer::mediaStatusChanged, this, &MediaBrowser::on_media_status_changed);

    m_media_player->setSource(QUrl::fromLocalFile(media_filename));

    m_video_widget = std::make_unique<QVideoWidget>(this);
    m_video_widget->setMinimumSize(QSize(580, 320));

    m_media_player->setVideoOutput((m_video_widget.get()));

    m_audio_output = std::make_unique<QAudioOutput>();
    m_media_player->setAudioOutput(m_audio_output.get());
    m_audio_output->setVolume(0);

    ui->hlMedia->addWidget(m_video_widget.get());
    m_video_widget->show();

}

void MediaBrowser::on_media_status_changed(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::LoadedMedia)
    {
        m_media_player->play();
        QTimer::singleShot(100, m_media_player.get(), &QMediaPlayer::pause);
    }

}
