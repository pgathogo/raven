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
    this->setTitle("Client Group");
    populateFields();
}

ClientGroupDetailDlg::~ClientGroupDetailDlg()
{
    delete ui;
}

void ClientGroupDetailDlg::saveRecord()
{
    clientGroup->setName(ui->edtName->text().toStdString());
    clientGroup->setDescription(ui->edtDesc->toPlainText().toStdString());
}

void ClientGroupDetailDlg::populateFields()
{
    ui->edtName->setText(QString::fromStdString(clientGroup->name()->valueToString()));
    ui->edtDesc->setText(QString::fromStdString(clientGroup->description()->valueToString()));
}

