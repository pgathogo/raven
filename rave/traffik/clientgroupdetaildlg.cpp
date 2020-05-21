#include "clientgroupdetaildlg.h"
#include "ui_baseentitydetaildlg.h"
#include "ui_clientgroupdetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "clientgroup.h"
#include "../utils/daypartgrid.h"

ClientGroupDetailDlg::ClientGroupDetailDlg(
                ClientGroup* cg, QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::ClientGroupDetailDlg),
    clientGroup{cg}
{
    ui->setupUi(bui->baseContainer);
    bindWidgets();
    setTitle(windowTitle());
    //DayPartGrid* dpg = new DayPartGrid(ui->vlGrid);
}

std::string ClientGroupDetailDlg::windowTitle()
{
    return "Client Group";
}

ClientGroupDetailDlg::~ClientGroupDetailDlg()
{
    delete ui;
    //delete clientGroup;
}

ActionResult ClientGroupDetailDlg::saveRecord()
{
    clientGroup->populateEntity();
    ActionResult ar =  clientGroup->validate();
    return ar;
}


void ClientGroupDetailDlg::bindWidgets()
{
    clientGroup->name()->setWidget(ui->edtName);
    clientGroup->description()->setWidget(ui->edtDesc);
}
