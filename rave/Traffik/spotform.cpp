#include <QApplication>
#include <filesystem>
#include <sstream>

#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

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
#include "../audio/audiopropertywidget.h"
#include "../audiolib/headers/cueeditor.h"

#include "../audio/spotaudioform.h"

#include "spotform.h"
#include "ui_spotform.h"
#include "client.h"
#include "spotvoiceover.h"
#include "spottypeexclusion.h"
#include "spotaudio.h"
#include "spotaudioform.h"
#include "timeband.h"

namespace fs = std::filesystem;

SpotForm::SpotForm(std::shared_ptr<Client> client,
                   std::shared_ptr<TRAFFIK::Spot> spot,
                   QDialog* parent)
    :BaseEntityDetailDlg{parent}
     ,ui{new Ui::SpotForm}
     ,m_client{client}
     ,m_spot{spot}
     ,m_audio{nullptr}
    ,m_audio_prop_widget{nullptr}
{
    ui->setupUi(bui->baseContainer);

    setTitle(windowTitle());

    //add_time_band_widget();

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

    connect(ui->cbBrands, QOverload<int>::of(&QComboBox::currentIndexChanged),
           this, &SpotForm::brandsComboChanged);

    make_audio_widget();

    if (spot->audio()->value() > 0)
        add_audio_property_widget(spot->audio()->value());

    //connect(ui->btnCueEdit, &QPushButton::clicked, this, &SpotForm::cue_edit);

    ui->tabWidget->setCurrentIndex(0);

    m_edm_setup = std::make_unique<EntityDataModel>(
                           std::make_unique<RavenSetup>());
    m_edm_setup->all();
    m_setup = std::dynamic_pointer_cast<RavenSetup>(m_edm_setup->firstEntity());

    ui->edtRealDuration->setMaximum(999999);

    int w = 720;
    int h = 650;

    int max_width = 780;
    int max_height = 900;

    setMinimumSize(w, h);
    setMaximumSize(max_width, max_height);
}

void SpotForm::make_audio_widget()
{

    auto hl = hline(this);
    ui->vlAudio->addWidget(hl);

    // Make audio toolbar
    QSize button_size = QSize(100, 30);

    QHBoxLayout* hlToolbar = new QHBoxLayout();
    QPushButton* btnImport = new QPushButton("Import Audio...");
    connect(btnImport, &QPushButton::clicked, this, &SpotForm::on_import_audio);

    btnImport->setMinimumSize(button_size);

    QPushButton* btnAttach = new QPushButton("Attach Audio...");
    connect(btnAttach, &QPushButton::clicked, this, &SpotForm::on_attach_audio);
    btnAttach->setMinimumSize(button_size);

    hlToolbar->addWidget(btnImport);
    hlToolbar->addWidget(btnAttach);
    hlToolbar->addStretch(1);

    ui->vlAudio->addLayout(hlToolbar);

}

void SpotForm::make_property_widget(std::shared_ptr<AUDIO::Audio> audio)
{
    m_audio_prop_widget = new AUDIO::AudioPropertyWidget(audio);
    connect(m_audio_prop_widget, &AUDIO::AudioPropertyWidget::cue_edit_audio, this, &SpotForm::cue_edit);
    ui->vlAudio->addWidget(m_audio_prop_widget);
}

void SpotForm::add_audio_property_widget(int audio_id)
{
    auto edm = std::make_unique<EntityDataModel>(std::make_shared<AUDIO::Audio>());
    edm->getById({"id", "=", audio_id});

    if (edm->is_empty())
        return;

    m_audio = std::dynamic_pointer_cast<AUDIO::Audio>(edm->getEntity());
    make_property_widget(m_audio);
    // m_audio_prop_widget = new AUDIO::AudioPropertyWidget(m_audio);
    // connect(m_audio_prop_widget, &AUDIO::AudioPropertyWidget::cue_edit_audio, this, &SpotForm::cue_edit);
    //ui->vlAudio->addWidget(m_audio_prop_widget);

}

void SpotForm::on_import_audio()
{
    auto audio_filepath = QFileDialog::getOpenFileName(this,
                                                    tr("Import Audio"), QDir::currentPath(),
                                                    tr("OGG Files (*.ogg);; MP3 Files (*.mp3);; MTS Files (*.mts);; Wave Files (*.wav)"));
    if (audio_filepath.isEmpty())
         return;

    m_audio = std::make_shared<AUDIO::Audio>(audio_filepath.toStdString());

    m_audio->set_source_filepath(audio_filepath.toStdString());

  // if (m_audio == nullptr)
   //m_audio = std::make_shared<AUDIO::Audio>("D:/Home/Clients/PMS/RavenData/Audio/MP3/BRITANIA_WHEN_I_NEED.mp3");

   AUDIO::SpotAudioForm saf(m_audio,
                            ui->edtName->text(),
                            QString::fromStdString(m_client->name()->value()),
                            this);
   if (saf.exec()) {

       if (m_audio_prop_widget == nullptr) {
            make_property_widget(m_audio);
           // m_audio_prop_widget = new AUDIO::AudioPropertyWidget(m_audio);
           // connect(m_audio_prop_widget, &AUDIO::AudioPropertyWidget::cue_edit_audio, this, &SpotForm::cue_edit);
           //ui->vlAudio->addWidget(m_audio_prop_widget);
       } else {
           m_audio_prop_widget->update_widget(m_audio);
       }

       if (m_audio->duration()->value() > 0)
       {
           double msec = m_audio->duration()->value();
           double secs = m_audio->duration()->value() / 1000.0;

           ui->edtSpotDuration->setValue(secs);
           ui->edtRealDuration->setValue(msec);
       }

   } else {

       m_audio = nullptr;
   }

}

void SpotForm::on_attach_audio()
{

}

QFrame* SpotForm::hline(QWidget* parent)
{
    QFrame* hline = new QFrame(parent);
    hline->setFrameShape(QFrame::HLine);
    hline->setFrameShadow(QFrame::Sunken);

    return hline;
}

SpotForm::~SpotForm()
{
    delete ui;
}

bool SpotForm::audio_is_available()
{
    return (m_audio == nullptr) ? false : true;
}

ActionResult SpotForm::saveRecord()
{
    populateEntityFields();
    // Check that the spot has audio
    if (!audio_is_available() ) {
        return std::make_tuple(ActionResultType::arERROR, "No audio found!");
    }

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

std::shared_ptr<AUDIO::Audio> SpotForm::spot_audio()
{
    return m_audio;
}

int SpotForm::parentId() const
{
    return m_spot->id();
}

std::string SpotForm::windowTitle()
{
    return "Spot Details - Client: " +m_client->name()->displayName();
}

void SpotForm::add_time_band_widget()
{
    create_time_band_widget();
    create_dow_widget();
}

void SpotForm::create_time_band_widget()
{
    m_time_band_widget = new QTableWidget();
    m_time_band_widget->setMaximumWidth(200);
    m_time_band_widget->setColumnCount(2);
    m_time_band_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_time_band_widget->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_time_band_widget->setSelectionMode(QAbstractItemView::MultiSelection);

    connect(m_time_band_widget, &QTableWidget::itemClicked, this, &SpotForm::time_band_selected);

    QStringList header;
    header << "Band" << "Dist";

    //twTimeBand->setHorizontalHeaderLabels(header);

    m_edm_time_band = std::make_unique<EntityDataModel>(
                          std::make_unique<TimeBand>());

    m_edm_time_band->all();

    m_time_band_widget->setRowCount(m_edm_time_band->count());

    populate_time_band_widget(m_edm_time_band, m_time_band_widget);

    QGroupBox* gbTimeBand = new QGroupBox("Time Bands");
    gbTimeBand->setMaximumWidth(240);
    gbTimeBand->setLayout(new QHBoxLayout());
    gbTimeBand->layout()->addWidget(m_time_band_widget);

    ui->vlTimeBand->addWidget(gbTimeBand);
}

void SpotForm::create_dow_widget()
{
    /*
    QGridLayout* glDOW = new QGridLayout();
    int ROW0 = 0;
    int ROW1 = 1;
    int COL0 = 0;
    int COL1 = 1;

    QLabel*  lblMon = new QLabel("Mon");
    QLineEdit* edtMon = new QLineEdit();
    glDOW->addWidget(lblMon, ROW0, COL0);
    glDOW->addWidget(edtMon, ROW1, COL0);
    */

    m_dow_widget = std::make_unique<DowWidget>();
    QGroupBox* gbDOW = new QGroupBox("Days of Week");
    gbDOW->setLayout(m_dow_widget->grid_layout());
    ui->hlDOW->addWidget(gbDOW);
}



void SpotForm::populate_time_band_widget(const std::unique_ptr<EntityDataModel>& edm, QTableWidget* tab)
{
    auto provider = edm->getDBManager()->provider();
    if (provider->cacheSize() == 0)
        return;

    provider->cache()->first();
    int row = 0;

    do{
        auto it_begin = provider->cache()->currentElement()->begin();
        auto it_end = provider->cache()->currentElement()->end();

        int id = -1;
        QString tb_name;

        for(; it_begin != it_end; ++it_begin) {
            std::string field_name = (*it_begin).first;
            std::string field_value = (*it_begin).second;

            if (field_name == "id")
                id = std::stoi(field_value);
            if (field_name == "name")
                tb_name = QString::fromStdString(field_value);
        }

        auto twi = new QTableWidgetItem(tb_name);
        twi->setData(Qt::UserRole, id);
        tab->setItem(row, 0, twi);

        QSpinBox* sb = new QSpinBox();
        sb->setFixedWidth(50);
        sb->setMaximum(99);
        tab->setCellWidget(row, 1, sb);

        ++row;

        provider->cache()->next();

    } while(!provider->cache()->isLast());


}

void SpotForm::populateGrid()
{
    m_daypart = std::make_unique<DayPartGrid>(ui->vlDaypart);
    m_daypart->set_size_policy(GridSizePolicy::Auto);

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

    if (!m_spot->add_date()->value().isValid())
        m_spot->add_date()->setValue(QDate::currentDate());

}

void SpotForm::populateFormWidgets()
{
    ui->edtName->setText(stoq(m_spot->name()->value()));
    ui->edtSpotDuration->setValue(m_spot->spot_duration()->value());
    ui->edtRealDuration->setValue(m_spot->real_duration()->value());

    ui->cbBrands->setModel(m_spot->brand()->dataModel());
    ui->cbBrands->setCurrentIndex(ui->cbBrands->findText(
        stoq(m_spot->brand()->displayName())));

    ui->edtClient->setText(stoq(m_client->name()->value()));

    // If audio_id != null, load the Audio
   // if (m_spot->aud)
}

void SpotForm::brandsComboChanged(int i)
{
    if (i < 0)
        return;

    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbBrands->model());
    m_spot->brand()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}


void SpotForm::cue_edit()
{
   AUDIO::SpotAudioForm saf(m_audio,
                            ui->edtName->text(),
                            QString::fromStdString(m_client->name()->value()),
                            this);
   if (saf.exec())
   {
     // Update audio: Title, Notes, and Cue Markers
     // make_property_widget(m_audio);
   }
}

void SpotForm::update_audio_duration(int audio_duration)
{
    ui->edtRealDuration->setValue(audio_duration);
}

void SpotForm::closeEvent(QCloseEvent* event)
{
}

void SpotForm::time_band_selected()
{
    m_daypart->clear_all_cells();

    QItemSelectionModel* ism = m_time_band_widget->selectionModel();
    QModelIndexList mil = ism->selectedIndexes();

    if (mil.size() == 0)
        return;

    for(auto& index: mil) {
    }

}

