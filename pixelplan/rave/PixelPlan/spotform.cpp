#include <QApplication>
#include <filesystem>
#include <sstream>

#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

#include "../../../rave/utils/daypartgrid.h"
#include "../../../rave/utils/tools.h"

#include "../../../rave/framework/ui_baseentitydetaildlg.h"
#include "../../../rave/framework/baseentitydetaildlg.h"
#include "../../../rave/framework/manytomanybrowser.h"
#include "../../../rave/framework/manytomany.h"
#include "../../../rave/framework/entitydatamodel.h"
#include "../../../rave/framework/ravensetup.h"
#include "../../../rave/framework/choicefield.h"

#include "../../../rave/audio/audiofile.h"
#include "../../../rave/audio/audiotool.h"
#include "../../../rave/audio/audio.h"
#include "../../../rave/audiolib/headers/cueeditor.h"

#include "spotform.h"
#include "ui_spotform.h"
#include "client.h"
#include "spotvoiceover.h"
#include "spottypeexclusion.h"
#include "spotaudio.h"
#include "spotaudioform.h"
#include "timeband.h"
#include "mediabrowser.h"
#include "advertmedia.h"

namespace fs = std::filesystem;

SpotForm::SpotForm(std::shared_ptr<Client> client,
                   std::shared_ptr<TRAFFIK::Spot> spot,
                   QDialog* parent)
    :BaseEntityDetailDlg{parent}
    ,ui{new Ui::SpotForm}
    ,m_client{client}
    ,m_spot{spot}
    ,m_setup{nullptr}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());

    add_time_band_widget();

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


    m_media_browser = std::make_unique<MediaBrowser>(m_spot, m_client);
    ui->vlMedia->addWidget(m_media_browser.get());

    connect(m_media_browser.get(), &MediaBrowser::audio_duration, this, &SpotForm::on_audio_duration);

    // m_spot_audio_browser =
    //         std::make_unique<SpotAudioBrowser>(&m_spot->spot_audio(),
    //                                            ui->vlSpotAudio,
    //                                            this);
    // m_spot_media_browser =
    //         std::make_unique<SpotMediaBrowser>(&m_spot->spot_media(),
    //                                            ui->vlSpotAudio,
    //                                            this);


    // connect(m_spot_audio_browser.get(), &SpotAudioBrowser::audio_duration, this, &SpotForm::update_audio_duration);

    connect(ui->cbBrands, QOverload<int>::of(&QComboBox::currentIndexChanged),
           this, &SpotForm::brandsComboChanged);

    //connect(ui->btnCueEdit, &QPushButton::clicked, this, &SpotForm::cue_edit);

    ui->tabWidget->setCurrentIndex(0);

    m_edm_setup = std::make_unique<EntityDataModel>(
                std::make_unique<RavenSetup>());
    m_edm_setup->all();


    if (m_edm_setup->count() > 0)
        m_setup = std::dynamic_pointer_cast<RavenSetup>(m_edm_setup->firstEntity());
    else
        m_setup = std::make_shared<RavenSetup>();



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

std::shared_ptr<PIXELPLAN::AdvertMedia> SpotForm::advert_media()
{
    return m_media_browser->advert_media();
}

ActionResult SpotForm::saveRecord()
{

    populateEntityFields();
    return m_spot->validate();
}

void SpotForm::on_audio_duration(qint64 duration)
{

    ui->edtSpotDuration->setTime(duration_to_time(duration));
    ui->edtRealDuration->setValue(duration);
    m_spot->set_spot_duration(duration);
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
    // return m_spot_audio_browser;
}

std::string SpotForm::windowTitle()
{
    return "Spot Details - Client: " +m_client->name()->displayName();
}

void SpotForm::add_time_band_widget()
{
    //create_time_band_widget();
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

    if (m_edm_time_band->count() > 0) {
        m_time_band_widget->setRowCount(m_edm_time_band->count());
        populate_time_band_widget(m_edm_time_band, m_time_band_widget);
    }

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

    // m_spot->set_spot_duration(ui->edtRealDuration()->value());

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
    ui->edtSpotDuration->setTime(duration_to_time(m_spot->spot_duration()->value()));
    ui->edtRealDuration->setValue(m_spot->real_duration()->value());

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
    // m_spot_audio_browser->stop_play();
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

