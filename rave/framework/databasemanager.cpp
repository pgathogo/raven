#include <tuple>
#include <algorithm>

#include "databasemanager.h"
#include "baseentity.h"
#include "dataprovider.h"

#include <QDebug>
#include "../utils/tools.h"

void BaseDatabaseManager::setObjectID(BaseEntity& entity, int id)
{
    //entity.setEntityId(id);
}

BaseDatabaseManager::~BaseDatabaseManager(){}

std::string BaseDatabaseManager::columnsForSelection(const BaseEntity& entity)
{

    std::string flds{};
    std::size_t i = 1;
    auto cols = entity.dbColumnNames();
    for (auto& col : cols){
           flds += col;
           if (i<cols.size())
               flds +=",";
           ++i;
    }

    return flds;
}

std::string BaseDatabaseManager::columnsForInsert(const BaseEntity &entity)
{
    std::string cols{};
    std::size_t i = 1;

    for (auto& [name, field] : entity.fields()){

        if ( name == "id" ){
            ++i;
            continue;
        }

        if ( field->formOnly() || field->readOnly() ){
            ++i;
            continue;
        }

        if (!cols.empty())
            cols += ",";
        cols += field->dbColumnName();
        ++i;

    }

    return cols;
}

std::string BaseDatabaseManager::commaSepColumns(const BaseEntity& entity)
{
    std::string flds{};
    std::size_t i = 1;
    auto cols = entity.dbColumnNames();
    for (auto& col : cols){
        if (col != "id"){
           flds += col;
           if (i<cols.size())
               flds +=",";
        }
           ++i;
    }

    return flds;
}


std::string BaseDatabaseManager::commaSepValues(const BaseEntity& entity)
{
    std::string vals{};

    for (auto& [name, field] : entity.fields()){

        if ( name == "id")
            continue;

        if (field->formOnly() || field->readOnly())
            continue;

        if (!vals.empty())
            vals +=",";
        vals += field->dbValueFormatter();
    }

    return vals;
}

std::string BaseDatabaseManager::makeInsertString(const BaseEntity& entity)
{
    //std::string flds = commaSepColumns(entity);
    std::string flds = columnsForInsert(entity);
    std::string vals = commaSepValues(entity);
    std::string in_a = "INSERT INTO "+entity.tableName()+"( "+flds+")";
    std::string in_b = " VALUES ("+vals+") ";
    std::string insert_stmt = in_a + in_b;
    return insert_stmt;
}

std::string BaseDatabaseManager::make_insert_returning_string(const BaseEntity& entity)
{
    std::string flds = columnsForInsert(entity);
    std::string vals = commaSepValues(entity);
    std::string in_a = "INSERT INTO "+entity.tableName()+"( "+flds+")";
    std::string in_b = " VALUES ("+vals+") RETURNING id; ";
    std::string insert_stmt = in_a + in_b;
    return insert_stmt;
}


std::string BaseDatabaseManager::makeUpdateString(const BaseEntity& entity)
{
   // std::string flds = commaSepColumns(entity);
    std::string flds = columnsForInsert(entity);
    std::string vals = commaSepValues(entity);
    std::string in_a = "UPDATE "+entity.tableName()+" SET ("+flds+")";
    std::string in_b = "=("+vals+") WHERE "+ entity.uniqueId()->dbColumnName() +"="+std::to_string(entity.id());
    std::string update_stmt = in_a + in_b;

    return update_stmt;
}

PostgresDatabaseManager::PostgresDatabaseManager()
{
    dataProvider =  new PostgresDataProvider;
    dataProvider->openConnection(mConninfo);
}

PostgresDatabaseManager::PostgresDatabaseManager(const std::string conninfo)
{

    dataProvider =  new PostgresDataProvider;
    dataProvider->openConnection(conninfo);
    mConninfo = conninfo;
}

PostgresDatabaseManager::~PostgresDatabaseManager()
{
    //delete dataProvider;
}

int PostgresDatabaseManager::createEntity(const BaseEntity& entity)
{
    std::string sqlQuery;

    sqlQuery = make_insert_returning_string(entity);

//    provider()->executeQuery(sqlQuery);

    int last_id = provider()->insert_returning_id(sqlQuery);
    return last_id;

}

void PostgresDatabaseManager::updateEntity(const BaseEntity& entity)
{
    std::string sqlQuery;
    sqlQuery = makeUpdateString(entity);
    provider()->executeQuery(sqlQuery);
}

int PostgresDatabaseManager::fetchAll(const BaseEntity& entity)
{
    std::string sql{};
    std::string flds = columnsForSelection(entity);
    sql = "SELECT " + flds + " FROM "+entity.tableName();  //+" ORDER BY "+entity.orderBy();

    return provider()->read(sql);
}

int PostgresDatabaseManager::searchByStr(const BaseEntity& entity, std::tuple<std::string, std::string> sf)
{
    std::string sql{};
    std::string flds = columnsForSelection(entity);
    sql = "SELECT "+flds+" FROM "+entity.tableName()+
                    " WHERE LOWER("+ std::get<0>(sf)+") LIKE '%"+
                    std::get<1>(sf)+"%'";
    return provider()->read(sql);
}

int PostgresDatabaseManager::searchByInt(const BaseEntity& entity, std::tuple<std::string, std::string, int> field_value)
{
    std::string sql{};
    std::string flds = columnsForSelection(entity);
    sql = "SELECT "+flds+" FROM "+entity.tableName()+
                    " WHERE "+ std::get<0>(field_value)+
             std::get<1>(field_value)+std::to_string(std::get<2>(field_value));

    return provider()->read(sql);
}

int PostgresDatabaseManager::search(const BaseEntity& entity, const std::string filter)
{
    std::string sql{};
    std::string flds = columnsForSelection(entity);
    sql = "SELECT "+flds+" FROM "+entity.tableName()+
                    " WHERE "+ filter+" ORDER BY "+entity.order_by();

    return provider()->read(sql);
}

int PostgresDatabaseManager::starts_with(const BaseEntity &entity, std::tuple<std::string, std::string>& sf)
{
    std::string sql{};
    std::string flds = columnsForSelection(entity);
    auto& [field_name, value] = sf;

    std::for_each(value.begin(), value.end(), [](char& c){
        c = ::tolower(c);
    });

    sql = "SELECT "+flds+" FROM "+entity.tableName()+
                    " WHERE LOWER("+ field_name +") LIKE '"+ value+"%'";
    return provider()->read(sql);
}

void PostgresDatabaseManager::executeRawSQL(const std::string sql)
{
    provider()->executeQuery(sql);
}

int PostgresDatabaseManager::readRaw(const std::string sql)
{
    return provider()->read(sql);
}

int PostgresDatabaseManager::insert_returning_id(const std::string sql)
{
    return provider()->insert_returning_id(sql);
}

int PostgresDatabaseManager::deleteEntity(const BaseEntity& entity)
{
    std::string sql{};
    sql = "DELETE FROM "+entity.tableName()+" WHERE ID ="+std::to_string(entity.id());
    return provider()->executeQuery(sql);
}

int PostgresDatabaseManager::deleteEntityByValue(const std::string table_name,
            std::tuple<ColumnName, ColumnValue> column)
{
    std::string sql{};
    sql = "DELETE FROM "+table_name+" WHERE "+
            std::get<0>(column)+" = "+
           std::to_string(std::get<1>(column));
    return provider()->executeQuery(sql);
}

void PostgresDatabaseManager::loadEntity(BaseEntity& entity)
{
    srand(time(NULL));

    int randID = rand() % 100 + 1;

    // result = Select * from <TableName> where id = entity.ID;
    setObjectID(entity, randID);
    // updateFields
    populateObject(entity);
}

BaseDataProvider* PostgresDatabaseManager::provider()
{
    return dataProvider;
}

std::string PostgresDatabaseManager::make_insert_stmt(const BaseEntity &entity)
{
    return makeInsertString( entity );
}

PostgresDataProvider *PostgresDatabaseManager::pgProvider()
{
    return dataProvider;
}

std::string PostgresDatabaseManager::manager_type()
{
    return "POSTGRES";
}

/* -------- SQLiteDatabaseManager ------------------------------------ */

SQLiteDatabaseManager::SQLiteDatabaseManager()
{
    //data_provider = new SQLiteDataProvider;

}

SQLiteDatabaseManager::SQLiteDatabaseManager(const std::string dbname)
{
    m_data_provider =  new SQLiteDataProvider(dbname.c_str());

}

SQLiteDatabaseManager::~SQLiteDatabaseManager()
{

}

std::string SQLiteDatabaseManager::makeUpdateString(const BaseEntity& entity)
{
   std::string flds = columnsForInsert(entity);
   std::string vals = commaSepValues(entity);

   std::vector<std::string> fld_tokens = split_string(flds, ',');
   std::vector<std::string> val_tokens = split_string(vals, ',');

   std::string upd_field{""};

   for(int i=0; i<fld_tokens.size(); ++i){
       upd_field += fld_tokens[i]+"="+val_tokens[i];

       if (i < fld_tokens.size()-1)
           upd_field += ",";
   }

   std::string in_a = "UPDATE "+entity.tableName()+" SET "+ upd_field;
   std::string in_b = " WHERE "+entity.uniqueId()->dbColumnName()+"="+std::to_string(entity.id());
   std::string update_stmt = in_a + in_b;

   return update_stmt;

}

void SQLiteDatabaseManager::updateEntity(const BaseEntity& entity)
{
    std::string sqlQuery;
    sqlQuery = makeUpdateString(entity);

    provider()->executeQuery(sqlQuery);

}

int SQLiteDatabaseManager::createEntity(const BaseEntity& entity)
{
    std::string sql_string;

    sql_string = makeInsertString(entity);

    int last_id = m_data_provider->insert_returning_id(sql_string);

    return last_id;
}

int SQLiteDatabaseManager::deleteEntity(const BaseEntity& entity)
{

}
int SQLiteDatabaseManager::deleteEntityByValue(
    const std::string tabale_name,
    std::tuple<ColumnName, ColumnValue>)
{

}

int SQLiteDatabaseManager::fetchAll(const BaseEntity& entity)
{
    std::string sql{};
    std::string flds = columnsForSelection(entity);
    sql = "SELECT " + flds + " FROM "+entity.tableName();  //+" ORDER BY "+entity.orderBy();

    return provider()->read(sql);
}

int SQLiteDatabaseManager::searchByStr(const BaseEntity& entity,
          std::tuple<std::string, std::string> sf)
{

}

int SQLiteDatabaseManager::searchByInt(const BaseEntity& entity,
           std::tuple<std::string,std::string,  int> field_value)
{

}

int SQLiteDatabaseManager::search(const BaseEntity& entity, const std::string filter)
{

}

int SQLiteDatabaseManager::starts_with(const BaseEntity& entity,
        std::tuple<std::string, std::string>& sf)
{

}
void SQLiteDatabaseManager::executeRawSQL(const std::string sql)
{

}
int SQLiteDatabaseManager::readRaw(const std::string sql)
{

}
int SQLiteDatabaseManager::insert_returning_id(const std::string sql)
{

}

BaseDataProvider* SQLiteDatabaseManager::provider()
{
    return m_data_provider;
}

std::string SQLiteDatabaseManager::make_insert_stmt(const BaseEntity& entity)
{

}

void SQLiteDatabaseManager::loadEntity(BaseEntity& entity)
{

}
std::string SQLiteDatabaseManager::db_name()
{
    auto s = m_data_provider->db_name();
    return s;
}

std::string SQLiteDatabaseManager::manager_type()
{
    return m_data_provider->db_name();
}


