#include <algorithm>
#include "bookingwizard.h"
#include "ui_bookingwizard.h"

#include "order.h"
#include "orderbooking.h"
#include "bookingsegment.h"
#include "spotaudio.h"
#include "client.h"
#include "spotvoiceover.h"
#include "spotform.h"
#include "spottypeexclusion.h"
#include "treeviewmodel.h"

#include "../framework/entitydatamodel.h"
#include "../framework/ravenexception.h"
#include "../framework/schedule.h"
#include "../utils/qchecklist.h"
#include "../utils/daypartgrid.h"

#define DEBUG_MODE

BookingWizard::BookingWizard(Order* order, QWidget *parent)
    :QWizard(parent)
    ,ui(new Ui::BookingWizard)
    ,m_order{order}
    ,m_spot_EDM{nullptr}
    ,m_timeband_EDM{nullptr}
    ,m_daypart_grid{nullptr}
    ,m_booking_EDM{nullptr}
    ,m_spot_ctx_menu{nullptr}
    ,m_spot_ctx_action{nullptr}
{
    ui->setupUi(this);

    populate_spots_table(m_order->client()->value());

    ui->tvSpots->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvSpots->setModel(m_spot_EDM.get());
    ui->tvSpots->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tvSpots, &QTableView::customContextMenuRequested, this, &BookingWizard::show_spot_details);

    m_timeband_EDM = new EntityDataModel(
                std::make_unique<TimeBand>());

    m_timeband_EDM->all();
    ui->cbTimeband->setModel(m_timeband_EDM);

    connect(ui->cbTimeband, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BookingWizard::timeBandChanged);

    connect(ui->rbAllBreaks, &QRadioButton::toggled, this, &BookingWizard::all_breaks);
    connect(ui->rbTimeband, &QRadioButton::toggled, this, &BookingWizard::toggleTimeBand);
    connect(ui->btnBuildBreaks, &QPushButton::clicked, this, &BookingWizard::build_breaks);

    connect(ui->twBreakSelect, &QTableWidget::itemSelectionChanged, this, [this](){
        this->ui->edtSelBreaks->setText(QString::number(this->ui->twBreakSelect->selectedRanges().size()));
        this->ui->lblTotalBookings->setText(QString::number(this->ui->twBreakSelect->selectedRanges().size()));
    });

    m_daypart_grid = std::make_unique<DayPartGrid>(ui->vlDaypart);

    timeBandChanged(0);
    ui->rbTimeband->toggle();

    ui->edtStartDate->setDate(QDate::currentDate());
    ui->edtEndDate->setDate(order->endDate()->value());

    m_rule_engine = std::make_unique<TRAFFIK::RuleEngine>(m_engine_data);

    show_order_details(order);

    ui->lwSelBreaks->setSpacing(5);

    connect(ui->btnBreakSelApply, &QPushButton::clicked, this, &BookingWizard::apply_selection);
    connect(ui->btnClearSel, &QPushButton::clicked, this, &BookingWizard::clear_selection);

    connect(ui->btnBreakSelect, &QPushButton::clicked, this, [&](){ this->toggle_selection(true);} );
    connect(ui->btnBreakUnselect, &QPushButton::clicked, this, [&](){ this->toggle_selection(false);} );

    //QPixmap* wpixmap = new QPixmap("D:/home/PMS/Raven/images/wizard_sidebanner.png");
    //setPixmap(QWizard::WatermarkPixmap, *wpixmap);
}

BookingWizard::~BookingWizard()
{
    delete ui;
}

void BookingWizard::populate_spots_table(int client_id)
{

    m_spot_EDM = std::make_unique<EntityDataModel>(
                std::make_unique<TRAFFIK::Spot>());

   auto spot = std::make_unique<TRAFFIK::Spot>();

    auto spotFilter = std::make_tuple(
                spot->client()->dbColumnName(),
                " = ",
                client_id);

    m_spot_EDM->search(m_spot_EDM->prepareFilter(spotFilter));

}

void BookingWizard::populate_grid(TimeBand* timeBand)
{
    std::map<int, std::string> dayparts;
    dayparts[1] = timeBand->daypart1()->valueToString();
    dayparts[2] = timeBand->daypart2()->valueToString();
    dayparts[3] = timeBand->daypart3()->valueToString();
    dayparts[4] = timeBand->daypart4()->valueToString();
    dayparts[5] = timeBand->daypart5()->valueToString();
    dayparts[6] = timeBand->daypart6()->valueToString();
    dayparts[7] = timeBand->daypart7()->valueToString();

    m_daypart_grid->update_grid(dayparts);

}

void BookingWizard::setup_break_select_grid()
{
#ifdef DEBUG_MODE
    qDebug() << "setup_break_select_grid" ;
#endif

    QStringList header_labels;
    header_labels << "Date" << "Time" << "Remaining Time (secs)";

    ui->twBreakSelect->setColumnCount(3);

    ui->twBreakSelect->setHorizontalHeaderLabels(header_labels);

    ui->twBreakSelect->setRowCount(m_engine_data.available_breaks);
    ui->twBreakSelect->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = 0;

    for (auto& [name, entity] : m_engine_data.m_schedule_EDM->modelEntities()){
        Schedule* comm_break = dynamic_cast<Schedule*>(entity.get());
        if (comm_break->break_availability() == Schedule::BreakAvailability::Break_Available){

            QString date_str = comm_break->schedule_date()->value().toString();
            QString time_str = comm_break->schedule_time()->value().toString("HH:mm");
            QString dur_str = QString::number(comm_break->break_duration_left()->value());

            QTableWidgetItem* date_item = new QTableWidgetItem(date_str);
            date_item->setData(Qt::UserRole, comm_break->id());

            ui->twBreakSelect->setItem(row, 0, date_item);

            QTableWidgetItem* time_item = new QTableWidgetItem(time_str);
            ui->twBreakSelect->setItem(row, 1, time_item);
            time_item->setTextAlignment(Qt::AlignCenter);

            int int_date = comm_break->schedule_date()->value().day()+
                    comm_break->schedule_date()->value().month()+
                    comm_break->schedule_date()->value().year();
            time_item->setData(Qt::UserRole, int_date);

            QTableWidgetItem* dur_item = new QTableWidgetItem(dur_str);
            ui->twBreakSelect->setItem(row, 2, dur_item);
            dur_item->setTextAlignment(Qt::AlignCenter);
            dur_item->setData(Qt::UserRole, comm_break->schedule_date()->value().dayOfWeek());

            //m_selected_breaks.insert(time_str);

            SelectedBreak sel_break;
            sel_break.break_id = comm_break->id();
            sel_break.break_date = comm_break->schedule_date()->value();
            sel_break.break_time = comm_break->schedule_time()->value();
            sel_break.break_hour = comm_break->schedule_hour()->value();
            m_selected_breaks[comm_break->id()] = sel_break;

            ++row;
        }
    }
}

bool BookingWizard::make_booking()
{
    if (QMessageBox::question(this, tr("Traffik"),
                                tr("Are you sure you want to commit the booking(s)?"),
                                QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes){
        commit_booking();
        return true;
    }else{
        return false;
    }

}

void BookingWizard::commit_booking()
{
    qDebug() << "Commit booking... ";
    try{

        qDebug() << "Book segment";

        BookingSegment book_segment;
        book_segment.set_booking_date(QDate::currentDate());
        book_segment.set_start_date(ui->edtStartDate->date());
        book_segment.set_end_date(ui->edtEndDate->date());
        book_segment.set_booking_count(ui->twBreakSelect->selectedRanges().size());
        book_segment.set_segment_value(0);
        book_segment.set_comments("Comm Booking");
        book_segment.set_order(m_order->id());
        book_segment.set_add_login("postgres");
        book_segment.set_add_date_time(QDateTime::currentDateTime());

        EntityDataModel edm;

        auto selected_breaks = ui->twBreakSelect->selectedItems();

        int book_segment_id = edm.createEntityDB(book_segment);

        for (int i=0; i<selected_breaks.count(); ++i){
            auto item = selected_breaks.at(i);

            if (item->column() == 0){
                OrderBooking order_booking;

                int break_id = item->data(Qt::UserRole).toInt();

                SelectedBreak selected_break = m_selected_breaks[break_id];

                qDebug() << selected_break.break_date;
                qDebug() << selected_break.break_time;
                qDebug() << selected_break.break_hour;

                order_booking.set_booking_status("READY");
                order_booking.set_schedule(break_id);
                order_booking.set_spot(m_engine_data.spot_to_book.spot_id);
                order_booking.set_booking_segment(book_segment_id);
                order_booking.play_date()->setReadOnly(true);
                order_booking.play_time()->setReadOnly(true);

                order_booking.set_book_date(selected_break.break_date);
                order_booking.set_book_time(selected_break.break_time);
                order_booking.set_book_hour(selected_break.break_hour);
                edm.createEntityDB(order_booking);

                // Deduct time remainining on this break
                std::stringstream  sql ;
                sql << "Update rave_schedule set break_duration_left =  break_duration_left - "
                    << std::to_string(m_engine_data.spot_to_book.spot_duration)
                    << " Where id = "+std::to_string(break_id);

                edm.executeRawSQL(sql.str());
            }
        }

        std::stringstream update_order;
        update_order << "Update rave_order set spots_booked = spots_booked + "
                     << std::to_string(book_segment.booking_count()->value())
                     << " Where order_number = "+std::to_string(m_order->orderNumber()->value());

        edm.executeRawSQL(update_order.str());

    }

    catch(DatabaseException& de){
        showMessage(de.errorMessage());
    }

}

void BookingWizard::show_order_details(Order* order)
{
    ui->lblOrderNo->setText(QString::number(order->orderNumber()->value()));
    ui->lblSpotsOrdered->setText(QString::number(order->spotsOrdered()->value()));
    ui->lblSpotsBooked->setText(QString::number(order->spotsBooked()->value()));
    ui->lblSpotsPending->setText(QString::number(
                                     order->spotsOrdered()->value() - order->spotsBooked()->value()));
}

std::size_t BookingWizard::fetch_breaks(QDate start_date, QDate end_date, std::set<int> uniq_hours)
{
//    m_schedule_EDM = std::make_unique<EntityDataModel>(
//                std::make_unique<Schedule>());

    Schedule schedule;
    QString DATE_FORMAT = "yyyy-MM-dd";
    QString date_range = "'"+start_date.toString(DATE_FORMAT)+"' and '"+end_date.toString(DATE_FORMAT)+"'";

    auto date_range_filter = std::make_tuple(
                schedule.schedule_date()->dbColumnName(),
                " between ",
                date_range.toStdString()
                );


    //FIXME: Refactor the following code
    std::size_t i = 0;
    std::string  hr_str;
    for (auto it=uniq_hours.begin(); it != uniq_hours.end(); ++it){
        hr_str += std::to_string(*it);
        if (i < uniq_hours.size()-1)
            hr_str += ",";
        ++i;
    }
    hr_str = "("+hr_str+")";

    auto hours_filter = std::make_tuple(
                schedule.schedule_hour()->dbColumnName(),
                " in ",
                hr_str);

    try{
        m_engine_data.m_schedule_EDM->search(m_engine_data.m_schedule_EDM->prepareFilter(date_range_filter, hours_filter));
    } catch(DatabaseException& de){
        showMessage(de.errorMessage());
    }

    return m_engine_data.m_schedule_EDM->count();
}

void BookingWizard::timeBandChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbTimeband->model());
    m_engine_data.current_time_band = dynamic_cast<TimeBand*>(std::get<1>(*(edm->vecBegin()+i)).get());
    populate_grid(m_engine_data.current_time_band);
    m_engine_data.target_daypart =  m_daypart_grid->daypart_to_hours(m_daypart_grid->read_grid());
}

void BookingWizard::all_breaks(bool state)
{
     ui->cbTimeband->setEnabled(!state);
}

void BookingWizard::toggleTimeBand(bool state)
{
    ui->cbTimeband->setEnabled(state);
}

void BookingWizard::build_breaks()
{
//    test_booking();
//    return;

    try{
        init_rules_state();

        TRAFFIK::Spot* spot = selected_spot();

        m_engine_data.spot_to_book.spot_id = spot->id();
        m_engine_data.spot_to_book.client_id = spot->client()->value();
        m_engine_data.spot_to_book.brand_id = spot->brand()->value();
        m_engine_data.spot_to_book.spot_name = spot->name()->value();
        m_engine_data.spot_to_book.spot_duration = spot->spot_duration()->value();
        m_engine_data.spot_to_book.real_duration = spot->real_duration()->value();
        m_engine_data.spot_to_book.spot_daypart = fetch_spot_daypart(*spot);

        fetch_type_exclusions(m_engine_data);

        fetch_voice_exclusions(m_engine_data);

        auto unique_hours = selected_unique_hours();

        m_engine_data.break_count = fetch_breaks(ui->edtStartDate->date(), ui->edtEndDate->date(), unique_hours);

        if (m_engine_data.break_count == 0){
            showMessage("No Breaks for the selected data range!");
            return;
        }

        find_existing_bookings(m_engine_data);

        m_engine_data.available_breaks = find_available_breaks();

        show_available_breaks();

        setup_break_select_grid();

    } catch(DatabaseException& de){
        showMessage(de.errorMessage());
    }

}

void BookingWizard::apply_selection()
{

    m_dow_selection.clear();

    auto sel_days = ui->twDOW->selectedItems();

    for (int j=0; j < sel_days.count(); ++j){

        auto day_of_week = sel_days.at(j);

        QComboBox* combo = (QComboBox*)ui->twDOW->cellWidget(day_of_week->row(), 1);

        auto sel_items = ui->lwSelBreaks->selectedItems();

        for (int i=0; i < sel_items.size(); ++i){
            auto time_str = sel_items[i]->text();
            combo->addItem(time_str);
            m_dow_selection[day_of_week->row()+1].push_back(time_str.toStdString());
        }
    }
}

void BookingWizard::clear_selection()
{
    auto sel_days = ui->twDOW->selectedItems();
    for (int i=0; i < sel_days.count(); ++i){
        auto item = sel_days.at(i);
        QComboBox* combo = (QComboBox*)ui->twDOW->cellWidget(item->row() , 1);
        combo->clear();
    }
}

void BookingWizard::show_spot_details(const QPoint& pos)
{
    if (m_spot_ctx_action == nullptr)
        m_spot_ctx_action = new QAction("Spot details ...", this);

    if (m_spot_ctx_menu == nullptr)
        m_spot_ctx_menu = new QMenu(this);

    m_spot_ctx_menu->addAction(m_spot_ctx_action);

    auto sel_spot = selected_spot();
    connect(m_spot_ctx_action, &QAction::triggered, this, [&](){
        spot_details(sel_spot->id());
    });

    auto action = m_spot_ctx_menu->exec(ui->tvSpots->viewport()->
                                            mapToGlobal(pos));
}

void BookingWizard::spot_details(int spot_id)
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

std::set<int> BookingWizard::selected_unique_hours()
{

    // map[1] = ("00011...", vector<5,6,9...>)
    auto daypart = m_daypart_grid->daypart_to_hours(m_daypart_grid->read_grid());

    std::set<int> unique_hours;
    for (auto& [day, dp_str_hours] : daypart){
        auto& [dp_str, hours] = dp_str_hours;
            for (auto& h : hours){
                unique_hours.insert(h);
            }
    }

    return unique_hours;
}

void BookingWizard::test_booking()
{
    init_rules_state();

    //populate_spots_table(5);
    //ui->tvSpots->selectRow(0);
    TRAFFIK::Spot* spot{nullptr};

    try{
        spot = selected_spot();
        if (spot == nullptr){
            showMessage("Select a spot to book!");
            return;
        }
    } catch(DatabaseException& de){
        showMessage(de.errorMessage());
    }

    m_engine_data.spot_to_book.spot_id = 3;
    m_engine_data.spot_to_book.client_id = spot->client()->value();
    m_engine_data.spot_to_book.brand_id = spot->brand()->value();
    m_engine_data.spot_to_book.spot_name = "Test Spot";
    m_engine_data.spot_to_book.spot_duration = 25.0;
    m_engine_data.spot_to_book.real_duration = 0.0;
    m_engine_data.spot_to_book.spot_daypart = fetch_spot_daypart(*spot);

    fetch_type_exclusions(m_engine_data);

    fetch_voice_exclusions(m_engine_data);

    //ui->cbTypeDaypart->setChecked(true);

    ui->cbTimeband->setCurrentIndex(1);

    auto uniq_hours = selected_unique_hours();

    m_engine_data.break_count = fetch_breaks(QDate(2020,9,9), QDate(2020,9,30), uniq_hours);

    if (m_engine_data.break_count == 0){
        showMessage("No Breaks for the selected data range!");
        return;
    }

    find_existing_bookings(m_engine_data);

    m_engine_data.available_breaks = find_available_breaks();

    show_available_breaks();

    setup_break_select_grid();
}

void BookingWizard::add_days_of_week()
{
    int i=1;
    int row = 0;

    ui->twDOW->clear();
    QStringList header;
    header << "DOW" <<	"Breaks";
    ui->twDOW->setHorizontalHeaderLabels(header);

    ui->twDOW->setRowCount(days_of_week.size());
    for (std::string dow : days_of_week){

        QTableWidgetItem* dow_item = new QTableWidgetItem(QString::fromStdString(dow));
        //QTableWidgetItem* dow_breaks = new QTableWidgetItem();

        dow_item->setData(Qt::UserRole, i++);
        ui->twDOW->setItem(row, 0, dow_item);

        QComboBox* cb = new QComboBox();
        ui->twDOW->setCellWidget(row, 1, cb);
        //ui->twDOW->setItem(row, 1, dow_breaks);
        ++row;
    }
}

void BookingWizard::show_breaks_for_current_timeband()
{
    ui->lwSelBreaks->clear();
    for (auto [break_id, selected_break] : m_selected_breaks){
        QListWidgetItem* item = new QListWidgetItem(selected_break.break_time.toString("HH:mm"));
        ui->lwSelBreaks->addItem(item);
    }
}

void BookingWizard::toggle_selection(bool mode)
{
    auto breaks = ui->lwSelBreaks;

    for (int i=0; i < ui->lwSelBreaks->count(); ++i){
        auto item = breaks->item(i);
        item->setSelected(mode);
    }

}

void BookingWizard::auto_select_breaks_by_dow()
{
    for (auto [dow, breaks] : m_dow_selection){
        for (auto brk : breaks){
            for (int i=0; i < ui->twBreakSelect->rowCount(); ++i){
                auto item = ui->twBreakSelect->item(i, 2);
                if (item->data(Qt::UserRole).toInt() == dow){
                    auto time_str = ui->twBreakSelect->item(i, 1)->text().toStdString();
                    if (brk == time_str){
                        ui->twBreakSelect->selectRow(i);
                    }
                }

            }
        }
    }
}

bool BookingWizard::spot_has_audio(const TRAFFIK::Spot* spot)
{
    auto edm = EntityDataModel(
               std::make_unique<TRAFFIK::SpotAudio>());

    auto saudio = std::make_unique<TRAFFIK::SpotAudio>();

    auto audio_filter = std::make_tuple(
                saudio->parentId()->dbColumnName(),
                " = ",
                spot->id()
                );

    std::string filter = edm.prepareFilter(audio_filter);

    edm.search(filter);

    return edm.count() > 0 ? true : false;
}

void BookingWizard::find_existing_bookings(TRAFFIK::EngineData& engine_data)
{
    std::string schedule_ids;
    std::size_t i = 0;
    for(auto& [name, entity] : m_engine_data.m_schedule_EDM->modelEntities()){
        schedule_ids += std::to_string(entity->id());
        if ( i<m_engine_data.m_schedule_EDM->modelEntities().size()-1)
            schedule_ids += ",";
        ++i;
    }

    schedule_ids = "("+schedule_ids+")";

    std::stringstream sql;
    sql << "SELECT a.id AS booking_id, a.schedule_id, a.spot_id, a.bookingsegment_id, "
        << " b.name, b.spot_duration, b.real_duration, b.brand_id, b.client_id, "
        << " b.daypart1 AS spotDP1, b.daypart2 AS spotDP2, b.daypart3 AS spotDP3, "
        << " b.daypart4 AS spotDP4, b.daypart5 AS spotDP5, b.daypart6 AS spotDP6,b.daypart7 AS spotDP7, "
        << " e.daypart1 AS typeDP1, e.daypart2 AS typeDP2, e.daypart3 AS typeDP3, "
        << " e.daypart4 AS typeDP4, e.daypart5 AS typeDP5, e.daypart6 AS typeDP6,e.daypart7 AS typeDP7, "
        << " f.daypart1 AS voDP1, f.daypart2 AS voDP2, f.daypart3 AS voDP3, "
        << " f.daypart4 AS voDP4, f.daypart5 AS voDP5, f.daypart6 AS voDP6, f.daypart7 AS voDP7, "
        << " c.detail_id AS spot_excl, d.detail_id AS spot_vo "
        << " FROM rave_orderbooking a "
        << " left join rave_spot b on a.spot_id = b.id "
        << " left join rave_spottypeexclusion c on a.spot_id = c.parent_id "
        << " left join rave_spotvoiceover d on a.spot_id = d.parent_id "
        << " left join rave_typeexclusion e on c.detail_id = e.id "
        << " left join rave_voiceover f on d.detail_id = f.id "
        << "WHERE a.schedule_id in " + schedule_ids+" ORDER BY a.id " ;

    EntityDataModel edm;
    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();

    if (provider->cacheSize() > 0 ) {
        provider->cache()->first();
        do {
            auto itB = provider->cache()->currentElement()->begin();
            auto itE = provider->cache()->currentElement()->end();

            TRAFFIK::BookingRecord br;
            Daypart typeDaypart;
            Daypart voDaypart;
            int sd = 1; // spot daypart postfix
            int td = 1; // type excl daypart postfix
            int vdp = 1; // type excl daypart postfix

            int type_excl_id = -1;
            int vo_excl_id = -1;

            for (; itB != itE; ++itB){
                std::string field = (*itB).first;
                std::string value = (*itB).second;

                if (field == "booking_id")
                    br.booking_id = to_int(value);
                if (field == "schedule_id")
                    br.schedule_id = to_int(value);
                if (field == "bookingsegment_id")
                    br.bookingsegment_id = to_int(value);
                if (field == "brand_id")
                    br.booked_spot.brand_id = to_int(value);

                if (field == "client_id")
                    br.booked_spot.client_id = to_int(value);

                if (field == "spot_excl")
                    type_excl_id = to_int(value);
                if (field == "spot_vo")
                    vo_excl_id = to_int(value);

                if (field == "spot_id")
                    br.booked_spot.spot_id = to_int(value);
                if (field == "name")
                    br.booked_spot.spot_name = value;

                if (field == "spot_duration"){
                    br.booked_spot.spot_duration = to_double(value);
                }

                if (field == "real_duration"){
                    br.booked_spot.real_duration = to_double(value);
                }

                if (field == "spotdp"+std::to_string(sd)){
                    std::vector<int> hrs = selected_hours(value);
                    br.booked_spot.spot_daypart[sd++] = std::make_tuple(value, hrs);
                }

                if (field == "typedp"+std::to_string(td)){
                    std::vector<int> hrs = selected_hours(value);
                    typeDaypart[td++] = std::make_tuple(value, hrs);
                }

                if (field == "vodp"+std::to_string(vdp)){
                    std::vector<int> hrs = selected_hours(value);
                    voDaypart[vdp++] = std::make_tuple(value, hrs);
                }
            }

            br.booked_spot.type_ex_keys.push_back(type_excl_id);
            br.booked_spot.type_exclusions.push_back(std::make_tuple(type_excl_id, typeDaypart));

            br.booked_spot.voice_ex_keys.push_back(vo_excl_id);
            br.booked_spot.voice_exclusions.push_back(std::make_tuple(vo_excl_id, voDaypart));

            engine_data.prev_bookings.push_back(br);

            provider->cache()->next();

        } while (!provider->cache()->isLast());

    }

}

std::size_t BookingWizard::find_available_breaks()
{
    return m_rule_engine->find_breaks();
}

bool BookingWizard::validateCurrentPage()
{
    switch (currentId()){
        case 0:
           {
            TRAFFIK::Spot* sel_spot = selected_spot();
            qDebug() << sel_spot->id();
            bool has_audio = spot_has_audio(sel_spot);
            if (!has_audio){
                showMessage("Selected spot has no audio!");
                return false;
            }
            }
            break;
        case BookingWizard::Page_Dates:
            if (ui->edtStartDate->date() < QDate::currentDate()){
                showMessage("Bookings for past dates not allowed!");
                return false;
            }
            if (ui->edtStartDate->date() > ui->edtEndDate->date()){
                showMessage("Start date greater than End date");
                return false;
            }
            break;
        case BookingWizard::Page_Build_Breaks:
        {
            add_days_of_week();
            show_breaks_for_current_timeband();
            break;
        }
        case BookingWizard::Page_Select_By_Day:
        {
            auto_select_breaks_by_dow();
            break;
        }
        case BookingWizard::Page_Select_By_Date:
        {
            TRAFFIK::TreeViewModel* tvm  = new TRAFFIK::TreeViewModel(ui->twBreakSelect->selectedItems());
            ui->tvSummary->setModel(tvm);
            break;
        }
        case BookingWizard::Page_Final:
            return make_booking();
            //break;

    }

    return true;
}

TRAFFIK::Spot* BookingWizard::selected_spot()
{
    TRAFFIK::Spot* spot{nullptr};

    QVariant q_col_name = ui->tvSpots->model()->data(
                            ui->tvSpots->model()->index(
                                    ui->tvSpots->currentIndex().row(), 0));


    std::string spot_name = q_col_name.toString().toStdString();

    if (!spot_name.empty()){
        BaseEntity* be = m_spot_EDM->findEntityByName(spot_name);
        if (be != nullptr){
            spot = dynamic_cast<TRAFFIK::Spot*>(be);
        }
    }

    return spot;
}

Daypart BookingWizard::fetch_spot_daypart(TRAFFIK::Spot& spot)
{
    Daypart daypart;
    daypart[1] = std::make_tuple(spot.daypart1()->value(),
                                selected_hours(spot.daypart1()->value()));
    daypart[2] = std::make_tuple(spot.daypart2()->value(),
                                selected_hours(spot.daypart2()->value()));
    daypart[3] = std::make_tuple(spot.daypart3()->value(),
                                selected_hours(spot.daypart3()->value()));
    daypart[4] = std::make_tuple(spot.daypart4()->value(),
                                selected_hours(spot.daypart4()->value()));
    daypart[5] = std::make_tuple(spot.daypart5()->value(),
                                selected_hours(spot.daypart5()->value()));
    daypart[6] = std::make_tuple(spot.daypart6()->value(),
                                selected_hours(spot.daypart6()->value()));
    daypart[7] = std::make_tuple(spot.daypart7()->value(),
                                selected_hours(spot.daypart7()->value()));

    return daypart;
}

void BookingWizard::fetch_type_exclusions(TRAFFIK::EngineData& engine_data)
{

    std::stringstream sql;
    sql << "SELECT rave_typeexclusion.* "
       << " FROM rave_spottypeexclusion, rave_typeexclusion "
       << " WHERE rave_spottypeexclusion.detail_id = rave_typeexclusion.id "
       << " AND rave_spottypeexclusion.parent_id = "+std::to_string(engine_data.spot_to_book.spot_id);

    fetch_spot_exclusions(sql.str(),
                        m_engine_data.spot_to_book.type_exclusions,
                        m_engine_data.spot_to_book.type_ex_keys);
}

void BookingWizard::fetch_voice_exclusions(TRAFFIK::EngineData& engine_data)
{
    std::stringstream sql;
    sql << "SELECT rave_voiceover.* "
       << " FROM rave_spotvoiceover, rave_voiceover "
       << " WHERE rave_spotvoiceover.detail_id = rave_voiceover.id "
       << " AND rave_spotvoiceover.parent_id = "+std::to_string(engine_data.spot_to_book.spot_id);

    fetch_spot_exclusions(sql.str(),
                        m_engine_data.spot_to_book.voice_exclusions,
                        m_engine_data.spot_to_book.voice_ex_keys);
}

void BookingWizard::fetch_spot_exclusions(const std::string query,
                                        std::vector<Exclusion>& exclusions,
                                        std::list<int>& keys)
{
    EntityDataModel edm;
    edm.readRaw(query);

    auto provider = edm.getDBManager()->provider();

    if (provider->cacheSize() > 0){
        provider->cache()->first();

        std::string value;
        int exclusion_id;

        do{
            auto itB = provider->cache()->currentElement()->begin();
            auto itE = provider->cache()->currentElement()->end();

            int i = 1;
            std::string daypart_name = "daypart";
            Daypart daypart;

            std::string key{}, val{}, dp{};

            for (; itB != itE; ++itB){

                if ((*itB).first == "id")
                    exclusion_id = std::stoi((*itB).second);

                if ((*itB).first == daypart_name+std::to_string(i)){
                    auto dp_str = (*itB).second;
                    std::vector<int> hrs = selected_hours(dp_str);
                    daypart[i++] = std::make_tuple(dp_str, hrs);
                }
            }

            keys.push_back(exclusion_id);
            exclusions.push_back(std::make_tuple(exclusion_id, daypart));

            provider->cache()->next();

        } while(!provider->cache()->isLast());

    }

}

void BookingWizard::init_rules_state()
{
    TRAFFIK::FullBreakRule::enable_or_disable(ui->cbBreakDuration->checkState());
    TRAFFIK::TypeExclusionRule::enable_or_disable(ui->cbEnforceTypeExRule->checkState());
    TRAFFIK::VoiceExclusionRule::enable_or_disable(ui->cbVoiceExclusion->checkState());
    TRAFFIK::TypeDaypartRule::enable_or_disable(ui->cbTypeDaypart->checkState());
    TRAFFIK::VoiceDaypartRule::enable_or_disable(ui->cbVoiceDaypart->checkState());
    TRAFFIK::SpotDaypartRule::enable_or_disable(ui->cbSpotDaypart->checkState());
    TRAFFIK::SameClientRule::enable_or_disable(ui->cbNoSameClient->checkState());

    QString zero{"0"};
    ui->lblFullBreaks->setText(zero);
    ui->lblTypeExclusion->setText(zero);
    ui->lblVoiceExclusion->setText(zero);
    ui->lblTypeDaypart->setText(zero);
    ui->lblVoiceDaypart->setText(zero);
    ui->lblSpotDaypart->setText(zero);
    ui->lblNoSameClient->setText(zero);
}

void BookingWizard::show_available_breaks()
{
    QPixmap pm_breaks(":/images/icons/images/icons/error_mark.png");

    ui->lblBreaksChecked->setText(
                stoq(std::to_string(m_engine_data.break_count)));

    if (TRAFFIK::FullBreakRule::failed_break_count() > 0){
        ui->lblFullBreaks->setText(
                    stoq(std::to_string(TRAFFIK::FullBreakRule::failed_break_count())));
        ui->imgFullBreaks->setPixmap(pm_breaks);
    }

    if (TRAFFIK::TypeExclusionRule::failed_break_count() > 0){
        ui->lblTypeExclusion->setText(
                    stoq(std::to_string(TRAFFIK::TypeExclusionRule::failed_break_count())));
        ui->imgTypeExcl->setPixmap(pm_breaks);
    }

    if (TRAFFIK::VoiceExclusionRule::failed_break_count() > 0){
        ui->lblVoiceExclusion->setText(
                    stoq(std::to_string(TRAFFIK::VoiceExclusionRule::failed_break_count())));
        ui->imgVoiceExcl->setPixmap(pm_breaks);
    }

    if (TRAFFIK::TypeDaypartRule::failed_break_count() > 0){
        ui->lblTypeDaypart->setText(
                    stoq(std::to_string(TRAFFIK::TypeDaypartRule::failed_break_count())));
        ui->imgTypeDaypart->setPixmap(pm_breaks);
    }

    if (TRAFFIK::VoiceDaypartRule::failed_break_count() > 0){
        ui->lblVoiceDaypart->setText(
                    stoq(std::to_string(TRAFFIK::VoiceDaypartRule::failed_break_count())));
        ui->imgVoiceDaypart->setPixmap(pm_breaks);
    }

    if (TRAFFIK::SpotDaypartRule::failed_break_count() > 0){
        ui->lblSpotDaypart->setText(
                    stoq(std::to_string(TRAFFIK::SpotDaypartRule::failed_break_count())));
        ui->imgSpotDaypart->setPixmap(pm_breaks);
    }

    if (TRAFFIK::SameClientRule::failed_break_count() > 0){
        ui->lblNoSameClient->setText(
                    stoq(std::to_string(TRAFFIK::SameClientRule::failed_break_count())));
        ui->imgNoSameClient->setPixmap(pm_breaks);
    }

    ui->lblTotalBreaks->setText(QString::number(m_engine_data.available_breaks));
    ui->lblAvilBreaks->setText(QString::number(m_engine_data.available_breaks));
}

std::vector<int> BookingWizard::selected_hours(const std::string& dp_str)
{
    int hr = 0;
    std::vector<int> hrs;
    for(std::string::const_iterator it=dp_str.begin(); it != dp_str.end(); ++it){
        if (*it=='1')
            hrs.push_back(hr);
        ++hr;
    }

    return hrs;

}


