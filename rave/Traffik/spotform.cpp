#include <QApplication>
#include <filesystem>
#include <sstream>

#include <QFileDialog>

#include "../utils/daypartgrid.h"
#include "../utils/tools.h"

#include "../framework/ui_baseentitydetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/manytomanybrowser.h"
#include "../framework/manytomany.h"
#include "../framework/entitydatamodel.h"
#include "../framework/ravensetup.h"
#include "../framework/choicefield.h"

#include "../audio/audiofile.h"
#include "../audio/audiotool.h"
#include "../audio/audio.h"
#include "../audiolib/headers/cueeditor.h"

#include "spotform.h"
#include "ui_spotform.h"
#include "client.h"
#include "spotvoiceover.h"
#include "spottypeexclusion.h"
#include "spotaudio.h"
#include "spotaudioform.h"

namespace fs = std::filesystem;

SpotForm::SpotForm(Client* client, TRAFFIK::Spot* spot, QDialog* parent)
    :BaseEntityDetailDlg{parent}
     ,ui{new Ui::SpotForm}
     ,m_client{client}
     ,m_spot{spot}
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

    connect(m_spot_audio_browser.get(), &SpotAudioBrowser::audio_duration, this, &SpotForm::update_audio_duration);

    connect(ui->cbBrands, QOverload<int>::of(&QComboBox::currentIndexChanged),
           this, &SpotForm::brandsComboChanged);

    //connect(ui->btnCueEdit, &QPushButton::clicked, this, &SpotForm::cue_edit);

    ui->tabWidget->setCurrentIndex(0);

    m_edm_setup = std::make_unique<EntityDataModel>(
                std::make_unique<RavenSetup>());
    m_edm_setup->all();
    m_setup = dynamic_cast<RavenSetup*>(m_edm_setup->firstEntity().get());


    ui->edtRealDuration->setMaximum(999999);

    int w = 720;
    int h = 530;

    int max_width = 780;
    int max_height = 550;

    setMinimumSize(w, h);
    setMaximumSize(max_width, max_height);
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

const AudioCreationMode SpotForm::get_audio_creation_mode() const
{
    return m_spot_audio_browser->audio_creation_mode();
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
    return "Spot Details - Client: " +m_client->name()->displayName();
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

    qDebug() << "EDM Count: "<<  m_spot->brand()->dataModel()->count();

    ui->cbBrands->setModel(m_spot->brand()->dataModel());
    ui->cbBrands->setCurrentIndex(ui->cbBrands->findText(
        stoq(m_spot->brand()->displayName())));

    ui->edtClient->setText(stoq(m_client->name()->value()));
}

void SpotForm::brandsComboChanged(int i)
{
    if (i < 0)
        return;

    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbBrands->model());
    m_spot->brand()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}

void SpotForm::on_import_audio()
{
}

void SpotForm::cue_edit()
{
}

void SpotForm::update_audio_duration(int audio_duration)
{
    ui->edtRealDuration->setValue(audio_duration);
}

void SpotForm::closeEvent(QCloseEvent* event)
{
    m_spot_audio_browser->stop_play();
}

