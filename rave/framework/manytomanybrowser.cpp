#include <QVBoxLayout>

#include "manytomanybrowser.h"
#include "ui_manytomanybrowser.h"
#include "ui_baseentitybrowserdlg.h"
#include "picklistbrowser.h"
#include "manytomany.h"

ManyToManyBrowser::ManyToManyBrowser(
        ManyToMany* mtom,
        QVBoxLayout* layout,
        QWidget *parent
        ):
    BaseEntityBrowserDlg(parent,
                         ManyToManyBrowser::createMtoM(mtom)),
    ui(new Ui::ManyToManyBrowser),
    plb{},
    mMtoM{mtom}
{
    ui->setupUi(this);
    hideEditButton();
    layout->addWidget(this);

    if (mMtoM->parentId()->value() > 0){
        std::string columnName = mtom->parentId()->dbColumnName();
        int value = mMtoM->parentId()->value();
        auto needle = std::make_tuple(columnName, value);
        // VoiceExclusion...
        entityDataModel()->searchById(needle);
    }

}

ManyToManyBrowser::~ManyToManyBrowser()
{
    delete plb;
    delete ui;
}

void ManyToManyBrowser::addRecord()
{
    PickListSetting plset;
    plset.listEntity = mMtoM->detailEntity();

    plb = new PickListBrowser(plset);
    plb->exec();

    auto it = plset.selectedEntities.begin();
    for(; it != plset.selectedEntities.end(); ++it){
        BaseEntity* be = *it;
        if (be->dbAction() == DBAction::dbaCREATE){
            auto m2m = createMtoM(mMtoM, be);
            m2m->setParentId(mMtoM->parentId()->value());
            m2m->setDetailId(be->id());
            m2m->setTableName(mMtoM->tableName());
            entityDataModel()->cacheEntity(m2m);
        }
    }
}

void ManyToManyBrowser::updateRecord()
{
}

void ManyToManyBrowser::deleteRecord()
{
   std::string searchName = selectedRowName().toStdString();
   BaseEntity* entity = entityDataModel()->findEntityByName(searchName);
   entity->setDBAction(DBAction::dbaDELETE);
   bui->tvEntity->hideRow(selectedRowId());
   //removeSelectedRow();
}

void ManyToManyBrowser::selectedItem(BaseEntity* entity)
{

    qDebug() << QString::fromStdString(entity->tableName());
}

std::string ManyToManyBrowser::typeID()
{
    return "ManyToManyBrowser";
}
