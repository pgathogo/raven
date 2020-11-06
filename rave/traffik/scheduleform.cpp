#include <set>
#include <QMdiArea>
#include "scheduleform.h"
#include "ui_scheduleform.h"

#include "schedule.h"
#include "../framework/entitydatamodel.h"
#include "../framework/choicefield.h"
#include "breakcreateform.h"

ScheduleForm::ScheduleForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScheduleForm),
    edmSchedule{nullptr},
    mMdiArea{nullptr}
{
    ui->setupUi(this);
    connect(ui->dtSchedule, &QDateEdit::dateChanged, this, &ScheduleForm::scheduleDateChanged);
    connect(ui->btnCreate, &QPushButton::clicked, this, &ScheduleForm::createBreaks);

    connect(ui->lwHours, &QListWidget::itemClicked, this, &ScheduleForm::hourClicked);

    edmSchedule = std::make_unique<EntityDataModel>(
                std::make_unique<Schedule>());

    ui->dtSchedule->setDate(QDate::currentDate());

    setupScheduleGrid();

    setWindowTitle("Schedule Management Form");
}

ScheduleForm::~ScheduleForm()
{
    delete ui;
}

void ScheduleForm::loadSchedule(const QDate &date)
{
    ui->lwHours->clear();
    edmSchedule->clearEntities();

    Schedule sched;
    auto date_filter = std::make_tuple(
                sched.scheduleDate()->dbColumnName(),
                "=",
                date);
    std::string filter = edmSchedule->prepareFilter(date_filter);

    edmSchedule->search(filter);

    std::set<int> hours{};

    for(auto& [name, entity] : edmSchedule->modelEntities()){
        Schedule* schedule = dynamic_cast<Schedule*>(entity.get());
        hours.insert(schedule->scheduleHour()->value());
    }

    for (auto& hr : hours){
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::DisplayRole,stoq("Hour: "+std::to_string(hr)));
        item->setData(Qt::UserRole,hr);
        ui->lwHours->addItem(item);
    }

}

void ScheduleForm::scheduleDateChanged(const QDate& date)
{
    loadSchedule(date);
}

void ScheduleForm::createBreaks()
{
    auto bcForm = std::make_unique<BreakCreateForm>(this);
    bcForm->exec();
}

void ScheduleForm::hourClicked(QListWidgetItem *item)
{
    populateScheduleGrid(item->data(Qt::UserRole).toInt());
}

void ScheduleForm::setMdiArea(QMdiArea* mdi)
{
    mMdiArea = mdi;
}

void ScheduleForm::setupScheduleGrid()
{
    ui->twSchedule->setColumnCount(6);
    ui->twSchedule->setHorizontalHeaderLabels(edmSchedule->getEntity().tableHeaders());
    ui->twSchedule->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void ScheduleForm::populateScheduleGrid(int hr)
{
    ui->twSchedule->setRowCount(0);
    //setupScheduleGrid();

    int row = 0;
    for(auto& [name, entity] : edmSchedule->modelEntities()){
        Schedule* schedule = dynamic_cast<Schedule*>(entity.get());

        if (schedule->scheduleHour()->value() == hr){
            ui->twSchedule->insertRow(row);

            QTableWidgetItem* schedTime = new QTableWidgetItem(stoq(schedule->scheduleTime()->displayName()));
            QTableWidgetItem* schedMode = new QTableWidgetItem(stoq(schedule->breakMode()->displayName()));
            QTableWidgetItem* schedMaxSpots = new QTableWidgetItem(stoq(schedule->breakMaxSpots()->displayName()));
            QTableWidgetItem* schedDur = new QTableWidgetItem(stoq(schedule->breakDuration()->displayName()));
            QTableWidgetItem* schedBooked = new QTableWidgetItem(stoq(schedule->bookedSpots()->displayName()));
            QTableWidgetItem* schedDurLeft = new QTableWidgetItem(stoq(schedule->breakDurationLeft()->displayName()));
            QTableWidgetItem* schedStatus = new QTableWidgetItem(stoq(schedule->breakStatus()->displayName()));

            ui->twSchedule->setItem(row, 0, schedTime);
            ui->twSchedule->setItem(row, 1, schedMode);
            ui->twSchedule->setItem(row, 2, schedMaxSpots);
            ui->twSchedule->setItem(row, 3, schedDur);
            ui->twSchedule->setItem(row, 4, schedBooked);
            ui->twSchedule->setItem(row, 5, schedDurLeft);
            ui->twSchedule->setItem(row, 6, schedStatus);

            ++row;
        }
    }
}
