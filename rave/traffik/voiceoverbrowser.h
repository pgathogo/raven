#ifndef VOICEOVERBROWSER_H
#define VOICEOVERBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

namespace Ui {
class VoiceOverBrowser;
}

class VoiceOverForm;

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
    void saveVoiceExclusions(const VoiceOverForm& vof);

    std::string typeID() override;
private:
    Ui::VoiceOverBrowser *ui;
};

#endif // VOICEOVERBROWSER_H
