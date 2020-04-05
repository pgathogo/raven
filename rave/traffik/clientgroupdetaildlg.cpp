#include "clientgroupdetaildlg.h"
#include "ui_baseentitydetaildlg.h"
#include "ui_clientgroupdetaildlg.h"
#include "clientgroup.h"

ClientGroupDetailDlg::ClientGroupDetailDlg(EntityDataModel* edm,
                QWidget *parent) :
    BaseEntityDetailDlg(edm, parent),
    ui(new Ui::ClientGroupDetailDlg)
{
    ui->setupUi(bui->baseContainer);
}

ClientGroupDetailDlg::~ClientGroupDetailDlg()
{
    delete ui;
}

void ClientGroupDetailDlg::saveRecord()
{
    ClientGroup* cg = new ClientGroup;
    cg->setName(ui->edtName->text().toStdString());
    cg->setDescription(ui->edtDesc->toPlainText().toStdString());
    this->save(cg);


}

