#ifndef SPOTFORM_H
#define SPOTFORM_H

#include <memory>

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

#include "spot.h"

namespace Ui {
class SpotForm;
}

class Client;
class DayPartGrid;
class ManyToManyBrowser;
class EntityDataModel;
class TraffikSetup;
class AudioFile;
class CueEditor;
class SpotAudioBrowser;

using EntityRecord = std::tuple<std::string, std::unique_ptr<BaseEntity>>;

class SpotForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit SpotForm(Client* client, TRAFFIK::Spot* spot,
                      QDialog* parent = nullptr);
    ~SpotForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;
    void populateGrid();

    void populateEntityFields() override;
    void populateFormWidgets() override;
    std::vector<EntityRecord> const& voiceOvers() const;
    std::vector<EntityRecord> const& typeExclusions() const;

    int parentId() const;

public slots:
    void cue_edit();

private slots:
    void brandsComboChanged(int);
    void on_import_audio();

private:
    Ui::SpotForm* ui;
    Client* m_client;
    TRAFFIK::Spot* m_spot;
    std::unique_ptr<DayPartGrid> m_daypart;
    std::unique_ptr<ManyToManyBrowser> m_MtoMVoiceOverBrowser;
    std::unique_ptr<ManyToManyBrowser> m_MtoMTypeExBrowser;
    std::unique_ptr<SpotAudioBrowser> m_spot_audio_browser;

    std::unique_ptr<EntityDataModel> m_edm_setup;
    TraffikSetup* m_setup;
};

#endif // SPOTFORM_H
