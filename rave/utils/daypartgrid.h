#ifndef DAYPARTGRID_H
#define DAYPARTGRID_H

#include <QWidget>
#include <QTableWidgetItem>
#include <memory>
#include <set>

class QVBoxLayout;

namespace Ui {
class DayPartGrid;
}

struct CellSelectionState {
    QBrush virtual color()=0;
    int virtual toggle()=0;
    virtual ~CellSelectionState(){}
};
struct DefaultState : public CellSelectionState{
    QBrush color(){ return Qt::red; }
    int toggle(){ return 0; }
};
struct Selected : public CellSelectionState{
    QBrush color(){ return Qt::green; }
    int toggle(){ return 1; }
};
struct Unselected: public CellSelectionState{
    QBrush color(){ return Qt::red; }
    int toggle(){ return 0; }
};

struct StartPoint{
    int row;
    int col;
};

using Presets = std::map<std::string, std::map<int, std::string>>;
using DaypartExt = std::map<int, std::tuple<std::string, std::vector<int>>>;

class DayPartGrid : public QWidget
{
    Q_OBJECT

public:

    explicit DayPartGrid(QVBoxLayout* layout, Presets psets = {}, QWidget *parent = nullptr);
    ~DayPartGrid() override;

    void update_grid(std::map<int, std::string> dayparts);

    std::map<int, std::string> read_grid();

    DaypartExt daypart_to_hours(std::map<int,std::string>&&);

    std::vector<int> daypart_str_to_hours(const std::string&);


private slots:
    //void saveGrid();
    void cell_entered(int row, int col);
    void cell_clicked(int row, int col);
    void showContextMenu(QPoint);
    void select_all_cells();
    void clear_all_cells();
    void selectPreset();
private:
    Ui::DayPartGrid *ui;
    std::map<int, std::string> m_dayparts;
    QVBoxLayout* m_layout;
    StartPoint m_start_point;
    bool m_right_clicked;
    std::unique_ptr<QMenu> m_context_menu;
    Presets m_preset;

    constexpr static int days_in_week{7};
    constexpr static int hrs_in_day{24};

    std::array<std::array<int, hrs_in_day>, days_in_week> m_grid_cells{};

    void set_start_point(int row, int col);
    StartPoint start_point();
    bool is_cell_selected(QTableWidgetItem* cell);
    void prepare_grid();

    bool eventFilter(QObject* obj, QEvent* event) override;

    template<typename T>
    struct item_return{ typedef T type; };

    template<typename T>
    void update_cell_state(QTableWidgetItem* cell)
    {
        static_assert(std::is_base_of<CellSelectionState, T>::value, "`T` must be derived from `CellSelectionState`");
        T state;
        cell->setBackground(state.color());
        m_grid_cells[cell->row()][cell->column()] = state.toggle();
    }

    template<typename T>
    void set_all_cells(QTableWidget* tableWidget)
    {
        for(int r=0; r<= days_in_week-1; ++r){
           for (int c=0; c<= hrs_in_day-1; ++c){
               QTableWidgetItem* cell = tableWidget->item(r, c);
               this->update_cell_state<T>(cell);
            }
        }
    }

    template<typename T>
    void update_multiple_cells(int row, int col, QTableWidget* table_widget)
    {
        if (col > m_start_point.col){
            for(int r=m_start_point.row; r<row; ++r){
                QTableWidgetItem* cell = table_widget->item(r, col);
                this->update_cell_state<T>(cell);
            }
        }

    }

};

#endif // DAYPARTGRID_H
