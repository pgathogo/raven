#include "stationform.h"
#include "ui_stationform.h"

#include "../../../rave/framework/ui_baseentitydetaildlg.h"
#include "../../../rave/framework/baseentitydetaildlg.h"

#include "station.h"

StationForm::StationForm(ClusterManager::Station* station, QDialog* parent)
    :BaseEntityDetailDlg(parent)
    ,ui(new Ui::StationForm)
    ,m_station{station}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    populateFormWidgets();

    ui->edtStation->setFocusPolicy(Qt::StrongFocus);
    ui->edtStation->setFocus();
}

StationForm::~StationForm()
{
    delete ui;
}

ActionResult StationForm::saveRecord()
{
    populateEntityFields();
    ActionResult ar = m_station->validate();
    return ar;
}

std::string StationForm::windowTitle()
{
    return "Station";
}

void StationForm::populateEntityFields()
{
    m_station->station_name()->setValue(ui->edtStation->text().toStdString());
    m_station->db_name()->setValue(ui->edtDBName->text().toStdString());
    m_station->station_code()->setValue(ui->edtStationCode->text().toStdString());
}

void StationForm::populateFormWidgets()
{
    ui->edtStation->setText(stoq(m_station->station_name()->value()));
    ui->edtDBName->setText(stoq(m_station->db_name()->value()));
    ui->edtStationCode->setText(stoq(m_station->station_code()->value()));
}
