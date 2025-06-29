#ifndef SPOTAUDIOBROWSER_H
#define SPOTAUDIOBROWSER_H

#include <functional>
#include <QWidget>

#include "../../../rave/framework/manytomanybrowser.h"

// #include "../../../rave/audio/trackbrowser.h"
// #include "../../../rave/audio/trackpickerdialog.h"

namespace Ui {
    class SpotAudioBrowser;
}

namespace TRAFFIK{
    class SpotAudio;
}

namespace AUDIO{
 class TrackPickerDialog;
 class Audio;
 class AudioWaveForm;
 class AudioPlayer;
}

// namespace FRAMEWORK{
//     class ApplicationContext;
// }

class ManyToMany;
class RavenSetup;

enum class AudioCreationMode {None, Import, Attach};

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

    bool okay_to_delete(std::shared_ptr<BaseEntity>) override;

    std::string typeID() override;

    void print(std::string);

    std::vector<std::unique_ptr<TRAFFIK::SpotAudio>> const& audios() const;

    TRAFFIK::SpotAudio& get_spot_audio() const;

    void create_button(const QString, QString, Slot, QSize size=QSize(0,0), QString icon="");

    void create_separator();

    AUDIO::Audio* audio_from_selection();
    std::string get_audio_file(AUDIO::Audio*);

    AudioCreationMode audio_creation_mode();

protected:
    void closeEvent(QCloseEvent*) override;
    void resize_window() override;

public slots:
    void stop_play();
    void selected_audio(std::shared_ptr<AUDIO::Audio>);

private slots:
    void import_audio();
    void attach_audio();
    void audio_properties();
    void play_back();
    void cue_edit();

signals:
     void audio_duration(int);

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

    std::unique_ptr<AUDIO::TrackPickerDialog> m_track_picker_dlg;

    AudioCreationMode m_audio_creation_mode;


};

#endif // SPOTAUDIOBROWSER_H
