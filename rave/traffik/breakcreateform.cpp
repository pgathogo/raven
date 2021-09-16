#include "breakcreateform.h"
#include "ui_breakcreateform.h"

#include "../framework/entitydatamodel.h"
#include "../framework/choicefield.h"
#include "../framework/ravenexception.h"
#include "breaklayout.h"
#include "breaklayoutline.h"
#include "../framework/schedule.h"


BreakCreateForm::BreakCreateForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BreakCreateForm)
{
    ui->setupUi(this);
    m_edm_break_layout = std::make_unique<EntityDataModel>(
                std::make_unique<BreakLayout>());
    ui->tvBreakLayouts->setModel(m_edm_break_layout.get());
    ui->tvBreakLayouts->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_edm_break_layout->all();

    connect(ui->tvBreakLayouts, &QTableView::clicked, this, &BreakCreateForm::break_layout_selected);

    m_edm_break_line = std::make_unique<EntityDataModel>(
                std::make_unique<BreakLayoutLine>());

    ui->tvBreakLines->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvBreakLines->setModel(m_edm_break_line.get());

    connect(ui->btnCreate, &QPushButton::clicked, this, &BreakCreateForm::create_breaks);
    connect(ui->btnCancel, &QPushButton::clicked, this, &BreakCreateForm::close_form);

    set_defaults();
}

BreakCreateForm::~BreakCreateForm()
{
    delete ui;
}

void BreakCreateForm::set_defaults()
{
    setWindowTitle("Generate Schedule Breaks Form");
    ui->dtFrom->setDate(QDate::currentDate());
    ui->dtTo->setDate(QDate::currentDate());
}

void BreakCreateForm::break_layout_selected(const QModelIndex &index)
{

    if (index.row() > -1){
        int sel_row = index.row();
        std::string layout_name = ui->tvBreakLayouts->model()->data(
                    ui->tvBreakLayouts->model()->index(
                        sel_row, 0)).toString().toStdString();

        BaseEntity* be = m_edm_break_layout->findEntityByName(layout_name);
        BreakLayout* breakLayout = dynamic_cast<BreakLayout*>(be);

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

    auto reply = QMessageBox::question(this, "Create Breaks",
                                       "Continue with Break Creation?",
                                       QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
        return;

    if (ui->dtTo->date() < ui->dtFrom->date()){
        showMessage("`To` date less than `From` date!");
        return;
    }

    if (m_edm_break_line->count() == 0){
        showMessage("Select a break layout");
        return;
    }

    std::string sql = make_break_sql(ui->dtFrom->date(), ui->dtTo->date());

    if (sql.empty())
        return;

    if (write_breaks_to_db(sql))
        close_form();
}

std::string BreakCreateForm::make_break_sql(QDate from, QDate to)
{
    Schedule sched;
    std::string insert_stmts;
    Vectored<Field> fields;

    QDate tmpDate = from;
    while (tmpDate <= to){

        for (auto& [name, entity] : m_edm_break_line->modelEntities()){
            BreakLayoutLine* bll = dynamic_cast<BreakLayoutLine*>(entity.get());

            fields << sched.set_schedule_date(tmpDate)
                    << sched.set_schedule_time(bll->breakTime()->value())
                    << sched.set_schedule_hour(bll->breakHour()->value())
                    << sched.set_break_duration(bll->duration()->value())
                    << sched.set_break_max_spots(bll->maxSpots()->value())
                    << sched.set_break_duration_left(bll->duration()->value())
                    << sched.set_booked_spots(0)
                    << sched.set_schedule_item_type("COMM-BREAK")
                    << sched.set_break_mode("MIXED");

            insert_stmts += sched.make_insert_stmt(fields.vec);
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
