#ifndef CONTENTAUTHBROWSER_H
#define CONTENTAUTHBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

namespace Ui {
class ContentAuthBrowser;
}

class ContentAuthBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit ContentAuthBrowser(QWidget *parent = nullptr);
    ~ContentAuthBrowser() override;

    void addRecord() override;
    void updateRecord() override;
private:
    Ui::ContentAuthBrowser *ui;
};

#endif // CONTENTAUTHBROWSER_H
