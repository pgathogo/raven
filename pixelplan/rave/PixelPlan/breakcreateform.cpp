#include <sstream>
#include <ranges>
#include <format>
#include <tuple>
#include <thread>
#include <chrono>

#include <QAbstractItemModel>

#include "breakcreateform.h"
#include "ui_breakcreateform.h"

#include "../../../rave/framework/entitydatamodel.h"
#include "../../../rave/framework/choicefield.h"
#include "../../../rave/framework/ravenexception.h"
#include "../../../rave/framework/schedule.h"
#include "../../../rave/framework/datetimeselector.h"

#include "breaklayout.h"
#include "breaklayoutform.h"

struct BreakLineColumn {
    static constexpr int Title=0;
    static constexpr int BreakTime=1;
    static constexpr int Duration=2;
    static constexpr int MaxSpots=3;
    static constexpr int BreakFillMethod=4;
    static constexpr int Id=5;
};

BreakCreateForm::BreakCreateForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BreakCreateForm)
    , m_selected_breaklayout{nullptr}
{
    ui->setupUi(this);

    m_break_layout = std::make_shared<BreakLayout>();
    m_edm_break_layout = std::make_unique<EntityDataModel>(m_break_layout);

    ui->tvBreakLayouts->setModel(m_edm_break_layout.get());
    ui->tvBreakLayouts->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    fetch_break_layouts();

    make_progids_exclusion_list();

    connect(ui->tvBreakLayouts, &QTableView::clicked, this, &BreakCreateForm::break_layout_selected);

    m_edm_break_line = std::make_unique<EntityDataModel>(std::make_shared<BreakLayoutLine>());

    // ui->tvBreakLines->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvBreakLines->setModel(m_edm_break_line.get());

    ui->tvBreakLines->setColumnWidth(0, 300);

    connect(ui->btnCreate, &QPushButton::clicked, this, &BreakCreateForm::create_breaks);
    connect(ui->btnCancel, &QPushButton::clicked, this, &BreakCreateForm::close_form);

    connect(ui->btnAddHour, &QPushButton::clicked, this, &BreakCreateForm::add_hour);
    connect(ui->btnRemoveHour, &QPushButton::clicked, this, &BreakCreateForm::remove_hour);

    set_defaults();

    setup_ui();


}

void BreakCreateForm::fetch_break_layouts()
{
    m_edm_break_layout->clear();
    auto deleted_filter = std::make_tuple(m_break_layout->deleted()->dbColumnName(), "=", 0);
    std::string filter = m_edm_break_layout->prepareFilter(deleted_filter);
    m_edm_break_layout->search(filter);
}

BreakCreateForm::~BreakCreateForm()
{
    delete ui;
}

void BreakCreateForm::make_progids_exclusion_list()
{
    for(auto& [name, entity]: m_edm_break_layout->modelEntities()) {
        std::shared_ptr<BreakLayout> bl = std::dynamic_pointer_cast<BreakLayout>(entity);
        m_excluded_progids.push_back(bl->tvprogram()->value() );
    }
}

void BreakCreateForm::set_defaults()
{
    setWindowTitle("Commercial Breaks");
    ui->dtFrom->setDate(QDate::currentDate());
    ui->dtTo->setDate(QDate::currentDate());
    //populate_hour_combo();
    setMinimumSize(850, 450);
}

void BreakCreateForm::setup_ui()
{
    QPixmap add_pixmap(":/images/media/icons/add.png");
    QIcon add_icon(add_pixmap);
    ui->btnAdd->setIcon(add_icon);

    QPixmap edit_pixmap(":/images/media/icons/edit.png");
    QIcon edit_icon(edit_pixmap);
    ui->btnEdit->setIcon(edit_icon);

    QPixmap delete_pixmap(":/images/media/icons/delete.png");
    QIcon delete_icon(delete_pixmap);
    ui->btnDelete->setIcon(delete_icon);

    QPixmap create_pix(":/images/media/icons/schedule2.png");
    QIcon create_icon(create_pix);
    ui->btnCreate->setIcon(create_icon);

    connect(ui->btnAdd, &QToolButton::clicked, this, &BreakCreateForm::create_layout);
    connect(ui->btnEdit, &QToolButton::clicked, this, &BreakCreateForm::edit_layout);
    connect(ui->btnDelete, &QToolButton::clicked, this, &BreakCreateForm::delete_layout);

    connect(ui->cbAll, &QCheckBox::toggled, this, &BreakCreateForm::select_all_breaks);

    ui->lblSelHour->setVisible(false);
    ui->lblSelectedHr->setVisible(false);
    ui->btnAddHour->setVisible(false);
    ui->cbSelectedHours->setVisible(false);
    ui->btnRemoveHour->setVisible(false);

}

void BreakCreateForm::break_layout_selected(const QModelIndex &index)
{
    auto indexes = ui->tvBreakLayouts->selectionModel()->selectedRows();

    if (indexes.size() == 0)
        return;

    std::vector<int> bl_ids;

    std::shared_ptr<BreakLayout> bl;

    for(auto& index : indexes)
    {
        auto be = m_edm_break_layout->get_entity_at_row(index.row());
        if (be == nullptr)
            continue;

        bl =  dynamic_pointer_cast<BreakLayout>(be);
        bl_ids.push_back(bl->id());
    }

    m_selected_breaklayout = bl;

    std::string dow = m_selected_breaklayout->weekDays()->value();


    std::string ids = join<std::vector<int>>(bl_ids);
    ids = "("+ ids +")";

    BreakLayoutLine bbl;

    auto break_line_filter = std::make_tuple(bbl.breakLayout()->dbColumnName(), " in ", ids);

    std::string filter = m_edm_break_line->prepareFilter(break_line_filter);

    m_edm_break_line->search(filter);

    if (m_edm_break_line->count() > 0) {
        ui->cbAll->setChecked(false);
        ui->cbAll->setChecked(true);
    }


}

void BreakCreateForm::print_model_items()
{
    for(const auto& entity: m_edm_break_line->modelEntities())
    {
        auto bbl = std::dynamic_pointer_cast<BreakLayoutLine>(std::get<1>(entity));
        qDebug() << bbl->breakTime()->value().toString("HH:mm");
    }

}

void BreakCreateForm::close_form()
{
    done(m_breaks_created);
}

void BreakCreateForm::create_breaks()
{
    if (m_edm_break_line->count() == 0){
        showMessage("Select a break layout");
        return;
    }

    QString break_count = QString("%1 break(s) selected. Continue with break creation?")
                              .arg(ui->tvBreakLines->selectionModel()->selectedRows().count());

    auto reply = QMessageBox::question(this, "Create Breaks",
                                       break_count,
                                       QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
        return;

    if (ui->dtTo->date() < ui->dtFrom->date()){
        showMessage("`To` date less than `From` date!");
        return;
    }

    std::vector<std::shared_ptr<BreakLayoutLine>> selected_break_lines;
    for (int r=0; r < ui->tvBreakLines->selectionModel()->selectedRows().count(); ++r)
    {
        QModelIndex index = ui->tvBreakLines->selectionModel()->selectedRows().at(r);
        auto bbl = std::dynamic_pointer_cast<BreakLayoutLine>(m_edm_break_line->get_entity_at_row(index.row()));
        selected_break_lines.push_back(bbl);
    }

    std::string insert_statements = make_insert_statements(ui->dtFrom->date(), ui->dtTo->date(), selected_break_lines);

    //std::cout << insert_statements << '\n';

    if (insert_statements.empty()) {
         return;
    }

    if (insert_breaks_to_db(insert_statements))
        close_form();
}

void BreakCreateForm::get_existing_schedules(ScheduleRecords& s_recs, QDate from, QDate to)
{
    std::stringstream sql;

    sql << " SELECT schedule_date, schedule_hour, schedule_time "
        << " FROM rave_schedule, rave_breaklayoutline, rave_breaklayout "
        << " WHERE rave_schedule.break_layout_line_id = rave_breaklayoutline.id "
        << " AND rave_breaklayoutline.break_layout_id = rave_breaklayout.id "
        << " AND rave_breaklayout.deleted = 0 " ;

    std::string where_filter = std::format(" AND rave_schedule.schedule_date between '{}' and '{}' ",
                                           from.toString("yyyy-MM-dd").toStdString(),
                                           to.toString("yyyy-MM-dd").toStdString());

    std::string order_str = " Order by rave_schedule.schedule_date, rave_schedule.schedule_hour, rave_schedule.schedule_time ";

    sql << where_filter << order_str;


    EntityDataModel edm;
    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();
    if (provider->cacheSize() == 0)
        return;

    provider->cache()->first();
    do{
        auto itB = provider->cache()->currentElement()->begin();
        auto itE = provider->cache()->currentElement()->end();

        // Get fields
        QDate sched_date;
        int sched_hr;
        QTime sched_time;

        for (; itB != itE; ++itB)
        {
            std::string field = (*itB).first;
            std::string value = (*itB).second;

            if (field == "schedule_date"){
                sched_date = QDate::fromString(stoq(value),"yyyy-MM-dd");
            }

            if (field == "schedule_hour"){
                sched_hr = std::stoi(value);
            }

            if (field == "schedule_time"){
                sched_time = QTime::fromString(stoq(value), "hh:mm:ss");
            }
        }


        if ( s_recs.find(sched_date) == s_recs.end() ){
            s_recs[sched_date].insert(std::pair(sched_hr, std::vector<QString>()));
            s_recs[sched_date][sched_hr].push_back(sched_time.toString("hh:mm"));
        }else{
            s_recs[sched_date][sched_hr].push_back(sched_time.toString("hh:mm"));
        }


        provider->cache()->next();

    }while(!provider->cache()->isLast());


}

std::string BreakCreateForm::make_insert_statements(QDate from, QDate to, const std::vector<std::shared_ptr<BreakLayoutLine>>& selected_break_lines)
{
    Schedule sched;
    std::string insert_stmts = "";
    Vectored<Field> fields;

    ScheduleRecords s_recs;

    get_existing_schedules(s_recs, from, to);

    // s_recs = map[schedule_date][schedule_hour] = vector of schedule_time
    // For example:
    // s_recs[2024-07-01][10] = ["10:00", "10:30", "10:45"]


    /*
    for (auto& [dt, hours]: s_recs){
        qDebug() << "Date: "<< dt;
        for(auto& [hr, breaktime]: hours){
            qDebug() << "Hour: "<< hr;
            for (auto& bt : breaktime)
                qDebug() << bt;
        }
    }
   */


    auto break_exists = [&](QDate sched_date, int sched_hr, QTime sched_time)
    {

        if (s_recs.find(sched_date) == s_recs.end() ) {
            s_recs[sched_date].insert(std::pair(sched_hr, std::vector<QString>()));
            s_recs[sched_date][sched_hr].push_back(sched_time.toString("hh:mm"));
            return false;
        }


        if (s_recs[sched_date].find(sched_hr) == s_recs[sched_date].end() ) {
            s_recs[sched_date].insert(std::pair(sched_hr, std::vector<QString>()));
            s_recs[sched_date][sched_hr].push_back(sched_time.toString("hh:mm"));
            return false;
        }


        for(auto& break_time: s_recs[sched_date][sched_hr])
        {

            if (break_time == sched_time.toString("hh:mm"))
                return true;
        }

        return false;
    };

    auto dow_allowed = [&](int dow) {
        std::string weekdays = m_selected_breaklayout->weekDays()->value();
        std::string s(1, weekdays[dow-1]);
        return ( std::stoi(s) == 1) ? true : false;
    };

    QDate tmpDate = from;

    while (tmpDate <= to)
    {

        if(!dow_allowed(tmpDate.dayOfWeek())) {
            tmpDate = tmpDate.addDays(1);
            continue;
        }

        //for (auto& [name, entity] : m_edm_break_line->modelEntities())
        for (auto bll : selected_break_lines)
        {
            // BreakLayoutLine* bll = dynamic_cast<BreakLayoutLine*>(entity.get());

            fields << sched.set_schedule_date(tmpDate)
                    << sched.set_schedule_time(bll->breakTime()->value())
                    << sched.set_schedule_hour(bll->breakHour()->value())
                    << sched.set_break_duration(bll->duration()->value())
                    << sched.set_break_max_spots(bll->maxSpots()->value())
                    << sched.set_break_duration_left(bll->duration()->value())
                    << sched.set_booked_spots(0)
                    << sched.set_schedule_item_type("COMM-BREAK")
                    << sched.set_break_mode("MIXED")
                    << sched.set_break_fill_method(bll->break_fill_method()->value())
                    << sched.set_comment(bll->title()->value())
                    << sched.set_break_layout_line(bll->id());

            // Do not create break if it already exists
            if (!break_exists(tmpDate, bll->breakHour()->value(), bll->breakTime()->value())) {
                insert_stmts += sched.make_insert_stmt(fields.vec);

            }

            fields.clear();
         }

        tmpDate = tmpDate.addDays(1);
    }

    return insert_stmts;
}


bool BreakCreateForm::insert_breaks_to_db(const std::string insert_stmnts)
{
    try{

        m_edm_break_line->executeRawSQL(insert_stmnts);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(500ms);

        showMessage("Breaks created successfully.");
        m_breaks_created = true;
        return true;
    } catch (DatabaseException& de) {
        showMessage(de.errorMessage());
        return false;
    }
}

void BreakCreateForm::populate_hour_combo()
{
}

void BreakCreateForm::add_hour()
{
    std::unique_ptr<DateTimeSelector> dts = std::make_unique<DateTimeSelector>(this);

    if (dts->exec() == 1) {
        DateTimeSelection selection = dts->selection();
        for(int hour : selection.sel_hours)
            ui->cbSelectedHours->addItem(QString::number(hour));
    }

}

void BreakCreateForm::remove_hour()
{
    ui->cbSelectedHours->removeItem(ui->cbSelectedHours->currentIndex());
}

void BreakCreateForm::create_layout()
{
    auto break_layout = std::make_shared<BreakLayout>();

    std::shared_ptr<BreakLayoutForm> blform =
        std::make_shared<BreakLayoutForm>(break_layout.get(), m_excluded_progids, this);

    if (blform->exec() > 0)
    {
        int layout_id = m_edm_break_layout->createEntity(std::move(break_layout));
        save_break_layout_lines(blform, layout_id);
    }
}

void BreakCreateForm::save_break_layout_lines(std::shared_ptr<BreakLayoutForm> blf,
                                              int layout_header_id)
{
    auto edm = std::make_unique<EntityDataModel>();

    /*
    auto& break_lines = blf->breakLines();

    for(auto& break_line : break_lines){
        BreakLayoutLine* bll = dynamic_cast<BreakLayoutLine*>(std::get<1>(break_line).get());
        bll->setBreakLayout(layout_header_id);
        edm->createEntityDB(*bll);
    }
   */

    auto model = blf->breakline_model();
    int row_count = model->rowCount();
    int col_count = model->columnCount();

    std::map<QString, QString> fill_method;
    fill_method["Sequence"] = "S";
    fill_method["Random"] = "R";

    int hour = -1;
    for (int row=0; row < row_count; ++row)
    {
        std::unique_ptr<BreakLayoutLine> bll = std::make_unique<BreakLayoutLine>();

        for (int col=0; col < col_count; ++col)
        {
            auto index = model->index(row, col);

            switch (col)
            {
            case 0:
                qDebug() << model->data(index).toString();
                bll->set_title(model->data(index).toString().toStdString());
            case 1:
                bll->setBreakTime(model->data(index).toTime());
                hour = model->data(index).toTime().hour();
                break;
            case 2:
                bll->setDuration(model->data(index).toInt());
                break;
            case 3:
                bll->setMaxSpots(model->data(index).toInt());
                break;
            case 4:
                bll->set_break_fill_method(
                    fill_method[model->data(index).toString()].toStdString());
                break;
            }
        }

        bll->setBreakHour(hour);
        bll->setWeekDay(1);
        bll->setBreakLayout(layout_header_id);
        bll->set_deleted(0);

        edm->createEntity(std::move(bll));
    }

}

void BreakCreateForm::edit_layout()
{

    if (m_selected_breaklayout == nullptr)
        return;

    auto bl_form = std::make_shared<BreakLayoutForm>(m_selected_breaklayout.get(), std::vector<int>());

    if (bl_form->exec() > 0){

        auto model = bl_form->breakline_model();

        int row_count = model->rowCount();
        int col_count = model->columnCount();

        std::map<QString, QString> fill_method;
        fill_method["Sequence"] = "S";
        fill_method["Random"] = "R";

        EntityDataModel edm(std::make_shared<BreakLayoutLine>());

        for(int row=0; row < row_count; ++row)
        {
            BreakLayoutLine bll;
            for(int column=0; column < col_count; ++column)
            {
                auto index = model->index(row, column);
                QVariant data = model->data(index);

                switch(column)
                {
                case BreakLineColumn::Title:
                    bll.set_title(data.toString().toStdString());
                    break;
                case BreakLineColumn::BreakTime:
                    bll.setBreakTime(data.toTime());
                    break;
                case BreakLineColumn::Duration:
                    bll.setDuration(data.toInt());
                    break;
                case BreakLineColumn::MaxSpots:
                    bll.setMaxSpots(data.toInt());
                    break;
                case BreakLineColumn::BreakFillMethod:
                    bll.set_break_fill_method(fill_method[data.toString()].toStdString());
                    break;
                case BreakLineColumn::Id:
                    bll.setId(data.toInt());
                    break;
                }
            }

            bll.setBreakHour(bll.breakTime()->value().hour());
            bll.setWeekDay(1);
            bll.setBreakLayout(m_selected_breaklayout->id());
            bll.set_deleted(0);

            if (bll.id() == -1) {
                edm.createEntityDB(bll);
            } else {
                edm.updateEntity(bll);
                // Update schedule where break_layout_line_id = bll.id()
                update_schedule_for_break_line(bll);
            }

        }


    }

  }

void BreakCreateForm::delete_layout()
{
    if (!confirmationMessage("Delete layout?"))
        return;

    QVariant col_name{};
    int selected_row = -1;

    /*
    auto model_indexes = ui->tvBreakLayouts->selectionModel()->selectedIndexes();
    if (model_indexes.size() > 0){
        auto model_index = model_indexes[0];
        selected_row  = model_index.row();
        col_name = ui->tvBreakLayouts->model()->data(model_index);
    }

    if (selected_row == -1)
        return;

    std::string search_name = col_name.toString().toStdString();
    std::shared_ptr<BaseEntity> be = m_edm_break_layout->findEntityByName(search_name);
    if (be == nullptr)
        return;

    auto break_layout = dynamic_cast<BreakLayout*>(be.get());
    */

    auto rows = ui->tvBreakLayouts->selectionModel()->selectedRows();
    if (rows.count() == 0)
        return;

    int row = rows[0].row();

    auto be = m_edm_break_layout->get_entity_at_row(row);
    if (be == nullptr)
        return;

    std::shared_ptr<BreakLayout> break_layout = std::dynamic_pointer_cast<BreakLayout>(be);

    int break_layout_id = break_layout->id();

    auto set_delete_status_breaklayoutline = [break_layout_id](int deleted_flag) {

        EntityDataModel edm; //(std::make_shared<BreakLayoutLine>());
                std::string update_stmt = std::format("UPDATE rave_breaklayoutline SET deleted = {} WHERE break_layout_id = {}",
                                            deleted_flag, break_layout_id);
                try {
                    edm.executeRawSQL(update_stmt);
                } catch (DatabaseException& de) {
                    showMessage(de.errorMessage());
                    return false;
                }
                return true;
    };

    auto set_delete_status_breaklayout = [break_layout_id](int deleted_flag) {
        EntityDataModel edm; //(std::make_shared<BreakLayout>());
        std::string update_layout_stmt = std::format("UPDATE rave_breaklayout SET deleted = {} WHERE id = {}",
                                        deleted_flag, break_layout_id);
        try {
            edm.executeRawSQL(update_layout_stmt);
        } catch (DatabaseException& de) {
            showMessage(de.errorMessage());
            return false;
        }
        return true;
    };

    int DELETED_FLAG   = 1;
    int UNDELETED_FLAG = 0;

    bool breakline_updated = set_delete_status_breaklayoutline(DELETED_FLAG);

    if (!breakline_updated) {
        return;
    }

    bool layout_updated = set_delete_status_breaklayout(DELETED_FLAG);

    if (!layout_updated) {
        // Rollback breakline delete status
        set_delete_status_breaklayoutline(UNDELETED_FLAG);
        return;
    }

    fetch_break_layouts();

    // All is good, now we can remove the break layout from the view
    //m_edm_break_layout->removeEntity(*break_layout.get());


}

bool BreakCreateForm::update_schedule_for_break_line(BreakLayoutLine& bll)
{
  // For now we just update the comment - break title
   std::string  update1 = std::format("UPDATE rave_schedule set comment = '{}'", bll.title()->value());

   std::stringstream sql;

   sql << update1
       << " where break_layout_line_id = " << bll.id();

   EntityDataModel edm;
   try {
       edm.executeRawSQL(sql.str());
       return true;
   } catch (DatabaseException& de) {
       showMessage(de.errorMessage());
       return false;
   }

}

void BreakCreateForm::select_all_breaks()
{
    if (ui->cbAll->isChecked()) {
        ui->tvBreakLines->selectAll();
    } else {
        ui->tvBreakLines->clearSelection();
    }

}
