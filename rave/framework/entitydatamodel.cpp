#include <QDebug>
#include "entitydatamodel.h"
#include "baseentity.h"
#include "manytomany.h"
#include "../utils/tools.h"

EntityModel::EntityModel()
    :mEntity{nullptr}
{
}

EntityModel::~EntityModel()
{
    mEntities.clear();
    clear();
    qDebug() << "EntityModel::dtor";
}

EntityModel::EntityModel(std::unique_ptr<BaseEntity> entity)
    :mEntity{std::move(entity)}
{
    setHeader();
}

void EntityModel::setHeader()
{
    this->setHorizontalHeaderLabels(mEntity->tableHeaders());
}

void EntityModel::addEntity(std::unique_ptr<BaseEntity> entity)
{
    BaseEntity* be = entity.get();
    addRow(be->tableViewColumns());
    std::string key = entity->searchColumn();
    // we need a way to check that key is not empty!!
    EntityRecord record = make_tuple(key, std::move(entity));
    mEntities.push_back(std::move(record));
}


void EntityModel::addRow(const std::list<std::string> entity_cols)
{
    QList<QStandardItem*> view_names;
    for (std::string c : entity_cols)
        view_names << new QStandardItem(QString::fromStdString(c));
    appendRow(view_names);
}

BaseEntity* EntityModel::findEntityByName(const std::string name)
{
    // Returns a stored pointer of BaseEntity
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

std::vector<EntityRecord> const& EntityModel::modelEntities() const
{
    return mEntities;
}

std::string EntityModel::entityTableName() const
{
    return mEntity->tableName();
}

BaseEntity& EntityModel::getEntity()
{
    return *mEntity;
}

/* ----------- EntityDataModel ------------------ */

EntityDataModel::EntityDataModel():
    EntityModel{}
{
    dbManager = std::make_unique<PostgresDatabaseManager>();
}

EntityDataModel::EntityDataModel(std::unique_ptr<BaseEntity> baseEntity)
    :EntityModel{std::move(baseEntity)},
     dbManager{}
{
    dbManager = std::make_unique<PostgresDatabaseManager>();
}


EntityDataModel::~EntityDataModel()
{
    //delete dbManager;
}

void EntityDataModel::populateEntities()
{
    clearEntities();
    dbManager->provider()->cache()->first();
    do{
       auto e = dbManager->provider()->cache()->currentElement();
       auto ent = getEntity().cloneAsUnique();

       ent->baseMapFields(e);

       ent->afterMapping(*ent.get());

       //auto eptr = getEntity().mapFields(e);
       //addEntity(std::move(eptr));

       addEntity(std::move(ent));
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

void EntityDataModel::createEntity(std::unique_ptr<BaseEntity> entity)
{
    int id = dbManager->createEntity(entity.get());
    if (id > 0){
        entity->setId(id);
        addEntity(std::move(entity)); // entity final resting place
    }
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
}

void EntityDataModel::deleteEntityByValue(std::tuple<ColumnName, ColumnValue> value)
{
    dbManager->deleteEntityByValue(entityTableName(), value);
}

void EntityDataModel::all()
{
    if (dbManager->fetchAll(&getEntity()) > 0)
        populateEntities();
}

void EntityDataModel::cacheEntity(std::unique_ptr<BaseEntity> entity)
{
    addEntity(std::move(entity));
}

size_t EntityDataModel::count()
{
    return entitiesCount();
}

void EntityDataModel::searchByField(std::tuple<std::string, std::string> searchItem)
{
    if (dbManager->searchByField(&getEntity(), searchItem) > 0)
        populateEntities();

}

void EntityDataModel::searchById(std::tuple<std::string, int> searchItem)
{
    if (dbManager->searchById(&getEntity(), searchItem) > 0)
        populateEntities();
}

void EntityDataModel::getById(std::tuple<std::string, int> searchItem)
{
    if (dbManager->searchById(&getEntity(), searchItem) > 0){
        dbManager->provider()->cache()->first();
        auto e = dbManager->provider()->cache()->currentElement();
        mapEntity(e, getEntity());
    }
}

void EntityDataModel::mapEntity(StringMap* map, BaseEntity& entity)
{
        FieldValues fval = entity.mapping(map);
        for(auto& v : fval)
            entity.setValueByField(std::get<0>(v), std::get<1>(v));
}
