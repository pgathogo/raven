#include <QDebug>
#include "agentform.h"
#include "ui_agentform.h"
#include "../framework/ui_baseentitydetaildlg.h"
#include "../utils/tools.h"

#include "agent.h"

AgentForm::AgentForm(Agent* agent,
        QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::AgentForm),
    mAgent{agent}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    populateFormWidgets();
}

AgentForm::~AgentForm()
{
    delete ui;
}

ActionResult AgentForm::saveRecord()
{
    populateEntityFields();
    ActionResult ar = mAgent->validate();
    return ar;
}

void AgentForm::populateFormWidgets()
{
    ui->edtName->setText(stoq(mAgent->name()->value()));
    ui->edtAddress->setText(stoq(mAgent->address()->value()));
    ui->edtTelephone->setText(stoq(mAgent->telephone()->value()));
    ui->edtTown->setText(stoq(mAgent->town()->value()));
    ui->edtEmail->setText(stoq(mAgent->email()->value()));
    ui->edtContact->setText(stoq(mAgent->contactName()->value()));
    ui->edtContactMobile->setText(stoq(mAgent->mobileNo()->value()));
}

void AgentForm::populateEntityFields()
{
    mAgent->name()->setValue(ui->edtName->text().toStdString());
    mAgent->address()->setValue(ui->edtAddress->toPlainText().toStdString());
    mAgent->telephone()->setValue(ui->edtTelephone->text().toStdString());
    mAgent->town()->setValue(ui->edtTown->text().toStdString());
    mAgent->email()->setValue(ui->edtEmail->text().toStdString());
    mAgent->contactName()->setValue(ui->edtContact->text().toStdString());
    mAgent->mobileNo()->setValue(ui->edtContactMobile->text().toStdString());
}

std::string AgentForm::windowTitle()
{
    return "Agent Details";
}

