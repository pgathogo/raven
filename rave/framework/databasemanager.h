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
    virtual void updateEntity(const BaseEntity& entity) = 0;
    virtual int createEntity(const BaseEntity& entity) = 0;
    virtual int deleteEntity(const BaseEntity& entity) = 0;

    virtual int deleteEntityByValue(
            const std::string table_name,
            std::tuple<ColumnName, ColumnValue>) = 0; // make it generic

    virtual int fetchAll(const BaseEntity& entity) = 0;
    virtual int searchByInt(const BaseEntity& entity,
                           std::tuple<std::string, std::string, int> field_value) = 0;
    virtual int searchByStr(const BaseEntity& entity,
                          std::tuple<std::string, std::string> sf) = 0;

    virtual int search(const BaseEntity& entity,
                       const std::string filter) = 0;
    virtual void executeRawSQL(const std::string sql)=0;
    virtual int readRaw(const std::string sql)=0;

    virtual BaseDataProvider* provider() = 0;
    virtual std::string make_insert_stmt(const BaseEntity& entity) =0;
protected:
    virtual void loadEntity(BaseEntity& entity) = 0;

    void setObjectID(BaseEntity& entity, int id);

    std::string columnsForSelection(const BaseEntity& entity);
    std::string columnsForInsert(const BaseEntity& entity);
    std::string commaSepColumns(const BaseEntity& entity);
    std::string commaSepValues(const BaseEntity& entity);

    virtual std::string makeInsertString(const BaseEntity& entity);
    virtual std::string makeUpdateString(const BaseEntity& entity);
};

class PostgresDatabaseManager : public BaseDatabaseManager
{
public:
    PostgresDatabaseManager();
    PostgresDatabaseManager(const std::string conninfo);
     ~PostgresDatabaseManager() override;
    virtual void populateFields(BaseEntity* /*entity*/){}
    virtual void populateObject(const BaseEntity& /*entity*/){}

    void updateEntity(const BaseEntity& entity) override;
    int createEntity(const BaseEntity& entity) override;
    int deleteEntity(const BaseEntity& entity)override;
    int deleteEntityByValue(
            const std::string tabale_name,
            std::tuple<ColumnName, ColumnValue>) override;
    int fetchAll(const BaseEntity& entity) override;
    int searchByStr(const BaseEntity& entity,
                          std::tuple<std::string, std::string> sf) override;
    int searchByInt(const BaseEntity& entity,
                           std::tuple<std::string,std::string,  int> field_value) override;
    int search(const BaseEntity& entity, const std::string filter) override;
    void executeRawSQL(const std::string sql) override;
    int readRaw(const std::string sql) override;
    BaseDataProvider* provider() override;
    std::string make_insert_stmt(const BaseEntity& entity) override;
    PostgresDataProvider* pgProvider();

protected:
    void loadEntity(BaseEntity& entity)override;
private:
    PostgresDataProvider* dataProvider;
    std::string mConninfo;
};


#endif // DATABASEMANAGER_H
