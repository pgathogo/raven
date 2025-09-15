#include <QApplication>
#include <QFileDialog>
#include <QDir>

#include "spotaudiobrowser.h"
#include "ui_spotaudiobrowser.h"

#include "../framework/manytomany.h"
#include "../framework/ravensetup.h"
#include "../framework/ravenexception.h"
#include "../framework/applicationcontext.h"
#include "../framework/Logger.h"

#include "../audio/audiofile.h"
#include "../audio/audiotool.h"
#include "../audio/audio.h"

//#include "../audiolib/headers/cueeditor.h"
//#include "../audiolib/headers/audioplayer.h"


#include "../audio/editor/audiowaveform.h"
#include "../audio/editor/audioplayer.h"

#include "../audio/audiowaveformgenerator.h"
#include "../audio/mp3oggconverter.h"
#include "../audio/oggtooggconverter.h"
#include "../audio/mtstomp3converter.h"
#include "../audio/audiofileinfo.h"

#include "../audio/trackpickerdialog.h"

#include "../AudioExplorer/audioform.h"

#include "spotaudio.h"
#include "spotaudioform.h"

namespace fs = std::filesystem;

SpotAudioBrowser::SpotAudioBrowser(TRAFFIK::SpotAudio* mtom,
                                    QVBoxLayout* layout, QWidget *parent)
        :BaseEntityBrowserDlg(parent, mtom->cloneAsShared())
        ,ui(new Ui::SpotAudioBrowser)
        ,m_mtom{mtom}
        ,m_audio_wave_form{nullptr}
        ,m_audio_creation_mode{AudioCreationMode::None}
{
    ui->setupUi(this);
    hideAddButton();
    hideEditButton();
    hideDeleteButton();

    create_button("btnImport", "Import", &SpotAudioBrowser::import_audio,
                  QSize(80, 20), "import_audio.bmp");

    create_button("btnAttach", "Attach Audio", &SpotAudioBrowser::attach_audio, QSize(80, 20));

    create_button("btnAudioProp", "Audio Properties", &SpotAudioBrowser::audio_properties,
                  QSize(80, 20), "audio_prop.png");

    create_button("btnCueEditor", "Cue Edit", &SpotAudioBrowser::cue_edit,
                  QSize(80, 20), "cue_edit.bmp");

    create_separator();
    create_button("btnPlayBack", "Play", &SpotAudioBrowser::play_back,
                  QSize(80, 20), "play.png");

    create_button("btnStopPlay", "Stop", &SpotAudioBrowser::stop_play,
                  QSize(80, 20), "stop_play.png");

    create_separator();
    show_delete_button(bui->hlExtra);

    QSpacerItem* si = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Fixed);
    bui->hlExtra->addSpacerItem(si);

    m_setup_edm = std::make_unique<EntityDataModel>(std::make_unique<RavenSetup>());
    m_setup_edm->all();

    m_setup = std::dynamic_pointer_cast<RavenSetup>(m_setup_edm->firstEntity());

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

SpotAudioBrowser::~SpotAudioBrowser()
{
    delete ui;
}

void SpotAudioBrowser::resize_window()
{
    qDebug() << "Spot Audio Browesr::Resize:: " ;
    this->setMinimumWidth(1100);
}

void SpotAudioBrowser::addRecord()
{

}

void SpotAudioBrowser::updateRecord()
{

}

void SpotAudioBrowser::deleteRecord()
{
    std::shared_ptr<BaseEntity> entity = findSelectedEntity();
    // Check if it is okay  to delete the selected entity

    BaseEntityBrowserDlg::deleteRecord();
}

bool SpotAudioBrowser::okay_to_delete(std::shared_ptr<BaseEntity> entity)
{
    // check that the Audio is not attached to a running Spot.
    return true;
}

void SpotAudioBrowser::create_button(const QString btn_name, QString btn_caption,
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

void SpotAudioBrowser::create_separator()
{
    QFrame* vline = new QFrame(this);
    vline->setFrameShape(QFrame::VLine);
    vline->setFrameShadow(QFrame::Sunken);
    bui->hlExtra->addWidget(vline);
}

AUDIO::Audio* SpotAudioBrowser::audio_from_selection()
{
    std::string search_name = selectedRowName().toStdString();
    if (search_name.empty())
        return nullptr;

    std::shared_ptr<BaseEntity> be = entityDataModel().findEntityByName(search_name);
    if (be == nullptr){
        qDebug() << "`be` is NULL! ";
        return nullptr;
    }

    auto s_audio = dynamic_cast<TRAFFIK::SpotAudio*>(be.get());

    auto audio = dynamic_cast<AUDIO::Audio*>(s_audio->detailEntity());

    return audio;

}

std::string SpotAudioBrowser::get_audio_file(AUDIO::Audio* audio)
{
    AUDIO::AudioTool at;
    auto filename = at.make_audio_filename(audio->id());

    auto audio_file = audio->file_path()->value()+filename+".ogg";

    if (!fs::exists(audio_file)){
        showMessage("File: `"+audio_file+"` does not exists");
        return "";
    }

    return audio_file;

}

std::string SpotAudioBrowser::typeID()
{
    return "SpotAudioBrowser";
}

void SpotAudioBrowser::print(std::string msg)
{
    qDebug() << stoq(msg);
}

const std::vector<std::unique_ptr<TRAFFIK::SpotAudio> > &SpotAudioBrowser::audios() const
{
    return m_audios;
}

TRAFFIK::SpotAudio& SpotAudioBrowser::get_spot_audio() const
{
    auto ptr = m_spot_audio.get();
    return *ptr;
}

void SpotAudioBrowser::import_audio()
{
    AUDIO::AudioTool audio_tool;

    auto audio_filename = QFileDialog::getOpenFileName(this,
                                                   tr("Import Audio"), QDir::currentPath(),
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

        m_audio_creation_mode = AudioCreationMode::Import;

    }
}

void SpotAudioBrowser::attach_audio()
{
    //TODO: Disallow attaching audio with no actual audio file
    m_track_picker_dlg = std::make_unique<AUDIO::TrackPickerDialog>();
    connect(m_track_picker_dlg.get(), &AUDIO::TrackPickerDialog::selected_audio, this, &SpotAudioBrowser::selected_audio);
    m_track_picker_dlg->exec();
}

void SpotAudioBrowser::selected_audio(std::shared_ptr<AUDIO::Audio> audio)
{
    auto spot_audio = std::make_unique<TRAFFIK::SpotAudio>(m_mtom->parentEntity(), audio.get());

    // spot_audio->setId(audio->id());
    spot_audio->setDBAction(DBAction::dbaCREATE);
    entityDataModel().cacheEntity(std::move(spot_audio));
    m_audio_creation_mode = AudioCreationMode::Attach;
}

AudioCreationMode SpotAudioBrowser::audio_creation_mode()
{
    return m_audio_creation_mode;
}


void SpotAudioBrowser::audio_properties()
{
    if (selectedRowId() < 0 )
        return;

    AUDIO::Audio* audio = audio_from_selection();
    audio->set_file_path(audio->audio_lib_path()->value());
    auto audio_form = std::make_unique<AudioForm>(audio, m_setup, FormMode::ReadOnly);
    audio_form->exec();

}

void SpotAudioBrowser::play_back()
{

    if (selectedRowId() < 0) {
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

}

void SpotAudioBrowser::stop_play()
{
    if (m_audio_player != nullptr)
        m_audio_player->stop_play();
}

void SpotAudioBrowser::cue_edit()
{
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
}

void SpotAudioBrowser::closeEvent(QCloseEvent*)
{
    stop_play();
}
