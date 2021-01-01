#include <sstream>
#include <QTreeWidgetItem>
#include <QTableWidget>

#include "bookingorderbrowser.h"
#include "ui_bookingorderbrowser.h"
#include "traffiksetup.h"
#include "../framework/entitydatamodel.h"
#include "../framework/picklistbrowser.h"
#include "client.h"

BookingOrderBrowser::BookingOrderBrowser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookingOrderBrowser)
{
    ui->setupUi(this);
    connect(ui->cbFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BookingOrderBrowser::search_field_changed);
    connect(ui->btnSearch, &QPushButton::clicked, this, &BookingOrderBrowser::search);
    connect(ui->btnSelectFilter, &QPushButton::clicked, this, &BookingOrderBrowser::select_filter);
    ui->cbBookPeriod->setCurrentIndex(1);
    ui->swSearch->setCurrentIndex(0);
    setWindowTitle("Order Booking");

}

BookingOrderBrowser::~BookingOrderBrowser()
{
    delete ui;
}

void BookingOrderBrowser::setMdiArea(QMdiArea *mdi)
{
    m_mdi_area = mdi;
}

void BookingOrderBrowser::search_field_changed(int i)
{
    if (i == FilterField::Client_Name_Field || i == FilterField::Order_Title_Field)
        ui->swSearch->setCurrentIndex(0);
    if (i == FilterField::Booking_Period_Field)
        ui->swSearch->setCurrentIndex(1);
}

void BookingOrderBrowser::search()
{
    std::stringstream sql;

    sql << "select a.name AS client_name, f.title AS order_title, f.id AS order_id, f.order_number, "
        << " f.order_date, f.start_date, f.end_date, "
        << " f.spots_ordered, f.spots_booked, "
        << " b.id AS spot_id, b.name AS spot_name, b.spot_duration, "
        << " c.booking_status, c.play_date, c.play_time, "
        << " e.schedule_date, e.schedule_time"
        << " from rave_client a, rave_spot b, rave_orderbooking c, rave_bookingsegment d, rave_schedule e,"
        << " rave_order f "
        << "where c.bookingsegment_id = d.id "
        << " and c.spot_id = b.id "
        << " and c.schedule_id = e.id "
        << " and b.client_id = a.id "
        << " and f.id = d.order_id "
        << " and f.spots_booked < f.spots_ordered ";

        sql << make_filter();

        qDebug() << stoq(sql.str());

        EntityDataModel edm;
        edm.readRaw(sql.str());

        auto provider = edm.getDBManager()->provider();

        Bookings bookings;

        if (provider->cacheSize() > 0 ) {
            provider->cache()->first();
            do {
                auto itB = provider->cache()->currentElement()->begin();
                auto itE = provider->cache()->currentElement()->end();

                Booking booking;

                for (; itB != itE; ++itB){
                    std::string field =  (*itB).first;
                    std::string value = (*itB).second;

                    if (field == "client_name")
                        booking.client_name = value;
                    if (field == "order_title")
                        booking.order_title = value;
                    if (field == "order_id")
                        booking.order_id = std::stoi(value);
                    if (field == "order_number")
                        booking.order_number = std::stoi(value);
                    if (field == "order_date")
                        booking.order_date = value;
                    if (field == "start_date")
                        booking.start_date = value;
                    if (field == "end_date")
                        booking.end_date = value;
                    if (field == "spots_ordered")
                        booking.spots_ordered = std::stoi(value);
                    if (field == "spots_booked")
                        booking.spots_booked = std::stoi(value);
                    if (field == "spot_id")
                        booking.spot_id = std::stoi(value);
                    if (field == "spot_name")
                        booking.spot_name = value;
                    if (field == "spot_duration")
                        booking.spot_duration = std::stod(value);
                    if (field == "booking_status")
                        booking.booking_status = value;
                    if (field == "play_date")
                        booking.play_date = value;
                    if (field == "play_time")
                        booking.play_time = value;
                    if (field == "schedule_date")
                        booking.schedule_date = value;
                    if (field == "schedule_time")
                        booking.schedule_time = value;
                }

                bookings[booking.order_id].push_back(booking);

            provider->cache()->next();
        } while(!provider->cache()->isLast());
      }

    if (bookings.size() > 0){
        set_treewidget(bookings);
    }

}

void BookingOrderBrowser::select_filter()
{
    if (ui->cbFilter->currentIndex() == FilterField::Client_Name_Field)
        filter_by_client();
    if (ui->cbFilter->currentIndex() == FilterField::Order_Title_Field)
        filter_by_order();

}

std::string BookingOrderBrowser::make_filter()
{

    std::unique_ptr<EntityDataModel> setupEDM;
    setupEDM = std::make_unique<EntityDataModel>(std::make_unique<TraffikSetup>());
    setupEDM->all();
    auto setup = dynamic_cast<TraffikSetup*>(setupEDM->firstEntity());

    std::string order_approval_filter{};
    if (setup->orderAprvdBB()->value()){
        order_approval_filter = " and f.approval_count = "+ std::to_string(setup->orderApprovalLevels()->value());
    }

    std::string client_name_filter;
    if (ui->cbFilter->currentIndex() ==  FilterField::Client_Name_Field){
        if (!ui->edtFilter->text().isEmpty())
            client_name_filter = " and a.id = "+std::to_string(m_client_id);
    }

    std::string order_title_filter{};
    if (ui->cbFilter->currentIndex() == FilterField::Order_Title_Field){
        if (!ui->edtFilter->text().isEmpty())
            order_title_filter = " and lower(f.title) like '%"+ui->edtFilter->text().toStdString()+"%'";
    }

    return  order_approval_filter+client_name_filter+order_title_filter;

}

void BookingOrderBrowser::set_treewidget(Bookings& records)
{
    int parent_id = 0;
    bool parent_is_created;

    ui->twOrders->setColumnCount(7);
    QStringList tree_labels;
    tree_labels << "Order Title" << "Order Number" << "Order Date";
    tree_labels << "Start Date" << "End Date" << "Spots Ordered" << "Spots Booked";
    ui->twOrders->setHeaderLabels(tree_labels);

    for (auto& [key, bookings] : records){
        int child_id = 0;
        parent_is_created = false;
        QTreeWidgetItem* order_node;
        QTreeWidgetItem* booking_node;
        QTableWidget* table;
        int row=0;
        for (auto booking : bookings ){
            if (!parent_is_created){
                order_node = new QTreeWidgetItem(ui->twOrders);
                order_node->setText(0, stoq(booking.order_title));
                order_node->setText(1, QString::number(booking.order_number));
                order_node->setText(2, stoq(booking.order_date));
                order_node->setText(3, stoq(booking.start_date));
                order_node->setText(4, stoq(booking.end_date));
                order_node->setText(5, QString::number(booking.spots_ordered));
                order_node->setText(6, QString::number(booking.spots_booked));

                parent_is_created = true;
                booking_node = new QTreeWidgetItem(order_node);

                table = new QTableWidget(ui->twOrders);
                QStringList header_labels ;
                table->setRowCount(bookings.size());

                table->insertColumn(0);
                table->insertColumn(1);
                table->insertColumn(2);
                table->insertColumn(3);
                table->insertColumn(4);
                table->insertColumn(5);
                table->insertColumn(6);
                table->insertColumn(7);

                header_labels << "Schedule Date" << "Schedule Time" << "Client Name";
                header_labels << "Spot Title" << "Spot Duration" << "TX Date";
                header_labels << "TX Time" << "Status";

                table->setHorizontalHeaderLabels(header_labels);
                table->horizontalHeader()->setVisible(true);
                table->setSelectionBehavior(QAbstractItemView::SelectRows);
                table->setSelectionMode(QAbstractItemView::MultiSelection);
                ui->twOrders->setItemWidget(booking_node, 0, table);

                booking_node->setFirstColumnSpanned(true);
            }

            QTableWidgetItem* book_date = new QTableWidgetItem(stoq(booking.schedule_date));
            QTableWidgetItem* book_time = new QTableWidgetItem(stoq(booking.schedule_time));
            QTableWidgetItem* client_name = new QTableWidgetItem(stoq(booking.client_name));
            QTableWidgetItem* spot_title = new QTableWidgetItem(stoq(booking.spot_name));
            QTableWidgetItem* spot_duration = new QTableWidgetItem(QString::number(booking.spot_duration));
            QTableWidgetItem* tx_date = new QTableWidgetItem(stoq(booking.play_date));
            QTableWidgetItem* tx_time = new QTableWidgetItem(stoq(booking.play_time));
            QTableWidgetItem* book_status = new QTableWidgetItem(stoq(booking.booking_status));

            table->setItem(row, 0, book_date);
            table->setItem(row, 1, book_time);
            table->setItem(row, 2, client_name);
            table->setItem(row, 3, spot_title);
            table->setItem(row, 4, spot_duration);
            table->setItem(row, 5, tx_date);
            table->setItem(row, 6, tx_time);
            table->setItem(row, 7, book_status);

            ++row;

        }
    }

   resizeColumnsToContents(*ui->twOrders);

}

void BookingOrderBrowser::resizeColumnsToContents(QTreeWidget& tree_widget)
{
  int cCols = tree_widget.columnCount();
  int cItems = tree_widget.topLevelItemCount();
  int w;
  int col;
  int i;
  for( col = 0; col < cCols; col++ ) {
    w = tree_widget.header()->sectionSizeHint( col );
    for( i = 0; i < cItems; i++ )
      w = qMax( w, tree_widget.topLevelItem( i )->text( col ).size()*7 + (col == 0 ? tree_widget.indentation() : 0) );
    tree_widget.header()->resizeSection( col, w );
  }
}

void BookingOrderBrowser::filter_by_client()
{
    PickListSetting set(std::make_unique<Client>());
    m_picklist_browser = std::make_unique<PickListBrowser>(set);
    m_picklist_browser->exec();

    for (const auto& base_entity : set.selectedEntities){
        auto entity = dynamic_cast<Client*>(base_entity);
        m_client_id = entity->id();
        ui->edtFilter->setText(QString::fromStdString(entity->name()->displayName()));
    }

}

void BookingOrderBrowser::filter_by_order()
{

}
