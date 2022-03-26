#ifndef BREAKLAYOUTFORM_H
#define BREAKLAYOUTFORM_H

#include <QDialog>
#include "../framework/baseentitydetaildlg.h"

namespace Ui {
class BreakLayoutForm;
}

class BreakLayout;
class EntityDataModel;
class RavenSetup;

template<typename T>
class ChoiceField;

class BreakLayoutForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit BreakLayoutForm(BreakLayout* bl,
                             QDialog* parent = nullptr);
    ~BreakLayoutForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

    void populateChoiceCombo(QComboBox* cbox, const ChoiceField<int>* cf);
    void populateCopyCB();
    void populateBreakLine();
    void setDefaults();
    void addBreakLines(int hour, int timeInterval=0);
    std::vector<EntityRecord> const& breakLines() const;
    void clearBreakTableView(int startRow, int endRow);
    void clearBreakLineModel(int startHr, int endHr);
    void copyHour(int fromHr, int toHr);
    void undoCopy(int fromHr, int toHr);
    void clearBreaks();

private slots:
    void copyHourClicked();
    void undoCopyClicked();
    void timeIntervalChanged(int);

private:
    Ui::BreakLayoutForm *ui;
    BreakLayout* mBreakLayout;
    std::unique_ptr<EntityDataModel> mEDMBreakLine;
    std::unique_ptr<EntityDataModel> mEdmTSetup;
    RavenSetup* mRavenSetup;
    int tempFromHr;
    int tempToHr;
    int startPos;
    int endPos;
};

#endif // BREAKLAYOUTFORM_H
