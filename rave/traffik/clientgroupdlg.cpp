#include <QDebug>
#include <QMdiArea>

#include "clientgroupdlg.h"
#include "clientgroupdetaildlg.h"
#include "clientgroup.h"
#include "ui_clientgroupdlg.h"
#include "../framework/entitydatamodel.h"

ClientGroupDlg::ClientGroupDlg(QWidget *parent) :
    BaseEntityBrowserDlg(parent, new ClientGroup()),
    ui(new Ui::ClientGroupDlg)
{
    ui->setupUi(this);
    setDialogTitle("Client Groups");
}

ClientGroupDlg::~ClientGroupDlg()
{
    delete ui;
    delete clientGroupDetailDlg;
}

void ClientGroupDlg::AddRecord()
{
    clientGroupDetailDlg = new ClientGroupDetailDlg(
                            entityDataModel());
    //mMdiArea->addSubWindow(clientGroupDetailDlg);
    clientGroupDetailDlg->show();
}

void ClientGroupDlg::searchRecord()
{
    qDebug() << "ClientGroupDlg()::searchRecord";
    entityDataModel()->all();
}
