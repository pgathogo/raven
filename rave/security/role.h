#ifndef ROLE_H
#define ROLE_H

#include <memory>

#include "../framework/baseentity.h"
#include "user.h"
#include "baserole.h"

class RoleMember;

namespace SECURITY
{
    class Role : public SECURITY::BaseRole
    {
    public:
        Role();
        ~Role();
        Role& operator=(const Role&);

        std::shared_ptr<BaseEntity> cloneAsShared() override;
        void afterMapping(BaseEntity& entity) override;
        std::string make_create_role_stmt() override;
        RoleMember& roleMember();

        int role_id();

    private:
        int m_id;
        std::unique_ptr<SECURITY::User> m_user;
        std::unique_ptr<RoleMember> m_role_member;
    };
}

#endif // ROLE_H
