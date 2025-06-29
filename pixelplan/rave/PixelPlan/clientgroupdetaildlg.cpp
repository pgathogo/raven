#include "clientgroupdetaildlg.h"
#include "ui_clientgroupdetaildlg.h"
#include "../../../rave/framework/ui_baseentitydetaildlg.h"
#include "../../../rave/framework/baseentitydetaildlg.h"
#include "clientgroup.h"
#include "../../../rave/utils/daypartgrid.h"
#include "../../../rave/utils/tools.h"

ClientGroupDetailDlg::ClientGroupDetailDlg(
                ClientGroup* cg, QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::ClientGroupDetailDlg),
    clientGroup{cg}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    populateFormWidgets();
}

std::string ClientGroupDetailDlg::windowTitle()
{
    return "Client Group";
}

ClientGroupDetailDlg::~ClientGroupDetailDlg()
{
    delete ui;
}

ActionResult ClientGroupDetailDlg::saveRecord()
{
    populateEntityFields();
    ActionResult ar =  clientGroup->validate();
    return ar;
}

void ClientGroupDetailDlg::populateFormWidgets()
{
    ui->edtName->setText(stoq(clientGroup->name()->value()));
    ui->edtDesc->setText(stoq(clientGroup->description()->value()));
}

void ClientGroupDetailDlg::populateEntityFields()
{
    clientGroup->name()->setValue(ui->edtName->text().toStdString());
    clientGroup->description()->setValue(ui->edtDesc->toPlainText().toStdString());
}

void ClientGroupDetailDlg::clear_widgets()
{
    ui->edtName->clear();
    ui->edtDesc->clear();
}

