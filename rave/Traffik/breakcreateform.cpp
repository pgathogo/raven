#include <sstream>
#include <QAbstractItemModel>

#include "breakcreateform.h"
#include "ui_breakcreateform.h"

#include "../framework/entitydatamodel.h"
#include "../framework/choicefield.h"
#include "../framework/ravenexception.h"
#include "../framework/schedule.h"
#include "../framework/datetimeselector.h"

#include "breaklayout.h"
#include "breaklayoutline.h"
#include "breaklayoutform.h"

struct BreakLineColumn {
    static constexpr int BreakTime=0;
    static constexpr int Duration=1;
    static constexpr int MaxSpots=2;
    static constexpr int BreakFillMethod=3;
    static constexpr int Id=4;
};

BreakCreateForm::BreakCreateForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BreakCreateForm)
{
    ui->setupUi(this);

    m_break_layout = std::make_shared<BreakLayout>();
    m_edm_break_layout = std::make_unique<EntityDataModel>(m_break_layout);
    ui->tvBreakLayouts->setModel(m_edm_break_layout.get());
    ui->tvBreakLayouts->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_edm_break_layout->all();


    connect(ui->tvBreakLayouts, &QTableView::clicked, this, &BreakCreateForm::break_layout_selected);

    m_edm_break_line = std::make_unique<EntityDataModel>(std::make_shared<BreakLayoutLine>());

    ui->tvBreakLines->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvBreakLines->setModel(m_edm_break_line.get());

    connect(ui->btnCreate, &QPushButton::clicked, this, &BreakCreateForm::create_breaks);
    connect(ui->btnCancel, &QPushButton::clicked, this, &BreakCreateForm::close_form);

    connect(ui->btnAddHour, &QPushButton::clicked, this, &BreakCreateForm::add_hour);
    connect(ui->btnRemoveHour, &QPushButton::clicked, this, &BreakCreateForm::remove_hour);

    set_defaults();

    setup_ui();
}

BreakCreateForm::~BreakCreateForm()
{
    delete ui;
}

void BreakCreateForm::set_defaults()
{
    setWindowTitle("Commercial Breaks");
    ui->dtFrom->setDate(QDate::currentDate());
    ui->dtTo->setDate(QDate::currentDate());
    //populate_hour_combo();
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
}

void BreakCreateForm::break_layout_selected(const QModelIndex &index)
{

    if (index.row() > -1){
        int sel_row = index.row();
        std::string layout_name = ui->tvBreakLayouts->model()->data(
                    ui->tvBreakLayouts->model()->index(
                        sel_row, 0)).toString().toStdString();

        std::shared_ptr<BaseEntity> be = m_edm_break_layout->findEntityByName(layout_name);

        std::shared_ptr<BreakLayout> breakLayout = dynamic_pointer_cast<BreakLayout>(be);

        BreakLayoutLine bbl;
        auto break_line_filter = std::make_tuple(bbl.breakLayout()->dbColumnName(),
                                                 "=",
                                                 breakLayout->id());
        std::string filter = m_edm_break_line->prepareFilter(break_line_filter);
        m_edm_break_line->search(filter);
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

    auto reply = QMessageBox::question(this, "Create Breaks",
                                       "Continue with Break Creation?",
                                       QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
        return;

    if (ui->dtTo->date() < ui->dtFrom->date()){
        showMessage("`To` date less than `From` date!");
        return;
    }

    std::string sql = make_break_sql(ui->dtFrom->date(), ui->dtTo->date());

    if (sql.empty())
        return;

    if (write_breaks_to_db(sql))
        close_form();
}

void BreakCreateForm::get_existing_schedules(ScheduleRecords& s_recs, QDate from, QDate to)
{
    std::stringstream sql;

    sql << " Select schedule_date, schedule_hour, schedule_time "
        << " From rave_schedule ";

    std::string where_filter = std::format(" Where schedule_date between '{}' and '{}' ",
                                           from.toString("yyyy-MM-dd").toStdString(),
                                           to.toString("yyyy-MM-dd").toStdString());

    std::string order_str = " Order by schedule_date, schedule_hour, schedule_time ";

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

std::string BreakCreateForm::make_break_sql(QDate from, QDate to)
{
    Schedule sched;
    std::string insert_stmts;
    Vectored<Field> fields;

    ScheduleRecords s_recs;
    get_existing_schedules(s_recs, from, to);

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
        if (s_recs.find(sched_date) == s_recs.end() )
            return false;

        if (s_recs[sched_date].find(sched_hr) == s_recs[sched_date].end() )
            return false;

        for(auto& break_time: s_recs[sched_date][sched_hr]){
            if (break_time == sched_time.toString("hh:mm"))
                return true;
        }

        return false;
    };

    QDate tmpDate = from;

    while (tmpDate <= to)
    {
        for (auto& [name, entity] : m_edm_break_line->modelEntities())
        {
            BreakLayoutLine* bll = dynamic_cast<BreakLayoutLine*>(entity.get());

            fields << sched.set_schedule_date(tmpDate)
                    << sched.set_schedule_time(bll->breakTime()->value())
                    << sched.set_schedule_hour(bll->breakHour()->value())
                    << sched.set_break_duration(bll->duration()->value())
                    << sched.set_break_max_spots(bll->maxSpots()->value())
                    << sched.set_break_duration_left(bll->duration()->value())
                    << sched.set_booked_spots(0)
                    << sched.set_schedule_item_type("COMM-BREAK")
                    << sched.set_break_mode("MIXED")
                    << sched.set_break_fill_method(bll->break_fill_method()->value());

            if (ui->cbSelectedHours->count() == 0)
            {
                insert_stmts += sched.make_insert_stmt(fields.vec);

            }else{
                if (ui->cbSelectedHours->findText(QString::number(bll->breakHour()->value())) > -1 )
                {
                    if (!break_exists(tmpDate, bll->breakHour()->value(), bll->breakTime()->value()))
                        insert_stmts += sched.make_insert_stmt(fields.vec);

                }
            }

            fields.clear();
         }

        tmpDate = tmpDate.addDays(1);
    }

    return insert_stmts;
}


bool BreakCreateForm::write_breaks_to_db(const std::string sql)
{
    try{
        m_edm_break_line->executeRawSQL(sql);
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

    if (dts->exec() == 1){
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
        std::make_shared<BreakLayoutForm>(break_layout.get(), this);

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
                bll->setBreakTime(model->data(index).toTime());
                hour = model->data(index).toTime().hour();
                break;
            case 1:
                bll->setDuration(model->data(index).toInt());
                break;
            case 2:
                bll->setMaxSpots(model->data(index).toInt());
                break;
            case 3:
                bll->set_break_fill_method(
                    fill_method[model->data(index).toString()].toStdString());
                break;
            }
        }

        bll->setBreakHour(hour);
        bll->setWeekDay(1);
        bll->setBreakLayout(layout_header_id);

        edm->createEntity(std::move(bll));
    }

}

void BreakCreateForm::edit_layout()
{
    auto model_indexes = ui->tvBreakLayouts->selectionModel()->selectedIndexes();
    QVariant col_name{};
    if (model_indexes.size() > 0){
        col_name = ui->tvBreakLayouts->model()->data(model_indexes[0]);
    }

    if (col_name.toString().isEmpty())
        return;

    std::string search_name = col_name.toString().toStdString();
    std::shared_ptr<BaseEntity> be = m_edm_break_layout->findEntityByName(search_name);
    auto break_layout = dynamic_cast<BreakLayout*>(be.get());
    auto bl_form = std::make_shared<BreakLayoutForm>(break_layout);


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
                // case static_cast<int>(BreakLineColumn::BreakTime):
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
            bll.setBreakLayout(break_layout->id());
            edm.updateEntity(bll);
        }

    }

}

void BreakCreateForm::delete_layout()
{
    if (!confirmationMessage("Delete layout?"))
        return;

    QVariant col_name{};
    int selected_row = -1;

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

    // Delete details first
    EntityDataModel edm(std::make_shared<BreakLayoutLine>());
    edm.deleteEntityByValue({"break_layout_id", break_layout->id()});
    m_edm_break_line->clearEntities();

    // Delete header
    m_edm_break_layout->deleteEntity(*break_layout);
    ui->tvBreakLayouts->model()->removeRow(selected_row);

}
