#ifndef APPROVERFORM_H
#define APPROVERFORM_H

#include <set>
#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

namespace Ui {
class ApproverForm;
}

class OrderApprover;

class ApproverForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit ApproverForm(OrderApprover* orderAprv,
                          std::set<int>& levels,
                          const std::list<std::string> approvers,
                          QDialog* parent = nullptr);
    ~ApproverForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

    void populateUserCombo();
    void populateLevels(std::set<int>& levels);
    bool approver_exist(std::string user);

private slots:
    void userCBChanged(int);

private:
    Ui::ApproverForm *ui;
    OrderApprover* mOrderAprv;
    std::list<std::string> mApprovers;
};

#endif // APPROVERFORM_H
