#ifndef SELECTSTATIONFORM_H
#define SELECTSTATIONFORM_H

#include <QDialog>
#include "structs.h"

namespace Ui {
class selectstationform;
}

class SelectStationForm : public QDialog
{
    Q_OBJECT

public:
    explicit SelectStationForm(std::map<int, StationInfo>&, QWidget *parent = nullptr);
    int station_id();
    ~SelectStationForm();

private slots:
    void select_station();
    void cancel();

private:
    void populate_table(std::map<int, StationInfo>&);
    Ui::selectstationform *ui;
    int m_sel_station_id ;
};

#endif // SELECTSTATIONFORM_H
