#ifndef ENTITYDATAMODEL_H
#define ENTITYDATAMODEL_H

#include <vector>
#include <memory>
#include <list>
#include <type_traits>

#include <QStandardItemModel>
#include <QDebug>
#include <QDate>

#include "databasemanager.h"
#include "baseentity.h"
#include "dataprovider.h"

using EntityRecord = std::tuple<std::string, std::unique_ptr<BaseEntity>>;
using VecIter = std::vector<EntityRecord>::iterator;

class EntityModel : public QStandardItemModel{
    public:
        EntityModel();
        //EntityModel(BaseEntity* entity);
        EntityModel(std::unique_ptr<BaseEntity> entity);
        ~EntityModel() override;
        size_t entitiesCount();
        BaseEntity* findEntityByName(const std::string name);
        void clearEntities();
        void setHeader();

        VecIter vecBegin();
        VecIter vecEnd();

        std::vector<EntityRecord> const& modelEntities() const;

        std::string entityTableName() const;

        BaseEntity& getEntity();

        BaseEntity* firstEntity();

        std::list<std::string> keys();
        void deleteFromModel();

        bool removeRows(int position, int rows, const QModelIndex& parent) override;

    protected:
        void addEntity(std::unique_ptr<BaseEntity> entity);
        //void addEntity(BaseEntity* entity);
    private:
        std::unique_ptr<BaseEntity> mEntity;
        std::vector<EntityRecord> mEntities;
        void addRow(const std::vector<std::string> entity_cols);
        //bool editEntity(BaseEntity* oldEntity, BaseEntity* newEntity);
};

class EntityDataModel : public EntityModel
{
public:
    EntityDataModel();
    EntityDataModel(std::unique_ptr<BaseEntity> baseEntity);
    ~EntityDataModel();

    int createEntity(std::unique_ptr<BaseEntity> entity);
    void cacheEntity(std::unique_ptr<BaseEntity> entity);

    void updateEntity(const BaseEntity& entity);
    void deleteEntity(const BaseEntity& entity);
    void deleteEntityByValue(std::tuple<ColumnName, ColumnValue> value);

    void deleteMarkedEntities();

    int createEntityDB(const BaseEntity& entity);

    void all();
    void searchByStr(std::tuple<std::string, std::string>);
    void searchByInt(std::tuple<std::string, std::string, int>);
    void getById(std::tuple<std::string, std::string, int>);
    void search(const std::string searchFilter);

    std::string make_insert_stmt(const BaseEntity& entity);
    void executeRawSQL(const std::string sql);
    int insert_returning_id(const std::string sql);
    void readRaw(const std::string sql);

    size_t count();


    void mapEntity(StringMap* map, BaseEntity& entity);
    void populateMToMDetails();

    void searchFilter(){}

    std::unique_ptr<BaseDatabaseManager> const& getDBManager() const;

    template<typename T>
    struct is_string :
            public std::disjunction<
            std::is_same<char*, typename std::decay<T>::type>,
            std::is_same<const char*, typename std::decay<T>::type>,
            std::is_same<std::string, typename std::decay<T>::type>
            >{
    };

    template<typename T, typename... Types>
    void searchFilter(T firstArg, Types... args)
    {
        // binding from a tuple
        auto [column, op, value] = firstArg;

        std::string filter = column;

        if constexpr(std::is_integral_v<decltype(value)> &&
                !std::is_same_v<decltype(value), bool>){
            //filter +=" = ";
            filter += op;
            filter += std::to_string(value);
        }else if constexpr(std::is_integral_v<decltype(value)> &&
                           std::is_same_v<decltype(value), bool>){
            //filter += " = ";
            filter += op;
            switch (value){
             case 0:
                filter += "false";
                break;
             case 1:
                filter += "true";
                break;
            default:
                filter += "false";
            }
        }else if constexpr(is_string<decltype(value)>::value){
            if (op == "LIKE"){
                filter += " LIKE ";
                std::string str_value = value;
                for(auto s : str_value)
                    std::tolower(s);
                filter += "'%"+str_value+"%'";
            }else{
                filter += op;
                filter += value;
            }

        }else if constexpr(std::is_same_v<decltype(value), QDate>){
            //filter += " = ";
            filter += op;
            std::string str_value = value.toString("yyyy-MM-dd").toStdString();
            for(auto s : str_value)
                std::tolower(s);
            filter += "'"+str_value+"'";
        }

        filterCache.push_back(filter);

        searchFilter(args...);
    }

    template<typename T, typename...Types>
    std::string prepareFilter(T firstArg, Types... args)
    {
        filterCache.clear();
        searchFilter(firstArg, std::forward<Types>(args)...);

        size_t n = 1;
        std::string sf{""};
        for (auto f : filterCache){
            sf += f;
            if ( n < filterCache.size())
                sf += " and ";
            ++n;
        }

        return sf;
    }

private:
    std::unique_ptr<BaseDatabaseManager> dbManager;
    void populateEntities();
    std::vector<std::string> filterCache;
};


#endif // ENTITYDATAMODEL_H
