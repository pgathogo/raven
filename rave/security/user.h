#ifndef USER_H
#define USER_H

#include "../framework/baseentity.h"
#include "baserole.h"

class StringField;

namespace SECURITY
{
    class User : public BaseRole
    {
    public:
        User();
        User(const std::string username, const std::string password);
        ~User() override;
        User& operator=(const User&);

        IntegerField* oid() const override;
        void set_OId(int id) override;

        StringField* role_name()const override;
        void set_role_name(const std::string) override;

        BooleanField* rol_super() override;
        void set_rol_super(bool) override;

        BooleanField* rol_inherit() override;
        void set_rol_inherit(bool) override;

        BooleanField* rol_can_create_role() override;
        void set_rol_can_create_role(bool) override;

        BooleanField* rol_can_create_db() override;
        void set_rol_can_create_db(bool) override;

        BooleanField* rol_can_login() override;
        void set_rol_can_login(bool) override;

        BooleanField* rol_replication() override;
        void set_rol_replication(bool) override;

        StringField* rol_password() override;
        void set_rol_password(const std::string) override;

        StringField* valid_until() const override;
        void set_valid_until(const std::string) override;

        std::string role_validity() override;


        std::string make_create_role_stmt() override;
        std::string make_grant_member_stmt(std::vector<std::string>&) override;
        std::string role_rights() override;
        std::string make_alter_role_stmt(const std::string username) override;
        std::string make_drop_role_stmt(const std::string username) override;
        void table_sequences(std::vector<std::string>& tseq) override;

        std::string make_create_stmt() override;
        std::string make_alter_stmt(const std::string name) override;
        std::string make_drop_stmt(const std::string name) override;


        std::shared_ptr<BaseEntity> cloneAsShared() override;
        void afterMapping(BaseEntity& entity) override;
        QStringList tableHeaders() const override;
        std::string tableName() const override;
        void setTableName(const std::string table_name) override;

        std::unique_ptr<BaseEntity> mapFields(StringMap* e) override;
        std::vector<std::string> tableViewColumns() const override;
        std::vector<std::string> tableViewValues() override;

        IntegerField* uniqueId() const override;
        std::string filter() override;
        std::string searchColumn() const override;

    private:
        IntegerField* m_id;

        IntegerField* m_OId;
        StringField* m_role_name;

        BooleanField* m_rol_super;
        BooleanField* m_rol_inherit;
        BooleanField* m_rol_can_create_role;
        BooleanField* m_rol_can_create_db;
        BooleanField* m_rol_can_login;
        BooleanField* m_rol_replication;
        StringField* m_rol_password;
        StringField* m_valid_until;

        QStringList mHeader;
        std::string mTableName;

    };
}

#endif // USER_H
