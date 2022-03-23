#include <QFileDialog>
#include <QDir>

#include "spotaudiobrowser.h"
#include "ui_spotaudiobrowser.h"

#include "../framework/manytomany.h"
#include "../audio/audiofile.h"
#include "../audio/audiotool.h"
#include "../audio/audio.h"
#include "../audiolib/headers/cueeditor.h"
#include "../audiolib/headers/audioplayer.h"

#include "spotaudio.h"
#include "spotaudioform.h"
#include "traffiksetup.h"

namespace fs = std::filesystem;

SpotAudioBrowser::SpotAudioBrowser(
            TRAFFIK::SpotAudio* mtom,
            QVBoxLayout* layout,
            QWidget *parent)
        :BaseEntityBrowserDlg(parent, mtom->cloneAsUnique()),
        ui(new Ui::SpotAudioBrowser),
        m_mtom{mtom},
        m_cue_editor{nullptr}
{
    ui->setupUi(this);
    hideAddButton();
    hideEditButton();
    hideDeleteButton();

    show_delete_button();

    create_button("btnImport", "Import", &SpotAudioBrowser::import_audio);
    create_button("btnPlayBack", "Listen", &SpotAudioBrowser::play_back);
    create_button("btnStopPlay", "Stop", &SpotAudioBrowser::stop_play);
    create_button("btnCueEditor", "Cue Edit", &SpotAudioBrowser::cue_edit);

    m_setup_edm = std::make_unique<EntityDataModel>(std::make_unique<TraffikSetup>());
    m_setup_edm->all();
    m_setup = dynamic_cast<TraffikSetup*>(m_setup_edm->firstEntity());

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
}

SpotAudioBrowser::~SpotAudioBrowser()
{
    delete ui;
}

void SpotAudioBrowser::addRecord()
{

}

void SpotAudioBrowser::updateRecord()
{

}

void SpotAudioBrowser::deleteRecord()
{
    BaseEntity* entity = findSelectedEntity();

    // Check if it is okay  to delete the selected entity

    BaseEntityBrowserDlg::deleteRecord();

}

bool SpotAudioBrowser::okay_to_delete(BaseEntity *entity)
{
    // check that the Audio is not attached to a running Spot.
    return true;
}

void SpotAudioBrowser::create_button(const QString btn_name, QString btn_caption, Slot slot)
{
    QPushButton* btn = new QPushButton(btn_caption);
    btn->setObjectName(btn_name);
    connect(btn, &QPushButton::clicked, this, slot);
    bui->hlExtra->addWidget(btn);

}

AUDIO::Audio* SpotAudioBrowser::audio_from_selection()
{
    std::string search_name = selectedRowName().toStdString();
    if (search_name.empty())
        return nullptr;

    BaseEntity* be = entityDataModel().findEntityByName(search_name);
    if (be == nullptr){
        qDebug() << "`be` is NULL! ";
        return nullptr;
    }

    auto s_audio = dynamic_cast<TRAFFIK::SpotAudio*>(be);

    auto audio = dynamic_cast<AUDIO::Audio*>(s_audio->detailEntity());

    return audio;

}

std::string SpotAudioBrowser::audio_file_name(AUDIO::Audio* audio)
{
    AudioTool at;
    auto ogg_file = at.generate_ogg_filename(audio->id())+".ogg";
    auto audio_file = audio->file_path()->value()+ogg_file;

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
    AudioTool audio_tool;

    auto audio_file_full_path = QFileDialog::getOpenFileName(this,
                                                   tr("Import Audio"), QDir::currentPath(),
                                                   tr("Audio Files (*.ogg *.mp3 *.wav)"));
    if (audio_file_full_path.isEmpty())
        return;

    auto audio = std::make_unique<AUDIO::Audio>(audio_file_full_path.toStdString());

    if (fs::exists(audio->audio_file().adf_file()) ){

        ADFRepository adf_repo;
        Marker marker =  adf_repo.read_markers(audio->audio_file().adf_file());
        audio->audio_file().set_marker(marker);

//        marker.start_marker = af_markers.start_marker;
//        marker.fade_in = af_markers.fade_in;
//        marker.intro = af_markers.intro;
//        marker.extro = af_markers.extro;
//        marker.fade_out = af_markers.fade_out;
//        marker.end_marker = af_markers.end_marker;
    }

    //auto spot_audio = std::make_unique<TRAFFIK::SpotAudio>(m_mtom->parentEntity(), new AUDIO::Audio(audio_file.audio_file()));
    auto spot_audio = std::make_unique<TRAFFIK::SpotAudio>(m_mtom->parentEntity(), audio.get());

    // Add some audio attributes - name, description etc.
    auto spot_audio_form = std::make_unique<SpotAudioForm>(spot_audio.get());

    printstr("Spot form created ...");

    if (spot_audio_form->exec() > 0){

        printstr("After spot_audio_form ...");

        if (!fs::exists(spot_audio->audio()->audio_file().wave_file()) ){

            if (!audio_tool.generate_wave_file(spot_audio->audio()->audio_file().audio_file(),
                                          spot_audio->audio()->audio_file().wave_file())){
                showMessage("Failed to generate audio wave file! Process aborted.");
                return;
            }

            qDebug() << "Wave generated.";
        }

        printstr("COMM AUDIO FOLDER: "+m_setup->comm_audio_folder()->value());

        spot_audio->set_audio_lib_path(m_setup->comm_audio_folder()->value());
        spot_audio->set_title(spot_audio->title()->value());
        spot_audio->set_file_path(audio_file_full_path.toStdString());
        spot_audio->set_file_path(m_setup->comm_audio_folder()->value());

        auto af = audio->audio_file();
//        fs::path path{audio_file_full_path.toStdString()};
        //std::string file_no_path = path.filename().u8string();

        af.set_ogg_filename(m_setup->comm_audio_folder()->value()+af.short_filename()+".ogg");

        qDebug() << QString::fromStdString(af.ogg_filename());

        auto cue_editor = std::make_unique<CueEditor>(af);

        printstr("CueEditor created.");

        if (cue_editor->editor() == 1){
            printstr("After cue_editor");

            spot_audio->audio()->set_audio_file(af);
            spot_audio->set_marker(cue_editor->marker());
            spot_audio->set_duration(af.duration());

            spot_audio->setDBAction(DBAction::dbaCREATE);
            //spot_audio->setDetailId() // We don't have this id yet!

            auto& p_audio = spot_audio->get_paudio();
            p_audio = *spot_audio->audio();

            entityDataModel().cacheEntity(std::move(spot_audio));
        }

    }
}

void SpotAudioBrowser::play_back()
{
    if (selectedRowId() < 0){
        showMessage("Select an Audio to Listen");
        return;
    }

    AUDIO::Audio* audio = audio_from_selection();
    std::string audio_file = audio_file_name(audio);
    if (audio_file.empty()){
        qDebug() << "No audio file to play!";
        return;
    }

    // FIXME: Player should be seperated from the CueEditor
    AudioFile af(audio_file);
    std::string name = "player-only";
    m_cue_editor = std::make_unique<CueEditor>(af, name);
    m_cue_editor->play_audio();
}

void SpotAudioBrowser::stop_play()
{
    if (m_cue_editor != nullptr)

        m_cue_editor->stop_audio();
}

void SpotAudioBrowser::cue_edit()
{
    AUDIO::Audio* audio = audio_from_selection();
    std::string audio_file = audio_file_name(audio);
    if (audio_file.empty()){
        qDebug() << "No audio file to edit!";
        return;
    }

    AudioFile af(audio_file);
    af.set_audio_title(audio->title()->value());
    //af.set_artist_name(audio->artist()->value());
    //CueEditor* cue_editor = new CueEditor(af);
    auto cue_editor = std::make_unique<CueEditor>(af);
    if (cue_editor->editor() == 1){
    }
}
