#ifndef VOICEOVERFORM_H
#define VOICEOVERFORM_H

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

class VoiceOver;
class DayPartGrid;
class ManyToManyBrowser;

namespace Ui {
class VoiceOverForm;
}

class VoiceOverForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit VoiceOverForm(VoiceOver* vo, QDialog *parent = nullptr);
    ~VoiceOverForm() override;

    ActionResult saveRecord() override;
    void bindWidgets() override;
    std::string title() override;
    void populateGrid();

private:
    Ui::VoiceOverForm *ui;
    VoiceOver* mVoiceOver;
    DayPartGrid* mDayPart;
    ManyToManyBrowser* mMtoM;

};

#endif // VOICEOVERFORM_H
