#include <QDebug>
#include <QMenu>
#include <QMouseEvent>

#include <QTableWidgetItem>
#include <QPalette>
#include "daypartgrid.h"
#include "ui_daypartgrid.h"

#include "tools.h"

DayPartGrid::DayPartGrid(QVBoxLayout* layout, Presets psets,  QWidget *parent) :
    QWidget{parent},
    ui{new Ui::DayPartGrid},
    m_dayparts{},
    m_layout{layout},
    m_start_point{0,0},
    m_right_clicked{false},
    m_context_menu{},
    m_preset{psets}
{
    ui->setupUi(this);
    connect(ui->twDaypart, &QTableWidget::cellEntered, this, &DayPartGrid::cell_entered);
    connect(ui->twDaypart, SIGNAL(cellClicked(int, int)), this, SLOT(cell_clicked(int, int)));

    prepare_grid();

    m_layout->addWidget(this);

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    //connect(this, &QTableWidget::customContextMenuRequested(QPoint), this, &DayPartGrid::showContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    m_context_menu = std::make_unique<QMenu>(tr("Grid menu"), this);

    int w = 550;
    int h = 200;
    setFixedSize(w, h);

}

DayPartGrid::~DayPartGrid()
{
    delete ui;
}

void DayPartGrid::set_size_policy(GridSizePolicy policy)
{
    switch(policy){
        case GridSizePolicy::Auto:
            setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
            break;
        case GridSizePolicy::Fixed:
        {
            int w = 550;
            int h = 200;
            setFixedSize(w, h);
        }

    }

}

void DayPartGrid::prepare_grid()
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

    if (m_right_clicked)
        return;

    QTableWidgetItem* cell = ui->twDaypart->item(row, col);
    if ( !is_cell_selected(cell) ){
        this->update_cell_state<Selected>(cell);
        update_multiple_cells<Selected>(row, col, ui->twDaypart);
    }else{
        this->update_cell_state<Unselected>(cell);
        update_multiple_cells<Unselected>(row, col, ui->twDaypart);

    }

}

void DayPartGrid::cell_clicked(int row, int col)
{
    if (m_right_clicked)
        return;

    // save start point
    set_start_point(row, col);

    QTableWidgetItem* cell = ui->twDaypart->item(row, col);

    if (is_cell_selected(cell)){
        this->update_cell_state<Unselected>(cell);
    }else{
        this->update_cell_state<Selected>(cell);
    }
}

void DayPartGrid::update_grid(std::map<int, std::string> dayparts)
{
    //clear_all_cells();

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

void DayPartGrid::set_start_point(int row, int col)
{
    m_start_point.row = row;
    m_start_point.col = col;
}

StartPoint DayPartGrid::start_point()
{
    return m_start_point;
}

bool DayPartGrid::is_cell_selected(QTableWidgetItem* cell)
{
     return(cell->background() == Qt::green) ? true : false;
}

std::map<int, std::string> DayPartGrid::read_grid()
{
    // Reading cell elements from m_grid_cells to create
    // a string of format "001011110000".
    // Return a map of type: m_dayparts[dow] = "00011110000"

    m_dayparts.clear();

    for(int dow=0; dow < days_in_week; ++dow)
    {
      std::string day{};

      for (int hour=0; hour<= hrs_in_day-1; ++hour)
      {
          day += std::to_string(m_grid_cells[dow][hour]);
      }

      m_dayparts[dow+1] = day;
    }

    return m_dayparts;
}

std::map<int, std::vector<int>> DayPartGrid::read_grid_by_time()
{
    std::map<int, std::vector<int>> grid_cells;

    for(int dow=0; dow < days_in_week; dow++)
    {
        for(int hour=0; hour <= hrs_in_day-1; ++hour)
        {

            if (m_grid_cells[dow][hour] == 0)
                continue;
            grid_cells[dow+1].push_back(hour);
        }
    }

    return grid_cells;

}

DaypartExt DayPartGrid::daypart_to_hours(std::map<int,std::string>&& dayparts)
{
    DaypartExt hours;

    for (auto& [day, daypart] : dayparts){
        int hr = 0;
        std::vector<int> hrs;
        for(std::string::iterator it=daypart.begin(); it != daypart.end(); ++it){
            if (*it=='1')
                hrs.push_back(hr);
            ++hr;
        }

        hours[day] = std::make_tuple(daypart, hrs);
    }

    return hours;
}

std::vector<int> DayPartGrid::daypart_str_to_hours(const std::string& dp_str)
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

void DayPartGrid::showContextMenu(QPoint pos)
{
    QAction actSelectAll("Select all", this);
    connect(&actSelectAll, &QAction::triggered, this, &DayPartGrid::select_all_cells);
    m_context_menu->addAction(&actSelectAll);

    QAction actClearAll("Clear all", this);
    connect(&actClearAll, &QAction::triggered, this, &DayPartGrid::clear_all_cells);
    m_context_menu->addAction(&actClearAll);

    m_context_menu->addSeparator();

    int i=1;
    for(auto& [band_name, dayparts]:m_preset){
        QAction* act = new QAction(stoq(band_name), this);
        act->setData(stoq(band_name));
        connect(act, &QAction::triggered, this, &DayPartGrid::selectPreset);
        m_context_menu->addAction(act);
    }

    m_context_menu->exec(mapToGlobal(pos));
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
        update_grid(m_preset[act->data().toString().toStdString()]);
    }
}


bool DayPartGrid::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress){
        //obj is of type "qt_scrollarea_viewport" so we look for its parent
        if (obj->parent() == ui->twDaypart){
            auto mbe = dynamic_cast<QMouseEvent*>(event);
            if (mbe->button() == Qt::RightButton)
                m_right_clicked = true;
            else if (mbe->button() == Qt::LeftButton)
                m_right_clicked = false;
        }

    }
    return QWidget::eventFilter(obj, event);

}

