#include <format>

#include <QDateTime>
#include <QFileDialog>
#include <QVBoxLayout>

#include <QVideoWidget>
#include <QAudioOutput>

#include <QVideoSink>
#include <QVideoFrame>

#include <QTimer>
#include <QFileInfo>
#include <QDir>

#include "mediabrowser.h"
#include "ui_mediabrowser.h"

#include "../../../rave/framework/entitydatamodel.h"
#include "../../../rave/audio/audiotool.h"
#include "../../../utils/tools.h"


#include "advertmedia.h"
#include "advertmediaform.h"
#include "spot.h"
#include "client.h"
#include "ravensetup.h"

MediaBrowser::MediaBrowser(std::shared_ptr<TRAFFIK::Spot> spot,
                           std::shared_ptr<Client> client,
                           QWidget *parent) :
     QWidget(parent)
    ,ui(new Ui::MediaBrowser)
    ,m_spot{spot}
    ,m_client{client}
    ,m_advert_media{nullptr}
    ,m_media_player{nullptr}
    ,m_video_widget{nullptr}
    ,m_audio_output{nullptr}
{
    ui->setupUi(this);

    m_setup_edm = std::make_unique<EntityDataModel>(std::make_unique<RavenSetup>());
    m_setup_edm->all();
    if (m_setup_edm->count() > 0)
        m_setup = std::dynamic_pointer_cast<RavenSetup>(m_setup_edm->firstEntity());
    else
        m_setup = std::make_shared<RavenSetup>();

    connect(ui->btnImport, &QPushButton::clicked, this, &MediaBrowser::import_media);
    connect(ui->btnPlay, &QPushButton::clicked, this, &MediaBrowser::play_media);
    connect(ui->btnPause, &QPushButton::clicked, this, &MediaBrowser::pause_media);
    connect(ui->btnStop, &QPushButton::clicked, this, &MediaBrowser::stop_media);
    connect(ui->btnProperties, &QPushButton::clicked, this, &MediaBrowser::on_properties);

    m_advert_media = std::move(find_spot_advert_media(spot->id()));

    std::string full_filename = "";

    if (m_advert_media == nullptr) {

        m_advert_media = std::make_shared<PIXELPLAN::AdvertMedia>();

    }else{

        full_filename = std::format("{}{}",m_advert_media->media_path()->value(),
                                                m_advert_media->title()->value());


        // Some titles have file extension - avoid appending extra extension
        auto file_ext = get_extension(m_advert_media->title()->value());

        if (file_ext.empty()) {

            if (!m_advert_media->file_extension()->value().empty()) {
                 full_filename = full_filename +"."+m_advert_media->file_extension()->value();
            }
        }

    }

    m_media_filename =  QString::fromStdString(full_filename);

    ui->wigProperties->setVisible(false);

}

void MediaBrowser::show_media_file()
{
    m_audio_output = std::make_unique<QAudioOutput>();

    m_audio_output->setVolume(0.5);

    m_media_player = std::make_unique<QMediaPlayer>(this);
    connect(m_media_player.get(), &QMediaPlayer::mediaStatusChanged, this, &MediaBrowser::media_status_changed);
    m_media_player->setAudioOutput(m_audio_output.get());


    //m_media_filename = m_media_filename.replace("//","\\");
    //m_media_filename = m_media_filename.replace('/', '\\');

    qDebug() << "Media File: " << m_media_filename;


    m_media_player->setSource( QUrl(m_media_filename) );

    m_video_widget = new QVideoWidget(this);
    m_video_widget->setMinimumSize(QSize(640, 360));
    m_video_widget->setAspectRatioMode(Qt::KeepAspectRatio);

    m_video_widget->setFullScreen(true);

    m_media_player->setVideoOutput(m_video_widget);

    m_hlplayer = new QHBoxLayout(ui->wigPlayer);
    m_hlplayer->addWidget(m_video_widget);

    m_video_widget->show();
}


MediaBrowser::~MediaBrowser()
{
    if (m_media_player) {
        m_media_player->stop();
        m_media_player->setVideoOutput(nullptr);
        m_media_player->setAudioOutput(nullptr);
    }
    if (m_video_widget) {
        if (m_hlplayer) {
            m_hlplayer->removeWidget(m_video_widget);
        }
        m_video_widget->deleteLater();
        m_video_widget = nullptr;
    }
    delete ui;
}

std::shared_ptr<PIXELPLAN::AdvertMedia> MediaBrowser::advert_media()
{
    return m_advert_media;
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
                QString dt = QString::fromStdString(field_value);
                QDateTime cdate = QDateTime::fromString(dt, Qt::ISODate);
                qDebug() << cdate;
                am->set_created_at(cdate);
            }
            if (field_name == "client_id")
                am->set_client(stoi(field_value));
            if (field_name == "spot_id")
                am->set_spot(stoi(field_value));
            if (field_name == "file_extension")
                am->set_file_extension(field_value);
        }

        // We pick the latest entry!!!!
        break;

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
    m_advert_media->set_file_extension(fi.suffix().toStdString());
    m_advert_media->set_dest_path(m_setup->audio_folder()->value());

    m_advert_media->set_created_at(QDateTime().currentDateTime());
    m_advert_media->set_client(m_spot->client()->value());
    m_advert_media->set_spot(m_spot->id());

    auto advert_media_form = std::make_unique<AdvertMediaForm>(m_advert_media);
    if (advert_media_form->exec() > 0)
    {
        // m_advert_media->set_media_file(fi.fileName().toStdString());

        // std::string ext = fi.suffix().toStdString();
        // m_advert_media->set_src_filepath(media_filename.toStdString());
        // m_advert_media->set_dest_filepath(m_setup->audio_folder()->value()+
        //                                   m_advert_media->title()->value()+"."+ext);

        m_advert_media->setDBAction(DBAction::dbaCREATE);

        show_media(media_filename);

        emit audio_duration(m_advert_media->duration()->value());

        // EntityDataModel edm;
        // int id = edm.createEntityDB(*m_advert_media);

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
    //m_media_player = std::make_unique<QMediaPlayer>(this);
    //connect(m_media_player.get(), &QMediaPlayer::mediaStatusChanged, this, &MediaBrowser::on_media_status_changed);


    //m_media_player->stop();
    m_media_player->setSource(QUrl::fromLocalFile(media_filename));
    m_video_widget->show();
    make_property_widget();


    /*
    m_video_widget = new QVideoWidget(ui->wigPlayer);
    m_video_widget->setMinimumSize(QSize(580, 320));

    m_media_player->setVideoOutput(m_video_widget);

    m_audio_output = std::make_unique<QAudioOutput>();
    m_media_player->setAudioOutput(m_audio_output.get());
    m_audio_output->setVolume(0);

    //m_hlplayer = new QVBoxLayout(ui->wigPlayer);
    //m_hlplayer->addWidget(m_video_widget);
   */

}

void MediaBrowser::make_property_widget()
{

    QLabel* lbl_fname = new QLabel("Filename:");
    QLabel* fname_value =  new QLabel(m_advert_media->title()->to_qstring()+
                                     "."+m_advert_media->file_extension()->to_qstring());

    QLabel* lbl_filepath = new QLabel("Media Folder:");
    QLabel* filepath = new QLabel(m_advert_media->media_path()->to_qstring());

    AUDIO::AudioTool at;

    QLabel* lbl_duration = new QLabel("Duration: ");
    QLabel* duration = new QLabel(at.msec_to_time(m_advert_media->duration()->value()).toString("HH:mm:ss"));

    QLabel* lbl_aspect_ratio = new QLabel("Aspect Ratio:");
    QLabel* aspect_ratio = new QLabel(m_advert_media->aspect_ratio()->to_qstring());

    QLabel* lbl_resolution = new QLabel("Resolution:");
    QLabel* resolution = new QLabel(m_advert_media->resolution()->to_qstring());

    QLabel* lbl_creation_date = new QLabel("Creation Date:");
    QLabel* creation_date = new QLabel(m_advert_media->created_at()->value().toString("yyyy-MM-dd HH:mm:sszzz"));

    QGridLayout* grid_layout = new QGridLayout(ui->wigProperties);

    grid_layout->addWidget(lbl_fname, 0, 0);
    grid_layout->addWidget(fname_value, 0, 1);

    grid_layout->addWidget(lbl_filepath, 1, 0);
    grid_layout->addWidget(filepath, 1, 1);

    grid_layout->addWidget(lbl_duration, 2, 0);
    grid_layout->addWidget(duration, 2, 1);

    grid_layout->addWidget(lbl_aspect_ratio, 3, 0);
    grid_layout->addWidget(aspect_ratio, 3, 1);

    grid_layout->addWidget(lbl_resolution, 4, 0);
    grid_layout->addWidget(resolution, 4, 1);

    grid_layout->addWidget(lbl_creation_date, 5,0);
    grid_layout->addWidget(creation_date, 5,1);

    grid_layout->setColumnStretch(1, 1);


    QSpacerItem* vspacer = new QSpacerItem(20,40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    grid_layout->addItem(vspacer, 6, 0);

}

void MediaBrowser::on_properties(bool state)
{
    make_property_widget();
    ui->wigProperties->setVisible(ui->btnProperties->isChecked());
}

void MediaBrowser::media_status_changed(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::LoadedMedia)
    {
        qDebug() << " --- media_status_changed ---";

        m_media_player->play();
        QTimer::singleShot(100, m_media_player.get(), &QMediaPlayer::pause);
    }

}
/*
QString MediaBrowser::convertNetworkPath(const QString& urlPath)
{
    if (urlPath.startsWith("file://")) {
        // Remove file:// prefix
        QString path = urlPath.mid(7);

        // For network paths starting with // or a server name
        if (path.startsWith("//") || path.contains(":/")) {
            // Convert to Windows native UNC path
            // Replace forward slashes with backslashes for Windows
            path = path.replace('/', '\\');
            // Remove any remaining forward slashes at start
            if (path.startsWith("\\")) {
                path = path.mid(2); // Remove leading \\
            }
            return path;
        }
    }
    return urlPath;
}
*/

QString MediaBrowser::conv(const QString& urlpath)
{
    if (urlpath.startsWith("file://")) {
        // Remove file:// prefix
        QString path = urlpath.mid(7);

        if (path.startsWith("//") || path.contains(":/")) {
            // Convert to Windows native UNC path
            path = path.replace('/', '\\');
            if (path.startsWith("\\")) {
                path = path.mid(2);
            }
            return path;
        }

    }

    return urlpath;

}
