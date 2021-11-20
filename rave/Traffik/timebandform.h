#ifndef TIMEBANDFORM_H
#define TIMEBANDFORM_H

#include <memory>

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

class TimeBand;
class DayPartGrid;

template<typename T>
class ChoiceField;

namespace Ui {
class TimeBandForm;
}

class TimeBandForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit TimeBandForm(TimeBand* tband,
                          QDialog* parent = nullptr);
    ~TimeBandForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;
    void populateGrid();
    void populateEntityFields() override;
    void populateFormWidgets() override;
    void setEditState(bool state);

private slots:
    void onIncPercent(bool checked);

private:
    Ui::TimeBandForm *ui;
    TimeBand* mTimeBand;
    std::unique_ptr<DayPartGrid> mDPG;
    //DayPartGrid* mDPG;
};

#endif // TIMEBANDFORM_H
