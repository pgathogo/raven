#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>

class BaseEntity;
class BaseDataProvider;
class PostgresDataProvider;

using SearchField = std::tuple<std::string, std::string>;
using ColumnName = std::string;
using ColumnValue = int;

class BaseDatabaseManager
{
public:
    virtual ~BaseDatabaseManager();
    virtual void updateEntity(BaseEntity* entity) = 0;
    virtual int createEntity(BaseEntity* entity) = 0;
    virtual int deleteEntity(BaseEntity* entity) = 0;

    virtual int deleteEntityByValue(
            const std::string table_name,
            std::tuple<ColumnName, ColumnValue>) = 0; // make it generic

    virtual int fetchAll(BaseEntity* entity) = 0;
    virtual int searchById(BaseEntity* entity,
                           std::tuple<std::string, int> field_value) = 0;
    virtual int searchByField(BaseEntity* entity,
                          std::tuple<std::string, std::string> sf) = 0;
    virtual BaseDataProvider* provider() = 0;
protected:
    virtual void loadEntity(BaseEntity& entity) = 0;

    void setObjectID(BaseEntity& entity, int id);

    std::string columnsForSelection(BaseEntity* entity);
    std::string commaSepColumns(BaseEntity* entity);
    std::string commaSepValues(BaseEntity* entity);

    virtual std::string makeInsertString(BaseEntity* entity);
    virtual std::string makeUpdateString(BaseEntity* entity);
};

class PostgresDatabaseManager : public BaseDatabaseManager
{
public:
    PostgresDatabaseManager();
     ~PostgresDatabaseManager() override;
    virtual void populateFields(BaseEntity* /*entity*/){}
    virtual void populateObject(const BaseEntity& /*entity*/){}

    void updateEntity(BaseEntity* entity) override;
    int createEntity(BaseEntity* entity) override;
    int deleteEntity(BaseEntity* entity)override;
    int deleteEntityByValue(
            const std::string tabale_name,
            std::tuple<ColumnName, ColumnValue>) override;
    int fetchAll(BaseEntity* entity) override;
    int searchByField(BaseEntity* entity,
                          std::tuple<std::string, std::string> sf) override;
    int searchById(BaseEntity* entity,
                           std::tuple<std::string, int> field_value) override;
    BaseDataProvider* provider() override;
protected:
    void loadEntity(BaseEntity& entity)override;
private:
    PostgresDataProvider* dataProvider;

};


#endif // DATABASEMANAGER_H
