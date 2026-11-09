#include <sstream>

#include "orderbrowser.h"
#include "ui_orderbrowser.h"
#include "../../../rave/framework/ui_baseentitybrowserdlg.h"

#include "client.h"
#include "order.h"
#include "orderform.h"
#include "client.h"
#include "bookingsegment.h"
#include "orderchangelog.h"

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
    if (search_name.empty())
        return;

    std::shared_ptr<BaseEntity> be = entityDataModel().findEntityByName(search_name);
    std::shared_ptr<Order> order = std::dynamic_pointer_cast<Order>(be);

    std::shared_ptr<PIXELPLAN::OrderChangeLog> ocl = std::make_shared<PIXELPLAN::OrderChangeLog>();

    // Capture previous order details for logging
    log_prev_data(ocl, order);

    auto order_form = std::make_unique<OrderForm>(mClient, order, this);
    if (order_form->exec() == 0)
        return;

    // Check if the order has been edited in the past, pick the last seqence number
    int next_seq = find_order_prev_sequence(order) + 1;

    ocl->set_seq(next_seq);

    // Capture new order details for logging
    log_current_data(ocl, order);

    try{
        updateTableViewRecord(order->tableViewValues());
        entityDataModel().updateEntity(*order);

        auto edm = std::make_unique<EntityDataModel>();
        int id = edm->createEntityDB(*ocl);

        auto si = std::make_tuple("id", "=", mClient->id());
        entityDataModel().searchByInt(si);
    } catch (DatabaseException& de){
        showMessage(de.errorMessage());
    }

}

void OrderBrowser::log_prev_data(std::shared_ptr<PIXELPLAN::OrderChangeLog> ocl, std::shared_ptr<Order> order)
{
    ocl->set_order(order->id());
    ocl->set_client(order->client()->value());
    ocl->set_order_number(order->orderNumber()->value());
    ocl->set_change_date(QDate::currentDate());
    ocl->set_prev_order_date(order->orderDate()->value());
    ocl->set_prev_start_date(order->startDate()->value());
    ocl->set_prev_end_date(order->endDate()->value());
    ocl->set_prev_spots_ordered(order->spotsOrdered()->value());
    ocl->set_prev_spots_booked(order->spotsBooked()->value());
    ocl->set_prev_spots_played(order->spotsPlayed()->value());
    ocl->set_prev_comment(order->comment()->value());
}

void OrderBrowser::log_current_data(std::shared_ptr<PIXELPLAN::OrderChangeLog> ocl, std::shared_ptr<Order> order)
{
    ocl->set_new_start_date(order->startDate()->value());
    ocl->set_new_end_date(order->endDate()->value());
    ocl->set_new_spots_ordered(order->spotsOrdered()->value());
    ocl->set_new_comment(order->comment()->value());
    ocl->set_add_login("postgres"); //TODO:: Map the current logged in username
    ocl->set_add_dtime(QDateTime::currentDateTime());
}

int OrderBrowser::find_order_prev_sequence(std::shared_ptr<Order> order)
{
    std::stringstream sql;
    sql << "SELECT max(seq) AS seq From rave_orderchangelog "
        << " WHERE order_id = "+std::to_string(order->id());

    EntityDataModel edm;
    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();
    if (provider->cacheSize() == 0)
        return 0;

    int seq = 0;
    provider->cache()->first();
    do {
        qDebug() << "Looping...";
        auto itB = provider->cache()->currentElement()->begin();
        auto itE = provider->cache()->currentElement()->end();

        for(; itB != itE; ++itB) {

            if ((*itB).first == "seq") {

                std::string seq_str = (*itB).second;

                qDebug() << "Max Seq = " << seq_str;

                if (!seq_str.empty())
                    seq = std::stoi(seq_str);
                break;
            }
        }
        provider->cache()->next();
    } while(!provider->cache()->isLast());

    return seq;
}

void OrderBrowser::searchRecord()
{
    search_related<Order, Client>(mClient);
}


bool OrderBrowser::okay_to_delete(std::shared_ptr<BaseEntity> entity)
{
   Order* order = dynamic_cast<Order*>(entity.get());
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
    search_related<Order, Client>(client);
}

void OrderBrowser::set_client(std::shared_ptr<Client> client)
{
    mClient = client;
}
