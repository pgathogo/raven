#ifndef STATIONFORM_H
#define STATIONFORM_H

#include <QDialog>

#include "../../../framework/baseentitydetaildlg.h"

namespace Ui {
class StationForm;
}

namespace ClusterManager {
  class Station;
}

class StationForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit StationForm(ClusterManager::Station* station, QDialog* parent = nullptr);
    ~StationForm();

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

private:
    Ui::StationForm *ui;
    ClusterManager::Station* m_station;
};

#endif // STATIONFORM_H
