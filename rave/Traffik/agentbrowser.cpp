#include "agentbrowser.h"
#include "ui_agentbrowser.h"

#include "agent.h"
#include "agentform.h"

AgentBrowser::AgentBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent,
                         std::make_shared<Agent>()),
    ui(new Ui::AgentBrowser)
{
    ui->setupUi(this);
    setDialogTitle("Agents");
}

AgentBrowser::~AgentBrowser()
{
    delete ui;
}

void AgentBrowser::addRecord()
{
    std::shared_ptr<Agent> agent = std::make_shared<Agent>();
    std::unique_ptr<AgentForm> aForm = std::make_unique<AgentForm>(agent.get());

    if (aForm->exec() > 0)
        entityDataModel().createEntity(std::move(agent));
}

void AgentBrowser::updateRecord()
{
    update<Agent, AgentForm>();
}

std::string AgentBrowser::typeID()
{
    return "AgentBrowser";
}
