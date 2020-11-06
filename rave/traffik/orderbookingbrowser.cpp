#include "orderbookingbrowser.h"
#include "ui_orderbookingbrowser.h"

#include "../framework/entitydatamodel.h"

#include "order.h"
#include "traffiksetup.h"
#include "bookingwizard.h"

OrderBookingBrowser::OrderBookingBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent,
                         std::make_unique<Order>()),
    ui(new Ui::OrderBookingBrowser)
{
    ui->setupUi(this);
    setDialogTitle("Orders Ready for Booking ");

    hideAddButton();
    hideEditButton();
    hideDeleteButton();

    addBookingButton();

    setupEDM = std::make_unique<EntityDataModel>(
                std::make_unique<TraffikSetup>());
    setupEDM->all();
}

OrderBookingBrowser::~OrderBookingBrowser()
{
    delete ui;
}

void OrderBookingBrowser::addRecord()
{

}

void OrderBookingBrowser::updateRecord()
{

}

void OrderBookingBrowser::deleteRecord()
{

}

void OrderBookingBrowser::searchRecord()
{
    auto setup = dynamic_cast<TraffikSetup*>(setupEDM->firstEntity());
    Order& order = dynamic_cast<Order&>(entityDataModel().getEntity());

    if (bui->edtFilter->text().isEmpty()){
        // check if orders need approval before booking
        if (setup->orderAprvdBB()->value()){

           auto approvedFilter = std::make_tuple(
                       order.approvalCount()->dbColumnName(),
                       " = ",
                       setup->orderApprovalLevels()->value());

           auto bookCountFilter = std::make_tuple(
                       order.spotsBooked()->dbColumnName(),
                       " < ",
                       order.spotsOrdered()->dbColumnName()
                       );


           //std::string filter = entityDataModel().prepareFilter(approvedFilter, bookCountFilter);
           entityDataModel().search(entityDataModel().prepareFilter(
                       approvedFilter, bookCountFilter));
        }

    }else{
    }

}

void OrderBookingBrowser::addBookingButton()
{
    QPushButton* btnNewBooking = new QPushButton("New Booking");
    btnNewBooking->setObjectName("btnNewBooking");
    connect(btnNewBooking, &QPushButton::clicked, this, &OrderBookingBrowser::newBooking);
    bui->hlExtra->addWidget(btnNewBooking);
}

void OrderBookingBrowser::newBooking()
{
    std::string order_name = selectedRowName().toStdString();
    if (!order_name.empty()){
        BaseEntity* be = entityDataModel().findEntityByName(order_name);
        if (be != nullptr){
            Order* order = dynamic_cast<Order*>(be);
            qDebug() << "ORDER: "<<order->startDate()->value();
            auto bw = std::make_unique<BookingWizard>(order);
            bw->exec();
        }
    }
}
