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

        std::shared_ptr<BaseEntity> cloneAsShared() override;
        void afterMapping(BaseEntity& entity) override;
        std::string make_create_role_stmt() override;

        IntegerField* uniqueId() const override;

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
