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
   appendRow(entity->tableViewColumns());
}

BaseEntity* EntityModel::findRecordByName(std::string name)
{
    for (auto& record : mEntities){
        if (std::get<0>(record) == name)
            return std::get<1>(record);
    }

    return nullptr;
}

void EntityModel::deleteFromModel(const std::string name)
{
    int i=0;
    for (auto& record : mEntities){
        if (std::get<0>(record) == name)
            break;
        ++i;
    }

    mEntities.erase(mEntities.begin()+i);
}

/* ----------- EntityDataModel ------------------ */

EntityDataModel::~EntityDataModel()
{
    for (auto record: entities())
        delete std::get<1>(record);

    delete mEntity;
    delete dbManager;
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
    if (entity->id() > 0){
        dbManager->updateEntity(entity);
    }else{
        dbManager->saveEntity(entity);
        addEntity(entity);
    }
}

void EntityDataModel::deleteEntity(const std::string name, BaseEntity* entity)
{
    if (entity->id() > 0)
        dbManager->deleteEntity(entity);
    deleteFromModel(name);
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
