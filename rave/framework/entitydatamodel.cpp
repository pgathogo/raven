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
    delete mEntity;
    mEntities.clear();
    clear();
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

void EntityModel::addEntity(BaseEntity* entity)
{
    //BaseEntity* be = entity.get();
    addRow(entity);
    std::string key = entity->searchColumn();
    // we need a way to check that key is not empty!!
    std::unique_ptr<BaseEntity> uPtr(entity);
    EntityRecord record = make_tuple(key, std::move(uPtr));
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

VecIter EntityModel::vecBegin()
{
    return mEntities.begin();
}

VecIter EntityModel::vecEnd()
{
    return mEntities.end();
}

/* ----------- EntityDataModel ------------------ */


EntityDataModel::EntityDataModel():
    EntityModel{},
    mEntity{},
    dbManager{}
{
    qDebug() << "EntityDataModel::Default Ctor";
    dbManager = new PostgresDatabaseManager;
}

EntityDataModel::EntityDataModel(BaseEntity* baseEntity)
    :EntityModel{baseEntity},
     mEntity{baseEntity},
     dbManager{}
{
    qDebug() << "EntityDataModel::ctor";
    dbManager = new PostgresDatabaseManager;
}

EntityDataModel::~EntityDataModel()
{
    delete mEntity;
    delete dbManager;
    qDebug() << "EntityDataModel::dtor";
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

/*
void EntityDataModel::createEntity(std::unique_ptr<BaseEntity> entity)
{
    auto ptr(entity.get());
    int id = dbManager->createEntity(ptr);
    entity->setId(id);
    // Create many to many entites
    //addEntity(std::move(entity));
}
*/


bool EntityDataModel::createEntity(BaseEntity* entity)
{
    //auto ptr(entity.get());
    bool succeded = false;
    int id = dbManager->createEntity(entity);
    if (id > 0){
        entity->setId(id);
        //std::unique_ptr<BaseEntity> uPtr = std::make_unique<BaseEntity>();
        // Create many to many entites
        addEntity(entity);
        succeded = true;
    }
    return succeded;
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

void EntityDataModel::cacheEntity(BaseEntity* entity)
{
    addEntity(entity);
}

void EntityDataModel::searchByField(std::tuple<std::string, std::string> searchItem)
{
    if (dbManager->searchByField(mEntity, searchItem) > 0)
        populateEntities();

}

EntityRecord EntityDataModel::record(int i)
{
}

