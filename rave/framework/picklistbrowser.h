#ifndef PICKLISTBROWSER_H
#define PICKLISTBROWSER_H

#include <QDialog>

#include "baseentitybrowserdlg.h"

class SelectCloseWidget;

namespace Ui {
class PickListBrowser;
}

class PickListBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit PickListBrowser(QWidget *parent = nullptr);
    ~PickListBrowser();

    void addRecord() override;
    void updateRecord() override;

private:
    Ui::PickListBrowser *ui;
    SelectCloseWidget* scw;
};

#endif // PICKLISTBROWSER_H
