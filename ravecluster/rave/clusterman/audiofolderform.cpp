#include "audiofolderform.h"
#include "ui_audiofolderform.h"

#include "../../../rave/framework/ui_baseentitydetaildlg.h"
#include "../../../rave/framework/baseentitydetaildlg.h"

#include "audiofolder.h"

AudioFolderForm::AudioFolderForm(std::string disk_name,
                                 ClusterManager::AudioFolder* audio_folder,
                                 QDialog* parent)
    :BaseEntityDetailDlg(parent)
    ,m_disk_name{disk_name}
    ,ui(new Ui::AudioFolderForm)
    ,m_audio_folder{audio_folder}
{
    ui->setupUi(bui->baseContainer);
    populateFormWidgets();

    ui->edtFolderName->setFocusPolicy(Qt::StrongFocus);
    ui->edtFolderName->setFocus();
}

AudioFolderForm::AudioFolderForm(ClusterManager::AudioFolder* audio_folder,
                                 QDialog* parent)
    :BaseEntityDetailDlg(parent)
    ,ui(new Ui::AudioFolderForm)
    ,m_disk_name{""}
    ,m_audio_folder{audio_folder}
{
    ui->setupUi(bui->baseContainer);
    populateFormWidgets();

    ui->edtFolderName->setFocusPolicy(Qt::StrongFocus);
    ui->edtFolderName->setFocus();
}

AudioFolderForm::~AudioFolderForm()
{
    delete ui;
}

ActionResult AudioFolderForm::saveRecord()
{
    populateEntityFields();
    ActionResult ar = m_audio_folder->validate();
    return ar;
}

std::string AudioFolderForm::windowTitle()
{
    return "Disk Audio Folder";
}

void AudioFolderForm::populateEntityFields()
{
    m_audio_folder->folder_name()->setValue(ui->edtFolderName->text().toStdString());
}

void AudioFolderForm::populateFormWidgets()
{
    ui->edtDiskName->setText(stoq(m_disk_name));
    ui->edtFolderName->setText(stoq(m_audio_folder->folder_name()->value()));
}
