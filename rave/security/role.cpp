#include <format>

#include "../framework/entitydatamodel.h"
#include "role.h"

#include "user.h"
#include "rolemember.h"

namespace SECURITY
{
    Role::Role()
    :m_id{-1}
    ,m_OId{nullptr}
    ,m_role_name{nullptr}
    {
        m_user =  std::make_unique<SECURITY::User>();
        m_role_member = std::make_unique<RoleMember> (this, m_user.get());

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
    }

    Role::~Role()
    {
    }

    Role& Role::operator=(const Role& other)
    {
       m_id = other.id();
       m_OId->setValue(other.m_OId->value());
       m_OId->setMandatory(true);

       m_role_name->setValue(other.role_name()->value());
       m_role_name->setMandatory(true);

       m_rol_super->setValue(other.m_rol_super->value());
       m_rol_inherit->setValue(other.m_rol_inherit->value());
       m_rol_can_create_role->setValue(other.m_rol_can_create_role->value());
       m_rol_can_create_db->setValue(other.m_rol_can_create_db->value());
       m_rol_can_login->setValue(other.m_rol_can_login->value());
       m_rol_replication->setValue(other.m_rol_replication->value());
       m_rol_password->setValue(other.m_rol_password->value());
       m_valid_until->setValue(other.m_valid_until->value());

       mHeader = other.mHeader;

       getId().setFormOnly(true);
       setTableName("pg_authid");

       return *this;

    }


    int Role::role_id()
    {
        return m_id;
    }

    IntegerField* Role::oid() const
    {
        return	m_OId;

    }

    void Role::set_OId(int id)
    {
       m_OId->setValue(id);
    }

    StringField* Role::role_name() const
    {
       return m_role_name;
    }

    void Role::set_role_name(const std::string name)
    {
       m_role_name->setValue(name);
    }

    /* ------------------------ */

    BooleanField* Role::rol_super()
    {
        return m_rol_super;
    }

    void Role::set_rol_super(bool mode)
    {
        m_rol_super->setValue(mode);
    }

    BooleanField* Role::rol_inherit()
    {
        return m_rol_inherit;
    }

    void Role::set_rol_inherit(bool mode)
    {
        m_rol_inherit->setValue( mode ) ;
    }

    BooleanField* Role::rol_can_create_role()
    {
        return m_rol_can_create_role;
    }

    void Role::set_rol_can_create_role(bool mode)
    {
        m_rol_can_create_role->setValue(mode);
    }

    BooleanField* Role::rol_can_create_db()
    {
        return m_rol_can_create_db;
    }

    void Role::set_rol_can_create_db(bool mode)
    {
        m_rol_can_create_db->setValue(mode);
    }

    BooleanField* Role::rol_can_login()
    {
        return m_rol_can_login;
    }

    void Role::set_rol_can_login(bool login)
    {
        m_rol_can_login->setValue(login);
    }

    BooleanField* Role::rol_replication()
    {
        return m_rol_replication;
    }

    void Role::set_rol_replication(bool mode)
    {
        m_rol_replication->setValue(mode);
    }


    StringField* Role::rol_password()
    {
        return m_rol_password;
    }

    void Role::set_rol_password(const std::string password)
    {
        m_rol_password->setValue(password);
    }

    StringField* Role::valid_until() const
    {
        return m_valid_until;
    }

    void  Role::set_valid_until(const std::string validity)
    {
        m_valid_until->setValue( validity );
    }

    std::string Role::role_validity()
    {
        std::string validity{};

        validity = " VALID UNTIL '"+valid_until()->value()+"'";

    //    if (!roleExpire()->value())
    //        validity = " VALID UNTIL '"+validUntil()->value()+"'";
    //    else
    //        validity = " VALID UNTIL 'infinity' ";

        return validity;
    }



    /* ------------------------- */


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

    QStringList Role::tableHeaders() const
    {
        return mHeader;
    }

    std::string Role::tableName() const
    {
        return mTableName;
    }

    void Role::setTableName(const std::string table_name)
    {
        mTableName = table_name;
    }

    std::string Role::searchColumn() const
    {
        return m_role_name->value();
    }

    std::unique_ptr<BaseEntity> Role::mapFields(StringMap* e)
    {
    }

    std::vector<std::string> Role::tableViewColumns() const
    {
        return tableViewCols<std::string>(role_name()->displayName());
    }

    std::vector<std::string> Role::tableViewValues()
    {
        return{role_name()->valueToString()};
    }

    std::string Role::make_create_stmt()
    {
        //return BaseRole::make_create_role_stmt();
    }

    std::string Role::make_alter_stmt(const std::string name)
    {
        //return BaseRole::make_alter_role_stmt(name);
    }

    std::string Role::make_drop_stmt(const std::string name)
    {
        //return BaseRole::make_drop_role_stmt( name );
    }


    std::string Role::make_create_role_stmt()
    {
        /*
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
       */
        //return BaseRole::make_create_role_stmt();
    }

    std::string Role::make_grant_member_stmt(std::vector<std::string>& members)
    {
        /*
       std::string roles;
       int count=0;
       for (auto& member_name : members){
        roles += member_name;
        if (count < members.size()-1)
            roles += ",";
        count++;
       }

       std::string grant = std::format("GRANT {} TO {};",role_name()->value(), roles);

       return grant;
      */
       // return BaseRole::make_grant_member_stmt(members);

    }

    std::string Role::make_drop_role_stmt(const std::string username)
    {
        //return "DROP ROLE "+username+";";
        //return BaseRole::make_drop_role_stmt(username);
    }

    std::string Role::role_rights()
    {
        //BaseRole::role_rights();
    }

    std::string Role::make_alter_role_stmt(const std::string username)
    {
        //BaseRole::make_alter_role_stmt(username);
    }

    void Role::table_sequences(std::vector<std::string>& tseq)
    {
        /*
       EntityDataModel edm;
       std::string sql = "SELECT sequence_name FROM information_schema.sequences;";
       edm.readRaw(sql);

       auto cache = edm.getDBManager()->provider()->cache();
       cache->first();
       do{
           auto elem = cache->currentElement();

           auto it = elem->begin();
           tseq.push_back((*it).second);

           cache->next();

       }while(!cache->isLast());
       */
        //BaseRole::table_sequences(tseq);


    }


}
