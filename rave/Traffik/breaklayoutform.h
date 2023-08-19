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
    void clear_widgets() override;

    void populate_choice_combo_int(QComboBox* cbox, const ChoiceField<int>* cf);
    void populate_choice_combo_string(QComboBox* cbox, const ChoiceField<std::string>* cf);
    void populateCopyCB();
    void populateBreakLine();
    void set_defaults();
    void addBreakLines(int hour, int timeInterval=0);
    std::vector<EntityRecord> const& breakLines() const;
    void clearBreakTableView(int startRow, int endRow);
    void clearBreakLineModel(int startHr, int endHr);
    void copyHour(int fromHr, int toHr);
    void undoCopy(int fromHr, int toHr);
    void clearBreaks();

    QAbstractItemModel* breakline_model();

    template<typename T>
    void populateChoiceCombo(QComboBox* cbox, const T* cf)
    {
        for (const auto& [title, value] : cf->choices())
        cbox->addItem(stoq(title), value);

        cbox->setCurrentIndex( cbox->findData(QVariant(cf->value())) );
    }
private slots:
    void copyHourClicked();
    void undoCopyClicked();
    void timeIntervalChanged(int);
    void break_fill_method_changed(int);
    void test_data();
    void test_model();


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
