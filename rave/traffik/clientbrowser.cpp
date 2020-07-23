#include <QPushButton>

#include "clientbrowser.h"
#include "ui_clientbrowser.h"
#include "ui_baseentitybrowserdlg.h"

#include "client.h"
#include "clientform.h"

#include "brandbrowser.h"

ClientBrowser::ClientBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent,
                         std::make_unique<Client>()),
    ui(new Ui::ClientBrowser)
{
    ui->setupUi(this);
    setDialogTitle("Clients");
    createBrandButton();
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

void ClientBrowser::openBrandBrowser()
{
    if (selectedRowId() < 0){
        showMessage("Select a Client to view Brands");
    }else{

       BaseEntity* be = findSelectedEntity();
       Client* client = dynamic_cast<Client*>(be);
       auto bb = std::make_unique<BrandBrowser>(client, this);
       bb->exec();
    }


}

