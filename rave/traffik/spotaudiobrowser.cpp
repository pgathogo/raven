#include <QFileDialog>

#include "spotaudiobrowser.h"
#include "ui_spotaudiobrowser.h"

#include "../framework/manytomany.h"
#include "../audio/audiofile.h"
#include "../audio/audiotool.h"
#include "../audio/audio.h"
#include "../audiolib/headers/cueeditor.h"

#include "spotaudio.h"
#include "spotaudioform.h"
#include "traffiksetup.h"

namespace fs = std::filesystem;

SpotAudioBrowser::SpotAudioBrowser(
            ManyToMany* mtom,
            QVBoxLayout* layout,
            QWidget *parent)
        :BaseEntityBrowserDlg(parent, mtom->cloneAsUnique()),
        ui(new Ui::SpotAudioBrowser),
        m_mtom{mtom}
{
    ui->setupUi(this);
    hideAddButton();
    hideEditButton();
    hideDeleteButton();

    createImportButton();

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

}

void SpotAudioBrowser::createImportButton()
{
    QPushButton* btnImport = new QPushButton("Import");
    btnImport->setObjectName("btnImport");
    connect(btnImport, &QPushButton::clicked, this, &SpotAudioBrowser::import_audio);
    bui->hlExtra->addWidget(btnImport);
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
    ADFRepository adf_repo;
    AudioTool audio_tool;
    Marker marker;

    auto audio_file_full_path = QFileDialog::getOpenFileName(this,
                                                   tr("Import Audio"), "/d/home/audio",
                                                   tr("Audio Files (*.ogg *.mp3 *.wav)"));
    if (audio_file_full_path.isEmpty())
        return;

    auto audio = std::make_unique<AUDIO::Audio>(audio_file_full_path.toStdString());

    audio->audio_file().set_marker(marker);

    if (fs::exists(audio->audio_file().adf_file()) ){

        AudioFile af = audio->audio_file();
        adf_repo.read_markers(af);

        auto af_markers = audio->audio_file().marker();

        marker.start_marker = af_markers.start_marker;
        marker.fade_in = af_markers.fade_in;
        marker.intro = af_markers.intro;
        marker.extro = af_markers.extro;
        marker.fade_out = af_markers.fade_out;
        marker.end_marker = af_markers.end_marker;
    }

    //auto spot_audio = std::make_unique<TRAFFIK::SpotAudio>(m_mtom->parentEntity(), new AUDIO::Audio(audio_file.audio_file()));
    auto spot_audio = std::make_unique<TRAFFIK::SpotAudio>(m_mtom->parentEntity(), audio.get());

    // Add some audio attributes - name, description etc.
    auto spot_audio_form = std::make_unique<SpotAudioForm>(spot_audio.get());

    if (spot_audio_form->exec() > 0){

        if (!fs::exists(spot_audio->audio()->audio_file().wave_file()) ){
            audio_tool.generate_wave_file(spot_audio->audio()->audio_file().audio_file(),
                                          spot_audio->audio()->audio_file().wave_file());
            qDebug() << "Wave generated.";
        }

        spot_audio->set_audio_lib_path(m_setup->comm_audio_folder()->value());
        spot_audio->set_title(spot_audio->title()->value());
        spot_audio->set_file_path(audio_file_full_path.toStdString());

        auto af = audio->audio_file();
        af.set_ogg_filename(m_setup->comm_audio_folder()->value()+af.ogg_short_filename());

        auto cue_editor = std::make_unique<CueEditor>(af);
        if (cue_editor->editor() == 1){

            spot_audio->audio()->set_audio_file(af);
            spot_audio->set_marker(cue_editor->marker());
            spot_audio->set_duration(af.duration());

            spot_audio->setDBAction(DBAction::dbaCREATE);
            //spot_audio->setDetailId() // We don't have this id yet!

            //adf_repo.write(spot_audio->audio()->audio_file());

            auto& p_audio = spot_audio->get_paudio();
            p_audio = *spot_audio->audio();

            //m_audios.push_back(std::move(m_spot_audio));
            entityDataModel().cacheEntity(std::move(spot_audio));
        }

    }
}
