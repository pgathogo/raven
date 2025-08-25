#include <QApplication>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>

#include "spotmediabrowser.h"
#include "ui_spotmediabrowser.h"

#include "../../../rave/framework/manytomany.h"
#include "../../../rave/framework/ravensetup.h"
#include "../../../rave/framework/ravenexception.h"
#include "../../../rave/framework/applicationcontext.h"
#include "../../../rave/framework/Logger.h"

#include "../../../rave/audio/audiofile.h"
#include "../../../rave/audio/audiotool.h"
#include "../../../rave/audio/audio.h"


#include "../../../rave/audio/editor/audiowaveform.h"
#include "../../../rave/audio/editor/audioplayer.h"

#include "../../../rave/audio/audiowaveformgenerator.h"
#include "../../../rave/audio/mp3oggconverter.h"
#include "../../../rave/audio/oggtooggconverter.h"
#include "../../../rave/audio/mtstomp3converter.h"
#include "../../../rave/audio/audiofileinfo.h"

#include "../../../rave/audio/trackpickerdialog.h"

#include "../../../rave/AudioExplorer/audioform.h"

#include "spotaudio.h"
#include "spotaudioform.h"
#include "advertmedia.h"
#include "spotmedia.h"
#include "advertmediaform.h"

namespace fs = std::filesystem;

SpotMediaBrowser::SpotMediaBrowser(PIXELPLAN::SpotMedia* mtom,
                                   QVBoxLayout* layout,
                                   QWidget *parent)
        :BaseEntityBrowserDlg(parent, mtom->cloneAsShared())
        ,ui(new Ui::SpotMediaBrowser)
        ,m_mtom{mtom}
        ,m_spot_media{mtom}
        ,m_audio_wave_form{nullptr}
        ,m_media_creation_mode{MediaCreationMode::None}
{
    ui->setupUi(this);
    hideAddButton();
    hideEditButton();
    hideDeleteButton();

    create_button("btnImport", "Import", &SpotMediaBrowser::import_media,
                  QSize(80, 20), "import_audio.bmp");

    // create_button("btnAttach", "Attach Media", &SpotMediaBrowser::attach_audio, QSize(80, 20));

    create_button("btnAudioProp", "Media Properties", &SpotMediaBrowser::media_properties,
                  QSize(80, 20), "audio_prop.png");

    // create_button("btnCueEditor", "Cue Edit", &SpotMediaBrowser::cue_edit,
    //               QSize(80, 20), "cue_edit.bmp");

    create_separator();
    create_button("btnPlayBack", "Play", &SpotMediaBrowser::play_back,
                  QSize(80, 20), "play.png");

    // create_button("btnStopPlay", "Stop", &SpotMediaBrowser::stop_play,
    //               QSize(80, 20), "stop_play.png");

    create_separator();
    show_delete_button(bui->hlExtra);

    QSpacerItem* si = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Fixed);
    bui->hlExtra->addSpacerItem(si);

    m_setup_edm = std::make_unique<EntityDataModel>(std::make_unique<RavenSetup>());
    m_setup_edm->all();
    if (m_setup_edm->count() > 0)
        m_setup = std::dynamic_pointer_cast<RavenSetup>(m_setup_edm->firstEntity());
    else
        m_setup = std::make_shared<RavenSetup>();

    layout->addWidget(this);

    if (m_mtom->parentId()->value() > 0){
        std::string columnName =m_mtom->parentId()->dbColumnName();
        int value = m_mtom->parentId()->value();
        auto needle = std::make_tuple(columnName, "=", value);

        try{
            entityDataModel().searchByInt(needle);

        } catch (DatabaseException& de) {
            showMessage(de.errorMessage());
        }
    }

    resize_window();

}

SpotMediaBrowser::~SpotMediaBrowser()
{
    delete ui;
}

void SpotMediaBrowser::resize_window()
{
    qDebug() << "Spot Media Browesr::Resize:: " ;
    this->setMinimumWidth(1100);
}

void SpotMediaBrowser::addRecord()
{

}

void SpotMediaBrowser::updateRecord()
{

}

void SpotMediaBrowser::deleteRecord()
{
    std::shared_ptr<BaseEntity> entity = findSelectedEntity();
    // Check if it is okay  to delete the selected entity

    BaseEntityBrowserDlg::deleteRecord();
}

bool SpotMediaBrowser::okay_to_delete(std::shared_ptr<BaseEntity> entity)
{
    // check that the Audio is not attached to a running Spot.
    return true;
}

void SpotMediaBrowser::create_button(const QString btn_name, QString btn_caption,
                                     Slot slot, QSize size, QString icon)
{
    QPushButton* btn = new QPushButton(btn_caption);
    btn->setObjectName(btn_name);
    connect(btn, &QPushButton::clicked, this, slot);

    if (!size.isNull())
        btn->setMinimumSize(size);


    if (!icon.isEmpty()) {
        QString icon_path = QString(":/images/media/icons/%1").arg(icon);
        btn->setIcon(QIcon(icon_path));
    }

    bui->hlExtra->addWidget(btn);
}

void SpotMediaBrowser::create_separator()
{
    QFrame* vline = new QFrame(this);
    vline->setFrameShape(QFrame::VLine);
    vline->setFrameShadow(QFrame::Sunken);
    bui->hlExtra->addWidget(vline);
}

PIXELPLAN::AdvertMedia* SpotMediaBrowser::media_from_selection()
{
    std::string search_name = selectedRowName().toStdString();
    if (search_name.empty())
        return nullptr;

    std::shared_ptr<BaseEntity> be = entityDataModel().findEntityByName(search_name);
    if (be == nullptr){
        qDebug() << "`be` is NULL! ";
        return nullptr;
    }

    auto s_media = dynamic_cast<PIXELPLAN::SpotMedia*>(be.get());

    auto media = dynamic_cast<PIXELPLAN::AdvertMedia*>(s_media->detailEntity());

    return media;

}

std::string SpotMediaBrowser::get_media_file(PIXELPLAN::AdvertMedia* media)
{
    /*
    AUDIO::AudioTool at;
    auto filename = at.make_audio_filename(audio->id());

    auto audio_file = audio->file_path()->value()+filename+".ogg";

    if (!fs::exists(audio_file)){
        showMessage("File: `"+audio_file+"` does not exists");
        return "";
    }

    return audio_file;
    */
    return "";

}

std::string SpotMediaBrowser::typeID()
{
    return "SpotMediaBrowser";
}

void SpotMediaBrowser::print(std::string msg)
{
    qDebug() << stoq(msg);
}

const std::vector<std::unique_ptr<TRAFFIK::SpotAudio> > &SpotMediaBrowser::audios() const
{
    return m_audios;
}

const std::vector<std::unique_ptr<PIXELPLAN::SpotMedia> > &SpotMediaBrowser::medias() const
{
    return m_medias;
}

/*
TRAFFIK::SpotAudio& SpotMediaBrowser::get_spot_audio() const
{
    auto ptr = m_spot_audio.get();
    return *ptr;
}
*/

PIXELPLAN::SpotMedia& SpotMediaBrowser::get_spot_media() const
{
    auto ptr = m_spot_media;
    return *ptr;
}

void SpotMediaBrowser::import_media()
{
    auto media_filename = QFileDialog::getOpenFileName(this,
                                                       tr("Import Media"), QDir::currentPath(),
                    tr("MOV Files (*.mov);; MP4 Files (*.mp4);;"));

    if (media_filename.isEmpty())
        return;

    auto advert_media = std::make_shared<PIXELPLAN::AdvertMedia>();
    QFileInfo fi = QFileInfo(media_filename);

    advert_media->set_title(fi.fileName().toStdString());
    advert_media->set_media_file(media_filename.toStdString());
    advert_media->set_media_path(m_setup->audio_folder()->value());

    //auto spot_media = std::make_shared<PIXELPLAN::SpotMedia>(m_mtom->parentEntity(), media.get());
    // auto spot_media = std::make_shared<PIXELPLAN::SpotMedia>(advert_media);

    auto advert_media_form = std::make_unique<AdvertMediaForm>(advert_media);
    if (advert_media_form->exec() > 0)
    {
        qDebug() << "Saved media form...";

        //auto spot_media = std::make_unique<PIXELPLAN::SpotMedia>(m_mtom->parentEntity(), advert_media.get());
       m_spot_media->set_advert_media(advert_media.get());
       m_spot_media->setDBAction(DBAction::dbaNONE);

        qDebug() << "XX Before caching ...";
        qDebug() << m_spot_media->advert_media()->title()->to_qstring();

        //entityDataModel().cacheEntity(std::move(spot_media));
    }

    m_media_creation_mode = MediaCreationMode::Import;

    qDebug() << "Import Done.";

}

void SpotMediaBrowser::import_audio()
{
    AUDIO::AudioTool audio_tool;

    auto audio_filename = QFileDialog::getOpenFileName(this,
                                                   tr("Import Media"), QDir::currentPath(),
                                                   tr("OGG Files (*.ogg);; MP3 Files (*.mp3);; MTS Files (*.mts);; Wave Files (*.wav)"));
    if (audio_filename.isEmpty())
        return;

    AUDIO::AudioFileInfo afi(audio_filename);
    QString file_format = afi.file_format();

    auto audio = std::make_unique<AUDIO::Audio>(audio_filename.toStdString());
    auto audio_shared = std::make_shared<AUDIO::Audio>(audio_filename.toStdString());

    audio->set_audio_lib_path(m_setup->audio_folder()->value());

    if (fs::exists(audio->audio_file().adf_file()) )
    {
        AUDIO::ADFRepository adf_repo;
        CueMarker cue_marker =  adf_repo.read_markers(audio->audio_file().adf_file());
        audio->audio_file().set_marker(cue_marker);
    }

    auto spot_audio = std::make_unique<TRAFFIK::SpotAudio>(m_mtom->parentEntity(), audio.get());

    // Add some audio attributes - name, description etc.
    auto spot_audio_form = std::make_unique<SpotAudioForm>(spot_audio.get());

    if (spot_audio_form->exec() > 0){

        if (!fs::exists(spot_audio->audio()->audio_file().wave_file()) )
        {
            try{
                auto wave_gen = std::make_unique<AUDIO::AudioWaveFormGenerator>(audio_filename);
                wave_gen->generate();
                audio->audio_file().set_wave_file(wave_gen->wave_filename().toStdString());
            } catch (AudioImportException& aie) {
                showMessage(aie.errorMessage());
            }

            if (file_format == "ogg")
            {
                auto audio_converter = std::make_unique<AUDIO::OggToOggConverter>(audio_filename);
                spot_audio->audio()->set_file_extension("OGG");
                spot_audio->audio()->audio_file().set_ogg_filename(audio_converter->ogg_filename().toStdString());
            }

            if (file_format == "mp3")
            {
                try{
                    auto audio_converter = std::make_unique<AUDIO::Mp3ToOggConverter>(audio_filename);
                    audio_converter->convert();

                    // File has been converted to OGG, change the extension
                    spot_audio->audio()->set_file_extension("OGG");

                    spot_audio->audio()->audio_file().set_ogg_filename(audio_converter->ogg_filename().toStdString());
                } catch (AudioImportException& aie) {
                    showMessage(aie.errorMessage());
                }
            }

            if (file_format == "mts")
            {
                auto audio_converter = std::make_unique<AUDIO::MtsToMp3Converter>(audio_filename);
                audio_converter->convert();
                auto mts_file = dynamic_cast<AUDIO::MtsToMp3Converter*>(audio_converter.get());
                spot_audio->audio()->set_audio_filename(mts_file->dest_mp3_filename().toStdString());
                spot_audio->audio()->set_file_extension("OGG");
            }

        }

        spot_audio->set_audio_lib_path(m_setup->comm_audio_folder()->value());

        spot_audio->set_title(spot_audio->title()->value());
        spot_audio->set_file_path(audio_filename.toStdString());

        spot_audio->set_file_path(m_setup->comm_audio_folder()->value());

        auto af = audio->audio_file();
        af.set_audio_filename(audio_filename.toStdString());
        af.set_audio_title(spot_audio->title()->value());
        af.set_artist_name("");

        auto wave_form = std::make_unique<AUDIO::AudioWaveForm>(af);
        if (wave_form->exec() == 1)
        {
            spot_audio->audio()->set_audio_file(af);
            spot_audio->set_marker(wave_form->cue_marker());
            spot_audio->set_duration(af.duration());

            spot_audio->setDBAction(DBAction::dbaCREATE);

            auto& p_audio = spot_audio->get_paudio();

            p_audio = *spot_audio->audio();

            // Emit audio duration signal
            emit audio_duration(af.duration());

            entityDataModel().cacheEntity(std::move(spot_audio));
        };

        m_media_creation_mode = MediaCreationMode::Import;

    }
}

/*
void SpotMediaBrowser::attach_audio()
{
    //TODO: Disallow attaching audio with no actual audio file
    m_track_picker_dlg = std::make_unique<AUDIO::TrackPickerDialog>();
    connect(m_track_picker_dlg.get(), &AUDIO::TrackPickerDialog::selected_audio, this, &SpotMediaBrowser::selected_audio);
    m_track_picker_dlg->exec();
}


void SpotMediaBrowser::selected_audio(std::shared_ptr<AUDIO::Audio> audio)
{
    auto spot_audio = std::make_unique<TRAFFIK::SpotAudio>(m_mtom->parentEntity(), audio.get());

    // spot_audio->setId(audio->id());
    spot_audio->setDBAction(DBAction::dbaCREATE);
    entityDataModel().cacheEntity(std::move(spot_audio));
    m_audio_creation_mode = AudioCreationMode::Attach;
}
*/

void SpotMediaBrowser::attach_media()
{
}

void SpotMediaBrowser::selected_media(std::shared_ptr<PIXELPLAN::AdvertMedia>)
{

}

MediaCreationMode SpotMediaBrowser::media_creation_mode()
{
    return m_media_creation_mode;
}

/*
void SpotMediaBrowser::audio_properties()
{
    if (selectedRowId() < 0 )
        return;

    AUDIO::Audio* audio = audio_from_selection();
    audio->set_file_path(audio->audio_lib_path()->value());
    auto audio_form = std::make_unique<AudioForm>(audio, m_setup.get(), FormMode::ReadOnly);
    audio_form->exec();

}
*/

void SpotMediaBrowser::media_properties()
{

}

void SpotMediaBrowser::play_back()
{
    /*

    if (selectedRowId() < 0){
        showMessage("Select an Audio to Listen");
        return;
    }

    AUDIO::Audio* audio = audio_from_selection();
    std::string audio_file = get_audio_file(audio);
    if (audio_file.empty()){
        qDebug() << "No audio file to play!";
        return;
    }

    AudioFile af(audio_file);

    m_audio_player = std::make_unique<AUDIO::AudioPlayer>(af);
    m_audio_player->play_audio(QString::fromStdString(audio_file));
   */

}

void SpotMediaBrowser::stop_play()
{
    /*
    if (m_audio_player != nullptr)
        m_audio_player->stop_play();
    */
}

void SpotMediaBrowser::cue_edit()
{
    /*
    if (selectedRowId() < 0 )
        return;

    AUDIO::Audio* audio = audio_from_selection();
    std::string audio_file = get_audio_file(audio);

    if (audio_file.empty()){
        qDebug() << "No audio file to edit!";
        return;
    }

    AudioFile af(audio_file);
    af.set_audio_title(audio->title()->value());
    af.set_artist_name(audio->artist_fullname()); // artist()->value());

    //CueEditor* cue_editor = new CueEditor(af);
//    auto cue_editor = std::make_unique<CueEditor>(af, m_app_context->application());
//    if (cue_editor->editor() == 1){
//    }

    auto wave_form = std::make_unique<AUDIO::AudioWaveForm>(af);
    if (wave_form->exec() == 1){
    }
  */


}

void SpotMediaBrowser::closeEvent(QCloseEvent*)
{
    stop_play();
}
