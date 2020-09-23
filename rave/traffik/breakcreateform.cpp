#include "breakcreateform.h"
#include "ui_breakcreateform.h"

#include "../framework/entitydatamodel.h"
#include "../framework/choicefield.h"
#include "../framework/ravenexception.h"
#include "breaklayout.h"
#include "breaklayoutline.h"
#include "schedule.h"


BreakCreateForm::BreakCreateForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BreakCreateForm)
{
    ui->setupUi(this);
    edmBreakLayout = std::make_unique<EntityDataModel>(
                std::make_unique<BreakLayout>());
    ui->tvBreakLayouts->setModel(edmBreakLayout.get());
    ui->tvBreakLayouts->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    edmBreakLayout->all();

    connect(ui->tvBreakLayouts, &QTableView::clicked, this, &BreakCreateForm::breakLayoutSelected);

    edmBreakLine = std::make_unique<EntityDataModel>(
                std::make_unique<BreakLayoutLine>());

    ui->tvBreakLines->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tvBreakLines->setModel(edmBreakLine.get());

    connect(ui->btnCreate, &QPushButton::clicked, this, &BreakCreateForm::createBreaks);
    connect(ui->btnCancel, &QPushButton::clicked, this, &BreakCreateForm::closeForm);

    setDefaults();
}

BreakCreateForm::~BreakCreateForm()
{
    delete ui;
}

void BreakCreateForm::setDefaults()
{
    setWindowTitle("Create Breaks Form");
    ui->dtFrom->setDate(QDate::currentDate());
    ui->dtTo->setDate(QDate::currentDate());
}

void BreakCreateForm::breakLayoutSelected(const QModelIndex &index)
{

    if (index.row() > -1){
        int sel_row = index.row();
        std::string layout_name = ui->tvBreakLayouts->model()->data(
                    ui->tvBreakLayouts->model()->index(
                        sel_row, 0)).toString().toStdString();

        BaseEntity* be = edmBreakLayout->findEntityByName(layout_name);
        BreakLayout* breakLayout = dynamic_cast<BreakLayout*>(be);

        BreakLayoutLine bbl;
        auto break_line_filter = std::make_tuple(bbl.breakLayout()->dbColumnName(),
                                                 breakLayout->id());
        std::string filter = edmBreakLine->prepareFilter(break_line_filter);
        edmBreakLine->search(filter);
    }

}

void BreakCreateForm::closeForm()
{
    done(0);
}

void BreakCreateForm::createBreaks()
{
    if (ui->dtTo->date() < ui->dtFrom->date()){
        showMessage("`To` date less than `From` date!");
        return;
    }

    Schedule sched;

    std::string inserts;

    Vectored<Field> fields;

    QDate tmpDate = ui->dtFrom->date();
    while (tmpDate <= ui->dtTo->date()){

        for (auto& [name, entity] : edmBreakLine->modelEntities()){
            BreakLayoutLine* bll = dynamic_cast<BreakLayoutLine*>(entity.get());

            fields << sched.setScheduleDate(tmpDate)
                    << sched.setScheduleTime(bll->breakTime()->value())
                    << sched.setScheduleHour(bll->breakHour()->value())
                    << sched.setBreakDuration(bll->duration()->value())
                    << sched.setBreakMaxSpots(bll->maxSpots()->value())
                    << sched.setBreakDurationLeft(bll->duration()->value())
                    << sched.setBookedSpots(0)
                    << sched.setTrackType("COMM")
                    << sched.setBreakMode("MIXED");

            inserts += sched.make_insert_stmt(fields.vec);
            fields.clear();
            }

        tmpDate = tmpDate.addDays(1);
    }

    try{
        edmBreakLine->executeRawSQL(inserts);
    } catch (DatabaseException& de) {
        showMessage(de.errorMessage());
    }
}
