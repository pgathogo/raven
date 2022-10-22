#include "serverform.h"
#include "ui_serverform.h"

#include "../../../rave/framework/ui_baseentitydetaildlg.h"
#include "../../../rave/framework/baseentitydetaildlg.h"
#include "../../../rave/framework/choicefield.h"

#include "server.h"

ServerForm::ServerForm(ClusterManager::Server* server, QDialog* parent)
    :BaseEntityDetailDlg(parent)
    ,ui(new Ui::ServerForm)
    ,m_server{server}
{
    ui->setupUi(bui->baseContainer);
    populateFormWidgets();

    connect(ui->cbServerType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ServerForm::order_type_changed);

    ui->edtServerName->setFocusPolicy(Qt::StrongFocus);
    ui->edtServerName->setFocus();
};

ServerForm::~ServerForm()
{
    delete ui;
}

ActionResult ServerForm::saveRecord()
{
    populateEntityFields();
    ActionResult ar = m_server->validate();
    return ar;
}

std::string ServerForm::windowTitle()
{
    return "Server";
}

void ServerForm::populateEntityFields()
{
    m_server->server_name()->setValue(ui->edtServerName->text().toStdString());
    m_server->server_ip()->setValue(ui->edtIPAddress->text().toStdString());
    setChoiceField(m_server->server_type(), ui->cbServerType);
}

void ServerForm::populateFormWidgets()
{
    ui->edtServerName->setText(stoq(m_server->server_name()->value()));
    ui->edtIPAddress->setText(stoq(m_server->server_ip()->value()));

    populateChoiceCombo(ui->cbServerType, m_server->server_type());

}

void ServerForm::populateChoiceCombo(QComboBox* cbox, const ChoiceField<std::string>* cf)
{
    for (const auto& c : cf->choices())
        cbox->addItem(stoq(std::get<1>(c)), stoq(std::get<0>(c)));

    setChoiceFieldDefault(cbox, cf->value());
}

void ServerForm::setChoiceFieldDefault(QComboBox *cbox, const std::string val)
{
    cbox->setCurrentIndex( cbox->findData(QVariant(stoq(val))) );
}

void ServerForm::order_type_changed(int i)
{
    m_server->server_type()->setValue(
                ui->cbServerType->itemData(i).toString().toStdString());

}
