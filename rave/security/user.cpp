#include <format>
#include <QDebug>
#include "user.h"

namespace SECURITY
{
    User::User()
        :BaseRole()
    {
        m_id = createField<IntegerField>("oid", "Unique Id");
        m_id->setReadOnly(true);

        /*
        m_OId = createField<IntegerField>("oid", "Role ID");
        m_OId->setReadOnly(true);

        m_role_name = createField<StringField>("rolname", "Role Name");
        m_role_name->setMandatory(true);

        m_rol_super = createField<BooleanField>("rolsuper", "Is Super Role");
        m_rol_inherit = createField<BooleanField>("rolinherit", "Can Inherit Role");
        m_rol_can_create_role = createField<BooleanField>("rolcreaterole", "Can Create Role");
        m_rol_can_create_db = createField<BooleanField>("rolcreatedb","Create DB");
        m_rol_can_login = createField<BooleanField>("rolcanlogin","Role can Login");
        m_rol_replication = createField<BooleanField>("rolreplication","Role can Replicate");
        m_rol_password = createField<StringField>("rolpassword","Role Password");
        m_valid_until = createField<StringField>("rolvaliduntil", "Role Valid Until");

        mHeader << QString::fromStdString(m_role_name->fieldLabel());
        setTableName("pg_authid");

        getId().setFormOnly(true);
        */

    }

    User::User(const std::string username, const std::string password)
        :BaseRole()
    {
        role_name()->setValue(username);
        rol_password()->setValue(password);
    }

    User::~User()
    {
    }

    std::shared_ptr<BaseEntity> User::cloneAsShared()
    {
        return std::make_shared<User>();
    }

    IntegerField* User::uniqueId() const
    {
        return m_id;
    }

    void User::afterMapping(BaseEntity& entity)
    {

    }

    std::string User::make_create_role_stmt()
    {
        std::string s1 = std::format("CREATE USER {} WITH PASSWORD '{}' ",role_name()->value(), rol_password()->value() ) ;

        std::string s2{};

        std::string s3 = std::format("GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO {};",role_name()->value());

        std::vector<std::string> seqs;
        table_sequences(seqs);

        std::string s4{};
        for(auto& seq : seqs){
            auto grant = std::format("GRANT ALL ON SEQUENCE public.{} TO GROUP {} WITH GRANT OPTION;",seq, role_name()->value());
            s4 += grant;
        }

        return s1+role_validity()+role_rights()+s2+s3+s4;
    }
}
