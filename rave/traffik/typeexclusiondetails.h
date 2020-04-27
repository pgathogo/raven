#ifndef TYPEEXCLUSIONDETAILS_H
#define TYPEEXCLUSIONDETAILS_H

#include <QDialog>
#include "../framework/baseentitydetaildlg.h"

class TypeExclusion;
class DayPartGrid;

namespace Ui {
class TypeExclusionDetails;
}

class TypeExclusionDetails : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit TypeExclusionDetails(TypeExclusion* tex, QDialog *parent = nullptr);
    ~TypeExclusionDetails();

    ActionResult saveRecord() override;
    void bindWidgets() override;
    std::string title() override;
    void populateGrid();

private:
    Ui::TypeExclusionDetails *ui;
    TypeExclusion* typeEx;
    DayPartGrid* dpg;
};

#endif // TYPEEXCLUSIONDETAILS_H
