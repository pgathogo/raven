#include <QDebug>
#include <QMdiArea>

#include "clientgroupdlg.h"
#include "clientgroupdetaildlg.h"
#include "clientgroup.h"
#include "ui_clientgroupdlg.h"
#include "../framework/entitydatamodel.h"


ClientGroupDlg::ClientGroupDlg(QWidget *parent) :
    BaseEntityBrowserDlg(parent, new ClientGroup()),
    ui(new Ui::ClientGroupDlg),
    clientGroupDetailDlg{nullptr}
{
    ui->setupUi(this);
    setDialogTitle("Client Groups");
}

ClientGroupDlg::~ClientGroupDlg()
{
    delete ui;
    if (clientGroupDetailDlg != nullptr)
        delete clientGroupDetailDlg;
}

void ClientGroupDlg::addRecord()
{
    ClientGroup* cg = new ClientGroup();
    clientGroupDetailDlg = new ClientGroupDetailDlg(cg);
    if (clientGroupDetailDlg->exec() > 0)
        entityDataModel()->saveEntity(cg);
}

void ClientGroupDlg::updateRecord()
{
   std::string searchName = selectedRowName().toStdString();

   if (!searchName.empty()){
       BaseEntity* baseEntity = entityDataModel()->findRecordByName(searchName);

       if (baseEntity != nullptr){

           ClientGroup* cg = dynamic_cast<ClientGroup*>(baseEntity);
            clientGroupDetailDlg = new ClientGroupDetailDlg(cg);

            if (clientGroupDetailDlg->exec() > 0)
                entityDataModel()->saveEntity(cg);
       }
   }
}

void ClientGroupDlg::searchRecord()
{
    qDebug() << "ClientGroupDlg()::searchRecord";
    entityDataModel()->all();
}

