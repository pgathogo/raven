#include <filesystem>
#include <sstream>

#include <QFileDialog>

#include "ui_baseentitydetaildlg.h"
#include "../utils/daypartgrid.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/manytomanybrowser.h"
#include "../framework/manytomany.h"
#include "../framework/entitydatamodel.h"
#include "../utils/tools.h"

#include "spotform.h"
#include "ui_spotform.h"
#include "client.h"
#include "spotvoiceover.h"
#include "spottypeexclusion.h"
#include "traffiksetup.h"

#include "../audio/audiofile.h"
#include "../audio/audiotool.h"
#include "../audio/audio.h"
#include "../audiolib/headers/cueeditor.h"

#include "spotaudiobrowser.h"
#include "spotaudio.h"
#include "spotaudioform.h"

namespace fs = std::filesystem;

SpotForm::SpotForm(Client* client, TRAFFIK::Spot* spot,
                   QDialog *parent) :
    BaseEntityDetailDlg{parent},
    ui{new Ui::SpotForm},
    m_client{client},
    m_spot{spot}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    populateGrid();
    populateFormWidgets();

    m_MtoMVoiceOverBrowser =
            std::make_unique<ManyToManyBrowser>(&m_spot->voice_over(),
                                                ui->vlVoiceOver,
                                                this);
    m_MtoMTypeExBrowser =
            std::make_unique<ManyToManyBrowser>(&m_spot->type_exclusion(),
                                                ui->vlTypeEx,
                                                this);

    m_spot_audio_browser =
            std::make_unique<SpotAudioBrowser>(&m_spot->spot_audio(),
                                                ui->vlSpotAudio,
                                                this);

    connect(ui->cbBrands, QOverload<int>::of(&QComboBox::currentIndexChanged),
           this, &SpotForm::brandsComboChanged);

    //connect(ui->btnCueEdit, &QPushButton::clicked, this, &SpotForm::cue_edit);

    ui->tabWidget->setCurrentIndex(0);

    m_edm_setup = std::make_unique<EntityDataModel>(
                std::make_unique<TraffikSetup>());
    m_edm_setup->all();
    m_setup = dynamic_cast<TraffikSetup*>(m_edm_setup->firstEntity());
}

SpotForm::~SpotForm()
{
    delete ui;
}

ActionResult SpotForm::saveRecord()
{
    populateEntityFields();
    return m_spot->validate();
}

std::vector<EntityRecord> const& SpotForm::voiceOvers() const
{
    return m_MtoMVoiceOverBrowser->entityDataModel().modelEntities();
}

std::vector<EntityRecord> const& SpotForm::typeExclusions() const
{
    return m_MtoMTypeExBrowser->entityDataModel().modelEntities();
}

const std::vector<EntityRecord> &SpotForm::spot_audios() const
{
    return  m_spot_audio_browser->entityDataModel().modelEntities();
}

int SpotForm::parentId() const
{
    return m_spot->id();
}

const std::unique_ptr<SpotAudioBrowser> &SpotForm::spot_browser() const
{
    return m_spot_audio_browser;
}

std::string SpotForm::windowTitle()
{
    return "Client:" +m_client->name()->displayName();
}

void SpotForm::populateGrid()
{
    m_daypart = std::make_unique<DayPartGrid>(ui->vlDaypart);

    std::map<int, std::string> dayparts;
    dayparts[1] = m_spot->daypart1()->valueToString();
    dayparts[2] = m_spot->daypart2()->valueToString();
    dayparts[3] = m_spot->daypart3()->valueToString();
    dayparts[4] = m_spot->daypart4()->valueToString();
    dayparts[5] = m_spot->daypart5()->valueToString();
    dayparts[6] = m_spot->daypart6()->valueToString();
    dayparts[7] = m_spot->daypart7()->valueToString();

    m_daypart->update_grid(dayparts);
}

void SpotForm::populateEntityFields()
{
    brandsComboChanged(ui->cbBrands->currentIndex());
    m_spot->set_name(ui->edtName->text().toStdString());
    m_spot->set_spot_duration(ui->edtSpotDuration->value());
    m_spot->set_real_duration(ui->edtRealDuration->value());
    m_spot->set_client(m_client->id());

    auto dayparts = m_daypart->read_grid();
    m_spot->set_daypart1(dayparts[1]);
    m_spot->set_daypart2(dayparts[2]);
    m_spot->set_daypart3(dayparts[3]);
    m_spot->set_daypart4(dayparts[4]);
    m_spot->set_daypart5(dayparts[5]);
    m_spot->set_daypart6(dayparts[6]);
    m_spot->set_daypart7(dayparts[7]);

}

void SpotForm::populateFormWidgets()
{
    ui->edtName->setText(stoq(m_spot->name()->value()));
    ui->edtSpotDuration->setValue(m_spot->spot_duration()->value());
    ui->edtRealDuration->setValue(m_spot->real_duration()->value());

    ui->cbBrands->setModel(m_spot->brand()->dataModel());
    EntityDataModel edm;

    ui->edtClient->setText(stoq(m_client->name()->value()));
}

void SpotForm::brandsComboChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbBrands->model());
    m_spot->brand()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}

void SpotForm::on_import_audio()
{
    auto audio_file_full_path = QFileDialog::getOpenFileName(this,
                                                   tr("Import Audio"), "/d/home/audio",
                                                   tr("Audio Files (*.ogg *.mp3 *.wav)"));
    if (audio_file_full_path.isEmpty())
        return;

    // open audio property dialog to enter basic audio details

    AudioFile audio_file(audio_file_full_path.toStdString());
    ADFRepository adf_repo;
    AudioTool audio_tool;

    Marker marker;

    if (fs::exists(audio_file.adf_file()) ){
        // FIXME: return markers instead of sending audio_file as reference.
        adf_repo.read_markers(audio_file);

        auto af_markers = audio_file.marker();

        marker.start_marker = af_markers.start_marker;
        marker.fade_in = af_markers.fade_in;
        marker.intro = af_markers.intro;
        marker.extro = af_markers.extro;
        marker.fade_out = af_markers.fade_out;
        marker.end_marker = af_markers.end_marker;
    }

    qDebug() << "AUDIO FILE: "<< QString::fromStdString(audio_file.audio_file());
    qDebug() << "WAVE FILE: "<< QString::fromStdString(audio_file.wave_file());

    if (!fs::exists(audio_file.wave_file()) ){
        audio_tool.generate_wave_file(audio_file.audio_file(), audio_file.wave_file());
        qDebug() << "Wave generated.";
    }


    auto spot_audio = std::make_unique<TRAFFIK::SpotAudio>(m_spot, new AUDIO::Audio(audio_file.audio_file()));
    auto spot_audio_form = std::make_unique<SpotAudioForm>(spot_audio.get());

    spot_audio_form->exec();

    CueEditor* cue_editor = new CueEditor(audio_file);
    if (cue_editor->editor() == 1)
        audio_file.set_marker(cue_editor->marker());



    /*

    QFileInfo file_info(audio_file_full_path);

    QString audio_file = file_info.fileName();

    auto comm_audio_folder = m_setup->comm_audio_folder()->value();


    qDebug() << "Filename: " << audio_file;
    qDebug() << "Folder: " << QString::fromStdString(comm_audio_folder);

    */



    /* Convert non-ogg audio files to ogg */

    /*
    std::string dest_audio_file = comm_audio_folder+"\\"+audio_file.toStdString();

    fs::copy(audio_file_full_path.toStdString(), dest_audio_file);

    EntityDataModel edm;

    std::stringstream sql;

    sql << "Insert into rave_track (title, filepath, artist_id, track_type) "
        << " Values ('Main Comm Audio', '"+comm_audio_folder+"', 2, 'COMM') RETURNING id;";

    */

//    try{
//        int id = edm.insert_returning_id(sql.str());
//        qDebug() << "Last Id: "<< id;
//    } catch (DatabaseException& de) {
//        showMessage(de.errorMessage());
//    }


}

void SpotForm::cue_edit()
{
    /*
    AudioFile audio_file("D:\\home\\cpp\\audiowaveform_win\\stereo.mp3");

    if (audio_file.is_valid()){

        audio_file.set_wave_file("D:\\home\\cpp\\audiowaveform_win\\test.png");
        audio_file.set_duration(7);
        audio_file.set_artist_name("Nightwish");
        audio_file.set_audio_title("Nemo");
        CueEditor* cue_editor = new CueEditor(audio_file);
        auto mark = cue_editor->editor();
        }
        */

}

