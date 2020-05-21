#ifndef ENTITYDATAMODEL_H
#define ENTITYDATAMODEL_H

#include <vector>
#include <memory>

#include <QStandardItemModel>
#include <QDebug>

#include "databasemanager.h"
#include "baseentity.h"
#include "dataprovider.h"


using EntityRecord = std::tuple<std::string, std::unique_ptr<BaseEntity>>;
using VecIter = std::vector<EntityRecord>::iterator;

class EntityModel : public QStandardItemModel{
    public:
        EntityModel();
        EntityModel(BaseEntity* entity);
        ~EntityModel();
        size_t entitiesCount();
        BaseEntity* findEntityByName(std::string name);
        void clearEntities();
        void setHeader();

        VecIter vecBegin();
        VecIter vecEnd();
    protected:
        void addEntity(std::unique_ptr<BaseEntity> entity);
        void addEntity(BaseEntity* entity);
        void deleteFromModel();
    private:
        BaseEntity* mEntity;
        std::vector<EntityRecord> mEntities;

        void addRow(BaseEntity* entity);
        BaseEntity* entityByID();
        bool editEntity(BaseEntity* oldEntity, BaseEntity* newEntity);
};

class EntityDataModel : public EntityModel
{
public:
    EntityDataModel();
    EntityDataModel(BaseEntity* baseEntity);
    EntityDataModel(BaseEntity& baseEntity);
    ~EntityDataModel();

    void populateFields(BaseEntity* baseEntity);

    //std::vector<T*> entities() const;
    //void createEntity(std::unique_ptr<BaseEntity> entity);
    bool createEntity(BaseEntity* entity);
    void updateEntity(BaseEntity* entity);
    //void deleteEntity(const std::string name, BaseEntity* entity);
    void deleteEntity(BaseEntity* entity);
    void deleteEntityByValue(std::tuple<ColumnName, ColumnValue> value);

    void cacheEntity(BaseEntity* entity);

    int createEntityDB(BaseEntity* entity);

    void all();
    void searchByField(std::tuple<std::string, std::string>);
    void searchById(std::tuple<std::string, int>);
    void getById(std::tuple<std::string, int>);

    BaseEntity* entity() const { return mEntity; }

    size_t count();

    void mapEntity(StringMap* map, BaseEntity& entity);
    void populateMToMDetails();

    class detail{
        public:
            template<typename Field, typename... Args, std::size_t... Indices>
                static auto create(const std::tuple<Field, Args...>& t, std::index_sequence<Indices...> seq){
                    return std::make_unique<Field>(std::get<Indices + 1>(t)...);
            }
    };

    template<typename Field, typename... Args>
    auto create(const std::tuple<Field, Args...>& t)->std::enable_if_t<std::is_constructible_v<Field, Args...>, std::unique_ptr<Field>>{
        return detail::create(t, std::index_sequence_for<Args...>{});
    }

    template<typename... Types>
    void filter(Types... args)
    {
        auto res = std::make_tuple( create (std::forward<Types>(args) )...);
    }


private:
    BaseEntity* mEntity;   // deleted at the base class
    BaseDatabaseManager* dbManager;

    void populateEntities();
};


#endif // ENTITYDATAMODEL_H
