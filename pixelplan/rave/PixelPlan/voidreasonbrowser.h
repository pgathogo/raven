#ifndef VOIDREASONBROWSER_H
#define VOIDREASONBROWSER_H

#include <QDialog>

#include "../../../rave/framework/baseentitybrowserdlg.h"

namespace Ui {
class VoidReasonBrowser;
}

class VoidReasonBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    static constexpr auto TYPEID = "VoidReasonBrowser";

    explicit VoidReasonBrowser(QWidget *parent = nullptr);
    ~VoidReasonBrowser();

    void addRecord() override;
    void updateRecord() override;
    std::string typeID() override;

private:
    Ui::VoidReasonBrowser *ui;
};

#endif // VOIDREASONBROWSER_H
