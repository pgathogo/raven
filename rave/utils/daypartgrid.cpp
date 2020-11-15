#include <QDebug>
#include <QMenu>
#include <QMouseEvent>

#include <QTableWidgetItem>
#include <QPalette>
#include "daypartgrid.h"
#include "ui_daypartgrid.h"

#include "tools.h"

DayPartGrid::DayPartGrid(QVBoxLayout* layout, Presets psets, QWidget *parent) :
    QWidget{parent},
    ui{new Ui::DayPartGrid},
    mDayParts{},
    mLayout{layout},
    sp{0,0},
    rightClicked{false},
    mContextMenu{},
    mPreset{psets}
{
    ui->setupUi(this);
    connect(ui->twDaypart, &QTableWidget::cellEntered, this, &DayPartGrid::cell_entered);
    connect(ui->twDaypart, SIGNAL(cellClicked(int, int)), this, SLOT(cell_clicked(int, int)));

    prepareGrid();

    mLayout->addWidget(this);

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    //connect(this, &QTableWidget::customContextMenuRequested(QPoint), this, &DayPartGrid::showContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    mContextMenu = std::make_unique<QMenu>(tr("Grid menu"), this);

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

    //ui->twDaypart->viewport()->setMouseTracking(true);
    ui->twDaypart->viewport()->installEventFilter(this);
}

void DayPartGrid::cell_entered(int row, int col)
{

    if (rightClicked)
        return;

    QTableWidgetItem* cell = ui->twDaypart->item(row, col);
    this->update_cell_state<Selected>(cell);

    if (col > sp.col){
        for(int r=sp.row; r<row; ++r){
            QTableWidgetItem* cell = ui->twDaypart->item(r, col);
            this->update_cell_state<Selected>(cell);
        }
    }

}

void DayPartGrid::cell_clicked(int row, int col)
{
    if (rightClicked)
        return;

    // save start point
    setStartPoint(row, col);

    QTableWidgetItem* cell = ui->twDaypart->item(row, col);

    if (is_cell_selected(cell)){
        this->update_cell_state<Unselected>(cell);
    }else{
        this->update_cell_state<Selected>(cell);
    }
}

void DayPartGrid::update_grid(std::map<int, std::string> dayparts)
{
    clear_all_cells();

    QTableWidgetItem* cell;
    int row=0;
    for (auto& [dpname, daypart] : dayparts){
        for(std::string::iterator it=daypart.begin(); it != daypart.end(); ++it){
            if (*it=='1'){
                auto col = it-daypart.begin();
                cell = ui->twDaypart->item(row, col);
                update_cell_state<Selected>(cell);
            }
        }
        ++row;
    }
}

void DayPartGrid::setStartPoint(int row, int col)
{
    sp.row = row;
    sp.col = col;
}

StartPoint DayPartGrid::startPoint()
{
    return sp;
}

bool DayPartGrid::is_cell_selected(QTableWidgetItem* cell)
{
     return(cell->background() == Qt::green) ? true : false;
}

std::map<int, std::string> DayPartGrid::read_grid()
{
    mDayParts.clear();
    for(int r=0; r< days_in_week; ++r){
      std::string day{};
      for (int c=0; c<= hrs_in_day-1; ++c){
          day += std::to_string(gridCells[r][c]);
      }
      mDayParts[r+1] = day;
    }
    return mDayParts;
}

std::set<int> DayPartGrid::daypart_to_hours()
{
    std::set<int> hours;
    auto dayparts = read_grid();
    for (auto& [day, daypart] : dayparts){
        int hr = 0;
        for(std::string::iterator it=daypart.begin(); it != daypart.end(); ++it){
            if (*it=='1')
                hours.insert(hr);
            ++hr;
        }
    }

    return hours;
}

void DayPartGrid::showContextMenu(QPoint pos)
{
    QAction actSelectAll("Select all", this);
    connect(&actSelectAll, &QAction::triggered, this, &DayPartGrid::select_all_cells);
    mContextMenu->addAction(&actSelectAll);

    QAction actClearAll("Clear all", this);
    connect(&actClearAll, &QAction::triggered, this, &DayPartGrid::clear_all_cells);
    mContextMenu->addAction(&actClearAll);

    mContextMenu->addSeparator();

    int i=1;
    for(auto& [band_name, dayparts]:mPreset){
        QAction* act = new QAction(stoq(band_name), this);
        act->setData(stoq(band_name));
        connect(act, &QAction::triggered, this, &DayPartGrid::selectPreset);
        mContextMenu->addAction(act);
    }

    mContextMenu->exec(mapToGlobal(pos));
}

void DayPartGrid::select_all_cells()
{
   set_all_cells<Selected>(ui->twDaypart);
}

void DayPartGrid::clear_all_cells()
{
   set_all_cells<Unselected>(ui->twDaypart);
}

void DayPartGrid::selectPreset()
{
    QAction* act = qobject_cast<QAction*>(sender());
    if (act){
        update_grid(mPreset[act->data().toString().toStdString()]);
    }
}


bool DayPartGrid::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress){
        //obj is of type "qt_scrollarea_viewport" so we look for its parent
        if (obj->parent() == ui->twDaypart){
            auto mbe = dynamic_cast<QMouseEvent*>(event);
            if (mbe->button() == Qt::RightButton)
                rightClicked = true;
            else if (mbe->button() == Qt::LeftButton)
                rightClicked = false;
        }

    }
    return QWidget::eventFilter(obj, event);

}
