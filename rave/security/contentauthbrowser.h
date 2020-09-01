#ifndef CONTENTAUTHBROWSER_H
#define CONTENTAUTHBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

namespace Ui {
class ContentAuthBrowser;
}

class ContentAuth;

class ContentAuthBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit ContentAuthBrowser(QWidget *parent = nullptr);
    ~ContentAuthBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void deleteRecord() override;
    void grant_revoke_content_access(ContentAuth* contAuth);
    int id_from_name(const std::string name);
private:
    Ui::ContentAuthBrowser *ui;
};

#endif // CONTENTAUTHBROWSER_H
