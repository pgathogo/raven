#include <QComboBox>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QTimer>

#include "advertmediaform.h"
#include "ui_advertmediaform.h"
#include "../../../rave/framework/ui_baseentitydetaildlg.h"
#include "../../../rave/framework/choicefield.h"

#include "advertmedia.h"

AdvertMediaForm::AdvertMediaForm(std::shared_ptr<PIXELPLAN::AdvertMedia> advert_media,
                             FormMode form_mode,
                             QDialog* parent
                                 )
    : BaseEntityDetailDlg(parent)
    , ui(new Ui::AdvertMediaForm)
    , m_advert_media{advert_media}
    , m_form_mode{form_mode}
    , m_media_player{nullptr}
    , m_video_widget{nullptr}
    , m_audio_output{nullptr}
{

    ui->setupUi(bui->baseContainer);

    m_title_tag = (advert_media->id() == -1) ? "New" : "Edit";

    connect(ui->btnPlay, &QPushButton::clicked, this, &AdvertMediaForm::play_media);
    connect(ui->btnStop, &QPushButton::clicked, this, &AdvertMediaForm::stop_media);

    setTitle(windowTitle());
    setMinimumWidth(685);
    setMinimumHeight(585);

    populateFormWidgets();
}

AdvertMediaForm::~AdvertMediaForm()
{
    delete ui;
}

ActionResult AdvertMediaForm::saveRecord()
{
    populateEntityFields();
    return m_advert_media->validate();
}

std::string AdvertMediaForm::windowTitle()
{
    return "Media Details- "+m_title_tag+" Media";

}

void AdvertMediaForm::populateEntityFields()
{
    m_advert_media->set_title(ui->edtTitle->text().trimmed().toStdString());
    m_advert_media->set_media_file(ui->edtFilename->text().toStdString());
    m_advert_media->set_media_path(ui->edtFolder->text().toStdString());
    set_choice_field(m_advert_media->rating(), ui->cbRating);

    std::string ext = m_advert_media->file_extension()->value();

    m_advert_media->set_src_filepath(m_advert_media->media_file()->value());

    // If you are editing a spot, check if we have a media file with the
    // same name

    std::string title = "";
    if (m_advert_media->spot()->value() > -1)
    {
        title = std::to_string(m_advert_media->spot()->value())+"_"+
                m_advert_media->title()->value();
        m_advert_media->set_title(title);
    } else {
        title = m_advert_media->title()->value();
    }

    m_advert_media->set_dest_filepath(m_advert_media->dest_path()->value()+
                      title+"."+ext);
}

void AdvertMediaForm::AdvertMediaForm::populateFormWidgets()
{
    ui->edtTitle->setText(m_advert_media->title()->to_qstring());
    ui->edtFilename->setText(m_advert_media->media_file()->to_qstring());
    ui->edtFolder->setText(m_advert_media->media_path()->to_qstring());
    ui->dtCreateDate->setDateTime(m_advert_media->created_at()->value());

    populate_rating_combo(ui->cbRating, m_advert_media->rating());


    load_media(m_advert_media->media_file()->to_qstring());

}

void AdvertMediaForm::load_media(const QString media_filename)
{
    m_media_player = std::make_unique<QMediaPlayer>(this);
    connect(m_media_player.get(), &QMediaPlayer::mediaStatusChanged, this, &AdvertMediaForm::on_media_status_changed);
    connect(m_media_player.get(), &QMediaPlayer::durationChanged, this, &AdvertMediaForm::on_duration_changed);

    m_media_player->setSource(QUrl::fromLocalFile(media_filename));

    m_video_widget = std::make_unique<QVideoWidget>(this);
    m_video_widget->setMinimumSize(QSize(150,200));
    m_video_widget->setFullScreen(true);

    m_media_player->setVideoOutput(m_video_widget.get());

    m_audio_output = std::make_unique<QAudioOutput>();
    m_media_player->setAudioOutput(m_audio_output.get());

    m_audio_output->setVolume(0.5);

    ui->vlMedia->addWidget(m_video_widget.get());
    m_video_widget->show();

}

void AdvertMediaForm::populate_rating_combo(QComboBox* cbox, const ChoiceField<std::string>* cf)
{
  for (const auto &c : cf->choices())
    cbox->addItem(stoq(std::get<1>(c)), stoq(std::get<0>(c)));

  setChoiceFieldDefault(cbox, cf->value());

}

void AdvertMediaForm::setChoiceFieldDefault(QComboBox *cbox, const std::string val) {
  cbox->setCurrentIndex(cbox->findData(QVariant(stoq(val))));
}

void AdvertMediaForm::set_choice_field(ChoiceField<std::string>* cf, QComboBox* cbox)
{
    cf->setValue(cbox->itemData(cbox->currentIndex()).toString().toStdString());
}

void AdvertMediaForm::set_rating_field_default(QComboBox*, const std::string)
{

}

int AdvertMediaForm::item_index(QComboBox*, const QString)
{

}

void AdvertMediaForm::play_media()
{
    m_media_player->play();
}

void AdvertMediaForm::stop_media()
{
    m_media_player->stop();
}

void AdvertMediaForm::on_media_status_changed(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::LoadedMedia)
    {
        m_media_player->play();

        QTimer::singleShot(100, m_media_player.get(), &QMediaPlayer::pause);

    }

}

void AdvertMediaForm::on_duration_changed(qint64 duration)
{
    m_advert_media->set_duration(duration);
    if (duration > 0) {
        QTime time = duration_to_time(duration);
        ui->edtDuration->setTime(time);
    }

}

QTime AdvertMediaForm::duration_to_time(qint64 duration)
{
    // Format the duration into HH:MM:SS
    int seconds = (duration / 1000) % 60;
    int minutes = (duration / 60000) % 60;
    int hours = (duration / 3600000) % 24; // Use % 24 for hours to handle cases over 24 hours

    QTime time(hours, minutes, seconds);
    //QString formattedDuration = time.toString("hh:mm:ss"); /

    return time;
}
