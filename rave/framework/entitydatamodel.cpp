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

std::vector<EntityRecord> EntityModel::entities() const
{
    return mEntities;
}

void EntityModel::addEntity(BaseEntity* entity)
{
    addRow(entity);
    std::string key = entity->searchColumn();
    // we need a way to check that key is not empty!!
    EntityRecord record = make_tuple(key, entity);
    mEntities.push_back(record);
}

void EntityModel::addRow(BaseEntity* entity)
{
   appendRow(entity->cols());
}

BaseEntity* EntityModel::findRecordByName(std::string name)
{
    for (auto& record : mEntities){
        if (std::get<0>(record) == name)
            return std::get<1>(record);
    }

    return nullptr;
}


/* ----------- EntityDataModel ------------------ */

EntityDataModel::~EntityDataModel()
{
    for (auto record: entities())
        delete std::get<1>(record);
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
