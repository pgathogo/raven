#ifndef SALESPERSON_H
#define SALESPERSON_H

#include "../framework/baseentity.h"


class SalesPerson : public BaseEntity
{
public:
    SalesPerson();
    ~SalesPerson() override;

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

    std::list<std::string> tableViewColumns() override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

    StringField* name() const;
    void setName(std::string aName);

    StringField* mobileno() const;
    void setMobileNo(std::string mobileno);

    ForeignKeyField* gender() const;
    void setGender( int i);
private:
    StringField* mName;
    StringField* mMobileNo;
    ForeignKeyField* mGender;

    QStringList mHeader;
    std::string mTableName;
};

#endif // SALESPERSON_H
