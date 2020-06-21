#include <QTableWidgetItem>
#include <QPalette>
#include "daypartgrid.h"
#include "ui_daypartgrid.h"

DayPartGrid::DayPartGrid(QVBoxLayout* layout, QWidget *parent) :
    QWidget{parent},
    ui{new Ui::DayPartGrid},
    mDayParts{},
    mLayout{layout}
{
    ui->setupUi(this);
    //connect(ui->twDaypart, SIGNAL(cellEntered(int, int)), this, SLOT(cell_entered(int, int)));
    connect(ui->twDaypart, &QTableWidget::cellEntered, this, &DayPartGrid::cell_entered);
    connect(ui->twDaypart, SIGNAL(cellClicked(int, int)), this, SLOT(cell_clicked(int, int)));

    prepareGrid();


    mLayout->addWidget(this);
}

DayPartGrid::~DayPartGrid()
{
    delete ui;
}

void DayPartGrid::prepareGrid()
{
    for(int hr=0; hr<=hrs_in_day-1; ++hr){
     ui->twDaypart->setHorizontalHeaderItem(hr, new QTableWidgetItem(QString::number(hr)));
    }

    std::array<std::string, days_in_week> weekdays {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    for(int day=0; day <= days_in_week-1; ++day){
       ui->twDaypart->setVerticalHeaderItem(day, new QTableWidgetItem(QString::fromStdString(weekdays[day])));
    }

    for(int r=0; r<= days_in_week-1; ++r){
       ui->twDaypart->setRowHeight(r, 20);
       for (int c=0; c<= hrs_in_day-1; ++c){
           ui->twDaypart->setItem(r,c, new QTableWidgetItem);
           ui->twDaypart->item(r,c)->setBackground(Qt::red);
           ui->twDaypart->setColumnWidth(c, 20);
        }
    }

    // Disable highlighting
    ui->twDaypart->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->twDaypart->setFocusPolicy(Qt::NoFocus);
    ui->twDaypart->setSelectionMode(QAbstractItemView::NoSelection);
}

void DayPartGrid::cell_entered(int row, int col)
{
    QTableWidgetItem* cell = ui->twDaypart->item(row, col);
    this->update_cell_state<Selected>(cell);

    /*
    if (is_cell_selected(cell)){
        this->update_cell_state<Unselected>(cell);
    }else{
        this->update_cell_state<Selected>(cell);
    }
    */
}

void DayPartGrid::cell_clicked(int row, int col)
{
    QTableWidgetItem* cell = ui->twDaypart->item(row, col);

    if (is_cell_selected(cell)){
        this->update_cell_state<Unselected>(cell);
    }else{
        this->update_cell_state<Selected>(cell);
    }
}

void DayPartGrid::updateGrid(std::map<std::string, std::string> dayparts)
{
    QTableWidgetItem* cell;
    int r=0;
    for (auto& [key, value] : dayparts){
        //for (size_t c=0; c <= hrs_in_day-1; ++c){
        for(std::string::iterator it=value.begin(); it != value.end(); ++it){
            if (*it=='1'){
                auto pos = it-value.begin();
                cell = ui->twDaypart->item(r, pos);
                update_cell_state<Selected>(cell);
            }
        }
        ++r;
    }
}

std::map<std::string, std::string> DayPartGrid::getDayparts()
{
        return mDayParts;
}

bool DayPartGrid::is_cell_selected(QTableWidgetItem* cell)
{
     return(cell->background() == Qt::green) ? true : false;
}

std::map<std::string, std::string> DayPartGrid::readGrid()
{
    mDayParts.clear();
    std::string prefix= "daypart";
    for(int r=0; r< days_in_week; ++r){
      std::string day{};
      for (int c=0; c<= hrs_in_day-1; ++c){
          day += std::to_string(gridCells[r][c]);
      }
      std::string key = prefix+std::to_string(r+1);
      mDayParts[key] = day;
    }
      return mDayParts;
}
