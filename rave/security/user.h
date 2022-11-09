#ifndef USER_H
#define USER_H

#include "../framework/baseentity.h"

class StringField;

class User : public BaseEntity
{
public:
    User();
    User(const std::string username, const std::string password);
    User& operator=(const User&);

    ~User() override;

    IntegerField* useSysId() const;
    void setUseSysId(int val);

    int id() const override;

    StringField* userName() const;
    void setUserName(const std::string uname);

    StringField* password() const;
    void setPassword(const std::string pword);

    StringField* confirmPassword() const;
    void setConfirmPassword(const std::string pword);

    StringField* validUntil() const;
    void setValidUntil(const std::string validity);

    BooleanField* passwordExpire() const;
    void setPasswordExpire(bool val);

    std::string password_validity();
    std::string make_create_user_stmt();
    std::string role_rights();
    std::string make_drop_user_stmt(const std::string username);
    std::string make_alter_user_stmt(const std::string username);

    std::unique_ptr<BaseEntity> mapFields(StringMap* e) override;

    std::vector<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::string searchColumn() const override;

    void populateEntity() override;
    std::unique_ptr<BaseEntity> cloneAsUnique() override;
    void afterMapping(BaseEntity& entity) override;

    IntegerField* uniqueId() const override;

    std::string displayName() override;
    std::string make_create_stmt() override;
    std::string make_alter_stmt(const std::string name) override;
    std::string make_drop_stmt(const std::string name) override;

private:
    IntegerField* mUseSysId;
    StringField* mUserName;
    StringField* mPassword;
    StringField* mConfirmPassword;
    StringField* mValidUntil;
    BooleanField* mPasswordExpire;

    QStringList mHeader;
    std::string mTableName;
};

#endif // USER_H
