#include "orderbrowser.h"
#include "ui_orderbrowser.h"
#include "ui_baseentitybrowserdlg.h"

#include "order.h"
#include "orderform.h"
#include "client.h"

OrderBrowser::OrderBrowser(Client* client, QWidget* parent) :
    BaseEntityBrowserDlg(parent, std::make_unique<Order>(client)),
    ui(new Ui::OrderBrowser),
    mClient{client}
{
    ui->setupUi(this);
    setDialogTitle("Client Orders: "+stoq(client->name()->value()));
}

OrderBrowser::~OrderBrowser()
{
    delete ui;
}

void OrderBrowser::addRecord()
{
    add_related_record<Order, OrderForm, Client>(mClient);
}

void OrderBrowser::updateRecord()
{
}

void OrderBrowser::searchRecord()
{
    search_related<Order, Client>(mClient);
}
