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
};

#endif // SPOTFORM_H
