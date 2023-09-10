#include "serverform.h"
#include "ui_serverform.h"

#include "../../../rave/framework/ui_baseentitydetaildlg.h"
#include "../../../rave/framework/baseentitydetaildlg.h"
#include "../../../rave/framework/choicefield.h"

#include "../security/authentication.h"
#include "../security/structs.h"

#include "server.h"

ServerForm::ServerForm(ClusterManager::Server* server, QDialog* parent)
    :BaseEntityDetailDlg(parent)
    ,ui(new Ui::ServerForm)
    ,m_server{server}
{
    ui->setupUi(bui->baseContainer);
    populateFormWidgets();

    connect(ui->cbServerType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ServerForm::server_type_changed);

    connect(ui->btnTestConn, &QPushButton::clicked, this, &ServerForm::test_connection);

    ui->edtServerName->setFocusPolicy(Qt::StrongFocus);
    ui->edtServerName->setFocus();

    ui->edtAdmin->setDisabled(true);
    ui->edtPassword->setDisabled(true);
    ui->btnTestConn->setDisabled(true);
    server_type_changed(ui->cbServerType->currentIndex());

}

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
    m_server->port_no()->setValue(ui->sbPort->value());

    setChoiceField(m_server->server_type(), ui->cbServerType);

    m_server->db_admin()->setValue(ui->edtAdmin->text().toStdString());
    m_server->db_admin_password()->setValue(encrypt_str(ui->edtPassword->text().toStdString()));

}

void ServerForm::populateFormWidgets()
{
    qDebug() << "** DB ADMIN: **" << m_server->db_admin()->to_qstring();

    ui->edtServerName->setText(m_server->server_name()->to_qstring());
    ui->edtIPAddress->setText(m_server->server_ip()->to_qstring());
    ui->sbPort->setValue(m_server->port_no()->value());
    ui->edtAdmin->setText(m_server->db_admin()->to_qstring());

    std::string password = decrypt_str(m_server->db_admin_password()->value());
    ui->edtPassword->setText(QString::fromStdString(password));

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

void ServerForm::server_type_changed(int i)
{
    m_server->server_type()->setValue(
                ui->cbServerType->itemData(i).toString().toStdString());

    auto srv_type = [](QString type){
        return ( type == "DBS") ? false : true;
    };

    QString type = ui->cbServerType->currentData().toString();

    ui->edtAdmin->setDisabled(srv_type(type));
    ui->edtPassword->setDisabled(srv_type(type));
    ui->btnTestConn->setDisabled(srv_type(type));
}

void ServerForm::test_connection()
{
    ConnInfo ci;

    ci.db_name = "postgres";
    ci.host = ui->edtIPAddress->text().toStdString();
    ci.port = ui->sbPort->value();
    ci.username = ui->edtAdmin->text().toStdString();
    ci.password = ui->edtPassword->text().toStdString();

    try{
        Authentication::test_connection(ci);
        qDebug() << "** Connection Successful **";
    }catch(PostgresException& pe){
        qDebug() << "** ERROR: connecting to database ...";
    }

}
