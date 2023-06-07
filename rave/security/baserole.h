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

        //int id() const override;

        virtual IntegerField* oid() const = 0;
        virtual void set_OId(int id) = 0;

        virtual StringField* role_name()const = 0;
        virtual void set_role_name(const std::string) = 0;

        virtual BooleanField* rol_super() = 0;
        virtual void set_rol_super(bool) = 0;

        virtual BooleanField* rol_inherit() = 0;
        virtual void set_rol_inherit(bool) = 0;

        virtual BooleanField* rol_can_create_role() = 0;
        virtual void set_rol_can_create_role(bool) = 0;

        virtual BooleanField* rol_can_create_db() = 0;
        virtual void set_rol_can_create_db(bool) = 0;

        virtual BooleanField* rol_can_login() = 0;
        virtual void set_rol_can_login(bool) = 0;

        virtual BooleanField* rol_replication() = 0;
        virtual void set_rol_replication(bool) = 0;

        virtual StringField* rol_password() = 0;
        virtual void set_rol_password(const std::string) = 0;

        virtual StringField* valid_until() const = 0;
        virtual void set_valid_until(const std::string) = 0;
        virtual std::string role_validity() = 0;

        virtual std::string make_create_role_stmt();
        virtual std::string make_grant_member_stmt(std::vector<std::string>&);
        virtual std::string role_rights();
        virtual std::string make_alter_role_stmt(const std::string);
        virtual std::string make_drop_role_stmt(const std::string);
        virtual void table_sequences(std::vector<std::string>& tseq);

        std::string make_create_stmt() override;
        std::string make_alter_stmt(const std::string name) override;
        std::string make_drop_stmt(const std::string name) override;


        std::unique_ptr<BaseEntity> mapFields(StringMap* e) override;

        std::vector<std::string> tableViewColumns() const override;
        std::vector<std::string> tableViewValues() override;

//        QStringList tableHeaders() const override;
//        std::string tableName() const override;
//        void setTableName(const std::string table_name) override;

        std::string searchColumn() const override;

        void populateEntity() override;
        std::shared_ptr<BaseEntity> cloneAsShared() override;
        void afterMapping(BaseEntity& entity) override;

        std::string displayName() override;
        std::string order_by() const override;


    protected:
        void set_id(int);

    private:
       /*
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
     */

    };
}

#endif // BASEROLE_H
