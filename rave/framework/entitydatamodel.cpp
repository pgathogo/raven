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
    mEntities.clear();
    clear();
    qDebug() << "model.rowCount: AFTER" << rowCount();
}

EntityModel::EntityModel(BaseEntity* entity)
    :mEntity{entity}
{
    setHeader();
}

void EntityModel::setHeader()
{
    this->setHorizontalHeaderLabels(mEntity->tableHeaders());
}

/*
std::vector<EntityRecord> EntityModel::entities() const
{
    return mEntities;
}
*/

void EntityModel::addEntity(std::unique_ptr<BaseEntity> entity)
{
    BaseEntity* be = entity.get();
    addRow(be);
    std::string key = entity->searchColumn();
    // we need a way to check that key is not empty!!
    EntityRecord record = make_tuple(key, std::move(entity));
    mEntities.push_back(std::move(record));
}


void EntityModel::addRow(BaseEntity* entity)
{
   appendRow(entity->tableViewColumns());
}

BaseEntity* EntityModel::findEntityByName(std::string name)
{
    BaseEntity* be = nullptr;

    for (auto& record : mEntities){
        if (std::get<0>(record) == name){
            be = std::get<1>(record).get();
            break;
        }

    }

    return be;
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

void EntityModel::clearEntities()
{
    clear();
    mEntities.clear();
    setHeader();
}

/* ----------- EntityDataModel ------------------ */

EntityDataModel::~EntityDataModel()
{
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

void EntityDataModel::populateEntities()
{
    clearEntities();
    dbManager->provider()->cache()->first();
    do{
       auto e = dbManager->provider()->cache()->currentElement();
       auto eptr = mEntity->mapFields(e);
       addEntity(std::move(eptr));
       dbManager->provider()->cache()->next();
    }while(!dbManager->provider()->cache()->isLast());
}

void EntityDataModel::createEntity(std::unique_ptr<BaseEntity> entity)
{
    auto ptr(entity.get());
    int id = dbManager->createEntity(ptr);
    entity->setId(id);
    // Create many to many entites
    addEntity(std::move(entity));
}

void EntityDataModel::updateEntity(BaseEntity* entity)
{
    if (entity->id() > 0)
        dbManager->updateEntity(entity);
}

void EntityDataModel::deleteEntity(const std::string name, BaseEntity* entity)
{
    if (entity->id() > 0)
        dbManager->deleteEntity(entity);
    deleteFromModel(name);
}

void EntityDataModel::all()
{
    if (dbManager->fetchAll(mEntity) > 0)
        populateEntities();

}

void EntityDataModel::searchByField(std::tuple<std::string, std::string> searchItem)
{
    if (dbManager->searchByField(mEntity, searchItem) > 0)
        populateEntities();

}
