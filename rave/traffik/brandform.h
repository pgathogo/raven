#ifndef BRANDFORM_H
#define BRANDFORM_H

#include <memory>

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

namespace Ui {
class BrandForm;
}

class Brand;
class PickListBrowser;
class Client;

class BrandForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit BrandForm(Brand* brand, QDialog *parent = nullptr);
    ~BrandForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;
    void populateEntityFields() override;
    void populateFormWidgets() override;

private slots:
    void selectClient();
    void brandManagerComboChanged(int i);

private:
    Ui::BrandForm *ui;
    Brand* mBrand;
    std::unique_ptr<PickListBrowser> mPlb;

    std::string getClientName();
};

#endif // BRANDFORM_H
