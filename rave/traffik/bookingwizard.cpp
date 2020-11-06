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
    mOrder{order},
    spotEDM{nullptr},
    timeBandEDM{nullptr},
    mDPG{nullptr},
    scheduleEDM{nullptr},
    bookingEDM{nullptr}
{
    ui->setupUi(this);

    populateSpotsTable(mOrder->client()->value());

    ui->tvSpots->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvSpots->setModel(spotEDM.get());

    timeBandEDM = new EntityDataModel(
                std::make_unique<TimeBand>());

    timeBandEDM->all();
    ui->cbTimeband->setModel(timeBandEDM);

    connect(ui->cbTimeband, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BookingWizard::timeBandChanged);

    connect(ui->rbAllBreaks, &QRadioButton::toggled, this, &BookingWizard::allBreaks);
    connect(ui->rbTimeband, &QRadioButton::toggled, this, &BookingWizard::toggleTimeBand);
    connect(ui->btnBuildBreaks, &QPushButton::clicked, this, &BookingWizard::buildBreaks);

    mDPG = std::make_unique<DayPartGrid>(ui->vlDaypart);

    timeBandChanged(0);
    ui->rbTimeband->toggle();

    ui->edtStartDate->setDate(order->startDate()->value());
    ui->edtEndDate->setDate(order->endDate()->value());

}

BookingWizard::~BookingWizard()
{
    delete ui;
}

void BookingWizard::populateSpotsTable(int client_id)
{

    spotEDM = std::make_unique<EntityDataModel>(
                std::make_unique<Spot>());

    auto spot = std::make_unique<Spot>();

    auto spotFilter = std::make_tuple(
                spot->client()->dbColumnName(),
                " = ",
                client_id);

    spotEDM->search(spotEDM->prepareFilter(spotFilter));

}

void BookingWizard::populateGrid(TimeBand* timeBand)
{

    std::map<std::string, std::string> dayparts;
    dayparts["daypart1"] = timeBand->daypart1()->valueToString();
    dayparts["daypart2"] = timeBand->daypart2()->valueToString();
    dayparts["daypart3"] = timeBand->daypart3()->valueToString();
    dayparts["daypart4"] = timeBand->daypart4()->valueToString();
    dayparts["daypart5"] = timeBand->daypart5()->valueToString();
    dayparts["daypart6"] = timeBand->daypart6()->valueToString();
    dayparts["daypart7"] = timeBand->daypart7()->valueToString();

    mDPG->updateGrid(dayparts);

}

std::size_t BookingWizard::fetchBreaks(QDate start_date, QDate end_date)
{
    scheduleEDM = std::make_unique<EntityDataModel>(
                std::make_unique<Schedule>());

    Schedule schedule;
    QString DATE_FORMAT = "yyyy-MM-dd";
    QString date_range = "'"+start_date.toString(DATE_FORMAT)+"' and '"+end_date.toString(DATE_FORMAT)+"'";


    auto date_range_filter = std::make_tuple(
                schedule.scheduleDate()->dbColumnName(),
                " between ",
                date_range.toStdString()
                );

    scheduleEDM->search(scheduleEDM->prepareFilter(date_range_filter));

    return scheduleEDM->count();
}

void BookingWizard::timeBandChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbTimeband->model());
    TimeBand* tband = dynamic_cast<TimeBand*>(std::get<1>(*(edm->vecBegin()+i)).get());
    populateGrid(tband);
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
    Spot* spot = selectedSpot();
    wizardData.current_spot.spot_id = spot->id();
    wizardData.current_spot.spot_name = spot->name()->value();
    wizardData.current_spot.spot_duration = spot->spotDuration()->value();
    wizardData.current_spot.real_duration = spot->realDuration()->value();

    fetchTypeExclusions(spot->id());


    wizardData.break_count = fetchBreaks(ui->edtStartDate->date(), ui->edtEndDate->date());

    if (wizardData.break_count == 0){
        showMessage("No Breaks for the selected data range!");
        return;
    }

    findExistingBookings();

    //print_exclusions(breakSum.existingBookings[0].spotRecord.type_exclusions);

    findAvailableBreaks();
}


void BookingWizard::findExistingBookings()
{
    std::string schedule_ids;
    std::size_t i = 0;
    for(auto& [name, entity] : scheduleEDM->modelEntities()){
        // get bookings for a given schedule line
        schedule_ids += std::to_string(entity->id());
        if ( i<scheduleEDM->modelEntities().size()-1)
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
        << "WHERE a.schedule_id in " + schedule_ids ;


    EntityDataModel edm;
    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();

    if (provider->cacheSize() > 0 ) {
        provider->cache()->first();

        do {
            auto itB = provider->cache()->currentElement()->begin();
            auto itE = provider->cache()->currentElement()->end();

            BookingRecord br;
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

                printstr(field);

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
                    br.spotRecord.spot_id = to_int(value);
                if (field == "name")
                    br.spotRecord.spot_name = value;

                if (field == "spot_duration"){
                    br.spotRecord.spot_duration = to_double(value);
                }

                if (field == "real_duration"){
                    br.spotRecord.real_duration = to_double(value);
                }

                if (field == "spotdp"+std::to_string(sd))
                    br.spotRecord.spot_daypart[sd++] = value;
                if (field == "typedp"+std::to_string(td))
                    typeDaypart[td++] = value;
                if (field == "vodp"+std::to_string(vdp))
                    voDaypart[vdp++] = value;
            }

            br.spotRecord.type_ex_keys.push_back(type_excl_id);
            br.spotRecord.type_exclusions.push_back(std::make_tuple(type_excl_id, typeDaypart));

            br.spotRecord.voice_ex_keys.push_back(vo_excl_id);
            br.spotRecord.voice_exclusions.push_back(std::make_tuple(vo_excl_id, voDaypart));

            wizardData.prev_bookings.push_back(br);

            provider->cache()->next();

        } while (!provider->cache()->isLast());

    }

}

int BookingWizard::findAvailableBreaks()
{
    /*
      - Breaks Checked
      - Full Breaks
      - Type Exclusion collusions
      - Voice Exclusion collusions
      - Type Daypart collusions
      - Voice Daypart collusions
      - Spot Daypart collusions
      - Same Client collusions
      - Override Same Client for Different brands **

      */

    for (auto& [name, entity] : scheduleEDM->modelEntities()){
        auto schedule = dynamic_cast<Schedule*>(entity.get());

        if (schedule->breakDurationLeft()->value() <
                wizardData.current_spot.spot_duration){
            wizardData.full_breaks += 1;

            log_failed_break(schedule, FailedBreakCode::BreakFull);

            continue;
        }



        for (auto& booking : wizardData.prev_bookings){

            if (booking.schedule_id == schedule->id()){

                if (ui->cbEnforceTypeExRule->isChecked()){

                    for (int& k : wizardData.current_spot.type_ex_keys){
                        if (std::find(booking.spotRecord.type_ex_keys.begin(),
                                                 booking.spotRecord.type_ex_keys.end(), k) !=
                                booking.spotRecord.type_ex_keys.end()) {

                            wizardData.type_excl_collusion += 1;
                            log_failed_break(schedule, FailedBreakCode::TypeExcl);
                        }
                    }

                }
            }


        }



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

Spot* BookingWizard::selectedSpot()
{
    Spot* spot{nullptr};

    QVariant q_col_name = ui->tvSpots->model()->data(
                            ui->tvSpots->model()->index(
                                    ui->tvSpots->currentIndex().row(), 0));

    std::string spot_name = q_col_name.toString().toStdString();

    if (!spot_name.empty()){
        BaseEntity* be = spotEDM->findEntityByName(spot_name);
        if (be != nullptr){
            spot = dynamic_cast<Spot*>(be);
        }
    }

    return spot;
}

void BookingWizard::fetchTypeExclusions(int spot_id)
{

    std::stringstream sql;
    sql << "SELECT rave_typeexclusion.* "
       << " FROM rave_spottypeexclusion, rave_typeexclusion "
       << " WHERE rave_spottypeexclusion.detail_id = rave_typeexclusion.id "
       << " AND rave_spottypeexclusion.parent_id = "+std::to_string(spot_id);

    fetchSpotExclusions(sql.str(),
                        wizardData.current_spot.type_exclusions,
                        wizardData.current_spot.type_ex_keys);
}

void BookingWizard::fetchVoiceExclusions(int spot_id)
{
    std::stringstream sql;
    sql << "SELECT rave_voiceover.* "
       << " FROM rave_spotvoiceover, rave_voiceover "
       << " WHERE rave_spotvoiceover.detail_id = rave_voiceover.id "
       << " AND rave_spotvoiceover.parent_id = "+std::to_string(spot_id);

    fetchSpotExclusions(sql.str(),
                        wizardData.current_spot.voice_exclusions,
                        wizardData.current_spot.voice_ex_keys);
}

void BookingWizard::fetchSpotExclusions(const std::string query, std::vector<Exclusion>& exclusions, std::list<int> keys)
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

void BookingWizard::print_exclusions(std::vector<BookingWizard::Exclusion>& exclusions)
{
    qDebug() << "printing ... ";
    for (auto& excl :  exclusions){
        auto& [key, dayparts] = excl;
        qDebug() << "ID: " << key;

        for (auto& [dow, daypart] : dayparts){
            qDebug() << "DOW: " << dow << ":" << stoq(daypart);

        }
    }
}

void BookingWizard::log_failed_break(Break* sched, FailedBreakCode fbc)
{
    wizardData.failed_breaks.push_back(
                std::make_tuple(sched->id(), fbc));
    sched->setSelected(0);

}

