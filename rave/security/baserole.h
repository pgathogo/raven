#ifndef BASEROLE_H
#define BASEROLE_H

#include <memory>

#include "../framework/baseentity.h"

namespace SECURITY
{
    class BaseRole : public BaseEntity
    {
    public:
        BaseRole();
        ~BaseRole() override;
        BaseRole& operator=(const BaseRole&);

        int id() const override;

        IntegerField* oid() const;
        void set_OId(int id);

        StringField* role_name()const;
        void set_role_name(const std::string);

        BooleanField* rol_super();
        void set_rol_super(bool);

        BooleanField* rol_inherit();
        void set_rol_inherit(bool);

        BooleanField* rol_can_create_role();
        void set_rol_can_create_role(bool);

        BooleanField* rol_can_create_db();
        void set_rol_can_create_db(bool);

        BooleanField* rol_can_login();
        void set_rol_can_login(bool);

        BooleanField* rol_replication();
        void set_rol_replication(bool);

        StringField* rol_password();
        void set_rol_password(const std::string);

        StringField* valid_until() const;
        void set_valid_until(const std::string);


        virtual std::string make_create_role_stmt();

        std::string role_validity();
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

        std::string make_grant_member_stmt(std::vector<std::string>&);

    protected:
        void set_id(int);

    private:
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

#endif // BASEROLE_H
