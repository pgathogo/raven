#include <sstream>

#include <QTableWidgetItem>

#include "../framework/ravenexception.h"
#include "../framework/entitydatamodel.h"

#include "selectstationform.h"
#include "ui_selectstationform.h"

SelectStationForm::SelectStationForm(QWidget* parent)
    :QDialog(parent)
{}

SelectStationForm::SelectStationForm(const QString username, QWidget *parent) :
    QDialog(parent)
    ,ui(new Ui::selectstationform)
    ,m_username{username}
{
    ui->setupUi(this);
    connect(ui->btnSelect, &QPushButton::clicked, this, &SelectStationForm::select_station);
    connect(ui->btnCancel, &QPushButton::clicked, this, &SelectStationForm::cancel);
    connect(ui->twStations, &QTableWidget::cellDoubleClicked, this, &SelectStationForm::select_station);

    ui->twStations->setAlternatingRowColors(true);

    setWindowTitle("Station Selection");
    setWindowIcon(QIcon(":/rave_images/media/icons/raven.bmp"));
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
    int station_id = model_index.data(Qt::UserRole).toInt();
    m_selected_station = m_stations_info[station_id];

    done(1);

}

int SelectStationForm::station_id()
{
    return m_sel_station_id;
}

StationInfo SelectStationForm::selected_station()
{
    return m_selected_station;
}

void SelectStationForm::cancel()
{
    done(0);
}

bool SelectStationForm::fetch_user_stations(const QString username)
{
    // TODO: Once we populate the station info, we need to memorize
    // this info to avoid DB refetching for the same user

    std::stringstream sql;

    std::string user_filter = std::format(" and ua.username = '{}'", username.toStdString());

    sql << " select svr.id AS server_id, svr.server_name, svr.server_ip, svr.port_no, "
        << " st.id AS station_id, st.station_name, st.db_name, st.cluster_id, cl.cluster_name "
        << " from rave_server svr, rave_station st, rave_useraccess ua, rave_cluster cl"
        << " Where svr.cluster_id = st.cluster_id "
        << " and st.id = ua.station_id "
        << " and cl.id = st.cluster_id "
        << " and svr.server_type = 'DBS' "
        << user_filter;


    EntityDataModel edm;

    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();

    if (provider->cacheSize() == 0)
        return false;


    provider->cache()->first();
    do{
        auto itb = provider->cache()->currentElement()->begin();
        auto ite = provider->cache()->currentElement()->end();

        StationInfo si;

        for (; itb != ite; ++itb)
        {
            std::string field = (*itb).first;
            std::string value = (*itb).second;

            if (field == "server_id")
                si.server_id = std::stoi(value);

            if (field == "server_name" && (!value.empty()))
                si.server_name = stoq(value);

            if (field == "server_ip" && (!value.empty()))
                si.ip_address = stoq(value);

            if (field == "port_no" && (!value.empty()))
                si.port_no = std::stoi(value);

            if (field == "station_id")
                si.station_id = std::stoi(value);

            if (field == "station_name" && (!value.empty()))
                si.station_name = stoq(value);

            if (field == "db_name" && (!value.empty()))
                si.db_name = stoq(value);

            if (field == "cluster_name" && (!value.empty()))
                si.cluster_name = stoq(value);

            if (field == "cluster_id")
                si.cluster_id = std::stoi(value);

        }

        m_stations_info[si.station_id] = si;

        provider->cache()->next();
    }while(!provider->cache()->isLast());

    return true;
}




void SelectStationForm::showEvent(QShowEvent *)
{
    fetch_user_stations(m_username);
    populate_table(m_stations_info);

    ui->btnSelect->setEnabled((m_stations_info.size() > 0));

    ui->twStations->selectRow(0);
}


