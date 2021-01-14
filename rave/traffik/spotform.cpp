#include <sstream>
#include <QFileDialog>
#include "spotform.h"
#include "ui_spotform.h"
#include "ui_baseentitydetaildlg.h"
#include "../utils/daypartgrid.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/manytomanybrowser.h"
#include "../framework/manytomany.h"
#include "../framework/entitydatamodel.h"
#include "../utils/tools.h"

#include "client.h"
#include "spotvoiceover.h"
#include "spottypeexclusion.h"

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
            std::make_unique<ManyToManyBrowser>(&m_spot->set_voice_over(),
                                                ui->vlVoiceOver,
                                                this);

    m_MtoMTypeExBrowser =
            std::make_unique<ManyToManyBrowser>(&m_spot->set_type_exclusion(),
                                                ui->vlTypeEx,
                                                this);

    connect(ui->cbBrands, QOverload<int>::of(&QComboBox::currentIndexChanged),
           this, &SpotForm::brandsComboChanged);

    connect(ui->btnImport, &QPushButton::clicked, this, &SpotForm::on_import_audio);

    ui->tabWidget->setCurrentIndex(0);
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

int SpotForm::parentId() const
{
    return m_spot->id();
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
    auto audio_file = QFileDialog::getOpenFileName(this,
                                                   tr("Import Audio"), "/d/home/audio",
                                                   tr("Audio Files (*.ogg *.mp3 *.wav)"));

    EntityDataModel edm;

    std::stringstream sql;

    sql << "Insert into rave_track (title, filepath, artist_id, track_type) "
        << " Values ('Main Comm Audio', 'd:\\home\\pms\\raven\\audio\\', 2, 'COMM') RETURNING id;";

    try{
        int id = edm.insert_returning_id(sql.str());
        qDebug() << "Last Id: "<< id;
    } catch (DatabaseException& de) {
        showMessage(de.errorMessage());
    }

}

