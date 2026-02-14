#include <sstream>
#include <format>

#include <QTableWidget>
#include <QTreeWidgetItem>
#include <QDateTime>
#include <QMessageBox>

#include "../../../framework/ravensetup.h"
#include "../../../framework/entitydatamodel.h"
#include "../../../utils/tools.h"

#include "bookingorderbrowser.h"
#include "ui_bookingorderbrowser.h"
#include "client.h"
#include "order.h"
#include "spot.h"
#include "spotform.h"
#include "bookingwizard.h"
#include "spotvoiceover.h"
#include "spottypeexclusion.h"
#include "spotaudio.h"
#include "voidbookingform.h"


BookingItem::BookingItem(Booking bk)
{
    m_book_date = new QTableWidgetItem(stoq(bk.schedule_date));
    m_book_date->setData(Qt::UserRole, bk.order_id);

    m_book_time = new QTableWidgetItem(stoq(bk.schedule_time));
    m_book_time->setData(Qt::UserRole, bk.booking_id);

    m_spot_title = new QTableWidgetItem(stoq(bk.spot_name));
    m_spot_title->setData(Qt::UserRole, bk.spot_id);

    m_spot_duration = new QTableWidgetItem(bk.formatted_duration);
    m_tx_date = new QTableWidgetItem(stoq(bk.play_date));
    m_tx_time = new QTableWidgetItem(stoq(bk.play_time));
    m_book_status = new QTableWidgetItem(stoq(bk.booking_status));
}

void BookingItem::set_font_color(QColor c)
{
    m_book_date->setForeground(QBrush(c));
    m_book_time->setForeground(QBrush(c));
    m_spot_title->setForeground(QBrush(c));
    m_spot_duration->setForeground(QBrush(c));
    m_tx_date->setForeground(QBrush(c));
    m_tx_time->setForeground(QBrush(c));
    m_book_status->setForeground(QBrush(c));
}

QTableWidgetItem* BookingItem::book_date() { return m_book_date; }
QTableWidgetItem* BookingItem::book_time() { return m_book_time; }
QTableWidgetItem* BookingItem::spot_title() { return m_spot_title; }
QTableWidgetItem* BookingItem::spot_duration() { return m_spot_duration; }
QTableWidgetItem* BookingItem::tx_date() { return m_tx_date; }
QTableWidgetItem* BookingItem::tx_time() { return m_tx_time; }
QTableWidgetItem* BookingItem::book_status() { return m_book_status; }


BookingOrderBrowser::BookingOrderBrowser(const std::string username, QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::BookingOrderBrowser)
    ,m_mdi_area{nullptr}
    ,m_grid_tables{nullptr}
    ,m_spot_ctx_menu{nullptr}
    ,m_spot_ctx_action{nullptr}
    ,m_client{nullptr}
    ,m_username{username}
{

    ui->setupUi(this);

    connect(ui->cbFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BookingOrderBrowser::search_field_changed);

    connect(ui->btnSearch, &QPushButton::clicked, this, &BookingOrderBrowser::search);
    connect(ui->btnSelectFilter, &QPushButton::clicked, this, &BookingOrderBrowser::select_filter);
    connect(ui->btnClearFilter, &QPushButton::clicked, this, &BookingOrderBrowser::clear_filter);
    connect(ui->btnCancel, &QPushButton::clicked, this, &BookingOrderBrowser::cancel_clicked);
    connect(ui->btnSkip, &QPushButton::clicked, this, &BookingOrderBrowser::skip_clicked);
    connect(ui->btnNew, &QPushButton::clicked, this, &BookingOrderBrowser::new_booking);

    ui->cbBookPeriod->setCurrentIndex(1);
    ui->swSearch->setCurrentIndex(0);
    setWindowTitle("Order Booking");

    //ui->twOrders->setAlternatingRowColors(true);
    m_item_delegate.setHeight(30);
    ui->twOrders->setItemDelegate(&m_item_delegate);

    PrintBookingMenu* menu = new PrintBookingMenu(ui->btnPrint, this);

    QAction* actAll = new QAction(tr("&All"));
    QAction* actReady = new QAction(tr("&Ready"), this);
    QAction* actPlayed = new QAction(tr("&Played"), this);
    QAction* actCancelled = new QAction(tr("&Cancelled"), this);
    QAction* actSkipped = new QAction(tr("&Skipped"), this);

    connect(actAll, &QAction::triggered, this, &BookingOrderBrowser::print_all_bookings);

    menu->addAction(actAll);
    menu->addAction(actReady);
    menu->addAction(actPlayed);
    menu->addAction(actCancelled);
    menu->addAction(actSkipped);

    ui->btnPrint->setMenu(menu);

    ui->btnNew->setIcon(QIcon(":/images/media/icons/booking.bmp"));
    ui->btnNew->setIconSize(QSize(32,32));
    ui->btnCancel->setIcon(QIcon(":/images/media/icons/cancelbooking.bmp"));
    ui->btnCancel->setIconSize(QSize(32, 32));

    ui->btnSkip->setIconSize(QSize(32, 32));
    ui->btnSkip->setIcon(QIcon(":/images/media/icons/skip.png"));

    ui->btnPrint->setIcon(QIcon(":/images/media/icons/printbooking.bmp"));
    ui->btnPrint->setIconSize(QSize(32, 32));

    set_autocompleter();

    fill_cbox_date_filter();


    connect(ui->cbDateFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &BookingOrderBrowser::date_filter_changed);

}

BookingOrderBrowser::~BookingOrderBrowser()
{
    delete ui;
}

void BookingOrderBrowser::setMdiArea(QMdiArea *mdi)
{
    m_mdi_area = mdi;
}

void BookingOrderBrowser::fill_cbox_date_filter()
{
    ui->cbDateFilter->addItem("All");
    ui->cbDateFilter->addItem("Current Month");
    ui->cbDateFilter->addItem("Last Three Month");
    ui->cbDateFilter->setCurrentIndex(1);
}

void BookingOrderBrowser::date_filter_changed(int index)
{
    if (index < 0) return;

    if (m_client == nullptr)
        return;

    search(m_client->id());

    // std::string filter = make_order_date_filter();
    // qDebug() << stoq(filter);
}

void BookingOrderBrowser::set_client(std::shared_ptr<Client> client)
{
    m_client = client;
}

void BookingOrderBrowser::search_by_client(std::shared_ptr<Client> client)
{
    if (client != nullptr){
        set_client(client);

        search(client->id());

    }
}

void BookingOrderBrowser::search_field_changed(int i)
{
    if (i == FilterField::Client_Name_Field || i == FilterField::Order_Title_Field)
        ui->swSearch->setCurrentIndex(0);
    if (i == FilterField::Booking_Period_Field)
        ui->swSearch->setCurrentIndex(1);
}

void BookingOrderBrowser::search(int client_id)
{
    ui->twOrders->clear();
    m_client_orders.clear();

    std::string order_date_filter = make_order_date_filter();

    build_client_orders(client_id, order_date_filter, m_client_orders);

    build_order_booking_table(m_client_orders);

    // if (order_bookings.size() > 0){
    //     sort_bookings(order_bookings);
    //     set_treewidget(bookings, id, date_filter);
    // }

}

void BookingOrderBrowser::build_order_bookings(int order_id, std::vector<Booking>& order_bookings)
{
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
        << " where f.spots_booked < f.spots_ordered ";

    std::string order_filter = std::format(" and f.id = {} ", order_id);
    sql << order_filter;

    EntityDataModel edm;


    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();

    if (provider->cacheSize() == 0)
        return;

        //Bookings bookings;

        if (provider->cacheSize() > 0 )
        {
            provider->cache()->first();

            do {
                auto itB = provider->cache()->currentElement()->begin();
                auto itE = provider->cache()->currentElement()->end();

                Booking booking;

                for (; itB != itE; ++itB) {

                    std::string field =  (*itB).first;
                    std::string value = (*itB).second;

                    if (field == "client_name")
                        booking.client_name = value;
                    if (field == "order_title")
                        booking.order_title = value;
                    if (field == "order_id")
                        booking.order_id = std::stoi(value);
                    if (field == "order_number")
                        booking.order_number = value;
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

                    if (field == "spot_duration") {
                        booking.spot_duration = str_to_double(value);
                        QString formatted_duration = duration_to_time(booking.spot_duration).toString("HH:mm:ss");
                        booking.formatted_duration = formatted_duration;
                    }

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

                order_bookings.push_back(booking);

            provider->cache()->next();
        } while(!provider->cache()->isLast());
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


void BookingOrderBrowser::cancel_clicked()
{
    void_query(VoidType::Cancel);
}

void BookingOrderBrowser::skip_clicked()
{
    void_query(VoidType::Skip);
}

std::tuple<std::string, std::string> BookingOrderBrowser::tag_n_type(VoidType vt)
{
    std::string qry_tag{""};
    std::string void_type{""};

    switch (vt)
    {
        case VoidType::Cancel:
        {
            qry_tag = "Cancel";
            void_type = "CANCELLED";
            break;
        }
        case VoidType::Skip:
        {
            qry_tag = "Skip";
            void_type = "SKIPPED";
            break;
        }
    }

    std::tuple<std::string, std::string> tag_type = std::make_tuple(qry_tag, void_type);
    return tag_type;
}


void BookingOrderBrowser::void_query(VoidType vt)
{
    auto [qry_tag, void_type] = tag_n_type(vt);

    auto [order_id, bookings] = get_selected_bookings();

    QString tag = QString::fromStdString(qry_tag);

    if (bookings.size() == 0 ) {
        auto msg = QString("Please select bookings to %1").arg(tag);
        auto title = QString("%1 Booking(s)").arg(tag);
        QMessageBox::information(this, title, msg);
        return;
    }

    std::string qry_msg = std::format("{} selected bookings?", qry_tag);

    if (m_grid_tables.size() > 0)
    {
        int ret = QMessageBox::warning(this, tr("Booking Order"),
                                       QString::fromStdString(qry_msg),
                                       QMessageBox::Yes | QMessageBox::No) ;
        switch(ret){
            case QMessageBox::Yes:
            {
                auto vbf = std::make_unique<PIXELPLAN::VoidBookingForm>(QString::fromStdString(qry_tag));

                if (vbf->exec() == 1) {
                    Reason reason = vbf->void_reason();

                    VoidReason vr;
                    vr.reason_id = reason.reason_id;
                    vr.other_reason = reason.other_reason;
                    vr.void_type = void_type;
                    vr.username = m_username;

                    void_booking(order_id, bookings, vr);
                }

                break;
            }

        case QMessageBox::No:
            break;
        }

    }
}

void BookingOrderBrowser::new_booking()
{
    if ( ui->twOrders->selectedItems().size() == 0 ||
        ui->twOrders->currentItem()->data(0, Qt::UserRole).toInt() == 0)
    {
        auto msg = QString("Please select an order to book");
        QMessageBox::information(this, "Booking Order", msg);
        return;
    }

    int order_id = ui->twOrders->currentItem()->data(0, Qt::UserRole).toInt();

    qDebug() << "Order Selected: " << order_id;


    auto order_edm = std::make_unique<EntityDataModel>(std::make_shared<Order>());

    order_edm->getById({"id", "=", order_id});

    Order* order = dynamic_cast<Order*>(order_edm->getEntity().get());

    if (order != nullptr){
        auto bw = std::make_unique<BookingWizard>(m_username, order, this);
        bw->exec();
    }

}

void BookingOrderBrowser::show_spot_details(const QPoint& pos)
{


    QTableWidget* selected_table{nullptr};

    bool item_found{false};
    int spot_id {-1};

    constexpr int COLUMN_TITLE = 2;

    for (auto table: m_grid_tables) {

        if (table != nullptr) {


            for (int r=0; r < table->rowCount(); ++r) {

                for (int c=0; c < table->columnCount(); ++c) {

                    if (table->item(r, c) == nullptr)
                        continue;

                    QTableWidgetItem* item = table->item(r, c);
                    if (!item->isSelected())
                        continue;

                    if (!item) {
                        break;
                    }

                    if (item_found) break;


                    if (item->column() == COLUMN_TITLE && item->isSelected()) {

                        selected_table = table;

                        if (t_row == r && t_col == item->column())
                            break;


                        spot_id = item->data(Qt::UserRole).toInt();

                        if (m_spot_ctx_action == nullptr)
                            m_spot_ctx_action = new QAction("Spot details ...", this);

                        if (m_spot_ctx_menu == nullptr)
                            m_spot_ctx_menu = new QMenu(this);

                        m_spot_ctx_menu->addAction(m_spot_ctx_action);

                        connect(m_spot_ctx_action, &QAction::triggered, this, [&](){
                          spot_details(spot_id);
                         });

                        m_spot_ctx_menu->exec(selected_table->viewport()->mapToGlobal(pos));

                        t_row = r;
                        t_col = item->column();

                        item_found = true;
                        break;
                   }

                } // columnCount

                if (item_found)
                    break;

            } // rowCount

            if (item_found)
                break;
        }

        if (item_found)
            break;

    } // m_grid_tables

}

void BookingOrderBrowser::spot_details(int spot_id)
{

    auto spot_edm = std::make_unique<EntityDataModel>(std::make_shared<TRAFFIK::Spot>());
    spot_edm->getById({"id", "=", spot_id});

    std::shared_ptr<TRAFFIK::Spot> spot = std::dynamic_pointer_cast<TRAFFIK::Spot>(spot_edm->getEntity());

    auto client_edm = std::make_unique<EntityDataModel>(std::make_shared<Client>());
    client_edm->getById({"id", "=", spot->client()->value()});

    std::shared_ptr<Client> client = std::dynamic_pointer_cast<Client>(client_edm->getEntity());

    spot->voice_over().setParentId(spot_id);
    spot->type_exclusion().setParentId(spot_id);
    spot->spot_audio().setParentId(spot_id);

    std::cout << "Client Name: "<< client->name()->value() << '\n';
    std::cout << "Spot Name: "<< spot->name()->value() << '\n';

    std::unique_ptr<SpotForm> spot_form =
            std::make_unique<SpotForm>(client, spot, this);

    if (spot_form->exec() == 0){}

    m_spot_ctx_action = nullptr;
    m_spot_ctx_menu = nullptr;
}

void BookingOrderBrowser::find_orders(QString text)
{
    auto entity = m_client_edm->findEntityByName(text.toStdString()).get();
    if (entity == nullptr)
        return;

    search(entity->id());
}

QTableWidget* BookingOrderBrowser::get_selected_grid()
{
    QTableWidget* tbl{nullptr};

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
                std::make_shared<Client>());

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

std::tuple<int, std::vector<int>> BookingOrderBrowser::get_selected_bookings()
{
    using ORDER_NUM = int;
    using BOOKINGS = std::vector<int>;

    std::tuple<ORDER_NUM, BOOKINGS> selected_bookings;

    int order_id{-1};

    for (auto node : m_tree_nodes)
    {
        auto w = ui->twOrders->itemWidget(node, 0);
        QTableWidget* table = dynamic_cast<QTableWidget*>(w);

        std::vector<int> booking_ids;

        for (auto& item : table->selectedItems())
        {
            if (item->column() == 0)
            {
                 order_id = item->data(Qt::UserRole).toInt();
            }

            if (item->column() == 1)
            {
                int booking_id = item->data(Qt::UserRole).toInt();
                booking_ids.push_back(booking_id);
            }
        }

        selected_bookings = std::make_tuple(order_id, booking_ids);

    }

    return selected_bookings;

}

std::string BookingOrderBrowser::vector_to_comma_sep(const std::vector<int>& vec)
{
    std::stringstream ss;
    for(size_t i=0; i < vec.size(); ++i) {
        ss << vec[i];
        if (i < vec.size() - 1) {
            ss << ",";
        }
    }

    return ss.str();
}

void BookingOrderBrowser::void_booking(int order_id, std::vector<int> bookings, VoidReason vr)
{

    std::string selected_ids = vector_to_comma_sep(bookings);

    std::stringstream sql;

    std::string bk_status = std::format("booking_status = '{}' ", vr.void_type);

    std::string bk_reason_id{""};
    if (vr.reason_id == -1) {
        bk_reason_id = std::format("void_reason_id = null");
    } else {
        bk_reason_id = std::format("void_reason_id = {}", std::to_string(vr.reason_id));
    }

    std::string bk_comments = std::format("comments = '{}'", vr.other_reason);
    std::string bk_void_dtime = std::format("void_dtime = '{}'", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm").toStdString());
    std::string bk_username = std::format("void_login = '{}'", vr.username);

    sql << "Update rave_orderbooking set "+bk_status+""
        << ","+bk_reason_id+""
        << ","+bk_comments+""
        << ","+bk_void_dtime+""
        << ","+bk_username+""
        << " Where id in ( "+ selected_ids+ "); ";

    EntityDataModel edm;

    try{
       edm.executeRawSQL(sql.str());
    }catch(DatabaseException de){
        showMessage(de.errorMessage());
    }

    std::stringstream revert_spots_booked;

    // for (auto [order_id, count] : revert_counter)
    //{
    revert_spots_booked << "Update rave_order set "
                        << " spots_booked = spots_booked - "+ std::to_string(bookings.size())
                        << " where id = "+ std::to_string(order_id)+";";

    EntityDataModel edm_update;
    try{
        edm_update.executeRawSQL(revert_spots_booked.str());
    }catch(DatabaseException de){
        showMessage(de.errorMessage());
    }

    //}

}

void BookingOrderBrowser::make_spot_menu()
{
    m_spot_ctx_menu = new QMenu(this);
    m_spot_ctx_menu->addAction("Properties...");
}

std::string BookingOrderBrowser::make_filter(int client_id)
{
    std::unique_ptr<EntityDataModel> setupEDM;
    setupEDM = std::make_unique<EntityDataModel>(std::make_shared<RavenSetup>());
    setupEDM->all();

    std::shared_ptr<RavenSetup> setup;
    if (setupEDM->count() > 0)
        setup = std::dynamic_pointer_cast<RavenSetup>(setupEDM->firstEntity());
    else
        setup = std::make_shared<RavenSetup>();


    std::string order_approval_filter{};

    if (setup->orderAprvdBB()->value()) {
        order_approval_filter = " and f.approval_count = "+ std::to_string(setup->orderApprovalLevels()->value());
    }

    std::string client_id_filter;
    std::string order_id_filter{};

    if (client_id > -1 ){
        if (ui->cbFilter->currentIndex() ==  FilterField::Client_Name_Field)
                client_id_filter = " and a.id = "+std::to_string(client_id);

        if (ui->cbFilter->currentIndex() == FilterField::Order_Title_Field)
                order_id_filter = " and f.id = "+std::to_string(client_id);
    }

    return  order_approval_filter+client_id_filter+order_id_filter;

}

std::string BookingOrderBrowser::make_order_date_filter()
{
    enum DateFilter{All=0, Current_Month, Three_Months};

    std::string filter{""};

    switch (ui->cbDateFilter->currentIndex())
    {
        case DateFilter::All:
        break;
        case DateFilter::Current_Month:
        {
            int month = QDate::currentDate().month();
            int last_day = QDate::currentDate().daysInMonth();
            int year = QDate::currentDate().year();
            QDate first_date = QDate(year, month, 1);
            QDate second_date = QDate(year, month, last_day);

            filter = std::format(" and f.order_date between '{}' and '{}' ",
                                 first_date.toString("yyyy-MM-dd").toStdString(),
                                 second_date.toString("yyyy-MM-dd").toStdString());

            break;
        }
        case DateFilter::Three_Months:
        {
            QDate prev_date = QDate::currentDate().addMonths(-3);
            int prev_month = prev_date.month();
            int prev_year = prev_date.year();
            int curr_month = QDate::currentDate().month();
            int last_day = QDate::currentDate().daysInMonth();
            int curr_year = QDate::currentDate().year();

            QDate first_date = QDate(prev_year, prev_month, 1);
            QDate second_date = QDate(curr_year, curr_month, last_day);

            filter = std::format(" and f.order_date between '{}' and '{}' ",
                                 first_date.toString("yyyy-MM-dd").toStdString(),
                                 second_date.toString("yyyy-MM-dd").toStdString());
            break;
        }
        default:
            break;
    }

        return filter;
}

std::string BookingOrderBrowser::order_by(int client_id)
{
    std::string order_by_str{};

    if (client_id > -1){
        order_by_str = " Order By f.order_date, e.schedule_time ";
    }

    return order_by_str;
}

void BookingOrderBrowser::build_client_orders(int client_id, std::string date_filter,
                         std::vector<ClientOrder>& client_orders)
{

    std::stringstream sql;

    sql << " select f.id, f.title, f.order_number, f.order_date, f.start_date,"
        << " end_date, f.spots_ordered, f.spots_booked, f.client_id, g.name "
        << " From rave_order f, rave_client g "
        << " Where f.client_id = g.id ";

    std::string filter = std::format(" and f.client_id = {}", client_id);
    sql << filter;
    sql << date_filter;
    sql << " Order by f.order_date ";

    EntityDataModel edm;
    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();
    if (provider->cacheSize() == 0)
        return;

    provider->cache()->first();

    do{
        auto itB = provider->cache()->currentElement()->begin();
        auto itE = provider->cache()->currentElement()->end();

        ClientOrder client_order;

        for (; itB != itE; ++itB)
        {
            std::string field = (*itB).first;
            std::string value = (*itB).second;

            if (field == "id"){
                client_order.order_id = std::stoi(value);
            }

            if (field == "order_number"){
                client_order.order_number = value;
            }
            if (field == "title"){
                client_order.title = value;
            }
            if (field == "order_date") {
                client_order.order_date = QDate::fromString(stoq(value), "yyyy-MM-dd");
            }
            if (field == "start_date") {
                client_order.start_date = QDate::fromString(stoq(value), "yyyy-MM-dd");
            }
            if (field == "end_date") {
                client_order.end_date = QDate::fromString(stoq(value), "yyyy-MM-dd");
            }
            if (field == "spots_ordered") {
                client_order.spots_ordered = std::stoi(value);
            }
            if (field == "spots_booked"){
                client_order.spots_booked = std::stoi(value);
            }
            if (field == "client_id") {
                client_order.client_id = std::stoi(value);
            }
            if (field == "name"){
                client_order.client_name = value;
            }

        }

        build_order_bookings(client_order.order_id, client_order.order_bookings);

        client_orders.push_back(client_order);

        provider->cache()->next();

    } while (!provider->cache()->isLast());

}

void BookingOrderBrowser::build_order_booking_table(std::vector<ClientOrder>& client_orders)
{
    ui->twOrders->setStyleSheet(
        "QHeaderView::section {;"
        "font: 75 10pt 'MS Shell Dlg 2';"
        "font-weight: bold;"
        "background-color: #87CEFA;"
        "color: rgb(255, 255, 255);"
        "border: none;"
        "height: 32px;}"
    );

    ui->twOrders->clear();
    m_grid_tables.clear();
    ui->twOrders->setColumnCount(7);

    QStringList tree_labels;
    tree_labels << "Order Title" << "Order Number" << "Order Date";
    tree_labels << "Start Date" << "End Date" << "Spots Ordered";
    tree_labels << "Spots Booked" << "Client Name";

    ui->twOrders->setHeaderLabels(tree_labels);
    ui->twOrders->setColumnWidth(0, 300);
    ui->twOrders->setColumnWidth(0, 200);
    ui->twOrders->setColumnWidth(0, 200);
    ui->twOrders->setColumnWidth(0, 200);
    ui->twOrders->setColumnWidth(0, 200);
    ui->twOrders->setColumnWidth(0, 150);
    ui->twOrders->setColumnWidth(0, 150);
    ui->twOrders->setColumnWidth(0, 300);

    bool parent_is_created;
    for (auto& co: client_orders)
    {
        parent_is_created = false;

        QTreeWidgetItem* wi_order_node  = new QTreeWidgetItem(ui->twOrders);

        wi_order_node->setText(0, stoq(co.title));
        wi_order_node->setData(0, Qt::UserRole, co.order_id);

        wi_order_node->setText(1, stoq(co.order_number));
        wi_order_node->setText(2, co.order_date.toString("dd-MM-yyyy"));
        wi_order_node->setText(3, co.start_date.toString("dd-MM-yyyy"));
        wi_order_node->setText(4, co.end_date.toString("dd-MM-yyyy"));
        wi_order_node->setText(5, QString::number(co.spots_ordered));
        wi_order_node->setText(6, QString::number(co.spots_booked));
        wi_order_node->setText(7, stoq(co.client_name));

        QTableWidget* table = new QTableWidget(ui->twOrders);
        QTreeWidgetItem* booking_node = new QTreeWidgetItem(wi_order_node);
        booking_node->setFirstColumnSpanned(true);

        make_inner_table_headers(table, co.order_bookings.size() );

        ui->twOrders->setItemWidget(booking_node, 0, table);

        m_grid_tables.push_back(table);

        m_tree_nodes.push_back(booking_node);

        // FIXME: Can this table be sorted only ones, probably outside this loop!?
        if (co.order_bookings.size() > 0)
            sort_bookings(co.order_bookings);

        int row = 0;
        for (auto& booking : co.order_bookings)
        {
            if (booking.schedule_date.empty())
                continue;

            auto bi = make_booking_item(booking);

            table->insertRow(row);

            table->setItem(row, 0, bi.book_date());
            table->setItem(row, 1, bi.book_time());
            table->setItem(row, 2, bi.spot_title());
            table->setItem(row, 3, bi.spot_duration());

            table->setItem(row, 4, bi.tx_date());
            table->setItem(row, 5, bi.tx_time());
            table->setItem(row, 6, bi.book_status());

            table->setContextMenuPolicy(Qt::CustomContextMenu);

            connect(table, &QTableWidget::customContextMenuRequested, this, &BookingOrderBrowser::show_spot_details);

            ++row;
        }

    }

}

BookingItem BookingOrderBrowser::make_booking_item(Booking bk)
{
    BookingItem bi(bk);

    if (bk.booking_status == "READY") {
        bi.set_font_color(QColor(Qt::black));
    }

    if (bk.booking_status == "CANCELLED") {
        bi.set_font_color(QColor(Qt::red));
    }

    if (bk.booking_status == "SKIPPED") {
        bi.set_font_color(QColor(Qt::blue));
    }

    return bi;
}



void BookingOrderBrowser::make_inner_table_headers(QTableWidget* table, int row_count)
{
    table->setRowCount(row_count);
    table->insertColumn(0);
    table->insertColumn(1);
    table->insertColumn(2);
    table->insertColumn(3);
    table->insertColumn(4);
    table->insertColumn(5);
    table->insertColumn(6);

    QStringList header_labels ;

    header_labels << "Schedule Date" << "Schedule Time";
    header_labels << "Spot Title" << "Spot Duration" << "TX Date";
    header_labels << "TX Time" << "Status";

    table->setHorizontalHeaderLabels(header_labels);
    table->resizeColumnsToContents();
    table->horizontalHeader()->setVisible(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    table->setColumnWidth(0, 80);
    table->setColumnWidth(1, 80);
    table->setColumnWidth(2, 300);
    table->setColumnWidth(3, 80);
    table->setColumnWidth(4, 80);
    table->setColumnWidth(5, 80);
    table->setColumnWidth(6, 100);
}

void BookingOrderBrowser::resizeColumnsToContents(QTreeWidget& tree_widget)
{
  int cCols = tree_widget.columnCount();
  int cItems = tree_widget.topLevelItemCount();
  int w;

  for( int col = 0; col < cCols; col++ )
  {
    w = tree_widget.header()->sectionSizeHint( col );
    for( int i = 0; i < cItems; i++ )
      w = qMax( w, tree_widget.topLevelItem( i )->text( col ).size()*10 + (col == 0 ? tree_widget.indentation() : 0) );
    tree_widget.header()->resizeSection( col, w );
  }
}

void BookingOrderBrowser::sort_bookings(std::vector<Booking>& bookings)
{
    std::sort(bookings.begin(), bookings.end(),
          [](const Booking& lhs, Booking& rhs ) {
        return (lhs.order_date < rhs.order_date );
    });

}

void BookingOrderBrowser::print_all_bookings()
{
    qDebug() << "printing all bookings...";
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

