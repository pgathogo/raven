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


PostgresDatabaseManager::PostgresDatabaseManager()
{
    dataProvider =  new PostgresDataProvider;
}

void PostgresDatabaseManager::saveEntity(BaseEntity* entity)
{
    //populateFields(entity);

    std::string queryStatement;

   // We need a way to identify new entities
    /*
    if (entity->getEntityId() > 0){
        // queryStatment = make_UPDATE_statement
    } else {
        // queryStatment = make_INSERT_statement
    }
    */

     provider()->executeQuery(queryStatement);

}

int PostgresDatabaseManager::fetchAll(BaseEntity* entity)
{
    // build query string from Entity
    // Using the formating libary - check that from the
    // Fig tool
    auto cols = entity->dbColumnNames();
    std::string SELECT = "SELECT ";
    std::string sql;
    std::string flds;
    std::size_t i = 1;

    qDebug() << "COL COUNT: "<< cols.size();
    for (auto& col : cols){
    qDebug() << "count: "<< i;
       flds += col;
       if (i<cols.size())
           flds +=",";
       ++i;
    }
    sql = SELECT + flds + " FROM "+entity->tableName();
    qDebug() << strtoqstr(sql) ;
    //const std::string query = "Select first_name, surname from ug_applicants where first_name like '%James%'";
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
