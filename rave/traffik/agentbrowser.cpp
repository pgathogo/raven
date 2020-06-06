#include "agentbrowser.h"
#include "ui_agentbrowser.h"

#include "agent.h"
#include "agentform.h"

AgentBrowser::AgentBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent, new Agent),
    ui(new Ui::AgentBrowser),
    mAgent{},
    mAgentForm{}
{
    ui->setupUi(this);
    setDialogTitle("Agents");
}

AgentBrowser::~AgentBrowser()
{
    delete mAgent;
    delete mAgentForm;
    delete ui;
}

void AgentBrowser::addRecord()
{
    mAgent = new Agent();
    mAgentForm = new AgentForm(mAgent);
    if (mAgentForm->exec() > 0)
        entityDataModel()->createEntity(mAgent);
}

void AgentBrowser::updateRecord()
{
    update<Agent, AgentForm>();
}

std::string AgentBrowser::typeID()
{
    return "AgentBrowser";
}
