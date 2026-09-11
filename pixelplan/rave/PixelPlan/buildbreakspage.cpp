#include <tuple>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QPushButton>

#include "../../../rave/utils/tools.h"
#include "../../../rave/framework/entitydatamodel.h"
#include "../../../rave/framework/ravenexception.h"
#include "../../../rave/framework/ravensetup.h"

#include "advertmedia.h"
#include "tvprogram.h"
#include "spot.h"
#include "schedule.h"
#include "buildbreakspage.h"
#include "orderbookingwizard.h"

namespace PIXELPLAN
{

BuildBreaksPage::BuildBreaksPage(OrderBookingWizard* wiz)
    :m_wizard(wiz)
{
    setTitle("Build Breaks");
    setSubTitle("Find breaks for order booking");
    setup_ui();

    m_engine_data = std::make_shared<PIXELPLAN::EngineData>();
    m_rule_engine = std::make_shared<PIXELPLAN::RuleEngine>(*m_engine_data);

    m_wizard->booking_data.m_engine_data = m_engine_data;
}

void BuildBreaksPage::setup_ui()
{
    init_collisions_data();

    QVBoxLayout* main_layout = new QVBoxLayout();

    QGridLayout* gl_breaks = new QGridLayout();

    int COLUMN_WIDTH = 200;

    // --- Heading
    QLabel* lbl_bchecked = new QLabel("Breaks Checked");
    QFont font  = lbl_bchecked->font();
    font.setBold(true);
    lbl_bchecked->setFont(font);
    lbl_bchecked->setMinimumWidth(COLUMN_WIDTH);

    m_lbl_bchecked_value = new QLabel("0");
    QFont bc_font = m_lbl_bchecked_value->font();
    bc_font.setBold(true);
    m_lbl_bchecked_value->setFont(bc_font);

    int ROW = 0;

    gl_breaks->addWidget(new QLabel(""), ROW, 0);
    gl_breaks->setColumnMinimumWidth(0, COLUMN_WIDTH);

    gl_breaks->addWidget(lbl_bchecked, ROW, 1);
    gl_breaks->setColumnMinimumWidth(1, COLUMN_WIDTH);

    gl_breaks->addWidget(m_lbl_bchecked_value, ROW, 2);
    gl_breaks->setColumnMinimumWidth(2, COLUMN_WIDTH);

    gl_breaks->setColumnStretch(3, 1);

    ROW++;

    // -- Header line in Row 1
    gl_breaks->addWidget(new QLabel(""), ROW, 0);
    gl_breaks->addWidget(horizontal_line(this),ROW, 1);
    gl_breaks->setColumnMinimumWidth(1, COLUMN_WIDTH);
    gl_breaks->addWidget(horizontal_line(this), ROW, 2);

    ROW++;

    // --- Rules Collisions
    //for (int row=0; row <= collisions.size()-1; ++row ) {
    for (auto& collision : m_collisions ) {

        //QLabel* lbl = new QLabel(collisions.at(row));

        QLabel* lbl = new QLabel(collision.desc);
        QLabel* lbl_val = new QLabel("0");
        lbl_val->setObjectName(collision.name);
        QLabel* lbl_img = new QLabel();
        lbl_img->setObjectName(QString("img_%1").arg(collision.name));
        collision.label = lbl_val;
        collision.image_label = lbl_img;

        gl_breaks->addWidget(new QLabel(""), ROW, 0);
        gl_breaks->setColumnMinimumWidth(0, COLUMN_WIDTH);

        gl_breaks->addWidget(lbl, ROW, 1);
        gl_breaks->setColumnMinimumWidth(1, COLUMN_WIDTH);

        gl_breaks->addWidget(lbl_val, ROW, 2);
        gl_breaks->setColumnMinimumWidth(2, COLUMN_WIDTH);

        gl_breaks->setColumnStretch(3, 1);

        gl_breaks->setRowMinimumHeight(ROW, 30);

        ROW++;

    }

    ROW++;

    gl_breaks->addWidget(new QLabel(""), ROW, 0);
    gl_breaks->addWidget(horizontal_line(this),ROW, 1);
    gl_breaks->setColumnMinimumWidth(1, COLUMN_WIDTH);
    gl_breaks->addWidget(horizontal_line(this), ROW, 2);

    ROW++;

    QLabel* lbl_total_cap = new QLabel("Total Breaks Available:");
    QFont t_font = lbl_total_cap->font();
    t_font.setBold(true);
    lbl_total_cap->setFont(t_font);

    m_lbl_total_val = new QLabel("0");

    gl_breaks->addWidget(new QLabel(""), ROW, 0);
    gl_breaks->addWidget(lbl_total_cap, ROW, 1);
    gl_breaks->addWidget(m_lbl_total_val, ROW, 2);

    ROW++;

    gl_breaks->addWidget(new QLabel(""), ROW, 0);
    gl_breaks->addWidget(horizontal_line(this),ROW, 1);
    gl_breaks->setColumnMinimumWidth(1, COLUMN_WIDTH);
    gl_breaks->addWidget(horizontal_line(this), ROW, 2);

    // Build breaks button

    ROW++;

    QPushButton* btn_build_breaks = new QPushButton("Build Breaks");
    connect(btn_build_breaks, &QPushButton::clicked, this, &BuildBreaksPage::on_build_breaks);

    gl_breaks->addWidget(new QLabel(""), ROW, 0);
    gl_breaks->setColumnMinimumWidth(0, COLUMN_WIDTH);
    gl_breaks->addWidget(btn_build_breaks, ROW, 1);
    gl_breaks->addWidget(new QLabel(""), ROW, 2);

    main_layout->addLayout(gl_breaks);

    setLayout(main_layout);

}

void BuildBreaksPage::on_build_breaks()
{
    reset_values();

    m_wizard->init_rules_state();

    m_engine_data->spot_to_book.spot_id = m_wizard->booking_data.spot->id();
    m_engine_data->spot_to_book.client_id = m_wizard->booking_data.spot->client()->value();
    m_engine_data->spot_to_book.brand_id = m_wizard->booking_data.spot->brand()->value();
    m_engine_data->spot_to_book.spot_name = m_wizard->booking_data.spot->name()->value();
    m_engine_data->spot_to_book.spot_duration = m_wizard->booking_data.spot->spot_duration()->value();
    m_engine_data->spot_to_book.real_duration = m_wizard->booking_data.spot->real_duration()->value();

    m_engine_data->spot_to_book.spot_daypart = fetch_spot_daypart(*m_wizard->booking_data.spot);

    m_engine_data->spot_to_book.audio_id = get_spot_media(m_wizard->booking_data.spot->id());

    fetch_type_exclusions(*m_engine_data);

    fetch_voice_exclusions(*m_engine_data);

    std::set<int> unique_hours;

    // if (ui->rbAllBreaks->isChecked())
    if (m_wizard->booking_data.is_all_breaks)
    {
        m_engine_data->break_count = fetch_breaks_from_db(
            m_wizard->booking_data.start_date,
            m_wizard->booking_data.end_date,
            unique_hours);
    }


    // auto selected_breaks = get_selected_program_breaks();

    m_engine_data->break_count = fetch_program_breaks_from_db(
        m_wizard->booking_data.start_date,
        m_wizard->booking_data.end_date,
        m_wizard->booking_data.selected_breaks);


    /*
    if (ui->rbTimeband->isChecked())
    {
        // Get hours from the selected timeband
        unique_hours = selected_unique_hours();
        m_engine_data.break_count = fetch_breaks_from_db(ui->edtStartDate->date(), ui->edtEndDate->date(), unique_hours);
    }
   */

    if (m_engine_data->break_count == 0)
    {
        showMessage("No Breaks for the selected date range!");
        //this->button(QWizard::NextButton)->setDisabled(true);
        return;
    }

    find_existing_bookings(*m_engine_data);

    m_engine_data->available_breaks = find_available_breaks();

    show_available_breaks();

    //setup_break_select_grid();

    /*
    if (ui->rbAllBreaks->isChecked())
    {
        AllBreaks all_breaks_sel;
        all_breaks_sel.from_hour = ui->cbFromHour->currentData().toInt();
        all_breaks_sel.to_hour = ui->cbToHour->currentData().toInt();
        all_breaks_sel.hourly_intervals = get_hourly_distribution();

        auto_select_breaks(all_breaks_sel);
    }
    */

}

void BuildBreaksPage::show_available_breaks()
{
    QPixmap pm_breaks(":/images/media/icons/error_mark.png");

    m_lbl_bchecked_value->setText(stoq(std::to_string(m_engine_data->break_count)));
    m_lbl_total_val->setText(QString::number(m_engine_data->available_breaks));

    for (auto& collision : m_collisions) {
        int bcount = collision.break_count();
        if (bcount > 0 ){
            collision.label->setText(stoq(std::to_string(bcount)));
            color_label(collision.label, Qt::red);
            collision.image_label->setPixmap(pm_breaks);
        }
    }

}

void BuildBreaksPage::reset_values()
{
    QString zero{"0"};

    for (auto& collision : m_collisions)
    {
        color_label(collision.label, Qt::black);
        collision.label->setText(zero);
    }
}

void BuildBreaksPage::init_collisions_data()
{
       Collision col1{
            "full_breaks",
            "Full Breaks:",
            PIXELPLAN::FullBreakRule::failed_break_count };

       Collision col2{
           "type_excl",
           "Type Exclusions Collisions:",
           PIXELPLAN::TypeExclusionRule::failed_break_count };

       Collision col3{
           "voice_excl",
           "Voice Exclusion Collisions:",
           PIXELPLAN::VoiceExclusionRule::failed_break_count};

       Collision col4{
           "type_daypart",
           "Type Daypart Collisions:",
            PIXELPLAN::TypeDaypartRule::failed_break_count};

       Collision col5{
           "voice_daypart",
           "Voice Daypart Collisions:",
           PIXELPLAN::VoiceDaypartRule::failed_break_count};

       Collision col6{
           "spot_daypart",
           "Spot Daypart Collisions:",
            PIXELPLAN::SpotDaypartRule::failed_break_count};

       Collision col7{
           "same_client",
           "No Same Client Collisions:",
            PIXELPLAN::SameClientRule::failed_break_count};

       Collision col8{
           "diff_brand",
           "Same Client Different Brands Collisions:",
           PIXELPLAN::OverrideSameClientRule::failed_break_count};

       m_collisions.push_back(col1);
       m_collisions.push_back(col2);
       m_collisions.push_back(col3);
       m_collisions.push_back(col4);
       m_collisions.push_back(col5);
       m_collisions.push_back(col6);
       m_collisions.push_back(col7);
       m_collisions.push_back(col8);
}

void BuildBreaksPage::fetch_type_exclusions(PIXELPLAN::EngineData& engine_data)
{
    std::stringstream sql;
    sql << "SELECT rave_typeexclusion.* "
       << " FROM rave_spottypeexclusion, rave_typeexclusion "
       << " WHERE rave_spottypeexclusion.detail_id = rave_typeexclusion.id "
       << " AND rave_spottypeexclusion.parent_id = "+std::to_string(engine_data.spot_to_book.spot_id);

    fetch_spot_exclusions(sql.str(),
                        engine_data.spot_to_book.type_exclusions,
                        engine_data.spot_to_book.type_ex_keys);
}

void BuildBreaksPage::fetch_voice_exclusions(PIXELPLAN::EngineData& engine_data)
{
    std::stringstream sql;
    sql << "SELECT rave_voiceover.* "
       << " FROM rave_spotvoiceover, rave_voiceover "
       << " WHERE rave_spotvoiceover.detail_id = rave_voiceover.id "
       << " AND rave_spotvoiceover.parent_id = "+std::to_string(engine_data.spot_to_book.spot_id);

    fetch_spot_exclusions(sql.str(),
                        engine_data.spot_to_book.voice_exclusions,
                        engine_data.spot_to_book.voice_ex_keys);
}

void BuildBreaksPage::fetch_spot_exclusions(const std::string query,
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

Daypart BuildBreaksPage::fetch_spot_daypart(TRAFFIK::Spot& spot)
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

std::size_t BuildBreaksPage::fetch_breaks_from_db(QDate start_date, QDate end_date, std::set<int> uniq_hours)
{
    if (m_engine_data->m_schedule_EDM->count() > 0)
    return m_engine_data->m_schedule_EDM->count();

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
                m_engine_data->m_schedule_EDM->search(m_engine_data->m_schedule_EDM->prepareFilter(date_range_filter, hours_filter));
            } catch(DatabaseException& de){
                showMessage(de.errorMessage());
        }
    } else {
        try {
            m_engine_data->m_schedule_EDM->search(m_engine_data->m_schedule_EDM->prepareFilter(date_range_filter));
        } catch (DatabaseException& de) {
            showMessage(de.errorMessage());
        }
    }

    return m_engine_data->m_schedule_EDM->count();
}

std::size_t BuildBreaksPage::fetch_program_breaks_from_db(QDate start_date, QDate end_date,
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

        m_engine_data->m_schedule_EDM->search( m_engine_data->m_schedule_EDM->prepareFilter(
                date_range_filter, hours_filter));
    }catch(DatabaseException& de) {
        showMessage(de.errorMessage());
    }


    return m_engine_data->m_schedule_EDM->count();
}




int BuildBreaksPage::get_spot_media(int spot_id)
{
    int audio_id {-1};

    auto edm = EntityDataModel(std::make_shared<PIXELPLAN::AdvertMedia>());

    auto am = std::make_shared<PIXELPLAN::AdvertMedia>();

    auto spot_filter = std::make_tuple(
        am->spot()->dbColumnName(),
        " = ",
        spot_id
        );

    std::string filter = edm.prepareFilter(spot_filter);

    edm.search(filter);

    if (edm.count() == 0)
        return -1;

    auto provider = edm.getDBManager()->provider();

    provider->cache()->first();

    bool audio_found = false;

    do {
        auto itB = provider->cache()->currentElement()->begin();
        auto itE = provider->cache()->currentElement()->end();

        for (; itB != itE; ++itB) {
            if ((*itB).first == "id") {
                audio_id = std::stoi((*itB).second);
                audio_found = true;
                break;
        }

        if (audio_found)
            break;
    }

      provider->cache()->next();

    } while (!provider->cache()->isLast());


    return audio_id;

}

void BuildBreaksPage::find_existing_bookings(PIXELPLAN::EngineData& engine_data)
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

            PIXELPLAN::BookingRecord br;
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



std::size_t BuildBreaksPage::find_available_breaks()
{
    return m_rule_engine->find_breaks();
}

std::vector<int> BuildBreaksPage::selected_hours(const std::string& dp_str)
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


void BuildBreaksPage::color_label(QLabel* label, Qt::GlobalColor c)
{
    QColor color(c);
    QPalette palette = label->palette();
    palette.setColor(QPalette::WindowText, color);
    label->setPalette(palette);
}

} // namespace