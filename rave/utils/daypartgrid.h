#ifndef DAYPARTGRID_H
#define DAYPARTGRID_H

#include <QWidget>
#include <QTableWidgetItem>
#include <memory>

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

using Presets = std::map<std::string, std::map<std::string, std::string>>;

class DayPartGrid : public QWidget
{
    Q_OBJECT

public:
    explicit DayPartGrid(QVBoxLayout* layout, Presets psets = {}, QWidget *parent = nullptr);
    ~DayPartGrid();

    constexpr static int days_in_week=7;
    constexpr static int hrs_in_day=24;

    std::array<std::array<int, hrs_in_day>, days_in_week> gridCells = {};

    bool is_cell_selected(QTableWidgetItem* cell);

    void updateGrid(std::map<std::string, std::string> dayparts);

    std::map<std::string, std::string> getDayparts();

    void prepareGrid();

    std::map<std::string, std::string> readGrid();

    void setStartPoint(int row, int col);
    StartPoint startPoint();

    bool eventFilter(QObject* obj, QEvent* event) override;

    template<typename T>
    struct item_return{ typedef T type; };

    template<typename T>
    void update_cell_state(QTableWidgetItem* cell)
    {
        static_assert(std::is_base_of<CellSelectionState, T>::value, "`T` must be derived from `CellSelectionState`");
        T state;
        cell->setBackground(state.color());
        gridCells[cell->row()][cell->column()] = state.toggle();
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


private slots:
    //void saveGrid();
    void cell_entered(int row, int col);
    void cell_clicked(int row, int col);
    void showContextMenu(QPoint);
    void selectAllHours();
    void clearAllHours();
    void selectPreset();
private:
    Ui::DayPartGrid *ui;
    std::map<std::string, std::string> mDayParts;
    QVBoxLayout* mLayout;
    StartPoint sp;
    bool rightClicked;
    std::unique_ptr<QMenu> mContextMenu;
    Presets mPreset;
};

#endif // DAYPARTGRID_H
