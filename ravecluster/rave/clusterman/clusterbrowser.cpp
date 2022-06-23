#include "clusterbrowser.h"
#include "ui_clusterbrowser.h"

#include "cluster.h"

ClusterBrowser::ClusterBrowser(QWidget *parent)
    :BaseEntityBrowserDlg(parent, std::make_unique<ClusterManager::Cluster>())
    ,ui(new Ui::ClusterBrowser)
{
    ui->setupUi(this);
    setDialogTitle("Cluster Browser");
}

ClusterBrowser::~ClusterBrowser()
{
    delete ui;
}

void ClusterBrowser::addRecord()
{

}

void ClusterBrowser::updateRecord()
{

}

void ClusterBrowser::searchRecord()
{

}
