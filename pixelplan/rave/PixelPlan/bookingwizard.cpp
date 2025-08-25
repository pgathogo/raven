#include <algorithm>
#include <random>
#include <ranges>
#include <cstdlib>
#include <ranges>
#include <format>
#include <print>

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QAbstractItemModel>
#include <QSpinBox>
#include <QTableWidgetItem>
#include <QListWidgetItem>

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

#include "breaklayout.h"
#include "breaklayoutline.h"

#include "traffiktreeviewmodel.h"
#include "tvprogram.h"

#include "../../../rave/framework/entitydatamodel.h"
#include "../../../rave/framework/ravenexception.h"
#include "../../../rave/framework/schedule.h"
#include "../../../rave/framework/ravensetup.h"

#include "../../../rave/utils/qchecklist.h"
#include "../../../rave/utils/daypartgrid.h"
#include "../../../rave/utils/togglebutton.h"
#include "../../../rave/utils/cachehandler.h"
#include "../../../rave/utils/tools.h"


#define DEBUG_MODE

BookingWizard::BookingWizard(Order* order,  QWidget *parent)
    :QWizard(parent)
    ,ui(new Ui::BookingWizard)
    ,m_order{order}
    ,m_spot_EDM{nullptr}
    ,m_timeband_EDM{nullptr}
    ,m_daypart_grid{nullptr}
    ,m_booking_EDM{nullptr}
    ,m_spot_ctx_menu{nullptr}
    ,m_spot_ctx_action{nullptr}
    ,m_edm_break_layout{nullptr}
    ,m_edm_breaks{nullptr}
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
    //ui->cbTimeband->setModel(m_timeband_EDM);

    //ui->lvTimeBand->setModel(m_timeband_EDM);
    //connect(ui->lvTimeBand, &QListView::clicked, this, &BookingWizard::time_band_selected);

    connect(ui->twTB, &QTableWidget::itemClicked, this, &BookingWizard::time_band_selected2);

    connect(ui->lwPrograms, &QListWidget::itemClicked, this, &BookingWizard::on_programs_clicked);

    connect(ui->cbAllPrograms, &QCheckBox::toggled, this, &BookingWizard::on_select_all);
    connect(ui->cbAllBreaks, &QCheckBox::toggled, this, &BookingWizard::on_select_all_breaks);


    // connect(ui->cbTimeband, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //         this, &BookingWizard::timeBandChanged);

    connect(ui->rbAllBreaks, &QRadioButton::toggled, this, &BookingWizard::all_breaks);
    connect(ui->rbPrograms, &QRadioButton::toggled, this, &BookingWizard::tv_programs);
    connect(ui->rbTimeband, &QRadioButton::toggled, this, &BookingWizard::toggleTimeBand);
    connect(ui->rbManualTime, &QRadioButton::toggled, this, &BookingWizard::manual_time);

    connect(ui->btnBuildBreaks, &QPushButton::clicked, this, &BookingWizard::build_breaks);

    ui->btnBuildBreaks->setIcon(QIcon(":/images/media/icons/build01.bmp"));
    ui->btnBuildBreaks->setIconSize(QSize(32, 32));

    connect(ui->twBreakSelect, &QTableWidget::itemSelectionChanged, this, [this](){
        this->ui->edtSelBreaks->setText(QString::number(this->ui->twBreakSelect->selectedRanges().size()));
        this->ui->lblTotalBookings->setText(QString::number(this->ui->twBreakSelect->selectedRanges().size()));
    });

    m_daypart_grid = std::make_unique<DayPartGrid>(ui->vlDaypart);


    ui->edtStartDate->setDate(QDate::currentDate());
    ui->edtEndDate->setDate(order->endDate()->value());


    m_rule_engine = std::make_unique<TRAFFIK::RuleEngine>(m_engine_data);

    show_order_details(order);

    ui->lwSelBreaks->setSpacing(5);

    connect(ui->btnBreakSelApply, &QPushButton::clicked, this, &BookingWizard::apply_selection);
    connect(ui->btnClearSel, &QPushButton::clicked, this, &BookingWizard::clear_selection);

    connect(ui->btnBreakSelect, &QPushButton::clicked, this, [&](){ this->toggle_selection(true);} );
    connect(ui->btnBreakUnselect, &QPushButton::clicked, this, [&](){ this->toggle_selection(false);} );

    auto break_lines = std::make_shared<BreakLayoutLine>();
    m_edm_breaks = std::make_unique<EntityDataModel>(break_lines);
    ui->tvBreaks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvBreaks->setModel(m_edm_breaks.get());

    m_break_layout = std::make_shared<BreakLayout>();
    m_edm_break_layout = std::make_unique<EntityDataModel>(m_break_layout);

    ui->cbBreakTemplate->setModel(m_edm_break_layout.get());
    m_edm_break_layout->all();

    connect(ui->cbBreakTemplate, QOverload<int>::of(&QComboBox::currentIndexChanged),
                  this, &BookingWizard::show_breaks);


    ui->cbBreakTemplate->currentIndexChanged(0);

    set_toggle_buttons();
    read_break_rules_cache();

    add_tb_widget();

    m_edm_setup = std::make_unique<EntityDataModel>( std::make_shared<RavenSetup>() );
    m_edm_setup->all();
    if (m_edm_setup->count() > 0)
        m_raven_setup = std::dynamic_pointer_cast<RavenSetup>(m_edm_setup->firstEntity());
    else
        m_raven_setup = std::make_shared<RavenSetup>();


    m_programs = get_tv_programs();

    std::vector<ProgramBreak> breaks = fetch_all_breaks(m_programs);
    assign_breaks_to_programs(breaks,  m_programs);
    show_tv_programs(m_programs);

    // ui->rbAllBreaks->toggle();
    ui->rbAllBreaks->setVisible(false);
    ui->rbPrograms->toggle();

    ui->rbManualTime->setVisible(false);

    //QPixmap* wpixmap = new QPixmap("D:/home/PMS/Raven/images/wizard_sidebanner.png");
    //setPixmap(QWizard::WatermarkPixmap, *wpixmap);
}


BookingWizard::~BookingWizard()
{
    delete ui;
}


void BookingWizard::add_break_interval()
{
    int time_interval = m_raven_setup->breakTimeInterval()->value();

    int breaks_per_hour = 1;
    if (time_interval > 0)
        breaks_per_hour = 60 / time_interval;


    int min = 0;

    for(int ti=1; ti <= breaks_per_hour; ++ti)
    {
        QTime time(0, min, 0);

        QString title;
        title = QString("%1").arg(time.toString("HH:mm"));
        if (ti == 1)
            title = QString("%1 - %2")
                                .arg(time.toString("HH:mm")).arg("Hour Top");

        if (ti == breaks_per_hour)
            title = QString("%1 - %2")
                                .arg(time.toString("HH:mm")).arg("Hour Bottom");

        auto lwi = new QListWidgetItem(title);
        lwi->setFlags(lwi->flags() | Qt::ItemIsUserCheckable);
        lwi->setCheckState(Qt::Unchecked);
        lwi->setData(Qt::UserRole, min);

        ui->lwHourSegments->addItem(lwi);

        min += time_interval;

    }
}

std::map<progid, Program> BookingWizard::get_tv_programs()
{
    std::map<progid, Program> programs;

    auto edm = std::make_unique<EntityDataModel>(std::make_unique<PIXELPLAN::TVProgram>());
    edm->all();

    if (edm->count() == 0)
        return programs;

    auto provider = edm->getDBManager()->provider();
    if (provider->cacheSize() == 0)
        return programs;

    provider->cache()->first();


    do {
        auto it_begin = provider->cache()->currentElement()->begin();
        auto it_end = provider->cache()->currentElement()->end();

        int id = -1;
        QString title;

        for(; it_begin != it_end; ++it_begin) {
            std::string field_name = (*it_begin).first;
            std::string field_value = (*it_begin).second;

            if (field_name == "id")
                id = std::stoi(field_value);

            if (field_name == "title")
                title = QString::fromStdString(field_value);
        }

        Program prg = {title};
        programs[id] = prg;

        provider->cache()->next();

    } while(!provider->cache()->isLast());


    return programs;

}

void BookingWizard::show_tv_programs(std::map<progid, Program> programs)
{
    for (const auto& [id, prg] : programs)
    {
        auto lwi = new QListWidgetItem(prg.title);
        lwi->setData(Qt::UserRole, id);

        // If program has no breaks, show it in red
        if (prg.breaks.size() == 0) {
            lwi->setForeground(QBrush(QColor(Qt::red)));
        }

        ui->lwPrograms->addItem(lwi);

    }
}


void BookingWizard::show_breaks(int index)
{
    if (index < 0)
        return;

    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbBreakTemplate->model());
    int bl_id = std::get<1>(*(edm->vecBegin()+index))->id() ;

    BreakLayoutLine bbl;
    auto breaks_filter = std::make_tuple(bbl.breakLayout()->dbColumnName(),
                                         "=",
                                         bl_id);

    std::string filter = m_edm_breaks->prepareFilter(breaks_filter);
    m_edm_breaks->search(filter);
}


void BookingWizard::add_tb_widget()
{
    ui->twTB->clear();
    QStringList header;
    header << "TBand" << "Dist.";
    ui->twTB->setHorizontalHeaderLabels(header);

    ui->twTB->setRowCount(m_timeband_EDM->count());

    auto provider = m_timeband_EDM->getDBManager()->provider();
    if (provider->cacheSize() == 0)
        return;

    provider->cache()->first();
    int row = 0;
    do{
        auto it_begin = provider->cache()->currentElement()->begin();
        auto it_end = provider->cache()->currentElement()->end();

        int id = -1;
        QString tb_name;

        for(; it_begin != it_end; ++it_begin)
        {
            std::string field_name = (*it_begin).first;
            std::string field_value = (*it_begin).second;

            if (field_name == "id")
                id = std::stoi(field_value);
            if (field_name == "name")
                tb_name = QString::fromStdString(field_value);
        }

        auto twi = new QTableWidgetItem(tb_name);
        twi->setData(Qt::UserRole, id);
        ui->twTB->setItem(row, 0, twi);

        QSpinBox* sb = new QSpinBox();
        sb->setMaximumWidth(60);
        ui->twTB->setCellWidget(row, 1, sb);
        ++row;

       provider->cache()->next();

    }while(!provider->cache()->isLast());


    // auto tb = dynamic_cast<TimeBand*>(e.get());
}

void BookingWizard::test_set_start_end_dates()
{
    ui->edtStartDate->setDate(QDate::currentDate());
    ui->edtEndDate->setDate(QDate(2025, 6, 30));
}

void BookingWizard::set_toggle_buttons()
{
    ui->lblBreakDuration->setEnabled(false);
    ui->lblTypeEx->setEnabled(false);
    ui->lblVoiceEx->setEnabled(false);
    ui->lblDaypart->setEnabled(false);
    ui->lblVoiceDaypartRule->setEnabled(false);
    ui->lblSpotDaypartRule->setEnabled(false);
    ui->lblSameClient->setEnabled(false);
    ui->lblOverride->setEnabled(false);

    auto lblSelectAll = new QLabel("Select All Rules", this);
    m_toggle_all = new ToggleButton(10,12);
    ui->hlAll->addWidget(lblSelectAll);
    ui->hlAll->addWidget(m_toggle_all);
    ui->hlAll->addStretch();

    m_toggle_break_duration = new ToggleButton(10, 12);
    m_toggle_type_ex = new ToggleButton(10, 12);
    m_toggle_voice_ex = new ToggleButton(10, 12);
    m_toggle_type_daypart = new ToggleButton(10, 12);
    m_toggle_voice_daypart = new ToggleButton(10, 12);
    m_toggle_spot_daypart = new ToggleButton(10, 12);
    m_toggle_same_client = new ToggleButton(10, 12);
    m_toggle_override = new ToggleButton(10, 12);

    m_toggle_buttons.push_back(m_toggle_break_duration);
    m_toggle_buttons.push_back(m_toggle_type_ex);
    m_toggle_buttons.push_back(m_toggle_voice_ex);
    m_toggle_buttons.push_back(m_toggle_type_daypart);
    m_toggle_buttons.push_back(m_toggle_voice_daypart);
    m_toggle_buttons.push_back(m_toggle_spot_daypart);
    m_toggle_buttons.push_back(m_toggle_same_client);
    m_toggle_buttons.push_back(m_toggle_override);

    auto lblRemember = new QLabel("Remember selected break rules");
    m_remember_rules = new ToggleButton(10, 12);

    ui->hlBottom->addWidget(lblRemember);
    ui->hlBottom->addWidget(m_remember_rules);
    ui->hlBottom->addStretch();

    connect(m_toggle_all, &ToggleButton::toggled, this, [&](bool togged){
        for (auto tb: m_toggle_buttons){
            tb->setChecked(togged);
        }
    });

    connect(m_toggle_break_duration, &ToggleButton::toggled, this, [&](bool togged){
        ui->lblBreakDuration->setEnabled(togged);
    });

    connect(m_toggle_type_ex, &ToggleButton::toggled, this, [&](bool togged){
        ui->lblTypeEx->setEnabled(togged);
    });

    connect(m_toggle_voice_ex, &ToggleButton::toggled, this, [&](bool togged){
        ui->lblVoiceEx->setEnabled(togged);
    });

    connect(m_toggle_type_daypart, &ToggleButton::toggled, this, [&](bool togged){
        ui->lblDaypart->setEnabled(togged);
    });

    connect(m_toggle_voice_daypart, &ToggleButton::toggled, this, [&](bool togged){
        ui->lblVoiceDaypartRule->setEnabled(togged);
    });

    connect(m_toggle_spot_daypart, &ToggleButton::toggled, this, [&](bool togged){
        ui->lblSpotDaypartRule->setEnabled(togged);
    });

    connect(m_toggle_same_client, &ToggleButton::toggled, this, [&](bool togged){
        ui->lblSameClient->setEnabled(togged);
    });

    connect(m_toggle_override, &ToggleButton::toggled, this, [&](bool togged){
        ui->lblOverride->setEnabled(togged);
    });


    ui->hlDuration->addWidget(m_toggle_break_duration);
    ui->hlTypeEx->addWidget(m_toggle_type_ex);
    ui->hlVoiceEx->addWidget(m_toggle_voice_ex);
    ui->hlDaypart->addWidget(m_toggle_type_daypart);
    ui->hlVoiceDayEx->addWidget(m_toggle_voice_daypart);
    ui->hlSpotEx->addWidget(m_toggle_spot_daypart);
    ui->hlSameClient->addWidget(m_toggle_same_client);
    ui->hlOverride->addWidget(m_toggle_override);
}

void BookingWizard::populate_spots_table(int client_id)
{

    m_spot_EDM = std::make_unique<EntityDataModel>(
                std::make_shared<TRAFFIK::Spot>());

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

    for (auto& [name, entity] : m_engine_data.m_schedule_EDM->modelEntities())
    {
        Schedule* comm_break = dynamic_cast<Schedule*>(entity.get());

        if (comm_break->break_availability() == Schedule::BreakAvailability::Break_Not_Available)
            continue;

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
        sel_break.booked_spots = comm_break->booked_spots()->value();
        sel_break.break_fill_method = comm_break->break_fill_method()->value();
        sel_break.max_spots = comm_break->break_max_spots()->value();
        m_selected_breaks[comm_break->id()] = sel_break;

        ++row;
    }
}

bool BookingWizard::make_booking()
{
    if (QMessageBox::question(this, tr("Traffik"),
                                tr("Are you sure you want to commit the booking(s)?"),
                                QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes){
        //print_selected_breaks();
        commit_booking();
        return true;
    }

    return false;

}


void BookingWizard::print_selected_breaks()
{
    for (auto& [key, sel_break]: m_selected_breaks)
    {
        qDebug() << "Key: "<< key;
        qDebug() << "Break Id: "<< sel_break.break_id;
        qDebug() << "Booked Spots: "<< sel_break.booked_spots;
        qDebug() << "Break Fill Method: "<< stoq(sel_break.break_fill_method);
    }
}

void BookingWizard::commit_booking()
{
    try{

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

        for (int i=0; i<selected_breaks.count(); ++i)
        {
            auto item = selected_breaks.at(i);

            qDebug() << ">>SEL TIME: " << item->text();

            if (item->column() == 0)
            {
                OrderBooking order_booking;

                int break_id = item->data(Qt::UserRole).toInt();

                SelectedBreak selected_break = m_selected_breaks[break_id];

                order_booking.set_booking_status("READY");
                order_booking.set_schedule(break_id);
                order_booking.set_spot(m_engine_data.spot_to_book.spot_id);
                order_booking.set_booking_segment(book_segment_id);
                order_booking.play_date()->setReadOnly(true);
                order_booking.play_time()->setReadOnly(true);

                order_booking.set_book_date(selected_break.break_date);
                order_booking.set_book_time(selected_break.break_time);
                order_booking.set_book_hour(selected_break.break_hour);

                order_booking.set_audio(m_engine_data.spot_to_book.audio_id);

                // Sequential slotting
                if (selected_break.break_fill_method == "S") {
                    order_booking.set_book_seq(++selected_break.booked_spots);
                }

                // Random slotting
                if (selected_break.break_fill_method == "R") {
                    int break_slot = find_break_slot(break_id, selected_break.max_spots);
                    order_booking.set_book_seq(break_slot);
                }

                qDebug() << "Creating New Oder..." ;

                edm.createEntityDB(order_booking);

                // Deduct time remainining on this break
                std::stringstream  sql ;
                sql << "Update rave_schedule set break_duration_left =  break_duration_left - "
                    << std::to_string(m_engine_data.spot_to_book.spot_duration)
                    << ", booked_spots = booked_spots + 1 "
                    << " Where id = "+std::to_string(break_id);

                edm.executeRawSQL(sql.str());
            }
        }

        std::stringstream update_order;
        update_order << "Update rave_order set spots_booked = spots_booked + "
                     << std::to_string(book_segment.booking_count()->value())
                     << " Where order_number = '"+m_order->orderNumber()->value()+"'";

        edm.executeRawSQL(update_order.str());

    }

    catch(DatabaseException& de){
        showMessage(de.errorMessage());
    }

}

int BookingWizard::find_break_slot(int break_id, int max_spots)
{
    std::vector<int> prev_seq;

    for(auto booking : m_engine_data.prev_bookings){
        if (booking.schedule_id == break_id){
            prev_seq.push_back(booking.book_seq);
        }
    }

    auto gen_next_seq = [&](int rand){
        return (
            ( std::find(prev_seq.begin(), prev_seq.end(), rand) != prev_seq.end() ) ? true : false );
    };

    int next_seq{-1};
    std::random_device engine;
    std::uniform_int_distribution<int> distrib(1, max_spots);

    do{
        next_seq = distrib(engine);
    }while(gen_next_seq(next_seq));


    return next_seq;
}

void BookingWizard::show_order_details(Order* order)
{
    ui->lblOrderNo->setText(order->orderNumber()->to_qstring());
    ui->lblSpotsOrdered->setText(QString::number(order->spotsOrdered()->value()));
    ui->lblSpotsBooked->setText(QString::number(order->spotsBooked()->value()));
    ui->lblSpotsPending->setText(QString::number(
                                     order->spotsOrdered()->value() - order->spotsBooked()->value()));
}



std::size_t BookingWizard::fetch_program_breaks_from_db(QDate start_date, QDate end_date,
                                                        std::vector<SelectedProgramBreak> sel_prog_breaks )
{
    if (sel_prog_breaks.size() == 0)
        return 0;


    QString prog_breaks = "";
    int i = 0;
    for(auto& spb : sel_prog_breaks) {
        prog_breaks  += "'"+ spb.break_time +"'";
        ++i;
        if (i < sel_prog_breaks.size())
            prog_breaks += ",";
    }

    prog_breaks = "("+prog_breaks+")";


    Schedule schedule;
    QString DATE_FORMAT = "yyyy-MM-dd";
    QString date_range = "'"+start_date.toString(DATE_FORMAT)+"' and '"+end_date.toString(DATE_FORMAT)+"')";

    auto date_range_filter = std::make_tuple(
                "("+schedule.schedule_date()->dbColumnName(),
                " between ",
                date_range.toStdString()
                );


    auto hours_filter = std::make_tuple(
        schedule.schedule_time()->dbColumnName(),
        " in ",
        prog_breaks.toStdString()
        );


    try {
        m_engine_data.m_schedule_EDM->search( m_engine_data.m_schedule_EDM->prepareFilter(
                date_range_filter, hours_filter));
    }catch(DatabaseException& de) {
        showMessage(de.errorMessage());
    }


    return m_engine_data.m_schedule_EDM->count();
}

std::vector<SelectedProgramBreak> BookingWizard::get_selected_program_breaks()
{
    std::vector<SelectedProgramBreak> selected_breaks{};

    QModelIndexList mil = ui->twBreaks->selectionModel()->selectedRows();

    if (mil.size() == 0)
        return selected_breaks;


    for (auto& index : mil) {
        int row = index.row();

        SelectedProgramBreak spb;
        QTableWidgetItem* twi_break_time = ui->twBreaks->item(row, 0);
        spb.break_time = twi_break_time->text();

        QTableWidgetItem* twi_duration = ui->twBreaks->item(row, 1);
        spb.duration =  twi_duration->text().toInt();

        QTableWidgetItem* twi_max_spots = ui->twBreaks->item(row, 2);
        spb.max_spots = twi_max_spots->text().toInt();


        QComboBox* combo = (QComboBox*)ui->twBreaks->cellWidget(row, 3);

        spb.fill_pos  = static_cast<FillPos>(combo->currentIndex());

        selected_breaks.push_back(spb);

    }

    return selected_breaks;
}


std::size_t BookingWizard::fetch_breaks_from_db(QDate start_date, QDate end_date, std::set<int> uniq_hours)
{

    if (m_engine_data.m_schedule_EDM->count() > 0)
    return m_engine_data.m_schedule_EDM->count();

    Schedule schedule;
    QString DATE_FORMAT = "yyyy-MM-dd";
    QString date_range = "'"+start_date.toString(DATE_FORMAT)+"' and '"+end_date.toString(DATE_FORMAT)+"')";

    auto date_range_filter = std::make_tuple(
                "("+schedule.schedule_date()->dbColumnName(),
                " between ",
                date_range.toStdString()
                );


    if (uniq_hours.size() > 0)
    {
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
            //std::string str  = m_engine_data.m_schedule_EDM->prepareFilter(date_range_filter, hours_filter);
            //std::cout << str << '\n';
                m_engine_data.m_schedule_EDM->search(m_engine_data.m_schedule_EDM->prepareFilter(date_range_filter, hours_filter));
            } catch(DatabaseException& de){
                showMessage(de.errorMessage());
        }
    } else {
        try {
            m_engine_data.m_schedule_EDM->search(m_engine_data.m_schedule_EDM->prepareFilter(date_range_filter));
        } catch (DatabaseException& de) {
            showMessage(de.errorMessage());
        }
    }

    return m_engine_data.m_schedule_EDM->count();
}

/*
void BookingWizard::timeBandChanged(int i)
{

    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbTimeband->model());
    m_engine_data.current_time_band = dynamic_cast<TimeBand*>(std::get<1>(*(edm->vecBegin()+i)).get());
    populate_grid(m_engine_data.current_time_band);
    m_engine_data.target_daypart =  m_daypart_grid->daypart_to_hours(m_daypart_grid->read_grid());
}
*/

void BookingWizard::all_breaks(bool state)
{

    std::set<int> unique_hours;
    fetch_breaks_from_db(ui->edtStartDate->date(), ui->edtEndDate->date(), unique_hours);
    ui->swSelection->setCurrentIndex(0);
    unique_hours = select_unique_hours_from_breaks();
    populate_from_to_combos(unique_hours);
}


void BookingWizard::tv_programs(bool state)
{
    ui->swSelection->setCurrentIndex(1);
}

void BookingWizard::populate_from_to_combos(const std::set<int>& hrs)
{
    for (int hr : hrs)
    {
        ui->cbFromHour->addItem(QString::number(hr), hr);
        ui->cbToHour->addItem(QString::number(hr), hr);
    }
    ui->cbFromHour->setCurrentIndex(0);
    ui->cbToHour->setCurrentIndex(ui->cbToHour->count()-1);

}

void BookingWizard::toggleTimeBand(bool state)
{
    ui->swSelection->setCurrentIndex(2);
    //ui->cbTimeband->setEnabled(state);
}

void BookingWizard::manual_time(bool state)
{
    ui->swSelection->setCurrentIndex(3);
}

void BookingWizard::build_breaks()
{
//    test_booking();
//    return;

    reset_values();

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
        m_engine_data.spot_to_book.audio_id = get_spot_audio(spot->id());

        fetch_type_exclusions(m_engine_data);

        fetch_voice_exclusions(m_engine_data);

        std::set<int> unique_hours;

        if (ui->rbAllBreaks->isChecked())
        {
            m_engine_data.break_count = fetch_breaks_from_db(ui->edtStartDate->date(), ui->edtEndDate->date(), unique_hours);
            //unique_hours = select_unique_hours_from_breaks();
        }

        if (ui->rbPrograms->isChecked())
        {
            auto selected_breaks = get_selected_program_breaks();

            m_engine_data.break_count = fetch_program_breaks_from_db(ui->edtStartDate->date(), ui->edtEndDate->date(),
                                           selected_breaks);
        }

        if (ui->rbTimeband->isChecked())
        {
            // Get hours from the selected timeband
            unique_hours = selected_unique_hours();
            m_engine_data.break_count = fetch_breaks_from_db(ui->edtStartDate->date(), ui->edtEndDate->date(), unique_hours);
        }


        if (m_engine_data.break_count == 0)
        {
            showMessage("No Breaks for the selected data range!");
            this->button(QWizard::NextButton)->setDisabled(true);
            return;
        }

        find_existing_bookings(m_engine_data);

        m_engine_data.available_breaks = find_available_breaks();

        show_available_breaks();

        setup_break_select_grid();

        if (ui->rbAllBreaks->isChecked())
        {
            AllBreaks all_breaks_sel;
            all_breaks_sel.from_hour = ui->cbFromHour->currentData().toInt();
            all_breaks_sel.to_hour = ui->cbToHour->currentData().toInt();
            all_breaks_sel.hourly_intervals = get_hourly_distribution();

            auto_select_breaks(all_breaks_sel);
        }

    } catch(DatabaseException& de){
        showMessage(de.errorMessage());
    }

}

void BookingWizard::reset_values()
{
    ui->imgFullBreaks->clear();
    ui->imgTypeDaypart->clear();
    color_label(ui->lblTotalBreaks, Qt::black);
    color_label(ui->lblFullBreaks, Qt::black);
    color_label(ui->lblTypeExclusion, Qt::black);
    color_label(ui->lblVoiceExclusion, Qt::black);
    color_label(ui->lblTypeDaypart, Qt::black);
    color_label(ui->lblVoiceDaypart, Qt::black);
    color_label(ui->lblSpotDaypart, Qt::black);
    color_label(ui->lblNoSameClient, Qt::black);
}

void BookingWizard::apply_selection()
{
    m_dow_selection.clear();

    auto sel_days = ui->twDOW->selectedItems();

    for (int j=0; j < sel_days.count(); ++j) {

        auto day_of_week = sel_days.at(j);

        QComboBox* combo = (QComboBox*)ui->twDOW->cellWidget(day_of_week->row(), 1);

        auto sel_items = ui->lwSelBreaks->selectedItems();

        for (int i=0; i < sel_items.size(); ++i) {
            auto time_str = sel_items[i]->text();
            if (combo->findText(time_str) == -1){
                combo->addItem(time_str);
                m_dow_selection[day_of_week->row()+1].push_back(time_str.toStdString());
            }
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

/*
void BookingWizard::time_band_selected(const QModelIndex& mi)
{
    m_daypart_grid->clear_all_cells();

    QItemSelectionModel* ism = ui->lvTimeBand->selectionModel();
    QModelIndexList mil = ism->selectedIndexes();

    if (mil.size() == 0)
        return;

    for(auto& index : mil){
        EntityDataModel* edm = dynamic_cast<EntityDataModel*>(const_cast<QAbstractItemModel*>(index.model()));
        m_engine_data.current_time_band = dynamic_cast<TimeBand*>(std::get<1>(*(edm->vecBegin()+index.row())).get());
        populate_grid(m_engine_data.current_time_band);
        m_engine_data.target_daypart =  m_daypart_grid->daypart_to_hours(m_daypart_grid->read_grid());
    }

}
*/

void BookingWizard::on_programs_clicked(QListWidgetItem* lwi)
{

    QItemSelectionModel* ism = ui->lwPrograms->selectionModel();
    QModelIndexList indexes = ism->selectedIndexes();

    if (indexes.size() == 0)
        return;

    ui->twBreaks->clearContents();
    ui->twBreaks->setRowCount(0);

    int prog_id = -1;

    for(auto& index : indexes)
    {
       prog_id = index.data(Qt::UserRole).toInt();
       show_program_breaks(m_programs[prog_id].breaks);
    }

}

void BookingWizard::on_select_all()
{
    if (ui->cbAllPrograms->isChecked()) {

        ui->lwPrograms->selectAll();
        auto selected_items = ui->lwPrograms->selectedItems();
        for(int i=0; i < selected_items.count(); i++)  {
            ui->lwPrograms->itemClicked(selected_items.at(i));
        }
    } else {
        ui->lwPrograms->clearSelection();
        ui->twBreaks->clearContents();
        ui->twBreaks->setRowCount(0);
    }
}


void BookingWizard::on_select_all_breaks()
{
    if (ui->cbAllBreaks->isChecked()) {
        ui->twBreaks->selectAll();
    } else {
        ui->twBreaks->clearSelection();
    }
}

void BookingWizard::set_breaks_table(int row_count)
{
    // Setup the table
    QStringList header;
    header << "Break Time" << "Duration" << "Max Spots" << "Fill Pos";
    ui->twBreaks->setColumnCount(4);
    ui->twBreaks->setHorizontalHeaderLabels(header);
    ui->twBreaks->setRowCount(row_count);
}

int BookingWizard::get_break_count(int prog_id)
{
    std::stringstream sql;

    sql << " SELECT rave_breaklayoutline.id "
        << " FROM rave_breaklayout, rave_breaklayoutline "
        << " WHERE rave_breaklayout.id = rave_breaklayoutline.break_layout_id "
        << "  AND rave_breaklayout.tvprogram_id = "+std::to_string(prog_id) ;

    EntityDataModel  edm;

    edm.readRaw(sql.str());
    auto provider = edm.getDBManager()->provider();

    return provider->cacheSize();

}

/*
void BookingWizard::fetch_program_breaks(std::string progids)
{
    ui->twBreaks->clearContents();
    ui->twBreaks->setRowCount(0);

    std::stringstream sql;
    sql << " SELECT rave_breaklayoutline.id, rave_breaklayoutline.break_time, rave_breaklayoutline.break_hour, "
        << " rave_breaklayoutline.duration, rave_breaklayoutline.max_spots, rave_breaklayoutline.break_fill_method, "
        << " rave_breaklayout.week_days "
        << " FROM rave_breaklayout, rave_breaklayoutline "
        << " WHERE rave_breaklayout.id = rave_breaklayoutline.break_layout_id "
        << "  AND rave_breaklayout.tvprogram_id in "+progids ;

    EntityDataModel  edm;
    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();
    if (provider->cacheSize() == 0)
        return;


    set_breaks_table(provider->cacheSize());

    provider->cache()->first();

    int row = 0;
    int BREAK_TIME_COL = 0;
    int DURATION_COL   = 1;
    int MAX_SPOTS_COL  = 2;
    int FILL_POS_COL   = 3;

    QStringList fill_pos = {"First", "In-Between", "Last"};

    do {
        auto itb = provider->cache()->currentElement()->begin();
        auto ite = provider->cache()->currentElement()->end();

        for(; itb != ite; ++itb) {
            std::string field_name = (*itb).first;
            std::string field_value = (*itb).second;

            int bll_id = 0;

            if (field_name == "id")
                bll_id = to_int(field_value);

            if (field_name == "break_time") {
                auto twi = new QTableWidgetItem(QString::fromStdString(field_value));
                twi->setData(Qt::UserRole, bll_id);
                ui->twBreaks->setItem(row, BREAK_TIME_COL, twi);
            }

            if (field_name == "duration") {
                auto twi = new QTableWidgetItem(QString::fromStdString(field_value));
                ui->twBreaks->setItem(row, DURATION_COL, twi);
            }

            if (field_name == "max_spots") {
                auto twi = new QTableWidgetItem(QString::fromStdString(field_value));
                ui->twBreaks->setItem(row, MAX_SPOTS_COL, twi);
            }

            QComboBox* cb = new QComboBox();
            cb->addItems(fill_pos);
            ui->twBreaks->setCellWidget(row, FILL_POS_COL, cb);

        }

        ++row;

        provider->cache()->next();

    } while (!provider->cache()->isLast());

}
*/


void BookingWizard::show_program_breaks(std::vector<ProgramBreak> breaks)
{
    if (breaks.size() == 0)
        return;

    set_breaks_table(breaks.size());

    int row = 0;
    int BREAK_TIME_COL = 0;
    int DURATION_COL   = 1;
    int MAX_SPOTS_COL  = 2;
    int FILL_POS_COL   = 3;

    QStringList fill_pos = {"First", "In-Between", "Last"};

    for (auto& pb: breaks) {

        // Break Time
        auto twi_btime = new QTableWidgetItem(pb.break_time);
        twi_btime->setData(Qt::UserRole, pb.line_id);
        ui->twBreaks->setItem(row, BREAK_TIME_COL, twi_btime);

        // Duration
        auto twi_duration = new QTableWidgetItem(QString::number(pb.duration));
        ui->twBreaks->setItem(row, DURATION_COL, twi_duration);

        // Max spots
        auto twi_max_spots = new QTableWidgetItem(QString::number(pb.max_spots));
        ui->twBreaks->setItem(row, MAX_SPOTS_COL, twi_max_spots);

        // Fill position combobox
        QComboBox* cb = new QComboBox();
        cb->addItems(fill_pos);
        ui->twBreaks->setCellWidget(row, FILL_POS_COL, cb);

        ++row;

    }

}

std::vector<int> BookingWizard::get_program_ids(const std::map<progid, Program>& programs)
{

    std::vector<int> p_ids;

    for(const auto&[pid, prog]: programs) {

        p_ids.push_back(pid);
    }

    return p_ids;

}



std::vector<ProgramBreak> BookingWizard::fetch_all_breaks(std::map<progid, Program> programs)
{
    std::vector<ProgramBreak> all_breaks;

    auto prog_ids = get_program_ids(programs);

    if (prog_ids.size() == 0)
        return all_breaks;

    std::string ids = join<int>(prog_ids);
    ids = "("+ids+")";

    std::stringstream sql;
    sql << " SELECT rave_breaklayoutline.id, rave_breaklayoutline.break_time,"
        <<" rave_breaklayoutline.break_hour,  rave_breaklayoutline.duration, "
        <<" rave_breaklayoutline.max_spots, rave_breaklayoutline.break_fill_method, "
        << " rave_breaklayout.week_days, rave_breaklayout.tvprogram_id "
        << " FROM rave_breaklayout, rave_breaklayoutline "
        << " WHERE rave_breaklayout.id = rave_breaklayoutline.break_layout_id "
        << "  AND rave_breaklayout.tvprogram_id in "+ids+""
        << " ORDER BY rave_breaklayout.tvprogram_id,  rave_breaklayoutline.break_time";


    EntityDataModel  edm;
    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();

    if (provider->cacheSize() == 0)
        return all_breaks;

    provider->cache()->first();
    do {
        auto itb = provider->cache()->currentElement()->begin();
        auto ite = provider->cache()->currentElement()->end();

        ProgramBreak pb;

        for(; itb != ite; ++itb)
        {
            std::string field_name = (*itb).first;
            std::string field_value = (*itb).second;

            if (field_name == "id")
                pb.line_id = to_int(field_value);

            if (field_name == "break_time")
                pb.break_time = QString::fromStdString(field_value);

            if (field_name == "duration")
                pb.duration = to_int(field_value);

            if (field_name == "max_spots")
                pb.max_spots = to_int(field_value);

            if (field_name == "tvprogram_id")
                pb.progid = to_int(field_value);
        }

        all_breaks.push_back(pb);

        provider->cache()->next();

    } while (!provider->cache()->isLast());

    return all_breaks;

}

void BookingWizard::assign_breaks_to_programs(const std::vector<ProgramBreak>& breaks,
                                              std::map<progid, Program>& programs)
{
    auto prog_ids = get_program_ids(programs);

    for(int pid: prog_ids)
    {
        auto program_breaks = breaks | std::views::filter([&](const ProgramBreak& pb){
                               return pb.progid == pid;
                           });

        programs[pid].breaks.assign(program_breaks.begin(), program_breaks.end());

    }

}

void BookingWizard::time_band_selected2(QTableWidgetItem* twi)
{

// int id = twi->data(Qt::UserRole).toInt();
// qDebug() << "ID: "<< id;

    m_daypart_grid->clear_all_cells();

    QItemSelectionModel* ism = ui->twTB->selectionModel();
    QModelIndexList mil = ism->selectedIndexes();

    if (mil.size() == 0)
        return;

     for(auto& index : mil){

        m_engine_data.current_time_band = dynamic_cast<TimeBand*>(std::get<1>(*(m_timeband_EDM->vecBegin()+index.row())).get());
        populate_grid(m_engine_data.current_time_band);
        m_engine_data.target_daypart =  m_daypart_grid->daypart_to_hours(m_daypart_grid->read_grid());

    }

    //using DaypartExt = std::map<int, std::tuple<std::string, std::vector<int>>>;
    for (auto& [day, tup] : m_engine_data.target_daypart) {
        qDebug() << " Day: "<< day;
        auto& [dp, hrs] = tup;
        qDebug() << QString::fromStdString(dp) << " : " ;
            for (auto& h : hrs) {
            qDebug() << h;
        }

    }

}

void BookingWizard::spot_details(int spot_id)
{
    auto spot_edm = std::make_unique<EntityDataModel>(std::make_unique<TRAFFIK::Spot>());
    spot_edm->getById({"id", "=", spot_id});

    std::shared_ptr<TRAFFIK::Spot> spot = std::dynamic_pointer_cast<TRAFFIK::Spot>(spot_edm->getEntity());

    auto client_edm = std::make_unique<EntityDataModel>(std::make_unique<Client>());
    client_edm->getById({"id", "=", spot->client()->value()});

    std::shared_ptr<Client> client = std::dynamic_pointer_cast<Client>(client_edm->getEntity());

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

std::set<int> BookingWizard::select_unique_hours_from_breaks()
{

    std::set<int> unique_hours;

    for (auto& entity_record : m_engine_data.m_schedule_EDM->modelEntities())
    {
        auto& [name, entity] = entity_record;

        auto a_break = dynamic_cast<Break*>(entity.get());

        unique_hours.insert(a_break->schedule_hour()->value());

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

    //ui->cbTimeband->setCurrentIndex(1);

    auto uniq_hours = selected_unique_hours();

    m_engine_data.break_count = fetch_breaks_from_db(QDate(2020,9,9), QDate(2020,9,30), uniq_hours);

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

std::vector<int> BookingWizard::get_hourly_distribution()
{
    std::vector<int>hr_dist;

    for(int i=0; i < ui->lwHourSegments->count(); ++i) {

        auto lwi = ui->lwHourSegments->item(i);

        if (lwi->checkState() == Qt::Checked) {
            int interval = lwi->data(Qt::UserRole).toInt();
            hr_dist.push_back(interval);
        }
    }
    return hr_dist;
}

int BookingWizard::pick_random_interval()
{
    int index = (rand() % ui->lwHourSegments->count());

    auto lwi = ui->lwHourSegments->item(index);

    int interval = lwi->data(Qt::UserRole).toInt();

    return interval;
}

void BookingWizard::auto_select_breaks(const AllBreaks& ab )
{
    int pending_spots = m_order->spotsOrdered()->value() - m_order->spotsBooked()->value();

    auto hours = std::views::iota(ab.from_hour, ab.to_hour+1);

    auto is_interval_selected = [](int interval, std::vector<int> intervals) ->bool {
        return ( (std::find(intervals.begin(), intervals.end(), interval) != intervals.end()) ? true : false);
    };

    int current_hour = -1;
    int interval = -1;

    for (int i=0; i < ui->twBreakSelect->rowCount(); ++i)
    {
        if (pending_spots == 0)
            break;

        auto time_str = ui->twBreakSelect->item(i, 1)->text();

        auto hr_str_list = time_str.split(":");

        int hr = hr_str_list[0].toInt();
        int min = hr_str_list[1].toInt();

        auto it = std::ranges::find(hours, hr);
        if (it != hours.end())
        {
            if (ab.hourly_intervals.size() == 0)
            {
                // No interval was selected, pick a random one.
                if (current_hour != hr)
                    interval = pick_random_interval();

                if (min == interval) {
                    ui->twBreakSelect->selectRow(i);
                    --pending_spots;
                }

            } else {
                // User selected intervals
                bool is_selected = is_interval_selected(min, ab.hourly_intervals);
                if (is_selected) {
                    ui->twBreakSelect->selectRow(i);
                    --pending_spots;
                }

            }
        }

        current_hour = hr;

    }
}


bool BookingWizard::spot_has_audio(const TRAFFIK::Spot* spot)
{
    auto edm = EntityDataModel(
               std::make_shared<TRAFFIK::SpotAudio>());

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

    for(auto& [name, entity] : engine_data.m_schedule_EDM->modelEntities()){
        schedule_ids += std::to_string(entity->id());
        if ( i<engine_data.m_schedule_EDM->modelEntities().size()-1)
            schedule_ids += ",";
        ++i;
    }

    schedule_ids = "("+schedule_ids+")";

    std::stringstream sql;
    sql << "SELECT a.id AS booking_id, a.schedule_id, a.spot_id, a.bookingsegment_id, a.book_seq, "
        << " b.name, b.spot_duration, b.real_duration, b.brand_id, b.client_id, "
        << " b.daypart1 AS spotDP1, b.daypart2 AS spotDP2, b.daypart3 AS spotDP3, "
        << " b.daypart4 AS spotDP4, b.daypart5 AS spotDP5, b.daypart6 AS spotDP6,b.daypart7 AS spotDP7, "
        << " e.daypart1 AS typeDP1, e.daypart2 AS typeDP2, e.daypart3 AS typeDP3, "
        << " e.daypart4 AS typeDP4, e.daypart5 AS typeDP5, e.daypart6 AS typeDP6,e.daypart7 AS typeDP7, "
        << " f.daypart1 AS voDP1, f.daypart2 AS voDP2, f.daypart3 AS voDP3, "
        << " f.daypart4 AS voDP4, f.daypart5 AS voDP5, f.daypart6 AS voDP6, f.daypart7 AS voDP7, "
        << " c.detail_id AS spot_excl, d.detail_id AS spot_vo, "
        << " g.audio_id AS audio_id "
        << " FROM rave_orderbooking a "
        << " left join rave_spot b on a.spot_id = b.id "
        << " left join rave_spottypeexclusion c on a.spot_id = c.parent_id "
        << " left join rave_spotvoiceover d on a.spot_id = d.parent_id "
        << " left join rave_typeexclusion e on c.detail_id = e.id "
        << " left join rave_voiceover f on d.detail_id = f.id "
        << " left join rave_spotaudio g on b.id = g.spot_id "
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
            int sd = 1;  // spot daypart postfix
            int td = 1;  // type excl daypart postfix
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
                if (field == "book_seq")
                    br.book_seq = to_int(value);

                if (field == "brand_id")
                    br.booked_spot.brand_id = to_int(value);
                if (field == "client_id")
                    br.booked_spot.client_id = to_int(value);
                if (field == "spot_excl")
                    type_excl_id = to_int(value);
                if (field == "spot_vo")
                    vo_excl_id = to_int(value);

                if (field == "audio_id")
                    br.booked_spot.audio_id = to_int(value);

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

void BookingWizard::initializePage(int currentId)
{

    qDebug() << "Initialize Page: "<< currentId;
    if (currentId == 3){
        QAbstractButton* next_btn = button(QWizard::NextButton);
        if (next_btn){
            qDebug() << "* Disable Next Button *";
            next_btn->setDisabled(true);
        }
    }

}


bool BookingWizard::validateCurrentPage()
{
    switch (currentId())
    {
        case BookingWizard::Page_Spots:
           // {
           //  TRAFFIK::Spot* sel_spot = selected_spot();
           //  bool has_audio = spot_has_audio(sel_spot);
           //  if (!has_audio){
           //      showMessage("Selected spot has no audio!");
           //      return false;
           //  }
           //  }
            qDebug() << " First page ..";
            break;
        case BookingWizard::Page_Dates:
            if (ui->edtStartDate->date() < QDate::currentDate())
            {
                showMessage("Bookings for past dates not allowed!");
                return false;

            }

            if (ui->edtStartDate->date() > ui->edtEndDate->date())
            {
                showMessage("Start date greater than End date");
                return false;
            }

            {
                auto rows = ui->tvBreaks->selectionModel()->selectedRows();
                qDebug() << "Selected Index: " << rows.count();
            }

            if (ui->rbAllBreaks->isChecked()) {

                int from_hour = ui->cbFromHour->currentData().toInt();
                int to_hour = ui->cbToHour->currentData().toInt();

                if (from_hour > to_hour) {
                    showMessage("`From` hour should be greater then `To` hour");
                    return false;
                }

            }

            break;

        case BookingWizard::Page_Rules:
        {
            cache_break_rules();
            qDebug() << "Page_Rules...";

            auto curr_page = currentPage();
            button(QWizard::NextButton)->setEnabled(false);

            break;
        }

        case BookingWizard::Page_Build_Breaks:
        {
            add_days_of_week();
            show_breaks_for_current_timeband();
            // Check the type of break selection
            // rbAllBreaks
            // rbTimeband
            // rbManualTime
            if (ui->rbAllBreaks->isChecked()) {
                // distribute_spot_to_all_breaks()  // Distribute selected spot to all the breaks (one spot per break)
            }

            if (ui->rbTimeband->isChecked()) {
                // distribute_spot_to_selected_timeband() // Distribute spots to selected timebands - based on the "Dist" value
            }

            if (ui->rbManualTime->isChecked()) {
                // distribute_spot_to_selected_breaks()
            }

            break;
        }
        case BookingWizard::Page_Select_By_Day:
        {
            auto_select_breaks_by_dow();
            qDebug() << "Validating selection by day of the WEEK *";
            break;
        }
        case BookingWizard::Page_Select_By_Date:
        {
            TRAFFIK::TraffikTreeViewModel* tvm  = new TRAFFIK::TraffikTreeViewModel(ui->twBreakSelect->selectedItems());
            ui->tvSummary->setModel(tvm);
            break;
        }
        case BookingWizard::Page_Final:
            return make_booking();
            //break;

    }

    return true;
}

void BookingWizard::distribute_spot_to_available_breaks()
{
    /*
    for (auto& [name, entity] : m_engine_data.m_schedule_EDM->modelEntities()())
    {

    }
    */

}


TRAFFIK::Spot* BookingWizard::selected_spot()
{

    TRAFFIK::Spot* spot{nullptr};

    QVariant q_col_name = ui->tvSpots->model()->data(
                            ui->tvSpots->model()->index(
                                    ui->tvSpots->currentIndex().row(), 0));


    std::string spot_name = q_col_name.toString().toStdString();

    if (!spot_name.empty()){
        std::shared_ptr<BaseEntity> be = m_spot_EDM->findEntityByName(spot_name);
        if (be != nullptr){
            spot = dynamic_cast<TRAFFIK::Spot*>(be.get());
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

    std::cout << sql.str() << '\n';

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

int BookingWizard::get_spot_audio(int spot_id)
{
    std::stringstream sql;
    sql << "SELECT audio_id "
        << " FROM rave_spotaudio "
        << " WHERE rave_spotaudio.spot_id = "+std::to_string(spot_id);

    EntityDataModel edm;
    edm.readRaw(sql.str());

    int audio_id {-1};

    auto provider = edm.getDBManager()->provider();
    if (provider->cacheSize() > 0) {
        provider->cache()->first();

        std::string value;
        bool audio_found = false;

        do {
            auto itB = provider->cache()->currentElement()->begin();
            auto itE = provider->cache()->currentElement()->end();

            for (; itB != itE; ++itB) {
                if ((*itB).first == "audio_id") {
                    audio_id = std::stoi((*itB).second);
                    audio_found = true;
                    break;
                }

                if (audio_found)
                    break;
            }

            provider->cache()->next();
        } while (!provider->cache()->isLast());
    }

    return audio_id;

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
    TRAFFIK::FullBreakRule::enable_or_disable(m_toggle_break_duration->isChecked());
    TRAFFIK::TypeExclusionRule::enable_or_disable(m_toggle_type_ex->isChecked());
    TRAFFIK::VoiceExclusionRule::enable_or_disable(m_toggle_voice_ex->isChecked());
    TRAFFIK::TypeDaypartRule::enable_or_disable(m_toggle_type_daypart->isChecked());
    TRAFFIK::VoiceDaypartRule::enable_or_disable(m_toggle_voice_daypart->isChecked());
    TRAFFIK::SpotDaypartRule::enable_or_disable(m_toggle_spot_daypart->isChecked());
    TRAFFIK::SameClientRule::enable_or_disable(m_toggle_same_client->isChecked());
    TRAFFIK::OverrideSameClientRule::enable_or_disable(m_toggle_override->isChecked());

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
    QPixmap pm_breaks(":/images/media/icons/error_mark.png");

    ui->lblBreaksChecked->setText(
                stoq(std::to_string(m_engine_data.break_count)));

    if (TRAFFIK::FullBreakRule::failed_break_count() > 0)
    {
        ui->lblFullBreaks->setText(
                    stoq(std::to_string(TRAFFIK::FullBreakRule::failed_break_count())));
        ui->imgFullBreaks->setPixmap(pm_breaks);
        color_label(ui->lblFullBreaks, Qt::red);
    }

    if (TRAFFIK::TypeExclusionRule::failed_break_count() > 0)
    {
        ui->lblTypeExclusion->setText(
                    stoq(std::to_string(TRAFFIK::TypeExclusionRule::failed_break_count())));
        ui->imgTypeEx->setPixmap(pm_breaks);
        color_label(ui->lblTypeExclusion, Qt::red);
    }

    if (TRAFFIK::VoiceExclusionRule::failed_break_count() > 0)
    {
        ui->lblVoiceExclusion->setText(
                    stoq(std::to_string(TRAFFIK::VoiceExclusionRule::failed_break_count())));
        ui->imgVoiceEx->setPixmap(pm_breaks);
        color_label(ui->lblVoiceExclusion, Qt::red);
    }

    if (TRAFFIK::TypeDaypartRule::failed_break_count() > 0)
    {
        ui->lblTypeDaypart->setText(
                    stoq(std::to_string(TRAFFIK::TypeDaypartRule::failed_break_count())));
        ui->imgTypeDaypart->setPixmap(pm_breaks);
        color_label(ui->lblTypeDaypart, Qt::red);
    }

    if (TRAFFIK::VoiceDaypartRule::failed_break_count() > 0)
    {
        ui->lblVoiceDaypart->setText(
                    stoq(std::to_string(TRAFFIK::VoiceDaypartRule::failed_break_count())));
        ui->imgVoiceDaypart->setPixmap(pm_breaks);
        color_label(ui->lblVoiceDaypart, Qt::red);
    }

    if (TRAFFIK::SpotDaypartRule::failed_break_count() > 0)
    {
        ui->lblSpotDaypart->setText(
                    stoq(std::to_string(TRAFFIK::SpotDaypartRule::failed_break_count())));
        ui->imgSpotDaypart->setPixmap(pm_breaks);
        color_label(ui->lblSpotDaypart, Qt::red);
    }

    if (TRAFFIK::SameClientRule::failed_break_count() > 0)
    {
        ui->lblNoSameClient->setText(
                    stoq(std::to_string(TRAFFIK::SameClientRule::failed_break_count())));
        ui->imgSameClient->setPixmap(pm_breaks);
        color_label(ui->lblNoSameClient, Qt::red);
    }

    if(TRAFFIK::OverrideSameClientRule::failed_break_count() > 0)
    {
        ui->lblSameClientDiffBrands->setText(
            stoq(std::to_string(TRAFFIK::OverrideSameClientRule::failed_break_count()))+"+" );
            ui->imgDiffBrand->setPixmap(pm_breaks);
            color_label(ui->lblSameClientDiffBrands, Qt::red);
    }

    if (m_engine_data.available_breaks > 0)
    {
        color_label(ui->lblTotalBreaks, Qt::darkGreen);
    }

    ui->lblTotalBreaks->setText(QString::number(m_engine_data.available_breaks));
    ui->lblAvilBreaks->setText(QString::number(m_engine_data.available_breaks));
}

void BookingWizard::color_label(QLabel* label, Qt::GlobalColor c)
{
    QColor color(c);
    QPalette palette = label->palette();
    palette.setColor(QPalette::WindowText, color);
    label->setPalette(palette);
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

void BookingWizard::read_break_rules_cache()
{
    QJsonObject data;
    data.insert("all", false);
    data.insert("remember", false);
    QJsonObject rules;
    rules.insert("break_duration", false);
    rules.insert("type_ex", false);
    rules.insert("voice_ex", false);
    rules.insert("type_daypart", false);
    rules.insert("voice_daypart", false);
    rules.insert("spot_daypart", false);
    rules.insert("same_client", false);
    rules.insert("override", false);
    data.insert("rules", rules);

    QString file = QDir::currentPath()+"/"+rules_cache_file;
    m_cache_handler = std::make_unique<JsonCacheHandler>(file, data);

    m_rules_cache = m_cache_handler->get_cache();

    toggle_rule_buttons(m_rules_cache);
}

void BookingWizard::toggle_rule_buttons(QJsonObject rule_cache)
{
    bool remember_rules = rule_cache["remember"].toBool();
    m_remember_rules->setChecked(remember_rules);
    if (remember_rules)
    {
        m_toggle_all->setChecked(rule_cache["all"].toBool());
        auto rules = rule_cache["rules"].toObject();
        m_toggle_break_duration->setChecked(rules["break_duration"].toBool());
        m_toggle_type_ex->setChecked(rules["type_ex"].toBool());
        m_toggle_voice_ex->setChecked(rules["voice_ex"].toBool());
        m_toggle_type_daypart->setChecked(rules["type_daypart"].toBool());
        m_toggle_voice_daypart->setChecked(rules["voice_daypart"].toBool());
        m_toggle_spot_daypart->setChecked(rules["spot_daypart"].toBool());
        m_toggle_same_client->setChecked(rules["same_client"].toBool());
        m_toggle_override->setChecked(rules["override"].toBool());
    }

}

void BookingWizard::cache_break_rules()
{

    m_rules_cache["remember"] = m_remember_rules->isChecked();
    m_rules_cache["all"] = m_toggle_all->isChecked();
    QJsonObject data = m_rules_cache["rules"].toObject();

    data["break_duration"] = m_toggle_break_duration->isChecked();
    data["type_ex"] = m_toggle_type_ex->isChecked();
    data["voice_ex"] = m_toggle_voice_ex->isChecked();
    data["type_daypart"] =  m_toggle_type_daypart->isChecked();
    data["voice_daypart"] = m_toggle_voice_daypart->isChecked();
    data["spot_daypart"] = m_toggle_spot_daypart->isChecked();
    data["same_client"] = m_toggle_same_client->isChecked();
    data["override"] = m_toggle_override->isChecked();

    m_rules_cache["rules"] = data;

    // QJsonDocument jdoc(m_rules_cache);
    // QString jstring =  jdoc.toJson(QJsonDocument::Compact);

    m_cache_handler->write_cache(m_rules_cache);

}


