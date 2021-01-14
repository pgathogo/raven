#include "orderbrowser.h"
#include "ui_orderbrowser.h"
#include "ui_baseentitybrowserdlg.h"

#include "order.h"
#include "orderform.h"
#include "client.h"
#include "bookingsegment.h"

OrderBrowser::OrderBrowser(Client* client, QWidget* parent) :
    BaseEntityBrowserDlg(parent, std::make_unique<Order>(client)),
    ui(new Ui::OrderBrowser),
    mClient{client}
{
    ui->setupUi(this);
    setDialogTitle("Client Orders: "+stoq(client->name()->value()));
    if (client != nullptr)
        searchRecord();

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

bool OrderBrowser::okay_to_delete(BaseEntity* entity)
{
   Order* order = dynamic_cast<Order*>(entity);
   EntityDataModel edm = EntityDataModel(std::make_unique<BookingSegment>());
   edm.searchByInt({"order_id", "=", order->id()});

   if (edm.count() > 0){
        showMessage("Cannot delete order with existing bookings!");
        return false;
   }

   return true;

}
