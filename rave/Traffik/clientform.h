#ifndef CLIENTFORM_H
#define CLIENTFORM_H

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

class Client;
class QComboBox;

class ForeignKeyField;
class TraffikSetup;

template<typename T>
class ChoiceField;


namespace Ui {
class ClientForm;
}

class ClientForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit ClientForm(Client* client,
                        QDialog *parent = nullptr);
    ~ClientForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;
    void populateChoiceCombo(QComboBox*, const ChoiceField<std::string>*);

private slots:
    void saluteComboChanged(int i);
    void revenueTypeComboChanged(int i);
    void billCycleComboChanged(int i);
    void agentComboChanged(int i);
    void clientGroupChanged(int i);
    void accountManagerChanged(int i);

private:
    Ui::ClientForm *ui;
    Client* m_client;
    TraffikSetup* m_traffik_setup {nullptr};
    std::unique_ptr<EntityDataModel> m_edm_setup {nullptr};

    void set_defaults();
    void set_choice_field_default(QComboBox* cbox, std::string value);
};

#endif // CLIENTFORM_H
