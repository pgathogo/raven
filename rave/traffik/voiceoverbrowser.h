#ifndef VOICEOVERBROWSER_H
#define VOICEOVERBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

class EntityDataModel;
class VoiceOver;
class VoiceOverForm;

namespace Ui {
class VoiceOverBrowser;
}

class VoiceOverBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit VoiceOverBrowser(QWidget *parent = nullptr);
    ~VoiceOverBrowser() override;

    void addRecord() override;
    void updateRecord() override;

private:
    Ui::VoiceOverBrowser *ui;
    VoiceOver* mVoiceOver;
    VoiceOverForm* mVOForm;
    EntityDataModel* edm;

};

#endif // VOICEOVERBROWSER_H
