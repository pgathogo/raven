#ifndef SETUPFORM_H
#define SETUPFORM_H

#include <set>
#include <QWidget>
#include "../../../rave/framework/baseentitydetaildlg.h"

namespace Ui {
class SetupForm;
}

class RavenSetup;
class OrderApprover;

template<typename T>
class ChoiceField;

class SetupForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit SetupForm(RavenSetup* setup,
                       QDialog* parent = nullptr);
    ~SetupForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

    void save_approvers();

private slots:
      void agencyCommTypeChanged(int);
      void saleRepCommTypeChanged(int);
      void billCycleChanged(int);
      void revenueTypeChanged(int);
      void billingTypeChanged(int);
      void billingBasisChanged(int);
      void addApprover();
      void deleteApprover();
      std::shared_ptr<BaseEntity> findSelectedEntity();
      QString selectedRowName();
      int selectedRowId() const;
      void set_audio_path();
      void set_comm_audio_path();
      void on_template_filepath();
      void on_output_path();
      void on_viewer_path();
      void on_runner_path();

private:
    Ui::SetupForm *ui;
    RavenSetup* m_setup;
    std::unique_ptr<EntityDataModel> m_edm_approver;

    void load_order_approvers();
    std::set<int> make_aprv_levels();
    void populate_choice_combo(QComboBox* cbox, const ChoiceField<std::string>* cf);

    QString get_audio_folder(QString default_folder);
};

#endif // SETUPFORM_H
