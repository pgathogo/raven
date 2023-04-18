#ifndef ORDERPACKAGE_H
#define ORDERPACKAGE_H

#include "../framework/baseentity.h"

class StringField;
class IntegerField;

class OrderPackage : public BaseEntity
{
public:
    OrderPackage();
    ~OrderPackage() override;

    StringField* name() const;
    void setName(const std::string);

    IntegerField* spotCount() const;
    void setSpotCount(int);

    IntegerField* mentionCount() const;
    void setMentionCount(int);

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

    std::vector<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

    std::shared_ptr<BaseEntity> cloneAsShared() override;
    void afterMapping(BaseEntity& entity) override;

private:
    StringField* mName;
    IntegerField* mSpotCount;
    IntegerField* mMentionCount;

    QStringList mHeader;
    std::string mTableName;

};

#endif // ORDERPACKAGE_H
