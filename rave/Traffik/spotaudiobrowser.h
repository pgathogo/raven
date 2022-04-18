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

namespace AUDIO{
 class Audio;
 class AudioWaveForm;
 class AudioPlayer;
}

namespace FRAMEWORK{
    class ApplicationContext;
}

class ManyToMany;
class RavenSetup;
//class CueEditor;

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

    bool okay_to_delete(BaseEntity*) override;

    std::string typeID() override;

    void print(std::string);

    std::vector<std::unique_ptr<TRAFFIK::SpotAudio>> const& audios() const;

    TRAFFIK::SpotAudio& get_spot_audio() const;

    void create_button(const QString, QString, Slot);

    AUDIO::Audio* audio_from_selection();
    std::string get_audio_file(AUDIO::Audio*);

private slots:
     void import_audio();
     void play_back();
     void stop_play();
     void cue_edit();

private:
    Ui::SpotAudioBrowser* ui;

    ManyToMany* m_mtom;
    std::unique_ptr<EntityDataModel> m_setup_edm;
    RavenSetup* m_setup;
    std::vector<std::unique_ptr<TRAFFIK::SpotAudio>> m_audios;
    std::unique_ptr<TRAFFIK::SpotAudio> m_spot_audio;
//    std::unique_ptr<CueEditor> m_cue_editor;
    std::unique_ptr<AUDIO::AudioWaveForm> m_audio_wave_form;
    std::unique_ptr<AUDIO::AudioPlayer> m_audio_player;
};

#endif // SPOTAUDIOBROWSER_H
