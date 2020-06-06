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
    static constexpr auto TYPEID = "VoiceOverBrowser";

    explicit VoiceOverBrowser(QWidget *parent = nullptr);
    ~VoiceOverBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void deleteRecord() override;

    std::string typeID() override;
private:
    Ui::VoiceOverBrowser *ui;
    VoiceOver* mVoiceOver;
    VoiceOverForm* mVOForm;
    EntityDataModel* edm;

};

#endif // VOICEOVERBROWSER_H
