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
        std::vector<EntityRecord> entities();
        BaseEntity* findEntityByName(std::string name);
        void clearEntities();
        void setHeader();

        VecIter vecBegin();
        VecIter vecEnd();
    protected:
        void addEntity(std::unique_ptr<BaseEntity> entity);
        void addEntity(BaseEntity* entity);
        void deleteFromModel(const std::string name);
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
    ~EntityDataModel();

    void populateFields(BaseEntity* baseEntity);

    //std::vector<T*> entities() const;
    //void createEntity(std::unique_ptr<BaseEntity> entity);
    bool createEntity(BaseEntity* entity);
    void updateEntity(BaseEntity* entity);
    void deleteEntity(const std::string name, BaseEntity* entity);

    void cacheEntity(BaseEntity* entity);

    EntityRecord record(int i);

    void all();

    BaseEntity* entity() const { return mEntity; }

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

    void searchByField(std::tuple<std::string, std::string>);

private:
    BaseEntity* mEntity;
    BaseDatabaseManager* dbManager;

    void populateEntities();
};


#endif // ENTITYDATAMODEL_H
