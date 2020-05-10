#ifndef VOICEOVERFORM_H
#define VOICEOVERFORM_H

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

class VoiceOver;
class DayPartGrid;
class VoiceExclusion;
class ManyToMany;
class ManyToManyBrowser;
class EntityDataModel;

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
    void bindWidgets() override;
    std::string title() override;
    void populateGrid();

    ManyToMany* getMtoM() const;

private slots:
    void comboChanged(int i);

private:
    Ui::VoiceOverForm *ui;
    VoiceOver* mVoiceOver;
    DayPartGrid* mDayPart;
    VoiceExclusion* mVoiceEx;
    ManyToManyBrowser* mMtoMBrowser;
    EntityDataModel* mGenderModel;

};

#endif // VOICEOVERFORM_H
