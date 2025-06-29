#ifndef ORDERAPPROVER_H
#define ORDERAPPROVER_H

#include "../../../rave/framework/baseentity.h"

class OrderApprover : public BaseEntity
{
public:
    OrderApprover();
    ~OrderApprover() override;

    ForeignKeyField* userId();
    void setUserId(int);

    StringField* userName() const;
    void setUserName(const std::string username);

    StringField* userTitle() const;
    void setUserTitle(const std::string title);

    IntegerField* level() const;
    void setLevel(int lvl);

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
    ForeignKeyField* mUserId;
    StringField* mUserName;
    StringField* mUserTitle;
    IntegerField* mLevel;

    QStringList mHeader;
    std::string mTableName;
};

#endif // ORDERAPPROVER_H
