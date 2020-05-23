#include "agentform.h"
#include "ui_agentform.h"
#include "ui_baseentitydetaildlg.h"

#include "agent.h"

AgentForm::AgentForm(Agent* agent,
        QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::AgentForm),
    mAgent{agent}
{
    ui->setupUi(bui->baseContainer);
    bindWidgets();
    setTitle(windowTitle());
}

AgentForm::~AgentForm()
{
    delete ui;
}

ActionResult AgentForm::saveRecord()
{
    mAgent->populateEntity();
    ActionResult ar = mAgent->validate();
    return ar;
}

std::string AgentForm::windowTitle()
{
    return "Agent Details";
}

void AgentForm::bindWidgets()
{
    mAgent->name()->setWidget(ui->edtName);
    mAgent->address()->setWidget(ui->edtAddress);
    mAgent->telephone()->setWidget(ui->edtTelephone);
    mAgent->town()->setWidget(ui->edtTown);
    mAgent->email()->setWidget(ui->edtEmail);
    mAgent->contactName()->setWidget(ui->edtContact);
    mAgent->mobileNo()->setWidget(ui->edtContactMobile);
}
