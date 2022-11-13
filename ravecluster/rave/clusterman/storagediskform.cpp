#include "storagediskform.h"
#include "ui_storagediskform.h"

#include "../../../rave/framework/ui_baseentitydetaildlg.h"
#include "../../../rave/framework/baseentitydetaildlg.h"

#include "storagedisk.h"

StorageDiskForm::StorageDiskForm(std::string server_name,
                                 ClusterManager::StorageDisk* storage_disk,
                                 QDialog* parent)
    :BaseEntityDetailDlg(parent)
    ,ui(new Ui::StorageDiskForm)
    ,m_storage_disk{storage_disk}
{
    ui->setupUi(bui->baseContainer);
    m_server_name = server_name;
    populateFormWidgets();

    ui->edtDiskName->setFocusPolicy(Qt::StrongFocus);
    ui->edtDiskName->setFocus();
}

StorageDiskForm::StorageDiskForm(ClusterManager::StorageDisk* storage_disk,
                                 QDialog* parent)
    :BaseEntityDetailDlg(parent)
    ,ui(new Ui::StorageDiskForm)
    ,m_storage_disk{storage_disk}
{
    ui->setupUi(bui->baseContainer);
    populateFormWidgets();

    ui->edtDiskName->setFocusPolicy(Qt::StrongFocus);
    ui->edtDiskName->setFocus();
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

void StorageDiskForm::populateEntityFields()
{
    m_storage_disk->disk_name()->setValue(ui->edtDiskName->text().toStdString());
    m_storage_disk->capacity()->setValue(ui->edtCapacity->value());
}

void StorageDiskForm::populateFormWidgets()
{
    ui->edtAudioServer->setText(stoq(m_server_name));
    ui->edtDiskName->setText(stoq(m_storage_disk->disk_name()->value()));
    ui->edtCapacity->setValue(m_storage_disk->capacity()->value());
}

