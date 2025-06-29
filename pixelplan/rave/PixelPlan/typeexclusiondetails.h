#ifndef TYPEEXCLUSIONDETAILS_H
#define TYPEEXCLUSIONDETAILS_H

#include <QDialog>
#include "../../../rave/framework/baseentitydetaildlg.h"
#include "../../../rave/framework/typeexclusion.h"

class DayPartGrid;

namespace Ui {
class TypeExclusionDetails;
}

class TypeExclusionDetails : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit TypeExclusionDetails(TRAFFIK::TypeExclusion* tex, QDialog *parent = nullptr);
    ~TypeExclusionDetails() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;
    void populateGrid();
    void populateEntityFields() override;
    void populateFormWidgets() override;

private:
    Ui::TypeExclusionDetails *ui;
    TRAFFIK::TypeExclusion* typeEx;
    DayPartGrid* dpg;
};

#endif // TYPEEXCLUSIONDETAILS_H
