#ifndef SPOTFORM_H
#define SPOTFORM_H

#include <memory>

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

namespace Ui {
class SpotForm;
}

class Client;
class Spot;
class DayPartGrid;
class ManyToManyBrowser;
class EntityDataModel;

using EntityRecord = std::tuple<std::string, std::unique_ptr<BaseEntity>>;

class SpotForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit SpotForm(Client* client, Spot* spot,
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

private:
    Ui::SpotForm *ui;
    Client* mClient;
    Spot* mSpot;
    std::unique_ptr<DayPartGrid> mDaypart;
    std::unique_ptr<ManyToManyBrowser> mMtoMVoiceOverBrowser;
    std::unique_ptr<ManyToManyBrowser> mMtoMTypeExBrowser;
};

#endif // SPOTFORM_H
