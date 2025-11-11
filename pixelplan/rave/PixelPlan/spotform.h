#ifndef SPOTFORM_H
#define SPOTFORM_H

#include <memory>

#include <QWidget>

#include "../../../rave/framework/baseentitydetaildlg.h"
#include "../../../rave/framework/dowwidget.h"

#include "spot.h"
#include "spotaudiobrowser.h"


namespace Ui {
class SpotForm;
}


class QApplication;

class Client;
class DayPartGrid;
class ManyToManyBrowser;
class EntityDataModel;
class RavenSetup;
class TimeBand;
class AudioFile;
class CueEditor;
class SpotAudioBrowser;
class SpotMediaBrowser;
class QTableWidget;

class MediaBrowser;

namespace PIXELPLAN {
class AdvertMedia;
}


using EntityRecord = std::tuple<std::string, std::shared_ptr<BaseEntity>>;

class SpotForm : public BaseEntityDetailDlg
{
    Q_OBJECT
public:
    explicit SpotForm(std::shared_ptr<Client> client,
                      std::shared_ptr<TRAFFIK::Spot> spot,
                      QDialog* parent = nullptr);
    ~SpotForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;
    void populateGrid();

    void populateEntityFields() override;
    void populateFormWidgets() override;

    std::vector<EntityRecord> const& voiceOvers() const;
    std::vector<EntityRecord> const& typeExclusions() const;
    std::vector<EntityRecord> const& spot_audios() const;

    int parentId() const;

    std::unique_ptr<SpotAudioBrowser> const& spot_browser() const;
    AudioCreationMode const get_audio_creation_mode() const;

    std::shared_ptr<PIXELPLAN::AdvertMedia> advert_media();

public slots:
    void cue_edit();
    void update_audio_duration(int);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void brandsComboChanged(int);
    void on_import_audio();
    void time_band_selected();
    void on_audio_duration(qint64);

private:
    void add_time_band_widget();
    void create_time_band_widget();
    void create_dow_widget();
    void populate_time_band_widget(const std::unique_ptr<EntityDataModel>&, QTableWidget*);

    Ui::SpotForm* ui;
    std::shared_ptr<Client> m_client;
    std::shared_ptr<TRAFFIK::Spot> m_spot;

    std::unique_ptr<DayPartGrid> m_daypart;
    std::unique_ptr<ManyToManyBrowser> m_MtoMVoiceOverBrowser;
    std::unique_ptr<ManyToManyBrowser> m_MtoMTypeExBrowser;
    std::unique_ptr<SpotAudioBrowser> m_spot_audio_browser;

    //std::unique_ptr<SpotMediaBrowser> m_spot_media_browser;
    std::unique_ptr<MediaBrowser> m_media_browser;

    std::unique_ptr<EntityDataModel> m_edm_setup;
    std::shared_ptr<RavenSetup> m_setup;

    std::unique_ptr<EntityDataModel> m_edm_time_band;
    std::unique_ptr<DowWidget> m_dow_widget;
    QTableWidget* m_time_band_widget;

};

#endif // SPOTFORM_H
