#ifndef VOICEOVERFORM_H
#define VOICEOVERFORM_H

#include <QWidget>
#include <memory>
#include "../framework/baseentitydetaildlg.h"

class VoiceOver;
class DayPartGrid;
class VoiceExclusion;
class ManyToMany;
class ManyToManyBrowser;
class EntityDataModel;

using EntityRecord = std::tuple<std::string, std::unique_ptr<BaseEntity>>;
using TimeBandDayParts = std::map<std::string, std::map<int, std::string>>;

namespace Ui {
class VoiceOverForm;
}

class VoiceOverForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    VoiceOverForm();
    explicit VoiceOverForm(VoiceOver* vo, QDialog *parent = nullptr);
    ~VoiceOverForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

    void populateDayPart(DayPartGrid&);
    void saveVoiceExclusions();
    int parentId() const;
    void setDayPart();

    std::vector<EntityRecord> const& typeExclusions() const;

private slots:
    void comboChanged(int i);

private:
    Ui::VoiceOverForm *ui;
    VoiceOver* mVoiceOver;
    std::unique_ptr<DayPartGrid> mDayPart;
    std::unique_ptr<ManyToManyBrowser> mMtoMBrowser;
    //EntityDataModel* mGenderModel;
    ////EntityDataModel* mVoiceExModel;

};

#endif // VOICEOVERFORM_H
