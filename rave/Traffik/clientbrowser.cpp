#include <QPushButton>

#include "clientbrowser.h"
#include "ui_clientbrowser.h"
#include "../framework/ui_baseentitybrowserdlg.h"

#include "client.h"
#include "order.h"
#include "clientform.h"

#include "brandbrowser.h"
#include "spotbrowser.h"
#include "orderbrowser.h"

ClientBrowser::ClientBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent,
                         std::make_shared<Client>()),
    ui(new Ui::ClientBrowser)
{
    ui->setupUi(this);
    setDialogTitle("Clients");
    //createBrandButton();
    //createSpotButton();
    //createOrderButton();
    show_letter_filter();
    connect(bui->tvEntity->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ClientBrowser::selection_changed);

    bui->edtFilter->setFocus();
}

ClientBrowser::~ClientBrowser()
{
    delete ui;
}

void ClientBrowser::addRecord()
{
    auto client = std::make_shared<Client>();

    std::unique_ptr<ClientForm> cForm =
            std::make_unique<ClientForm>(client.get());

    if (cForm->exec() > 0)
        entityDataModel().createEntity(std::move(client));
}

void ClientBrowser::updateRecord()
{
    update<Client, ClientForm>();
}

void ClientBrowser::deleteRecord()
{
    if (selectedRowId() > 0){
       std::shared_ptr<BaseEntity> entity = findSelectedEntity();
       Client* client = dynamic_cast<Client*>(entity.get());
       EntityDataModel edm = EntityDataModel(std::make_shared<Order>());
       edm.searchByInt({"client_id", "=", client->id()});

       if (edm.count() > 0){
            showMessage("Cannot delete client with existing orders!");
       }

    }

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
    QPushButton* btnSpot = new QPushButton("Spots");
    btnSpot->setObjectName("btnSpot");
    connect(btnSpot, &QPushButton::clicked, this, &ClientBrowser::openSpotBrowser);
    bui->hlExtra->addWidget(btnSpot);
}

void ClientBrowser::createOrderButton()
{
    QPushButton* btnOrder = new QPushButton("Orders");
    btnOrder->setObjectName("btnOrder");
    connect(btnOrder, &QPushButton::clicked, this, &ClientBrowser::openOrderBrowser);
    bui->hlExtra->addWidget(btnOrder);
}

void ClientBrowser::openBrandBrowser()
{
    openBrowserWindow<Client, BrandBrowser>();
}

void ClientBrowser::openSpotBrowser()
{
    try{
        openBrowserWindow<Client, SpotBrowser>();
    } catch (DatabaseException& de) {
        qDebug() << stoq(de.errorMessage());
    }
}

void ClientBrowser::openOrderBrowser()
{
    openBrowserWindow<Client, OrderBrowser>();
}

void ClientBrowser::selection_changed(const QItemSelection& /*selected*/, const QItemSelection& /*diselected*/)
{
        std::shared_ptr<BaseEntity> entity = findSelectedEntity();
        if (entity != nullptr)
            emit client_selection_changed(entity);

}

