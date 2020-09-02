#ifndef SETUPFORM_H
#define SETUPFORM_H

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

namespace Ui {
class SetupForm;
}

class TraffikSetup;

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

private slots:
      void agencyCommTypeChanged(int);
      void saleRepCommTypeChanged(int);
      void billCycleChanged(int);

private:
    Ui::SetupForm *ui;
    TraffikSetup* mSetup;
};

#endif // SETUPFORM_H
