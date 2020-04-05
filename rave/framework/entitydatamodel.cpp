#include <QDebug>
#include "entitydatamodel.h"
#include "baseentity.h"

EntityModel::EntityModel()
    :mEntity{nullptr}
{
    //QStringList qs(header_labels);
    //this->setHorizontalHeaderLabels(qs);
}

EntityModel::~EntityModel()
{
    qDebug() << "EntityModel::dtor";
    qDebug() << "model.rowCount: BEFORE" << rowCount();
    //mEntities.clear();
    clear();
    qDebug() << "model.rowCount: AFTER" << rowCount();
}

EntityModel::EntityModel(BaseEntity* entity)
    :mEntity{entity}
{
    this->setHorizontalHeaderLabels(entity->tableHeaders());
}

std::vector<BaseEntity*> EntityModel::entities() const
{
    return mEntities;
}

void EntityModel::addEntity(BaseEntity* entity)
{
    addRow(entity);
    mEntities.push_back(entity);
}

void EntityModel::addRow(BaseEntity* entity)
{
   appendRow(entity->cols());
}
/* ----------- EntityDataModel ------------------ */

EntityDataModel::~EntityDataModel()
{
    for (auto e : entities())
        delete e;
}

EntityDataModel::EntityDataModel(BaseEntity* baseEntity)
    :EntityModel{baseEntity},
     mEntity{baseEntity}
{
    qDebug() << "EntityDataModel::ctor";
    dbManager = new PostgresDatabaseManager;
}

void EntityDataModel::populateFields(BaseEntity* baseEntity)
{
    qDebug() << "EntityDataModel::PopulateFields" ;
}

void EntityDataModel::populateEntity(BaseEntity& baseEntity)
{
    qDebug() << "EntityDataModel::populateEntity";
}

void EntityDataModel::saveEntity(BaseEntity* entity)
{
    addEntity(entity);
    dbManager->saveEntity(entity);
}

void EntityDataModel::all()
{

if (dbManager->fetchAll(mEntity) > 0){
   dbManager->provider()->cache()->first();
   do{
       auto e = dbManager->provider()->cache()->currentElement();
       auto eptr = mEntity->mapFields(e);
       addEntity(eptr);
       dbManager->provider()->cache()->next();
   }while(!dbManager->provider()->cache()->isLast());

}

}
