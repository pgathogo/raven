#include <format>

#include "../framework/entitydatamodel.h"
#include "role.h"

#include "user.h"
#include "rolemember.h"

namespace SECURITY
{
    Role::Role()
        :BaseRole()
        ,m_id{-1}
    {
        m_user =  std::make_unique<SECURITY::User>();
        m_role_member = std::make_unique<RoleMember> (this, m_user.get());
    }

    Role::~Role()
    {
    }

    Role& Role::operator=(const Role& other)
    {
        m_id = other.id();
    }


    int Role::role_id()
    {
        return m_id;
    }


    std::shared_ptr<BaseEntity> Role::cloneAsShared()
    {
        return std::make_shared<Role>();
    }

    void Role::afterMapping(BaseEntity& entity)
    {

    }

    RoleMember& Role::roleMember()
    {
        return *m_role_member;
    }

    std::string Role::make_create_role_stmt()
    {
        std::string s1 = "CREATE ROLE "+role_name()->value();

        std::string s2{};

        std::string s3 = "GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO "+
            role_name()->value()+";";

        std::vector<std::string> seqs;
        table_sequences(seqs);
        std::string s4{};
        for(auto& seq : seqs){

        s4 += "GRANT ALL ON SEQUENCE public."+seq+" TO GROUP "+
            role_name()->value()+" WITH GRANT OPTION;";

        }

        return s1+role_validity()+role_rights()+s2+s3+s4;
    }
}
