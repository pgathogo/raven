#ifndef ROLEBROWSER_H
#define ROLEBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

class RoleForm;

namespace Ui {
class RoleBrowser;
}

class RoleBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit RoleBrowser(QWidget* parent = nullptr);
    ~RoleBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void deleteRecord() override;
    void searchRecord() override;
private:
    Ui::RoleBrowser *ui;
};

#endif // ROLEBROWSER_H
