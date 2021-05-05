#ifndef SPOTAUDIOBROWSER_H
#define SPOTAUDIOBROWSER_H

#include <QWidget>

#include "../framework/manytomanybrowser.h"

namespace Ui {
class SpotAudioBrowser;
}

namespace TRAFFIK{
class SpotAudio;
}
class ManyToMany;
class TraffikSetup;

class SpotAudioBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    friend class SpotBrowser;

    explicit SpotAudioBrowser(
            ManyToMany* mtom,
            QVBoxLayout* layout,
            QWidget *parent = nullptr
            );

    ~SpotAudioBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void deleteRecord() override;

    void createImportButton();

    std::string typeID() override;

    void print(std::string);

    std::vector<std::unique_ptr<TRAFFIK::SpotAudio>> const& audios() const;

    TRAFFIK::SpotAudio& get_spot_audio() const;

private slots:
    void import_audio();

private:
    Ui::SpotAudioBrowser* ui;
    ManyToMany* m_mtom;
    std::unique_ptr<EntityDataModel> m_setup_edm;
    TraffikSetup* m_setup;
    std::vector<std::unique_ptr<TRAFFIK::SpotAudio>> m_audios;
    std::unique_ptr<TRAFFIK::SpotAudio> m_spot_audio;
};

#endif // SPOTAUDIOBROWSER_H
