#include "clientgroupdetaildlg.h"
#include "ui_baseentitydetaildlg.h"
#include "ui_clientgroupdetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "clientgroup.h"

ClientGroupDetailDlg::ClientGroupDetailDlg(
                ClientGroup* cg, QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::ClientGroupDetailDlg),
    clientGroup{cg}
{
    ui->setupUi(bui->baseContainer);
    bindWidgets();
    setTitle(title());
}

std::string ClientGroupDetailDlg::title()
{
    return "Client Group";
}

ClientGroupDetailDlg::~ClientGroupDetailDlg()
{
    delete ui;
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
