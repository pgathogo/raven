#include "clientbrowser.h"
#include "ui_clientbrowser.h"

#include "client.h"
#include "clientform.h"

ClientBrowser::ClientBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent,
                         std::make_unique<Client>()),
    ui(new Ui::ClientBrowser)
{
    ui->setupUi(this);
    setDialogTitle("Clients");
}

ClientBrowser::~ClientBrowser()
{
    delete ui;
}

void ClientBrowser::addRecord()
{
    std::unique_ptr<Client> client = std::make_unique<Client>();

    std::unique_ptr<ClientForm> cForm =
            std::make_unique<ClientForm>(client.get());

    if (cForm->exec() > 0)
        entityDataModel().createEntity(std::move(client));
}

void ClientBrowser::updateRecord()
{
   update<Client, ClientForm>();
}

std::string ClientBrowser::typeID()
{
    return "ClientBrowser";
}
