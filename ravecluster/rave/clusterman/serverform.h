#ifndef SERVERFORM_H
#define SERVERFORM_H

#include <QDialog>
#include "../../../framework/baseentitydetaildlg.h"

namespace Ui {
    class ServerForm;
}

template<typename T>
class ChoiceField;

namespace ClusterManager {
  class Server;
}

class ServerForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit ServerForm(ClusterManager::Server*, QDialog* parent = nullptr);
    ~ServerForm();

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

    void populateChoiceCombo(QComboBox* cbox, const ChoiceField<std::string>* cf);
    void setChoiceFieldDefault(QComboBox *cbox, const std::string val);

    template<typename T>
    void setChoiceField(T* cf, QComboBox* cbox)
    {
        cf->setValue(cbox->itemData(cbox->currentIndex()).toString().toStdString());
    }

public slots:
    void order_type_changed(int);

private:
    Ui::ServerForm *ui;
    ClusterManager::Server* m_server;
};

#endif // SERVERFORM_H
