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
    explicit SelectStationForm(QWidget* parent=nullptr);
    explicit SelectStationForm(const QString username, QWidget *parent = nullptr);
    int station_id();
    StationInfo selected_station();
    ~SelectStationForm();

    StationInfo station_by_station_code(const QString station_code);

private slots:
    void select_station();
    void cancel();

protected:
    void showEvent(QShowEvent *) override;

private:
    void populate_table(std::map<int, StationInfo>&);
    bool fetch_user_stations(const QString);

    Ui::selectstationform *ui;
    int m_sel_station_id ;
    QString m_username{};
    std::map<int, StationInfo> m_stations_info;
    StationInfo m_selected_station;
};

#endif // SELECTSTATIONFORM_H
