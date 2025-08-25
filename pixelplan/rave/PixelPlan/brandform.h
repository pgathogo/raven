#ifndef BRANDFORM_H
#define BRANDFORM_H

#include <memory>

#include <QWidget>
#include "../../../rave/framework/baseentitydetaildlg.h"

#include "brand.h"

namespace Ui {
class BrandForm;
}


class PickListBrowser;
class Client;

class BrandForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit BrandForm(std::shared_ptr<Client> client, std::shared_ptr<TRAFFIK::Brand> brand,
                       QDialog *parent = nullptr);
    ~BrandForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;
    void populateEntityFields() override;
    void populateFormWidgets() override;
    void clear_widgets() override;
private slots:
    void selectClient();
    void brandManagerComboChanged(int i);

private:
    Ui::BrandForm *ui;
    std::shared_ptr<Client> mClient;
    std::shared_ptr<TRAFFIK::Brand> mBrand;
    std::unique_ptr<PickListBrowser> mPlb;
};

#endif // BRANDFORM_H
