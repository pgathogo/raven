#ifndef SPOTMEDIABROWSER_H
#define SPOTMEDIABROWSER_H

#include <functional>
#include <QWidget>

#include "../../../rave/framework/manytomanybrowser.h"

// #include "../../../rave/audio/trackbrowser.h"
// #include "../../../rave/audio/trackpickerdialog.h"

namespace Ui {
    class SpotMediaBrowser;
}

namespace TRAFFIK{
    class SpotAudio;
}

namespace PIXELPLAN {
    class SpotMedia;
    class AdvertMedia;
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

enum class MediaCreationMode {None, Import, Attach};

class SpotMediaBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:

    typedef void (SpotMediaBrowser::*Slot)();

    explicit SpotMediaBrowser(
            std::shared_ptr<PIXELPLAN::SpotMedia>,
            QVBoxLayout* layout,
            QWidget *parent = nullptr
            );

    ~SpotMediaBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void deleteRecord() override;

    bool okay_to_delete(std::shared_ptr<BaseEntity>) override;

    std::string typeID() override;

    void print(std::string);

    std::vector<std::unique_ptr<TRAFFIK::SpotAudio>> const& audios() const;
    std::vector<std::unique_ptr<PIXELPLAN::SpotMedia>> const& medias() const;

    //TRAFFIK::SpotAudio& get_spot_audio() const;
    PIXELPLAN::SpotMedia& get_spot_media() const;

    void create_button(const QString, QString, Slot, QSize size=QSize(0,0), QString icon="");

    void create_separator();

    // AUDIO::Audio* audio_from_selection();
    // std::string get_audio_file(AUDIO::Audio*);

    PIXELPLAN::AdvertMedia* media_from_selection();
    std::string get_media_file(PIXELPLAN::AdvertMedia*);

    MediaCreationMode media_creation_mode();

protected:
    void closeEvent(QCloseEvent*) override;
    void resize_window() override;

public slots:
    void stop_play();
    void selected_media(std::shared_ptr<PIXELPLAN::AdvertMedia>);
    //void selected_audio(std::shared_ptr<AUDIO::Audio>);

private slots:
    void import_audio();
    void import_media();
    // void attach_audio();
    void attach_media();
    // void audio_properties();
    void media_properties();
    void play_back();
    void cue_edit();

signals:
     void audio_duration(int);

private:
    Ui::SpotMediaBrowser* ui;

    ManyToMany* m_mtom;
    std::unique_ptr<EntityDataModel> m_setup_edm;
    std::shared_ptr<RavenSetup> m_setup;

    std::vector<std::unique_ptr<TRAFFIK::SpotAudio>> m_audios;
    std::vector<std::unique_ptr<PIXELPLAN::SpotMedia>> m_medias;

    std::unique_ptr<TRAFFIK::SpotAudio> m_spot_audio;
    std::shared_ptr<PIXELPLAN::SpotMedia> m_spot_media;

//    std::unique_ptr<CueEditor> m_cue_editor;
    std::unique_ptr<AUDIO::AudioWaveForm> m_audio_wave_form;
    std::unique_ptr<AUDIO::AudioPlayer> m_audio_player;

    std::unique_ptr<AUDIO::TrackPickerDialog> m_track_picker_dlg;

    MediaCreationMode m_media_creation_mode;

};

#endif // SPOTMEDIABROWSER_H
