#ifndef CLIENTFORM_H
#define CLIENTFORM_H

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

class Client;
class QComboBox;

class ForeignKeyField;

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
    Client* mClient;
};

#endif // CLIENTFORM_H
