#include <set>
#include <QMdiArea>
#include "scheduleform.h"
#include "ui_scheduleform.h"

#include "../framework/schedule.h"
#include "../framework/entitydatamodel.h"
#include "../framework/choicefield.h"
#include "../framework/ravenexception.h"
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
    connect(ui->btnDelete, &QPushButton::clicked, this, &ScheduleForm::delete_breaks);

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
                sched.schedule_date()->dbColumnName(),
                "=",
                date);

    std::string filter = m_edm_schedule->prepareFilter(date_filter);

    m_edm_schedule->search(filter);

    build_tree_view();

}

void ScheduleForm::scheduleDateChanged(const QDate& date)
{
    load_schedule(QDate::fromString(date.toString()));
}

void ScheduleForm::create_breaks()
{
    auto bcForm = std::make_unique<BreakCreateForm>(this);
    if (bcForm->exec() > 0)
        load_schedule(current_date());
}

void ScheduleForm::delete_breaks()
{
    //FIXME Show ids userdata for child nodes
    QModelIndex index = ui->tvSchedule->selectionModel()->currentIndex();

    if (!index.isValid())
        return;

    QVariant data = ui->tvSchedule->selectionModel()->model()->data(index, Qt::UserRole);
    QString text = data.toString();

    if (text.isEmpty())
        return;

    int hour = data.toInt();

    int result = QMessageBox::warning(this, tr("Schedule Breaks"),
                                      tr("Delete ALL EMPTY breaks for selected hour?"),
                                      QMessageBox::Yes | QMessageBox::No);
    switch(result){
        case QMessageBox::Yes:
            delete_all_empty_breaks(current_date(), hour);
            load_schedule(current_date());
            break;
        case QMessageBox::No:
            break;
    }

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
        comm_break.id = schedule->id();
        comm_break.schedule_date = schedule->schedule_date()->value().toString("yyyy-mm-dd").toStdString();
        comm_break.schedule_hour = schedule->schedule_hour()->value();
        comm_break.schedule_time = schedule->schedule_time()->value().toString("hh:mm").toStdString();
        comm_break.break_mode = schedule->break_mode()->displayName();
        comm_break.max_spots = schedule->break_max_spots()->value();
        comm_break.break_duration = schedule->break_duration()->value();
        comm_break.booked_spots = schedule->booked_spots()->value();
        comm_break.time_left = schedule->break_duration_left()->value();

        comm_breaks[comm_break.schedule_hour].push_back(comm_break);
    }

    if (comm_breaks.size() > 0){
        ScheduleManTreeViewModel* sched_model = new ScheduleManTreeViewModel(comm_breaks);
        ui->tvSchedule->setModel(sched_model);

        ui->tvSchedule->header()->setSectionResizeMode(QHeaderView::Stretch);
    }

}

QDate ScheduleForm::current_date()
{
    return ui->dtSchedule->date().currentDate();
}

void ScheduleForm::setMdiArea(QMdiArea* mdi)
{
    mMdiArea = mdi;
}

void ScheduleForm::delete_all_empty_breaks(QDate date, int hour)
{
    std::stringstream sql;

    sql << " DELETE From rave_schedule "
        << " Where schedule_date ='"+date.toString("yyyy-MM-dd").toStdString()+"'"
        << " And Schedule_hour = "+std::to_string(hour)
        << " And id NOT IN (SELECT schedule_id From rave_orderbooking) ";

    EntityDataModel edm;

    try{
        edm.executeRawSQL(sql.str());
    }catch(DatabaseException& de){
        showMessage(de.errorMessage());
    }

}

