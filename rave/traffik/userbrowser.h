#ifndef USERBROWSER_H
#define USERBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"


namespace Ui {
class UserBrowser;
}

class UserBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit UserBrowser(QWidget *parent = nullptr);
    ~UserBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void deleteRecord() override;

private:
    Ui::UserBrowser *ui;
};

#endif // USERBROWSER_H
