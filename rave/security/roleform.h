#ifndef ROLEFORM_H
#define ROLEFORM_H

#include <memory>

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

namespace SECURITY{
    class Role;
}

class ManyToManyBrowser;

namespace Ui {
class RoleForm;
}


using EntityRecord = std::tuple<std::string, std::shared_ptr<BaseEntity>>;

class RoleForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit RoleForm(SECURITY::Role* role, QDialog* parent = nullptr);
    ~RoleForm() override;

    std::string windowTitle() override;
    void populateEntityFields() override;
    void populateFormWidgets() override;
    ActionResult saveRecord() override;

    int parentId() const;

    std::vector<EntityRecord> const& role_members() const;

private slots:
    void updateValidity(int);

private:
    Ui::RoleForm *ui;
    SECURITY::Role* mRole;
    ManyToManyBrowser* mMtoMUserBrowser;

};

#endif // ROLEFORM_H
