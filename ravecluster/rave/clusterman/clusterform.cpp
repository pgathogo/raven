#include "clusterform.h"
#include "ui_clusterform.h"

#include "../../../rave/framework/ui_baseentitydetaildlg.h"
#include "../../../rave/framework/baseentitydetaildlg.h"

#include "cluster.h"

ClusterForm::ClusterForm(ClusterManager::Cluster* cluster, QDialog* parent) :
    BaseEntityDetailDlg(parent)
    ,ui(new Ui::ClusterForm)
    ,m_cluster{cluster}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    populateFormWidgets();

    ui->edtClusterName->setFocusPolicy(Qt::StrongFocus);
    ui->edtClusterName->setFocus();
}


ClusterForm::~ClusterForm()
{
    delete ui;
}

std::string ClusterForm::windowTitle()
{
    return "Cluster";
}

ActionResult ClusterForm::saveRecord()
{
    populateEntityFields();
    ActionResult ar = m_cluster->validate();
    return ar;
}

void ClusterForm::populateEntityFields()
{
    m_cluster->cluster_name()->setValue(ui->edtClusterName->text().toStdString());
    m_cluster->notes()->setValue(ui->edtNotes->toPlainText().toStdString());
}

void ClusterForm::populateFormWidgets()
{
    ui->edtClusterName->setText(stoq(m_cluster->cluster_name()->value()));
    ui->edtNotes->setText(stoq(m_cluster->notes()->value()));
}

