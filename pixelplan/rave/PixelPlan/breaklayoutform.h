#ifndef BREAKLAYOUTFORM_H
#define BREAKLAYOUTFORM_H

#include <QDialog>
#include "../../../rave/framework/baseentitydetaildlg.h"

namespace Ui {
class BreakLayoutForm;
}

class BreakLayout;
class EntityDataModel;
class RavenSetup;

template<typename T>
class ChoiceField;


namespace PIXELPLAN {
class TVProgram;
}

class BreakLayoutForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit BreakLayoutForm(BreakLayout* bl, std::vector<int> excl_progids,
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
    void setup_ui();
    void add_break_lines(std::shared_ptr<PIXELPLAN::TVProgram>, int timeInterval=0);
    std::vector<EntityRecord> const& breakLines() const;
    void clearBreakTableView(int startRow, int endRow);
    void clearBreakLineModel(int startHr, int endHr);
    void copyHour(int fromHr, int toHr);
    void undoCopy(int fromHr, int toHr);
    void clearBreaks();
    void set_break_fill_method(QString);

    QAbstractItemModel* breakline_model();
    // std::shared_ptr<EntityDataModel> breakline_model();
    QModelIndexList selected_indexes();

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
    void on_tvprogram_changed(int i);
    void break_fill_method_changed(int);
    void test_model();
    void insert_row();
    void delete_row_TEST();
    void delete_row();

private:
    void populate_program_combo(std::vector<int>);
    void show_breaklines_from_db(int);

    std::tuple<int, int> row_identity();

    Ui::BreakLayoutForm *ui;
    BreakLayout* mBreakLayout;
    std::shared_ptr<EntityDataModel> m_edm_breakline;
    std::unique_ptr<EntityDataModel> mEdmTSetup;
    RavenSetup* mRavenSetup;

    std::unique_ptr<EntityDataModel> m_edm_tvprogram;
    std::shared_ptr<PIXELPLAN::TVProgram> m_current_tvprogram;

    std::vector<QTime> m_deleted_item;


    int tempFromHr;
    int tempToHr;
    int startPos;
    int endPos;

    int m_line_no;
};

#endif // BREAKLAYOUTFORM_H
