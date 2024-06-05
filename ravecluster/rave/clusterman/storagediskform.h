#ifndef STORAGEDISKFORM_H
#define STORAGEDISKFORM_H

#include <QDialog>
#include "../../../framework/baseentitydetaildlg.h"

#include "../../../rave/utils/types.h"


namespace Ui {
    class StorageDiskForm;
}

namespace ClusterManager{
   class StorageDisk;
    class Server;
}

namespace NETWORK {
class ClientSocket;
class ServerSocket;
}

struct StorageDiskWrapper {
    std::shared_ptr<ClusterManager::StorageDisk> storage_disk;
    DBAction m_action {DBAction::dbaNONE};
};

using DiskContainer = std::vector<StorageDiskWrapper>;

class StorageDiskForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit StorageDiskForm(ClusterManager::Server* server,
                             ClusterManager::StorageDisk* storage_disk,
                             QDialog* parent = nullptr);

    StorageDiskForm(ClusterManager::StorageDisk* storage_disk, QDialog* parent = nullptr);

    ~StorageDiskForm();

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

    DiskContainer storage_disks() const;
    void show_storage_disks();

private slots:
    void get_disk_list();
    void socket_message(QString);

private:
    Ui::StorageDiskForm *ui;
    ClusterManager::StorageDisk* m_storage_disk;
    ClusterManager::Server* m_server;
    int m_server_id;
    QString m_server_name;

    std::unique_ptr<NETWORK::ClientSocket> m_client_socket;
    std::unique_ptr<NETWORK::ServerSocket> m_server_socket;

    DiskContainer m_storage_disks;

    void fetch_storage_disks(int);
};

#endif // STORAGEDISKFORM_H
