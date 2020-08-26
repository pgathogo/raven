#ifndef ROLEFORM_H
#define ROLEFORM_H

#include <memory>

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

class Role;
class ManyToManyBrowser;


namespace Ui {
class RoleForm;
}

class RoleForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit RoleForm(Role* role, QDialog* parent = nullptr);
    ~RoleForm() override;

    std::string windowTitle() override;
    void populateEntityFields() override;
    void populateFormWidgets() override;
    ActionResult saveRecord() override;

    int parentId() const;

private slots:
    void updateValidity(int);

private:
    Ui::RoleForm *ui;
    Role* mRole;
    ManyToManyBrowser* mMtoMUserBrowser;

};

#endif // ROLEFORM_H
