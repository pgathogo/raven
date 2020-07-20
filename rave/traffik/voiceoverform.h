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
    void populateGrid();

    //ManyToMany* getMtoM() const;
    void saveVoiceExclusions();
    void populateEntityFields() override;
    void populateFormWidgets() override;

    //VoiceOver* voiceOver();
    int parentId() const;

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
