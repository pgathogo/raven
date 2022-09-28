#include <sstream>
#include <QTreeWidgetItem>
#include <QTableWidget>

#include "bookingorderbrowser.h"
#include "ui_bookingorderbrowser.h"
#include "../framework/ravensetup.h"
#include "client.h"
#include "order.h"
#include "spot.h"
#include "spotform.h"
#include "bookingwizard.h"
#include "spotvoiceover.h"
#include "spottypeexclusion.h"
#include "spotaudio.h"
#include "../framework/entitydatamodel.h"
#include "../utils/tools.h"

BookingOrderBrowser::BookingOrderBrowser(QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::BookingOrderBrowser)
    ,m_mdi_area{nullptr}
    ,m_grid_tables{nullptr}
    ,m_spot_ctx_menu{nullptr}
    ,m_spot_ctx_action{nullptr}
{
    ui->setupUi(this);
    connect(ui->cbFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BookingOrderBrowser::search_field_changed);
    connect(ui->btnSearch, &QPushButton::clicked, this, &BookingOrderBrowser::search);
    connect(ui->btnSelectFilter, &QPushButton::clicked, this, &BookingOrderBrowser::select_filter);
    connect(ui->btnClearFilter, &QPushButton::clicked, this, &BookingOrderBrowser::clear_filter);
    connect(ui->btnCancel, &QPushButton::clicked, this, &BookingOrderBrowser::cancel_query);
    connect(ui->btnNew, &QPushButton::clicked, this, &BookingOrderBrowser::new_booking);

    ui->cbBookPeriod->setCurrentIndex(1);
    ui->swSearch->setCurrentIndex(0);
    setWindowTitle("Order Booking");

    //ui->twOrders->setAlternatingRowColors(true);
    m_item_delegate.setHeight(30);
    ui->twOrders->setItemDelegate(&m_item_delegate);

    PrintBookingMenu* menu = new PrintBookingMenu(ui->btnPrint, this);
    QAction* actReady = new QAction(tr("&Ready"), this);
    QAction* actPlayed = new QAction(tr("&Played"), this);
    QAction* actCancelled = new QAction(tr("&Cancelled"), this);
    menu->addAction(actReady);
    menu->addAction(actPlayed);
    menu->addAction(actCancelled);

    set_autocompleter();

    ui->btnPrint->setMenu(menu);

}

BookingOrderBrowser::~BookingOrderBrowser()
{
    delete ui;
}

void BookingOrderBrowser::setMdiArea(QMdiArea *mdi)
{
    m_mdi_area = mdi;
}

void BookingOrderBrowser::search_by_client(Client* client)
{
    if (client != nullptr)
        search(client->id());
}

void BookingOrderBrowser::search_field_changed(int i)
{
    if (i == FilterField::Client_Name_Field || i == FilterField::Order_Title_Field)
        ui->swSearch->setCurrentIndex(0);
    if (i == FilterField::Booking_Period_Field)
        ui->swSearch->setCurrentIndex(1);
}

void BookingOrderBrowser::search(int id)
{
    ui->twOrders->clear();

    std::stringstream sql;

    sql << " select a.name AS client_name, f.title AS order_title, f.id AS order_id, f.order_number, "
        << " f.order_date, f.start_date, f.end_date,  f.spots_ordered, f.spots_booked, "
        << " b.id AS spot_id, b.name AS spot_name, b.spot_duration,  c.id AS booking_id, c.booking_status, "
        << " c.play_date, c.play_time,  e.id AS schedule_id, "
        << " e.schedule_date, e.schedule_time "
        << " From rave_order f "
        << " left outer join rave_bookingsegment d on f.id = d.order_id "
        << " left outer join rave_orderbooking c on c.bookingsegment_id = d.id "
        << " left outer join rave_spot b on b.id = c.spot_id "
        << " left outer join rave_client a on a.id = f.client_id "
        << " left outer join rave_schedule e on e.id = c.schedule_id "
        << " where f.spots_booked < f.spots_ordered "
        << " and f.spots_booked < f.spots_ordered ";

        sql << make_filter(id);

        sql << order_by(id);

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
                        booking.order_number = str_to_int(value);
                    if (field == "order_date")
                        booking.order_date = value;
                    if (field == "start_date")
                        booking.start_date = value;
                    if (field == "end_date")
                        booking.end_date = value;
                    if (field == "spots_ordered")
                        booking.spots_ordered = str_to_int(value);
                    if (field == "spots_booked")
                        booking.spots_booked = str_to_int(value);
                    if (field == "spot_id")
                        booking.spot_id = str_to_int(value);
                    if (field == "spot_name")
                        booking.spot_name = value;
                    if (field == "spot_duration")
                        booking.spot_duration = str_to_double(value);
                    if (field == "booking_id")
                        booking.booking_id = str_to_int(value);
                    if (field == "booking_status")
                        booking.booking_status = value;
                    if (field == "play_date")
                        booking.play_date = value;
                    if (field == "play_time")
                        booking.play_time = value;
                    if (field == "schedule_id")
                        booking.schedule_id = str_to_int(value);
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
        sort_bookings(bookings);
        set_treewidget(bookings);
    }
}

void BookingOrderBrowser::select_filter()
{
    std::tuple<int, std::string> results;

    if (ui->cbFilter->currentIndex() == FilterField::Client_Name_Field)
        results = filter_query<Client>();

    if (ui->cbFilter->currentIndex() == FilterField::Order_Title_Field)
        results = filter_query<Order>();

    auto [_id, _label] = results;

    m_id = std::get<0>(results);
    m_label = std::get<1>(results);

    ui->edtFilter->setText(stoq(m_label));

    if (!m_label.empty())
        search(_id);
}

void BookingOrderBrowser::clear_filter()
{
    ui->edtFilter->clear();
    m_id = -1;
    m_label = "";
}

void BookingOrderBrowser::cancel_query()
{
    if (m_grid_tables.size() > 0){

        int ret = QMessageBox::warning(this, tr("Booking Order"),
                                       tr("Cancel selected bookings?"),
                                       QMessageBox::Yes | QMessageBox::No) ;
        switch(ret){
        case QMessageBox::Yes:
            cancel_booking();
            break;
        case QMessageBox::No:
            break;
        }

    }
}

void BookingOrderBrowser::new_booking()
{
    if (ui->twOrders->selectedItems().size() > 0){

        int order_id = ui->twOrders->currentItem()->data(0, Qt::UserRole).toInt();
        auto order_edm = std::make_unique<EntityDataModel>(std::make_unique<Order>());

        order_edm->getById({"id", "=", order_id});

        Order& order_ref = dynamic_cast<Order&>(order_edm->getEntity());
        Order* order = &order_ref;

        if (order != nullptr){
            auto bw = std::make_unique<BookingWizard>(order);
            bw->exec();
        }
    }
}

void BookingOrderBrowser::show_spot_details(const QPoint& pos)
{
    if (m_spot_ctx_action == nullptr)
        m_spot_ctx_action = new QAction("Spot details ...", this);

    if (m_spot_ctx_menu == nullptr)
        m_spot_ctx_menu = new QMenu(this);

    m_spot_ctx_menu->addAction(m_spot_ctx_action);

    QTableWidget* selected_table{nullptr};

    bool item_found{false};
    int spot_id {-1};

    for (auto table: m_grid_tables){

        if (table != nullptr) {

            for (auto& item : table->selectedItems()){
                selected_table = table;
                if (item->column() == 3){
                    spot_id = item->data(Qt::UserRole).toInt();
                    item_found = true;
                    break;
                }
            }

            if (item_found)
                break;
        }

    }

    connect(m_spot_ctx_action, &QAction::triggered, this, [&](){
     spot_details(spot_id);
    });

    auto act2 = m_spot_ctx_menu->exec(selected_table->viewport()->mapToGlobal(pos));
}

void BookingOrderBrowser::spot_details(int spot_id)
{
    auto spot_edm = std::make_unique<EntityDataModel>(std::make_unique<TRAFFIK::Spot>());
    spot_edm->getById({"id", "=", spot_id});
    TRAFFIK::Spot& spot_ref = dynamic_cast<TRAFFIK::Spot&>(spot_edm->getEntity());
    TRAFFIK::Spot* spot = &spot_ref;

    auto client_edm = std::make_unique<EntityDataModel>(std::make_unique<Client>());
    client_edm->getById({"id", "=", spot->client()->value()});
    Client& client_ref = dynamic_cast<Client&>(client_edm->getEntity());
    Client* client = &client_ref;

    spot->voice_over().setParentId(spot_id);
    spot->type_exclusion().setParentId(spot_id);
    spot->spot_audio().setParentId(spot_id);

    std::unique_ptr<SpotForm> spot_form =
            std::make_unique<SpotForm>(client, spot, this);

    if (spot_form->exec() == 0){}

}

void BookingOrderBrowser::find_orders(QString text)
{
    auto entity = m_client_edm->findEntityByName(text.toStdString());
    if (entity == nullptr)
        return;

    search(entity->id());
}

QTableWidget* BookingOrderBrowser::get_selected_grid()
{
    QTableWidget* tbl{nullptr};
    bool table_found{false};

    for (auto table: m_grid_tables){
        for (auto& item : table->selectedItems()){
            return table;
        }
    }

    return tbl;
}

void BookingOrderBrowser::set_autocompleter()
{
    m_client_edm = std::make_unique<EntityDataModel>(
                std::make_unique<Client>());

    m_client_edm->all();

    QStringList auto_list;
    for (const auto& [name, entity] : m_client_edm->modelEntities()){
        auto_list << QString::fromStdString(name);
    }

    m_completer = new QCompleter(auto_list);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);

    ui->edtFilter->setCompleter(m_completer);
    connect(ui->edtFilter, &QLineEdit::textChanged, this, &BookingOrderBrowser::find_orders);

}

void BookingOrderBrowser::cancel_booking()
{
    QStringList ids;

    std::map<int, int> revert_counter;

    for (auto table : m_grid_tables){
        int order_id = -1;

          for (auto& item : table->selectedItems()){

            if (item->column() == 0){
                if (order_id == -1)
                    order_id = item->data(Qt::UserRole).toInt();
            }

            if (item->column() == 1){
                int booking_id = item->data(Qt::UserRole).toInt();
                ids << QString::number(booking_id);
                ++revert_counter[order_id];
            }

        }
    }

    QString selected_ids = ids.join(",");

    std::stringstream sql;


    sql << "Update rave_orderbooking set booking_status = 'CANCELLED'"
        << " Where id in ( "+ selected_ids.toStdString()+ ")";

    EntityDataModel edm;

    try{
       edm.executeRawSQL(sql.str());
    }catch(DatabaseException de){
        showMessage(de.errorMessage());
    }

    std::stringstream revert_spots_booked;

    for (auto [order_id, count] : revert_counter){

        revert_spots_booked << "Update rave_order set "
                            << " spots_booked = spots_booked - "+ std::to_string(count)
                            << " where id = "+ std::to_string(order_id);
        try{
           edm.executeRawSQL(revert_spots_booked.str());
        }catch(DatabaseException de){
            showMessage(de.errorMessage());
            break;
        }

    }

}

void BookingOrderBrowser::make_spot_menu()
{
    m_spot_ctx_menu = new QMenu(this);
    m_spot_ctx_menu->addAction("Properties...");

}

std::string BookingOrderBrowser::make_filter(int id)
{

    std::unique_ptr<EntityDataModel> setupEDM;
    setupEDM = std::make_unique<EntityDataModel>(std::make_unique<RavenSetup>());
    setupEDM->all();
    auto setup = dynamic_cast<RavenSetup*>(setupEDM->firstEntity());

    std::string order_approval_filter{};
    if (setup->orderAprvdBB()->value()){
        order_approval_filter = " and f.approval_count = "+ std::to_string(setup->orderApprovalLevels()->value());
    }

    std::string client_id_filter;
    std::string order_id_filter{};

    if (id > -1 ){
        if (ui->cbFilter->currentIndex() ==  FilterField::Client_Name_Field)
                client_id_filter = " and a.id = "+std::to_string(id);

        if (ui->cbFilter->currentIndex() == FilterField::Order_Title_Field)
                order_id_filter = " and f.id = "+std::to_string(id);
    }

    return  order_approval_filter+client_id_filter+order_id_filter;

}

std::string BookingOrderBrowser::order_by(int id)
{
    std::string order_by_str{};

    if (id > -1){
        order_by_str = " Order By f.order_date DESC ";
    }

    return order_by_str;
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

    ui->twOrders->setColumnWidth(0, 300);
    ui->twOrders->setColumnWidth(0, 200);
    ui->twOrders->setColumnWidth(0, 200);
    ui->twOrders->setColumnWidth(0, 200);
    ui->twOrders->setColumnWidth(0, 200);
    ui->twOrders->setColumnWidth(0, 150);
    ui->twOrders->setColumnWidth(0, 150);

    for (auto& [key, bookings] : records){
        int child_id = 0;
        parent_is_created = false;
        QTreeWidgetItem* order_node;
        QTreeWidgetItem* booking_node;
        QTableWidget* table{nullptr};

        int row=0;
        for (auto booking : bookings ){
            if (!parent_is_created){
                order_node = new QTreeWidgetItem(ui->twOrders);
                order_node->setText(0, stoq(booking.order_title));
                order_node->setData(0, Qt::UserRole, booking.order_id);
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
                table->resizeColumnsToContents();
//                table->horizontalHeader()->setStretchLastSection(true);
                //table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                table->horizontalHeader()->setVisible(true);
                table->setEditTriggers(QAbstractItemView::NoEditTriggers);
                table->setSelectionBehavior(QAbstractItemView::SelectRows);
                table->setSelectionMode(QAbstractItemView::MultiSelection);

                table->setColumnWidth(0, 200);
                table->setColumnWidth(1, 200);
                table->setColumnWidth(2, 300);
                table->setColumnWidth(3, 300);
                table->setColumnWidth(4, 150);
                table->setColumnWidth(5, 200);
                table->setColumnWidth(6, 200);
                table->setColumnWidth(7, 100);

                ui->twOrders->setItemWidget(booking_node, 0, table);

                booking_node->setFirstColumnSpanned(true);

                m_grid_tables.push_back(table);
            }

            if (booking.schedule_date.empty())
                continue;

            QTableWidgetItem* book_date = new QTableWidgetItem(stoq(booking.schedule_date));
            book_date->setData(Qt::UserRole, booking.order_id); // Hide order_id in here

            QTableWidgetItem* book_time = new QTableWidgetItem(stoq(booking.schedule_time));
            book_time->setData(Qt::UserRole, booking.booking_id); // Hide bookind_id in here

            QTableWidgetItem* client_name = new QTableWidgetItem(stoq(booking.client_name));

            QTableWidgetItem* spot_title = new QTableWidgetItem(stoq(booking.spot_name));
            spot_title->setData(Qt::UserRole, booking.spot_id); // Hide spot_id in here

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

        table->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(table, &QTableWidget::customContextMenuRequested, this, &BookingOrderBrowser::show_spot_details);
    }

//    for (int i=0; i<6; ++i)
//        ui->twOrders->resizeColumnToContents(i);

//    ui->twOrders->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

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
      w = qMax( w, tree_widget.topLevelItem( i )->text( col ).size()*10 + (col == 0 ? tree_widget.indentation() : 0) );
    tree_widget.header()->resizeSection( col, w );
  }
}

void BookingOrderBrowser::sort_bookings(Bookings& orders)
{
    for (auto& [key, bookings] : orders){
        std::sort(bookings.begin(), bookings.end(),
                  [](const Booking& lhs, Booking& rhs ){
            return (lhs.schedule_id < rhs.schedule_id );
        });
    }
}

PrintBookingMenu::PrintBookingMenu(QPushButton* button, QWidget* parent)
    :QMenu{parent},
     m_push_button{button}{}

void PrintBookingMenu::showEvent(QShowEvent* event)
{
    QPoint p = this->pos();
    QRect geo = m_push_button->geometry();
    this->move(p.x()+geo.width()-this->geometry().width(), p.y());
}

