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
#include "relationmapper.h"

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

        std::unique_ptr<BaseEntity> const& get_entity() const;

        BaseEntity* firstEntity();

        std::list<std::string> keys();
        void deleteFromModel();

        bool removeRows(int position, int rows, const QModelIndex& parent) override;

        void add_entity(std::unique_ptr<BaseEntity> entity);

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

    void starts_with(std::tuple<std::string, std::string>);

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
            filter = "lower("+filter+")";
            if (str_tolower(op) == "like"){
                filter += " LIKE ";
                std::string str_value = value;
//                for(auto s : str_value)
//                    std::tolower(s);
                str_value = str_tolower(str_value);
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

    std::string str_tolower(std::string s){
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c);});
        return s;
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

    template<typename T, typename... Types>
    void related_query(const T& firstArg,  const Types&... args)
    {
        auto fields = firstArg.dbColumnNames();
        m_relation_mapper->append_related_tables(firstArg.tableName(), &typeid(T));

        for(auto f : fields){
            std::string full_field_name = firstArg.tableName()+"."+f;
            m_relation_mapper->append_query_fields(full_field_name);
        }

        if constexpr(sizeof...(args) > 0 ) {
            related_query(args...);
        }

    }

    template<typename T, typename... Types>
    FRAMEWORK::RelationMapper* select_related(T& firstArg, Types&... args)
    {
        m_relation_mapper->clear_query_fields();
        m_relation_mapper->clear_related_tables();
        m_relation_mapper->clear_foreign_key_fields();
        m_relation_mapper->clear_query_results();
        m_relation_mapper->clear_mapped_entities();

        auto fields = getEntity().dbColumnNames();

        m_relation_mapper->set_main_entity(&(getEntity()));
        m_relation_mapper->set_main_table(getEntity().tableName());

        for(auto f : fields){
            std::string full_field_name = getEntity().tableName()+"."+f;
            m_relation_mapper->append_query_fields(full_field_name);
        }

        for (auto const& [field_name, field] : getEntity().fields()){
            if (field->field_type() == "ForeignKeyField"){
                auto fk_field = dynamic_cast<ForeignKeyField*>(field.get());
                auto fk_field_entity = std::make_tuple(fk_field->fk_entity(), field->nullable());
                m_relation_mapper->append_foreign_key_fields(field_name, fk_field_entity);
            }
        }

        related_query(firstArg, std::forward<Types>(args)...);

        m_relation_mapper->print_query_fields();
        m_relation_mapper->print_related_tables();
        m_relation_mapper->print_foreign_key_fields();

        m_relation_mapper->build_relation_tree();
        m_relation_mapper->print_relation_tree();

        m_relation_mapper->prepare_query();

        return m_relation_mapper.get();

//		readRaw(m_relation_mapper.query());
    }

    template<typename T, typename...Types>
    void select_related_chain(T firstArg, Types...args)
    {
        m_relation_mapper->clear_query_fields();
        m_relation_mapper->clear_related_tables();
        m_relation_mapper->clear_foreign_key_fields();

        auto column_names = getEntity().dbColumnNames();

        m_relation_mapper->set_main_entity(&(getEntity()));
        m_relation_mapper->set_main_table(getEntity().tableName());

        for(auto column_name : column_names){
            std::string full_field_name = m_relation_mapper->main_table()+"."+column_name;
            m_relation_mapper->append_query_fields(full_field_name);
        }

        m_relation_mapper->fetch_fk_fields(&getEntity());
//        fetch_related_entity_columns(firstArg, std::forward<Types>(args)...);

        qDebug() << "Done fetch_related_entity_columns";

        m_relation_mapper->print_foreign_key_fields();

        /*
        m_relation_mapper->make_chain_join_statments(firstArg, std::forward<Types>(args)...);

        qDebug() << "After make_chain_join_statments";

        m_relation_mapper->print_join_statements_chain();

        qDebug() << "After print_join_statement";
        */


    }

    template<typename T, typename...Types>
    void fetch_related_entity_columns(T firstArg, Types...args)
    {
        auto [first_entity, second_entity] = firstArg;

        qDebug()<< "Second TableName: "<< stoq(second_entity->tableName());

//        m_relation_mapper->fetch_fk_fields(first_entity);

        m_relation_mapper->fetch_fk_fields(second_entity);

        return;


                /*
        m_relation_mapper->append_related_tables(first_entity->tableName(), &typeid(T));
        m_relation_mapper->append_related_tables(second_entity->tableName(), &typeid(T));

        auto first_column_names = first_entity->dbColumnNames();
        for(auto column : first_column_names){
            std::string full_column_name = first_entity->tableName()+"."+column;
            m_relation_mapper->append_query_fields(full_column_name);
        }

        m_relation_mapper->fetch_fk_fields(first_entity);

        auto second_column_names = second_entity->dbColumnNames();
        for(auto column : second_column_names){
            std::string full_column_name = second_entity->tableName()+"."+column;
            m_relation_mapper->append_query_fields(full_column_name);
        }

        m_relation_mapper->fetch_fk_fields(second_entity);
                */

        if constexpr(sizeof...(args) > 0 ){
            fetch_related_entity_columns(args...);
        }

    }

private:
    std::unique_ptr<BaseDatabaseManager> dbManager;
    void populateEntities();
    std::vector<std::string> filterCache;
    std::unique_ptr<FRAMEWORK::RelationMapper> m_relation_mapper;
};


#endif // ENTITYDATAMODEL_H
