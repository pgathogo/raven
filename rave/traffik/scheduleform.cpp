#include <set>
#include <QMdiArea>
#include "scheduleform.h"
#include "ui_scheduleform.h"

#include "schedule.h"
#include "../framework/entitydatamodel.h"
#include "../framework/choicefield.h"
#include "breakcreateform.h"
#include "schedulemantreeviewmodel.h"

ScheduleForm::ScheduleForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScheduleForm),
    m_edm_schedule{nullptr},
    mMdiArea{nullptr}
{
    ui->setupUi(this);
    connect(ui->dtSchedule, &QDateEdit::dateChanged, this, &ScheduleForm::scheduleDateChanged);
    connect(ui->btnCreate, &QPushButton::clicked, this, &ScheduleForm::create_breaks);

//    connect(ui->lwHours, &QListWidget::itemClicked, this, &ScheduleForm::hourClicked);

    m_edm_schedule = std::make_unique<EntityDataModel>(
                std::make_unique<Schedule>());

    ui->dtSchedule->setDate(QDate::currentDate());

    setWindowTitle("Schedule Management Form");
}

ScheduleForm::~ScheduleForm()
{
    delete ui;
}

void ScheduleForm::load_schedule(const QDate &date)
{
    m_edm_schedule->clearEntities();

    Schedule sched;
    auto date_filter = std::make_tuple(
                sched.scheduleDate()->dbColumnName(),
                "=",
                date);
    std::string filter = m_edm_schedule->prepareFilter(date_filter);

    m_edm_schedule->search(filter);

    build_tree_view();

}

void ScheduleForm::scheduleDateChanged(const QDate& date)
{
    load_schedule(date);
}

void ScheduleForm::create_breaks()
{
    auto bcForm = std::make_unique<BreakCreateForm>(this);
    bcForm->exec();
}

void ScheduleForm::hour_clicked(QListWidgetItem *item)
{
}

void ScheduleForm::build_tree_view()
{
    Breaks comm_breaks;

    for (auto& [name, entity] : m_edm_schedule->modelEntities()){
        Schedule* schedule = dynamic_cast<Schedule*>(entity.get());

        Break comm_break;
        comm_break.schedule_date = schedule->scheduleDate()->value().toString("yyyy-mm-dd").toStdString();
        comm_break.schedule_hour = schedule->scheduleHour()->value();
        comm_break.schedule_time = schedule->scheduleTime()->value().toString("hh:mm").toStdString();
        comm_break.break_mode = schedule->breakMode()->displayName();
        comm_break.max_spots = schedule->breakMaxSpots()->value();
        comm_break.break_duration = schedule->breakDuration()->value();
        comm_break.booked_spots = schedule->bookedSpots()->value();
        comm_break.time_left = schedule->breakDurationLeft()->value();

        comm_breaks[comm_break.schedule_hour].push_back(comm_break);
    }

    if (comm_breaks.size() > 0){
        ScheduleManTreeViewModel* sched_model = new ScheduleManTreeViewModel(comm_breaks);
        ui->tvSchedule->setModel(sched_model);

        ui->tvSchedule->header()->setSectionResizeMode(QHeaderView::Stretch);
    }

}

void ScheduleForm::setMdiArea(QMdiArea* mdi)
{
    mMdiArea = mdi;
}


