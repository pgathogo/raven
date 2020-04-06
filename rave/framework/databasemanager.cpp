#include "databasemanager.h"
#include "baseentity.h"
#include "dataprovider.h"

#include <QDebug>
#include "../utils/util.h"

void BaseDatabaseManager::setObjectID(BaseEntity& entity, int id)
{
    //entity.setEntityId(id);
}

BaseDatabaseManager::~BaseDatabaseManager(){}

std::string BaseDatabaseManager::commaSepColumns(BaseEntity* entity)
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

std::string BaseDatabaseManager::commaSepValues(BaseEntity* entity)
{
    std::string vals;

    size_t i = 1;
    size_t fcount = entity->fieldsCount();

    qDebug() << "Field Count: "<< fcount;

    auto cIter = entity->cBeginIter();
    for(; cIter != entity->cEndIter(); ++cIter){
        auto ptr(std::get<1>(*cIter).get());
         vals += ptr->dbValueFormatter();
         if(i<entity->fieldsCount())
             vals +=",";
        ++i;
    }

    return vals;
}

PostgresDatabaseManager::PostgresDatabaseManager()
{
    dataProvider =  new PostgresDataProvider;
}


void PostgresDatabaseManager::saveEntity(BaseEntity* entity)
{
    //populateFields(entity);

    qDebug() << "PostgresDatabaseManager::saveEntity";
    std::string queryStatement;

    std::string vals = commaSepValues(entity);

    qDebug() << "SaveEntity :" << QString::fromStdString(vals);

   // We need a way to identify new entities
    /*
    if (entity->getEntityId() > 0){
        // queryStatment = make_UPDATE_statement
    } else {
        // queryStatment = make_INSERT_statement
    }
    */

     //provider()->executeQuery(queryStatement);

}

int PostgresDatabaseManager::fetchAll(BaseEntity* entity)
{
    std::string sql;
    std::string flds = commaSepColumns(entity);
    sql = "SELECT " + flds + " FROM "+entity->tableName();
    return provider()->executeQuery(sql);
}

void PostgresDatabaseManager::deleteEntity(const BaseEntity& /*entity*/)
{
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
