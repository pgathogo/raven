#ifndef ORDERFORM_H
#define ORDERFORM_H

#include <QWidget>
#include "../../../rave/framework/baseentitydetaildlg.h"
#include "../../../rave/framework/entitydatamodel.h"

namespace Ui {
class OrderForm;
}

class Client;
class Order;
class RavenSetup;

template<typename T>
class ChoiceField;

class OrderForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit OrderForm(std::shared_ptr<Client> client, std::shared_ptr<Order> order,
                       QDialog* parent = nullptr);
    ~OrderForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;
    void populateEntityFields() override;
    void populateFormWidgets() override;
    void clear_widgets() override;

    void populateChoiceCombo(QComboBox* cbox, const ChoiceField<std::string>* cf);
    void prepare_for_edit(std::shared_ptr<Order>);
    void setDefaults();
    void setChoiceFieldDefault(QComboBox* cbox, const std::string val);
    void setForeignKeyField(QComboBox* cbox, ForeignKeyField* fkf);

    template<typename T>
    void setChoiceField(T* cf, QComboBox* cbox)
    {
        cf->setValue(cbox->itemData(cbox->currentIndex()).toString().toStdString());
    }

    template<typename T1, typename T2>
    void cbox_changed(T1* cbox, T2* field)
    {
        EntityDataModel* edm = dynamic_cast<EntityDataModel*>(cbox->model());
        field->setValue(std::get<1>(*(edm->vecBegin()+cbox->currentIndex()))->id());
    }

private slots:
    void revenueTypeCBChanged(int i);
    void billingTypeCBChanged(int i);
    void billingPeriodCBChanged(int i);
    void billingBasisCBChanged(int i);

    void brandCBChanged(int i);
    void agencyCBChanged(int i);
    void accountRepCBChanged(int i);
    void packageRepCBChanged(int i);

    void agencyCommTypeCBChanged(int i);
    void saleRepCommTypeCBChanged(int i);
    void tradeCreditTypeCBChanged(int i);

private:
    void prepare_new_order();
    void disable_controls();

    Ui::OrderForm *ui;
    std::shared_ptr<Client> mClient;
    std::shared_ptr<Order> mOrder;
    std::unique_ptr<EntityDataModel> dmSetup;
    RavenSetup* mSetup;
};
#endif // ORDERFORM_H
