#include <QPushButton>

#include "clientbrowser.h"
#include "ui_clientbrowser.h"
#include "ui_baseentitybrowserdlg.h"

#include "client.h"
#include "clientform.h"

#include "brandbrowser.h"
#include "spotbrowser.h"

ClientBrowser::ClientBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent,
                         std::make_unique<Client>()),
    ui(new Ui::ClientBrowser)
{
    ui->setupUi(this);
    setDialogTitle("Clients");
    createBrandButton();
    createSpotButton();
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

void ClientBrowser::createBrandButton()
{
    QPushButton* btnBrand = new QPushButton("Brand");
    btnBrand->setObjectName("btnBrand");
    connect(btnBrand, &QPushButton::clicked, this, &ClientBrowser::openBrandBrowser);
    bui->hlExtra->addWidget(btnBrand);
}

void ClientBrowser::createSpotButton()
{
    QPushButton* btnSpot = new QPushButton("Spot");
    btnSpot->setObjectName("btnSpot");
    connect(btnSpot, &QPushButton::clicked, this, &ClientBrowser::openSpotBrowser);
    bui->hlExtra->addWidget(btnSpot);
}

void ClientBrowser::openBrandBrowser()
{
    openBrowserWindow<Client, SpotBrowser>();
}

void ClientBrowser::openSpotBrowser()
{
    openBrowserWindow<Client, SpotBrowser>();
}

