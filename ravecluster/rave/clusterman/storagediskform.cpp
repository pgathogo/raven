#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>

#include "../../../rave/framework/ui_baseentitydetaildlg.h"
#include "../../../rave/framework/baseentitydetaildlg.h"

#include "../../../rave/utils/tools.h"

#include "../../../rave/Network/clientsocket.h"
#include "../../../rave/Network/serversocket.h"

#include "storagediskform.h"
#include "server.h"
#include "ui_storagediskform.h"
#include "storagedisk.h"


StorageDiskForm::StorageDiskForm(ClusterManager::Server* server,
                                 ClusterManager::StorageDisk* storage_disk,
                                 QDialog* parent)
    :BaseEntityDetailDlg(parent)
    ,ui(new Ui::StorageDiskForm)
    ,m_server{server}
    ,m_storage_disk{storage_disk}
{
    ui->setupUi(bui->baseContainer);

    populateFormWidgets();

    setWindowTitle("Disk Form");
    setTitle(storage_disk->disk_name()->value());

    connect(ui->btnRefresh, &QPushButton::clicked, this, &StorageDiskForm::get_disk_list);
}

StorageDiskForm::StorageDiskForm(ClusterManager::StorageDisk* storage_disk,
                                 QDialog* parent)
    :BaseEntityDetailDlg(parent)
    ,ui(new Ui::StorageDiskForm)
    ,m_storage_disk{storage_disk}
{
    ui->setupUi(bui->baseContainer);
    populateFormWidgets();

    // ui->edtDiskName->setFocusPolicy(Qt::StrongFocus);
    // ui->edtDiskName->setFocus();
}

StorageDiskForm::~StorageDiskForm()
{
    delete ui;
}

ActionResult StorageDiskForm::saveRecord()
{
    populateEntityFields();
    ActionResult ar = m_storage_disk->validate();
    return ar;
}

std::string StorageDiskForm::windowTitle()
{
    return "Storage Disk";
}

void StorageDiskForm::fetch_storage_disks(int server_id)
{
    std::stringstream sql;
    sql << " SELECT id, raw_disk_name, capacity, free_space, last_refresh "
        <<   " FROM rave_storagedisk ";
    std::string where = std::format(" WHERE audio_server_id = {} ", server_id);
    std::string order = " ORDER BY raw_disk_name ";

    sql << where << order;

    EntityDataModel edm;
    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();
    if (provider->cacheSize() == 0)
        return;

    provider->cache()->first();

    std::string field_name;
    std::string field_value;

    do {
        auto it_begin = provider->cache()->currentElement()->begin();
        auto it_end = provider->cache()->currentElement()->end();

        StorageDiskWrapper sd_wrapper;

        sd_wrapper.storage_disk  = std::make_shared<ClusterManager::StorageDisk>();
        sd_wrapper.m_action = DBAction::dbaNONE;

        for (; it_begin != it_end; ++it_begin) {
            field_name = (*it_begin).first;
            field_value = (*it_begin).second;

            if (field_name == "id")
                sd_wrapper.storage_disk->setId(std::stoi(field_value));

            if (field_name == "raw_disk_name")
                sd_wrapper.storage_disk->set_raw_disk_name(field_value);

            if (field_name == "capacity")
                sd_wrapper.storage_disk->set_capacity(str_to_double(field_value));

            if (field_name == "free_space")
                sd_wrapper.storage_disk->set_free_space(str_to_double(field_value));

            if (field_name == "last_refresh") {

                std:size_t i = field_value.find("+");
                    if (i != std::string::npos) {
                        field_value.erase(i, field_value.length());
                    }

                QDateTime lr = QDateTime::fromString(stoq(field_value), "yyyy-MM-dd hh:mm:ss");

                sd_wrapper.storage_disk->set_last_refresh(lr);

            }
        }

        m_storage_disks.push_back(std::move(sd_wrapper));

        provider->cache()->next();
    } while(!provider->cache()->isLast() );

}

void StorageDiskForm::show_storage_disks()
{
    for(const auto& sd_wrapper: m_storage_disks)
    {
        QString disk_name = QString::fromStdString(sd_wrapper.storage_disk->raw_disk_name()->value());
        QTableWidgetItem* twi_disk_name = new QTableWidgetItem(disk_name);

        QTableWidgetItem* twi_capacity = new QTableWidgetItem(QString::number(sd_wrapper.storage_disk->capacity()->value()));
        QTableWidgetItem* twi_free_space = new QTableWidgetItem(QString::number(sd_wrapper.storage_disk->free_space()->value()));

        QString last_refresh = sd_wrapper.storage_disk->last_refresh()->value().toString("yyyy-MM-dd hh:mm:ss");
        QTableWidgetItem* twi_last_refresh = new QTableWidgetItem(last_refresh);

        ui->twDisks->insertRow(ui->twDisks->rowCount());
        int row = ui->twDisks->rowCount()-1;
        ui->twDisks->setItem(row, 0, twi_disk_name);
        ui->twDisks->setItem(row, 1, twi_capacity);
        ui->twDisks->setItem(row, 2, twi_free_space);
        ui->twDisks->setItem(row, 3, twi_last_refresh);
    }

}

DiskContainer StorageDiskForm::storage_disks() const
{
    return m_storage_disks;
}

void StorageDiskForm::populateEntityFields()
{
    // m_storage_disk->disk_name()->setValue(ui->edtDiskName->text().toStdString());
    // m_storage_disk->capacity()->setValue(ui->edtCapacity->value());
}

void StorageDiskForm::populateFormWidgets()
{
    ui->edtAudioServer->setText(m_server->server_name()->to_qstring());
    fetch_storage_disks(m_server->id());
    show_storage_disks();
}

void StorageDiskForm::get_disk_list()
{

    m_client_socket = std::make_unique<NETWORK::ClientSocket>(this);

    socket_message("* Getting disks...");
    socket_message(m_server->server_ip()->to_qstring());

    QString sender_ip{"127.0.0.1"};
    int sender_port = 1234;

    m_server_socket = std::make_unique<NETWORK::ServerSocket>(sender_ip, sender_port, this);

    connect(m_client_socket.get(), &NETWORK::ClientSocket::log_message, this, &StorageDiskForm::socket_message);
    connect(m_server_socket.get(), &NETWORK::ServerSocket::log_server_message, this, &StorageDiskForm::socket_message);
    connect(m_server_socket.get(), &NETWORK::ServerSocket::processed_message, this, &StorageDiskForm::receive_message);

    QJsonObject msg_content;
    msg_content["type"] = "DISK-LIST";

    QString dest_ipaddress = m_server->server_ip()->to_qstring();
    int dest_port = m_server->port_no()->value();

    NETWORK::Message message(sender_ip, sender_port, msg_content);
    connect(&message, &NETWORK::Message::log_message, this, &StorageDiskForm::socket_message);

    message.test_emit("This is a test");

    //m_client_socket->send_message(message, dest_ipaddress, dest_port);

}

void StorageDiskForm::socket_message(QString msg)
{
    qDebug() << msg ;
}

void StorageDiskForm::receive_message(NETWORK::Message message)
{
    qDebug() << " * ----------------------------------- *";

    socket_message(message.message_response_tostring());

    qDebug() << " * ----------------------------------- *";

}

