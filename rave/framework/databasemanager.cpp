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

std::string BaseDatabaseManager::columnsForSelection(BaseEntity* entity)
{

    std::string flds;
    std::size_t i = 1;
    auto cols = entity->dbColumnNames();
    for (auto& col : cols){
           flds += col;
           if (i<cols.size())
           flds +=",";
           ++i;
    }

    return flds;
}

std::string BaseDatabaseManager::commaSepColumns(BaseEntity* entity)
{
    std::string flds;
    std::size_t i = 1;
    auto cols = entity->dbColumnNames();
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

std::string BaseDatabaseManager::commaSepValues(BaseEntity* entity)
{
    std::string vals;

    size_t i = 1;

    auto cIter = entity->cBeginIter();
    for(; cIter != entity->cEndIter(); ++cIter){
        auto ptr(std::get<1>(*cIter).get());
        if (ptr->fieldName() != "id"){
             vals += ptr->dbValueFormatter();
             if(i<entity->fieldsCount())
                 vals +=",";
        }

        ++i;
    }

    return vals;
}

std::string BaseDatabaseManager::makeInsertString(BaseEntity* entity)
{
    std::string flds = commaSepColumns(entity);
    std::string vals = commaSepValues(entity);
    std::string in_a = "INSERT INTO "+entity->tableName()+"( "+flds+")";
    std::string in_b = " VALUES ("+vals+")";
    std::string insert_stmt = in_a + in_b;
    return insert_stmt;
}

std::string BaseDatabaseManager::makeUpdateString(BaseEntity* entity)
{
    std::string flds = commaSepColumns(entity);
    std::string vals = commaSepValues(entity);
    std::string in_a = "UPDATE "+entity->tableName()+" SET ("+flds+")";
    std::string in_b = "=("+vals+") WHERE id ="+std::to_string(entity->id());
    std::string update_stmt = in_a + in_b;
    return update_stmt;
}

PostgresDatabaseManager::PostgresDatabaseManager()
{
    dataProvider =  new PostgresDataProvider;
}

PostgresDatabaseManager::~PostgresDatabaseManager()
{
    //delete dataProvider;
}

void PostgresDatabaseManager::saveEntity(BaseEntity* entity)
{
    qDebug() << "PostgresDatabaseManager::saveEntity";

    std::string sqlQuery;

    sqlQuery = makeInsertString(entity);

     provider()->executeQuery(sqlQuery);

}

void PostgresDatabaseManager::updateEntity(BaseEntity* entity)
{
    std::string sqlQuery;
    sqlQuery = makeUpdateString(entity);
     provider()->executeQuery(sqlQuery);
}

int PostgresDatabaseManager::fetchAll(BaseEntity* entity)
{
    std::string sql;
    std::string flds = columnsForSelection(entity);
    sql = "SELECT " + flds + " FROM "+entity->tableName();
    return provider()->read(sql);
}

int PostgresDatabaseManager::deleteEntity(BaseEntity* entity)
{
    std::string sql;
    sql = "DELETE FROM "+entity->tableName()+" WHERE ID ="+std::to_string(entity->id());
    qDebug() << stoq(sql);
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
