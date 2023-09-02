#include "useraccessform.h"
#include "ui_useraccessform.h"

#include "../../../security/user.h"
#include "../framework/entitydatamodel.h"
#include "../framework/ravenexception.h"
#include "station.h"

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
    setWindowTitle("Station Attachment");
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

        for(; itb != ite; ++itb)
        {
            std::string field = (*itb).first;
            std::string value = (*itb).second;

            int station_id;
            if (field == "id"){
                station_id = std::stoi(value);
            }

            if(field == "station_name"){
                QListWidgetItem* lwi = new QListWidgetItem(QString::fromStdString(value));
                lwi->setData(Qt::UserRole, station_id);
                ui->lwStations->addItem(lwi);
            }

        }

        provider->cache()->next();

    } while(!provider->cache()->isLast());

}

void UserAccessForm::load_user_stations(std::string username)
{
    std::stringstream sql;

    std::string filter = std::format(" and rave_useraccess.username = '{}'", username);

    sql << "Select rave_useraccess.id, rave_useraccess.station_id, rave_station.station_name "
        << " from rave_useraccess, rave_station "
        << " Where rave_useraccess.station_id = rave_station.id "
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

        for (; itb != ite; ++itb)
        {
            std::string field = (*itb).first;
            std::string value = (*itb).second;


            if (field == "id"){
                id = std::stoi(value);
                qDebug() << "Access ID: "<< id;
            }

            if(field == "station_id"){
                station_id = std::stoi(value);
            }

            if (field == "station_name"){
                name =QString::fromStdString(value);
                QListWidgetItem* lwi = new QListWidgetItem(QString::fromStdString(value));
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
    auto selected_items = ui->lwStations->selectedItems();
    if (selected_items.size() == 0)
        return;
    auto item = selected_items[0];

    int station_id = item->data(Qt::UserRole).toInt();
    QString name = item->text();

    if (m_user_access.contains(station_id))
        return;

    QListWidgetItem* s_item = new QListWidgetItem(name);
    s_item->setData(Qt::UserRole, station_id);

    ui->lwAccess->addItem(s_item);

    add_access(station_id, name);

}

void UserAccessForm::add_access(int station_id, QString name)
{
    if (m_user_access.contains(station_id))
        return;

    StationAccess sa;
    sa.station_id = station_id;
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
