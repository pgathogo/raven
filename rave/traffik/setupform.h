#ifndef SETUPFORM_H
#define SETUPFORM_H

#include <set>
#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

namespace Ui {
class SetupForm;
}

class TraffikSetup;
class OrderApprover;

template<typename T>
class ChoiceField;

class SetupForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit SetupForm(TraffikSetup* setup,
                       QDialog* parent = nullptr);
    ~SetupForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

    void populateChoiceCombo(QComboBox* cbox, const ChoiceField<std::string>* cf);

    void loadOrderApprovers();
    std::set<int> make_aprv_levels();

    void saveApprovers();

private slots:
      void agencyCommTypeChanged(int);
      void saleRepCommTypeChanged(int);
      void billCycleChanged(int);
      void revenueTypeChanged(int);
      void billingTypeChanged(int);
      void billingBasisChanged(int);
      void addApprover();
      void deleteApprover();
      BaseEntity* findSelectedEntity();
      QString selectedRowName();
      int selectedRowId() const;

private:
    Ui::SetupForm *ui;
    TraffikSetup* mSetup;
    std::unique_ptr<EntityDataModel> edmApprover;
};

#endif // SETUPFORM_H
