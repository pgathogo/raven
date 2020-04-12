#include <QDebug>
#include <QMdiArea>

#include "clientgroupdlg.h"
#include "clientgroupdetaildlg.h"
#include "clientgroup.h"
#include "ui_clientgroupdlg.h"
#include "../framework/entitydatamodel.h"

#include "../utils/tools.h"


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
    std::unique_ptr<ClientGroup> ucg = std::make_unique<ClientGroup>();
    auto ptr(ucg.get());
    clientGroupDetailDlg = new ClientGroupDetailDlg(ptr);
    if (clientGroupDetailDlg->exec() > 0)
        entityDataModel()->createEntity(std::move(ucg));
}

void ClientGroupDlg::updateRecord()
{
   std::string searchName = selectedRowName().toStdString();


   if (!searchName.empty()){
       BaseEntity* baseEntity = entityDataModel()->findEntityByName(searchName);

       if (baseEntity != nullptr){

           ClientGroup* cg = dynamic_cast<ClientGroup*>(baseEntity);

            printstr(cg->name()->valueToString());

            clientGroupDetailDlg = new ClientGroupDetailDlg(cg);

            if (clientGroupDetailDlg->exec() > 0){
                updateTableViewRecord(cg);
                entityDataModel()->updateEntity(cg);
             }
       }
   }
}



