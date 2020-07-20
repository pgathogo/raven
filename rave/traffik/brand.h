#ifndef BRAND_H
#define BRAND_H

#include "../framework/baseentity.h"

class StringField;

class Brand : public BaseEntity
{
public:
    Brand();
    ~Brand() override;

    void setBrandName(std::string brand_name);
    StringField* brandName() const;
    void setClient(int client_id);
    ForeignKeyField* client() const;
    void setBrandManager(int brand_manager_id);
    ForeignKeyField* brandManager() const;

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* e) override;

    std::list<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

    std::unique_ptr<BaseEntity> cloneAsUnique() override;
    void afterMapping(BaseEntity& entity) override;

private:
    StringField* mBrandName;
    ForeignKeyField* mClient;
    ForeignKeyField* mBrandManager;

    QStringList mHeader;
    std::string mTableName;

};

#endif // BRAND_H
