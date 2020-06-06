#include <QDebug>
#include "entitydatamodel.h"
#include "baseentity.h"
#include "manytomany.h"

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
    qDebug() << "EntityModel::dtor";
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

void EntityModel::deleteFromModel()
{
    int i=0;
    for (auto& record : mEntities){
        if (std::get<1>(record)->dbAction() == DBAction::dbaDELETE)
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
size_t EntityModel::entitiesCount()
{
    return mEntities.size();
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

EntityDataModel::EntityDataModel(BaseEntity& baseEntity)
    :EntityModel{}
    ,mEntity{&baseEntity}
    ,dbManager{}
{
    dbManager = new PostgresDatabaseManager;
}

EntityDataModel::~EntityDataModel()
{
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

void EntityDataModel::populateMToMDetails()
{
    std::vector<int> ids;
    VecIter it = vecBegin();
    for (; it != vecEnd(); ++it){
        ManyToMany* mtom = dynamic_cast<ManyToMany*>(std::get<1>(*it).get());
        ids.push_back(mtom->detailId()->value());
    }

}

bool EntityDataModel::createEntity(BaseEntity* entity)
{
    // This code need to be split into three!!
    bool succeded = false;
    int id = dbManager->createEntity(entity);
    if (id > 0){
        entity->setId(id);
        addEntity(entity);
        succeded = true;
    }
    return succeded;
}

int EntityDataModel::createEntityDB(BaseEntity* entity)
{
    return dbManager->createEntity(entity);
}

void EntityDataModel::updateEntity(BaseEntity* entity)
{
    if (entity->id() > 0)
        dbManager->updateEntity(entity);
}

void EntityDataModel::deleteEntity(BaseEntity* entity)
{
    // entities with ids>0 have already been save in database
    if (entity->id() > 0)
        dbManager->deleteEntity(entity);
    //deleteFromModel(); // No need for this!
}

//void EntityDataModel::deleteEntityById(BaseEntity* entity, int id)
void EntityDataModel::deleteEntityByValue(std::tuple<ColumnName, ColumnValue> value)
{
    dbManager->deleteEntityByValue(mEntity, value);
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

size_t EntityDataModel::count()
{
    return entitiesCount();
}

void EntityDataModel::searchByField(std::tuple<std::string, std::string> searchItem)
{
    if (dbManager->searchByField(mEntity, searchItem) > 0)
        populateEntities();

}

void EntityDataModel::searchById(std::tuple<std::string, int> searchItem)
{
    if (dbManager->searchById(mEntity, searchItem) > 0)
        populateEntities();
}

void EntityDataModel::getById(std::tuple<std::string, int> searchItem)
{
    if (dbManager->searchById(mEntity, searchItem) > 0){
        dbManager->provider()->cache()->first();
        auto e = dbManager->provider()->cache()->currentElement();
        mapEntity(e, *mEntity);
    }
}

void EntityDataModel::mapEntity(StringMap* map, BaseEntity& entity)
{
        FieldValues fval = entity.mapping(map);
        for(auto& v : fval)
            entity.setValueByField(std::get<0>(v), std::get<1>(v));
}
