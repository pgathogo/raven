#include <algorithm>
#include "bookingwizard.h"
#include "ui_bookingwizard.h"

#include "order.h"
#include "spot.h"
#include "../utils/daypartgrid.h"
#include "schedule.h"
#include "orderbooking.h"

BookingWizard::BookingWizard(Order* order, QWidget *parent) :
    QWizard(parent),
    ui(new Ui::BookingWizard),
    m_order{order},
    m_spot_EDM{nullptr},
    m_timeband_EDM{nullptr},
    m_daypart_grid{nullptr},
    m_schedule_EDM{nullptr},
    m_booking_EDM{nullptr}
{
    ui->setupUi(this);

    populate_spots_table(m_order->client()->value());

    ui->tvSpots->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvSpots->setModel(m_spot_EDM.get());

    m_timeband_EDM = new EntityDataModel(
                std::make_unique<TimeBand>());

    m_timeband_EDM->all();
    ui->cbTimeband->setModel(m_timeband_EDM);

    connect(ui->cbTimeband, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BookingWizard::timeBandChanged);

    connect(ui->rbAllBreaks, &QRadioButton::toggled, this, &BookingWizard::allBreaks);
    connect(ui->rbTimeband, &QRadioButton::toggled, this, &BookingWizard::toggleTimeBand);
    connect(ui->btnBuildBreaks, &QPushButton::clicked, this, &BookingWizard::buildBreaks);

    m_daypart_grid = std::make_unique<DayPartGrid>(ui->vlDaypart);

    timeBandChanged(0);
    ui->rbTimeband->toggle();

    ui->edtStartDate->setDate(order->startDate()->value());
    ui->edtEndDate->setDate(order->endDate()->value());

    m_rule_engine = std::make_unique<TRAFFIK::RuleEngine>(m_engine_data);
}

BookingWizard::~BookingWizard()
{
    delete ui;
}

void BookingWizard::populate_spots_table(int client_id)
{

    m_spot_EDM = std::make_unique<EntityDataModel>(
                std::make_unique<Spot>());

    auto spot = std::make_unique<Spot>();

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

std::size_t BookingWizard::fetch_breaks(QDate start_date, QDate end_date)
{
    m_schedule_EDM = std::make_unique<EntityDataModel>(
                std::make_unique<Schedule>());

    Schedule schedule;
    QString DATE_FORMAT = "yyyy-MM-dd";
    QString date_range = "'"+start_date.toString(DATE_FORMAT)+"' and '"+end_date.toString(DATE_FORMAT)+"'";

    auto date_range_filter = std::make_tuple(
                schedule.scheduleDate()->dbColumnName(),
                " between ",
                date_range.toStdString()
                );
    auto hours = m_daypart_grid->daypart_to_hours();

    std::size_t i = 0;
    std::string  hr_str;
    for (auto it=hours.begin(); it != hours.end(); ++it){
        hr_str += std::to_string(*it);
        if (i < hours.size()-1)
            hr_str += ",";
        ++i;
    }
    hr_str = "("+hr_str+")";

    auto hours_filter = std::make_tuple(
                schedule.scheduleHour()->dbColumnName(),
                " in ",
                hr_str);

    m_schedule_EDM->search(m_schedule_EDM->prepareFilter(date_range_filter, hours_filter));

    return m_schedule_EDM->count();
}

void BookingWizard::timeBandChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbTimeband->model());
    TimeBand* tband = dynamic_cast<TimeBand*>(std::get<1>(*(edm->vecBegin()+i)).get());
    populate_grid(tband);
    m_engine_data.target_daypart = m_daypart_grid->read_grid();
}

void BookingWizard::allBreaks(bool state)
{
     ui->cbTimeband->setEnabled(!state);
}

void BookingWizard::toggleTimeBand(bool state)
{
    ui->cbTimeband->setEnabled(state);
}

void BookingWizard::buildBreaks()
{

    init_rules_state();

    Spot* spot = selected_spot();
    m_engine_data.spot_to_book.spot_id = spot->id();
    m_engine_data.spot_to_book.spot_name = spot->name()->value();
    m_engine_data.spot_to_book.spot_duration = spot->spotDuration()->value();
    m_engine_data.spot_to_book.real_duration = spot->realDuration()->value();

    fetch_type_exclusions(spot->id());

    fetch_voice_exclusions(spot->id());

    m_engine_data.break_count = fetch_breaks(ui->edtStartDate->date(), ui->edtEndDate->date());

    if (m_engine_data.break_count == 0){
        showMessage("No Breaks for the selected data range!");
        return;
    }

    find_existing_bookings(m_engine_data);

    find_available_breaks();

    available_breaks_summary();
}

void BookingWizard::breakDuration(int state)
{

}


void BookingWizard::find_existing_bookings(TRAFFIK::EngineData& engine_data)
{
    std::string schedule_ids;
    std::size_t i = 0;
    for(auto& [name, entity] : m_schedule_EDM->modelEntities()){
        schedule_ids += std::to_string(entity->id());
        if ( i<m_schedule_EDM->modelEntities().size()-1)
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
                    br.brand_id = to_int(value);
                if (field == "client_id")
                    br.client_id = to_int(value);

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

                if (field == "spotdp"+std::to_string(sd))
                    br.booked_spot.spot_daypart[sd++] = value;
                if (field == "typedp"+std::to_string(td))
                    typeDaypart[td++] = value;
                if (field == "vodp"+std::to_string(vdp))
                    voDaypart[vdp++] = value;
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

int BookingWizard::find_available_breaks()
{

    m_engine_data.failed_breaks.clear();

    for (auto& [name, entity] : m_schedule_EDM->modelEntities()){

        auto schedule = dynamic_cast<Schedule*>(entity.get());

        m_rule_engine->run(schedule);
    }

    return 0;
}


bool BookingWizard::validateCurrentPage()
{
    switch (currentId()){
        case 0:
          return (ui->tvSpots->selectionModel()->selectedRows().size() > 0) ? true : false;
        case 1:
          break;
    }
}

Spot* BookingWizard::selected_spot()
{
    Spot* spot{nullptr};

    QVariant q_col_name = ui->tvSpots->model()->data(
                            ui->tvSpots->model()->index(
                                    ui->tvSpots->currentIndex().row(), 0));

    std::string spot_name = q_col_name.toString().toStdString();

    if (!spot_name.empty()){
        BaseEntity* be = m_spot_EDM->findEntityByName(spot_name);
        if (be != nullptr){
            spot = dynamic_cast<Spot*>(be);
        }
    }

    return spot;
}

void BookingWizard::fetch_type_exclusions(int spot_id)
{

    std::stringstream sql;
    sql << "SELECT rave_typeexclusion.* "
       << " FROM rave_spottypeexclusion, rave_typeexclusion "
       << " WHERE rave_spottypeexclusion.detail_id = rave_typeexclusion.id "
       << " AND rave_spottypeexclusion.parent_id = "+std::to_string(spot_id);

    fetch_spot_exclusions(sql.str(),
                        m_engine_data.spot_to_book.type_exclusions,
                        m_engine_data.spot_to_book.type_ex_keys);
}

void BookingWizard::fetch_voice_exclusions(int spot_id)
{
    std::stringstream sql;
    sql << "SELECT rave_voiceover.* "
       << " FROM rave_spotvoiceover, rave_voiceover "
       << " WHERE rave_spotvoiceover.detail_id = rave_voiceover.id "
       << " AND rave_spotvoiceover.parent_id = "+std::to_string(spot_id);

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
                    daypart[i++] = (*itB).second;
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
}

void BookingWizard::available_breaks_summary()
{
    QPixmap pm_breaks(":/images/icons/images/icons/error_mark.png");

    ui->lblBreaksChecked->setText(
                stoq(std::to_string(m_engine_data.break_count)));

    ui->lblFullBreaks->setText(
                stoq(std::to_string(TRAFFIK::FullBreakRule::failed_break_count())));
    if (TRAFFIK::FullBreakRule::failed_break_count() > 0)
        ui->imgFullBreaks->setPixmap(pm_breaks);

    ui->lblTypeExclusion->setText(
                stoq(std::to_string(TRAFFIK::TypeExclusionRule::failed_break_count())));

    if (TRAFFIK::TypeExclusionRule::failed_break_count() > 0)
        ui->imgTypeExcl->setPixmap(pm_breaks);

    ui->lblVoiceExclusion->setText(
                stoq(std::to_string(TRAFFIK::VoiceExclusionRule::failed_break_count())));

    if (TRAFFIK::VoiceExclusionRule::failed_break_count() > 0)
        ui->imgVoiceExcl->setPixmap(pm_breaks);


    ui->lblTotalBreaks->setText(
                stoq(std::to_string(m_engine_data.break_count-m_engine_data.failed_breaks.size())));
}


