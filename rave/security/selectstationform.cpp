#include <QTableWidgetItem>

#include "selectstationform.h"
#include "ui_selectstationform.h"

SelectStationForm::SelectStationForm(std::map<int, StationInfo>& si, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectstationform)
{
    ui->setupUi(this);
    connect(ui->btnSelect, &QPushButton::clicked, this, &SelectStationForm::select_station);
    connect(ui->btnCancel, &QPushButton::clicked, this, &SelectStationForm::cancel);
    connect(ui->twStations, &QTableWidget::cellDoubleClicked, this, &SelectStationForm::select_station);
    populate_table(si);

    setWindowTitle("Station Selection");
}

SelectStationForm::~SelectStationForm()
{
    delete ui;
}


void SelectStationForm::populate_table(std::map<int, StationInfo>& station_info)
{
    QStringList headers = {"Station", "Cluster"};

    ui->twStations->setHorizontalHeaderLabels(headers);
    ui->twStations->setRowCount(station_info.size());

    int row = 0;
    for(auto&[station_id, si]: station_info)
    {
        qDebug() << si.station_name;

        QTableWidgetItem* ti_station = new QTableWidgetItem(si.station_name);
        ti_station->setData(Qt::UserRole, station_id);

        QTableWidgetItem* ti_cluster = new QTableWidgetItem(si.cluster_name);

        ui->twStations->setItem(row, 0, ti_station);
        ui->twStations->setItem(row, 1, ti_cluster);

        ++row;

    }

    ui->twStations->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void SelectStationForm::select_station()
{
    auto model_indexes = ui->twStations->selectionModel()->selectedIndexes();
    if (model_indexes.size() == 0)
        return;

    auto model_index = model_indexes[0];
    m_sel_station_id = model_index.data(Qt::UserRole).toInt();
    done(1);

}

int SelectStationForm::station_id()
{
    return m_sel_station_id;
}

void SelectStationForm::cancel()
{
    done(0);

}
