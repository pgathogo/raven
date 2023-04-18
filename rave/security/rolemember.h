#ifndef ROLEMEMBER_H
#define ROLEMEMBER_H

#include "../framework/manytomany.h"

class RoleMember : public ManyToMany
{
public:
    RoleMember();
    RoleMember(BaseEntity* pEnt, BaseEntity* dEnt);
    ~RoleMember() override;

    std::unique_ptr<ManyToMany> copy(BaseEntity* pEnt, BaseEntity* dEnt) const override;
    void setTable(const std::string tablename) override;

    IntegerField* parentId() const override;
    void setParentId(int id) override;
    IntegerField* detailId() const override;
    void setDetailId(int id) override;

    IntegerField* grantor();
    void setGrantor(int val);
    BooleanField* adminOption();
    void setAdminOption(boolean val);

    BaseEntity* parentEntity() const override;
    BaseEntity* detailEntity() const override;
    void setDetailEntity(BaseEntity* other);

    std::unique_ptr<BaseEntity> mapFields(StringMap* sm) override;

    std::vector<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string tableName() const override;
    void setTableName(std::string table_name) override;
    std::string searchColumn() const override;
    void populateEntity() override;

    BaseEntity* mtomEntity() override;

    std::shared_ptr<BaseEntity> cloneAsShared() override;
    void afterMapping(BaseEntity& entity) override;

private:

    BaseEntity* mParentEntity;
    BaseEntity* mDetailEntity;

    IntegerField* mParentId;
    IntegerField* mDetailId;

    IntegerField* mGrantor;
    BooleanField* mAdminOption;

    QStringList mHeader;
    std::string mTableName;
};


#endif // ROLEMEMBER_H
