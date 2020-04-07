#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>

class BaseEntity;
class BaseDataProvider;
class PostgresDataProvider;

class BaseDatabaseManager
{
public:
    virtual ~BaseDatabaseManager();
    virtual void updateEntity(BaseEntity* entity) = 0;
    virtual void saveEntity(BaseEntity* entity) = 0;
    virtual int deleteEntity(BaseEntity* entity) = 0;
    virtual int fetchAll(BaseEntity* entity) = 0;
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
    void saveEntity(BaseEntity* entity) override;
    int deleteEntity(BaseEntity* entity)override;
    int fetchAll(BaseEntity* entity) override;
    BaseDataProvider* provider() override;
protected:
    void loadEntity(BaseEntity& entity)override;
private:
    PostgresDataProvider* dataProvider;

};


#endif // DATABASEMANAGER_H
