#ifndef STORAGEDISKFORM_H
#define STORAGEDISKFORM_H

#include <QDialog>
#include "../../../framework/baseentitydetaildlg.h"

namespace Ui {
    class StorageDiskForm;
}

namespace ClusterManager{
   class StorageDisk;
}

class StorageDiskForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit StorageDiskForm(std::string server_name,
                             ClusterManager::StorageDisk* storage_disk,
                             QDialog* parent = nullptr);
    ~StorageDiskForm();

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

private:
    Ui::StorageDiskForm *ui;
    ClusterManager::StorageDisk* m_storage_disk;
    std::string m_server_name;
};

#endif // STORAGEDISKFORM_H
