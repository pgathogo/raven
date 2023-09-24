#include "useraccessform.h"
#include "ui_useraccessform.h"

#include "../../../security/user.h"
#include "../framework/entitydatamodel.h"
#include "../framework/ravenexception.h"
#include "station.h"
#include "server.h"
#include "clustercontroller.h"
#include "../security/authentication.h"
#include "../security/user.h"

UserAccessForm::UserAccessForm(SECURITY::User* user, QWidget *parent)
    :QDialog(parent)
    , m_user{user}
    ,ui(new Ui::UserAccessForm)
{
    ui->setupUi(this);

    ui->lblUser->setText(user->role_name()->to_qstring());

    load_stations();
    load_user_stations(user->role_name()->value());

    connect(ui->lwStations, &QListWidget::itemClicked, this, &UserAccessForm::station_selected);
    connect(ui->btnCancel, &QPushButton::clicked, this, &UserAccessForm::close);
    connect(ui->btnSave, &QPushButton::clicked, this, &UserAccessForm::save);

    connect(ui->tbAdd, &QToolButton::clicked, this, &UserAccessForm::add_station);
    connect(ui->tbDelete, &QToolButton::clicked, this, &UserAccessForm::remove_station);

    m_cluster_controller = std::make_unique<ClusterManager::ClusterController>();

    setup_gui();
}

UserAccessForm::~UserAccessForm()
{
    delete ui;
}

std::map<int, StationAccess> UserAccessForm::user_access()
{
    return m_user_access;
}

void UserAccessForm::setup_gui()
{
    setWindowTitle("User Station Attachment");

    ui->tbAdd->setIconSize(QSize(30,30));
    ui->tbAdd->setIcon(QIcon(":/images/icons/right_arrow.png"));

    ui->tbDelete->setIconSize(QSize(30,30));
    ui->tbDelete->setIcon(QIcon(":/images/icons/left_arrow.png"));
}

void UserAccessForm::save()
{
    done(1);
}

void UserAccessForm::print_station_access()
{
    auto print_status = [&](AccessAction aa){
        switch(aa)
        {
        case AccessAction::None:
            return "None";
            break;
        case AccessAction::New:
            return "New";
            break;
        case AccessAction::Delete:
            return "Delete";
            break;
        default:
            return "None";
        }
    };

    for(auto&[id, sa] : m_user_access){
        qDebug() << "Station :"
                 << id <<" Name: "
                 << sa.name << " Status: "<< print_status(sa.status);
    }

}

void UserAccessForm::load_stations()
{
    std::unique_ptr<EntityDataModel> edm =
        std::make_unique<EntityDataModel>(std::make_shared<ClusterManager::Station>());

    edm->all();

    auto provider = edm->getDBManager()->provider();
    if (provider->cacheSize() == 0)
        return;

    provider->cache()->first();
    do{
        auto itb = provider->cache()->currentElement()->begin();
        auto ite = provider->cache()->currentElement()->end();

        int station_id;
        int cluster_id;
        QString station_name;
        QString db_name;

        for(; itb != ite; ++itb)
        {
            std::string field = (*itb).first;
            std::string value = (*itb).second;


            if (field == "id"){
                station_id = std::stoi(value);
            }

            if(field == "station_name"){
                station_name = QString::fromStdString(value);
                QListWidgetItem* lwi = new QListWidgetItem(station_name);
                lwi->setData(Qt::UserRole, station_id);
                ui->lwStations->addItem(lwi);
            }

            if (field == "db_name"){
                db_name = QString::fromStdString(value);
            }

            if(field == "cluster_id"){
                cluster_id = std::stoi(value);
            }

        }

        StationData sd;
        sd.station_id = station_id;
        sd.cluster_id = cluster_id;
        sd.station_name = station_name;
        sd.db_name = db_name;

        m_station_data[station_id] = sd;


        provider->cache()->next();

    } while(!provider->cache()->isLast());

}

void UserAccessForm::load_user_stations(std::string username)
{
    std::stringstream sql;

    std::string filter = std::format(" and rave_useraccess.username = '{}'", username);

    sql << "Select rave_useraccess.id, rave_useraccess.station_id, "
        << "rave_cluster.cluster_name, rave_station.station_name "
        << " from rave_useraccess, rave_station, rave_cluster "
        << " Where rave_useraccess.station_id = rave_station.id "
        << "  and rave_cluster.id = rave_station.cluster_id "
        << filter;

    qDebug() << QString::fromStdString(sql.str());

    EntityDataModel edm;

    try{
        edm.readRaw(sql.str());
    } catch (DatabaseException& de){
        qDebug() << "Log error message";
    }

    auto provider = edm.getDBManager()->provider();

    if (provider->cacheSize() == 0)
        return;

    provider->cache()->first();
    do{
        auto itb = provider->cache()->currentElement()->begin();
        auto ite = provider->cache()->currentElement()->end();

        int id = -1;
        int station_id = -1;
        QString name;
        QString cluster_name;

        for (; itb != ite; ++itb)
        {
            std::string field = (*itb).first;
            std::string value = (*itb).second;


            if (field == "id"){
                id = std::stoi(value);
            }

            if(field == "station_id"){
                station_id = std::stoi(value);
            }

            if (field == "cluster_name"){
                cluster_name = QString::fromStdString(value);
            }

            if (field == "station_name"){
                name =QString::fromStdString(value);

                QString title = name + " ("+cluster_name+")";
                QListWidgetItem* lwi = new QListWidgetItem(title);
                lwi->setData(Qt::UserRole, station_id);
                ui->lwAccess->addItem(lwi);
            }

        }
            StationAccess sa;
            sa.id = id;
            sa.station_id = station_id;
            sa.name = name;
            sa.status = AccessAction::None;
            m_user_access[station_id] = sa;

        provider->cache()->next();
    } while(!provider->cache()->isLast());


}

void UserAccessForm::station_selected(QListWidgetItem* selected_item)
{
    int station_id = selected_item->data(Qt::UserRole).toInt();
    qDebug() << "Station ID: "<< station_id;
}

void UserAccessForm::add_station()
{
    qDebug() << "AAA";

    auto selected_items = ui->lwStations->selectedItems();
    if (selected_items.size() == 0)
        return;

    qDebug() << "BBB";

    auto item = selected_items[0];
    int station_id = item->data(Qt::UserRole).toInt();

    if (m_user_access.contains(station_id))
        return;

    qDebug() << "CCC";

    StationData sd = m_station_data[station_id];

    QString error_title = "Error: Station Attachment";
    // Check if the station has DB name set.
    if (sd.db_name.isEmpty()){
        QMessageBox::critical(this,error_title,"Failed to attach station to user! Station database is not configured.");
        return;
    }

    qDebug() << "DDD";

    // Check if db name is a valid database
    ConnInfo ci = find_db_server(sd.db_name, sd.cluster_id);
    if (ci.db_name.empty()){
        QMessageBox::critical(this, error_title,"Failed to attach station to user! Station database NOT valid.");
        return;
    }

    qDebug() << "EEE";
    QString name = sd.station_name;

    QListWidgetItem* s_item = new QListWidgetItem(name);
    s_item->setData(Qt::UserRole, station_id);

    ui->lwAccess->addItem(s_item);

    add_access(station_id, sd.cluster_id, name);

    qDebug() << "FFF";
}

void UserAccessForm::add_access(int station_id, int cluster_id, QString name)
{
    if (m_user_access.contains(station_id))
        return;

    StationAccess sa;
    sa.station_id = station_id;
    sa.cluster_id = cluster_id;
    sa.name = name;
    sa.status = AccessAction::New;
    m_user_access[station_id] = sa;
}

void UserAccessForm::remove_access(int station_id)
{
    StationAccess sa = m_user_access[station_id];

    switch (sa.status)
    {
    case AccessAction::None:
        m_user_access[station_id].status = AccessAction::Delete;
        break;
    case AccessAction::New:
        auto iter = m_user_access.find(station_id);
        if (iter != m_user_access.end())
            m_user_access.erase(iter);
        break;
    }

}

void UserAccessForm::remove_station()
{
    auto selected_items = ui->lwAccess->selectedItems();
    if (selected_items.size() == 0)
        return;
    auto item = selected_items[0];

    int station_id = item->data(Qt::UserRole).toInt();
    QString name = item->text();

    remove_access(station_id);

    auto del_item = ui->lwAccess->takeItem(ui->lwAccess->row(item));
    delete del_item;

}

ConnInfo UserAccessForm::find_db_server(QString db_name, int cluster_id)
{
    auto servers = m_cluster_controller->servers(cluster_id, "DBS");

    ConnInfo ci;

    for (auto& server : servers)
    {
        ci.db_name = db_name.toStdString();
        ci.host = server->server_ip()->value();
        ci.port = server->port_no()->value();
        ci.username = server->db_admin()->value();
        ci.password = decrypt_str(server->db_admin_password()->value());

        std::cout << "DB Name: "<< ci.db_name << '\n';
        std::cout << "Host: "<< ci.host << '\n';
        std::cout << "Port: "<< ci.port << '\n';
        std::cout << "Username: "<< ci.username << '\n';
        std::cout << "Password: "<< ci.password << '\n';

        if (ci.host.empty())
            continue;

        if (ci.port == -1)
            continue;

        if (ci.username.empty())
            continue;

        if (ci.password.empty())
            continue;


        try{
            Authentication::test_connection(ci);
            std::cout << "DB server found." << '\n';
            break;
        }catch(PostgresException& pe) {
            ConnInfo other;
            ci = other;
        }

    }

    return ci;

}
