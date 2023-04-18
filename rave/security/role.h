#ifndef ROLE_H
#define ROLE_H

#include <memory>

#include "../framework/baseentity.h"

namespace SECURITY{
    class RoleUser;
}

class RoleMember;

class Role : public BaseEntity
{
public:
    Role();
    ~Role() override;
    Role& operator=(const Role&);

    int id() const override;

    IntegerField* uniqueId() const override;

    IntegerField* oid() const;
    void setOId(int id);

    StringField* roleName() const;
    void setRoleName(const std::string name);

    StringField* validUntil() const;
    void setValidUntil(const std::string validity);

    BooleanField* roleExpire() const;
    void setRoleExpire(bool val);

    StringField* comment() const;
    void setComment(const std::string comm);

    BooleanField* roleCanLogin() const;
    void setRoleCanLogin(bool val);

    std::string role_validity();
    std::string make_create_role_stmt();
    std::string role_rights();
    std::string make_alter_role_stmt(const std::string username);
    std::string make_drop_role_stmt(const std::string username);

    void table_sequences(std::vector<std::string>& tseq);

    std::unique_ptr<BaseEntity> mapFields(StringMap* e) override;

    std::vector<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::string searchColumn() const override;

    void populateEntity() override;
    std::shared_ptr<BaseEntity> cloneAsShared() override;
    void afterMapping(BaseEntity& entity) override;

    std::string displayName() override;
    std::string make_create_stmt() override;
    std::string make_alter_stmt(const std::string name) override;
    std::string make_drop_stmt(const std::string name) override;

    std::string order_by() const override;

    //SECURITY::RoleUser& user();
    RoleMember& roleMember();

private:
    IntegerField* mOId;
    StringField* mRoleName;
    StringField* mValidUntil;
    BooleanField* mRoleExpire;
    StringField* mComment;
    BooleanField* mRoleCanLogin;

    QStringList mHeader;
    std::string mTableName;

    std::unique_ptr<SECURITY::RoleUser> mUser;
    std::unique_ptr<RoleMember> mRoleMember;

};

#endif // ROLE_H
