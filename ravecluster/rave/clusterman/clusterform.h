#ifndef CLUSTERFORM_H
#define CLUSTERFORM_H

#include <QDialog>
#include "../../../framework/baseentitydetaildlg.h"

namespace Ui {
    class ClusterForm;
}

namespace ClusterManager{
  class Cluster;
}

class ClusterForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit ClusterForm(ClusterManager::Cluster* cluster, QDialog* parent = nullptr);
    ~ClusterForm();

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

private:
    Ui::ClusterForm *ui;

    ClusterManager::Cluster* m_cluster;
};

#endif // CLUSTERFORM_H
