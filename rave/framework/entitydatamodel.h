#ifndef ENTITYDATAMODEL_H
#define ENTITYDATAMODEL_H

#include <vector>
#include <memory>

#include <QStandardItemModel>
#include <QDebug>

#include "databasemanager.h"
#include "baseentity.h"
#include "dataprovider.h"


//class BaseEntity;

class EntityModel : public QStandardItemModel{
    public:
        EntityModel();
        EntityModel(BaseEntity* entity);
        ~EntityModel();
        std::vector<BaseEntity*> entities() const;
    protected:
        void addEntity(BaseEntity* entity);
    private:
        BaseEntity* mEntity;
        std::vector<BaseEntity*> mEntities;
        void addRow(BaseEntity* entity);
        BaseEntity* entityByID();
        bool editEntity(BaseEntity* oldEntity, BaseEntity* newEntity);
        bool deleteEntity(BaseEntity* entity);
        bool deleteItemByName(std::string name);
};

class EntityDataModel : public EntityModel
{
public:
    EntityDataModel(BaseEntity* baseEntity);
    ~EntityDataModel();

    void populateFields(BaseEntity* baseEntity);
    void populateEntity(BaseEntity& baseEntity);

    //std::vector<T*> entities() const;
    void saveEntity(BaseEntity* entity);

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

private:
    BaseEntity* mEntity;
    BaseDatabaseManager* dbManager;
};


#endif // ENTITYDATAMODEL_H
