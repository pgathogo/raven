#include <QDebug>
#include <QMdiArea>

#include "clientgroupdlg.h"
#include "clientgroupdetaildlg.h"
#include "clientgroup.h"
#include "ui_clientgroupdlg.h"
#include "../framework/entitydatamodel.h"
#include "../utils/tools.h"


ClientGroupDlg::ClientGroupDlg(QWidget *parent):
    BaseEntityBrowserDlg(parent, new ClientGroup),
    ui(new Ui::ClientGroupDlg)
{
    ui->setupUi(this);
    setDialogTitle("Client Groups");
}

ClientGroupDlg::~ClientGroupDlg()
{
    delete ui;
}

void ClientGroupDlg::addRecord()
{
    std::unique_ptr<ClientGroup> cg = std::make_unique<ClientGroup>();
    std::unique_ptr<ClientGroupDetailDlg> cgdForm =
            std::make_unique<ClientGroupDetailDlg>(cg.get());
    if (cgdForm->exec() > 0)
        entityDataModel()->createEntity(std::move(cg));
}

void ClientGroupDlg::updateRecord()
{

   update<ClientGroup, ClientGroupDetailDlg>();

}

std::string ClientGroupDlg::typeID()
{
    return "ClientGroupDlg";
}


