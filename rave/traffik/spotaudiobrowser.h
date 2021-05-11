#ifndef SPOTAUDIOBROWSER_H
#define SPOTAUDIOBROWSER_H

#include <functional>
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
class CueEditor;

class SpotAudioBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:

    typedef void (SpotAudioBrowser::*Slot)();

    explicit SpotAudioBrowser(
            TRAFFIK::SpotAudio* mtom,
            QVBoxLayout* layout,
            QWidget *parent = nullptr
            );

    ~SpotAudioBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void deleteRecord() override;

    std::string typeID() override;

    void print(std::string);

    std::vector<std::unique_ptr<TRAFFIK::SpotAudio>> const& audios() const;

    TRAFFIK::SpotAudio& get_spot_audio() const;

    void create_button(const QString, QString, Slot);

private slots:
     void import_audio();
     void play_back();
     void stop_play();

private:
    Ui::SpotAudioBrowser* ui;
    ManyToMany* m_mtom;
    std::unique_ptr<EntityDataModel> m_setup_edm;
    TraffikSetup* m_setup;
    std::vector<std::unique_ptr<TRAFFIK::SpotAudio>> m_audios;
    std::unique_ptr<TRAFFIK::SpotAudio> m_spot_audio;
    std::unique_ptr<CueEditor> m_cue_editor;
};

#endif // SPOTAUDIOBROWSER_H
