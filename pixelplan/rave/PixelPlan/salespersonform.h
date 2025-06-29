#ifndef SALESPERSONFORM_H
#define SALESPERSONFORM_H

#include <QWidget>
#include "../../../rave/framework/baseentitydetaildlg.h"

class SalesPerson;

namespace Ui {
class SalesPersonForm;
}

class SalesPersonForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit SalesPersonForm(SalesPerson* sp,
                             QDialog *parent=nullptr);
    ~SalesPersonForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;
    void populateEntityFields() override;
    void populateFormWidgets() override;

private slots:
    void genderComboChanged(int i);

private:
    Ui::SalesPersonForm *ui;
    SalesPerson* mSalesPerson;
};

#endif // SALESPERSONFORM_H
