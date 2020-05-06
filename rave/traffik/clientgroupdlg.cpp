#include <QDebug>
#include <QMdiArea>

#include "clientgroupdlg.h"
#include "clientgroupdetaildlg.h"
#include "clientgroup.h"
#include "ui_clientgroupdlg.h"
#include "../framework/entitydatamodel.h"
#include "../utils/tools.h"


ClientGroupDlg::ClientGroupDlg(QWidget *parent,
                               ClientGroup* cg) :
    BaseEntityBrowserDlg(parent, new ClientGroup),
    ui(new Ui::ClientGroupDlg),
    clientGroupDetailDlg{},
    clientGroup{cg}
{
    ui->setupUi(this);
    setDialogTitle("Client Groups");
}

ClientGroupDlg::~ClientGroupDlg()
{
    delete clientGroup;
    delete clientGroupDetailDlg;
    delete ui;
}

void ClientGroupDlg::addRecord()
{
    clientGroup = new ClientGroup();
    clientGroupDetailDlg = new ClientGroupDetailDlg(clientGroup);
    if (clientGroupDetailDlg->exec() > 0)
        entityDataModel()->createEntity(clientGroup);
}

void ClientGroupDlg::updateRecord()
{

   update<ClientGroup, ClientGroupDetailDlg>();

}



