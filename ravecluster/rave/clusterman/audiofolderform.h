#ifndef AUDIOFOLDERFORM_H
#define AUDIOFOLDERFORM_H

#include <QDialog>
#include "../../../framework/baseentitydetaildlg.h"

namespace Ui {
    class AudioFolderForm;
}

namespace ClusterManager{
    class AudioFolder;
}

class AudioFolderForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit AudioFolderForm(std::string disk_name,
                             ClusterManager::AudioFolder* audio_folder,
                             QDialog* parent = nullptr);
    AudioFolderForm(ClusterManager::AudioFolder* audio_folder, QDialog* parent=nullptr);
    ~AudioFolderForm();

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

private:
    Ui::AudioFolderForm *ui;
    ClusterManager::AudioFolder* m_audio_folder;
    std::string m_disk_name;
};

#endif // AUDIOFOLDERFORM_H
