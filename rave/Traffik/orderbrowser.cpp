#include "orderbrowser.h"
#include "ui_orderbrowser.h"
#include "../framework/ui_baseentitybrowserdlg.h"

#include "order.h"
#include "orderform.h"
#include "client.h"
#include "bookingsegment.h"

OrderBrowser::OrderBrowser(std::shared_ptr<Client> client, QWidget* parent) :
    BaseEntityBrowserDlg(parent, std::make_shared<Order>(client)),
    ui(new Ui::OrderBrowser),
    mClient{client}
{
    ui->setupUi(this);

    if (client != nullptr){
        setDialogTitle("Client Orders: "+stoq(client->name()->value()));
        searchRecord();
    }

  show_letter_filter();
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
    std::string search_name = selectedRowName().toStdString();
    if (!search_name.empty()){
        std::shared_ptr<BaseEntity> be = entityDataModel().findEntityByName(search_name);

        std::shared_ptr<Order> order = std::dynamic_pointer_cast<Order>(be);

        qDebug() << *order;

        auto order_form = std::make_unique<OrderForm>(mClient, order, this);
        if (order_form->exec() > 0){
            try{
                updateTableViewRecord(order->tableViewValues());
                entityDataModel().updateEntity(*order);
                entityDataModel().all();
            } catch (DatabaseException& de){
                showMessage(de.errorMessage());
            }
        }
    }
}

void OrderBrowser::searchRecord()
{
    search_related<Order, Client>(mClient);
}


bool OrderBrowser::okay_to_delete(std::shared_ptr<BaseEntity> entity)
{
   std::shared_ptr<Order> order = std::dynamic_pointer_cast<Order>(entity);
   EntityDataModel edm = EntityDataModel(std::make_shared<BookingSegment>());
   edm.searchByInt({"order_id", "=", order->id()});

   if (edm.count() > 0){
        showMessage("Cannot delete order with existing bookings!");
        return false;
   }

   return true;

}

void OrderBrowser::search_by_client(std::shared_ptr<Client> client)
{
    qDebug() << "ORDER BROWSER: "<< client->id();
    search_related<Order, Client>(client);
}

void OrderBrowser::set_client(std::shared_ptr<Client> client)
{
    mClient = client;
}
