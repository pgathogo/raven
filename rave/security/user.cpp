#include <format>
#include <QDebug>
#include "user.h"
#include "../framework/entitydatamodel.h"

namespace SECURITY
{
    User::User()
    {
        m_id = createField<IntegerField>("oid", "Unique Id");
        m_id->setReadOnly(true);


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

    User& User::operator=(const User& other)
    {
       m_id->setValue(other.id());
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

    IntegerField* User::oid() const
    {
        return m_OId;
    }

    void User::set_OId(int id)
    {
       m_OId->setValue(id);
    }

    StringField* User::role_name() const
    {
       return m_role_name;
    }

    void User::set_role_name(const std::string name)
    {
       m_role_name->setValue(name);
    }

    BooleanField* User::rol_super()
    {
        return m_rol_super;
    }

    void User::set_rol_super(bool mode)
    {
        m_rol_super->setValue(mode);
    }

    BooleanField* User::rol_inherit()
    {
        return m_rol_inherit;
    }

    void User::set_rol_inherit(bool mode)
    {
        m_rol_inherit->setValue( mode ) ;
    }

    BooleanField* User::rol_can_create_role()
    {
        return m_rol_can_create_role;
    }

    void User::set_rol_can_create_role(bool mode)
    {
        m_rol_can_create_role->setValue(mode);
    }

    BooleanField* User::rol_can_create_db()
    {
        return m_rol_can_create_db;
    }

    void User::set_rol_can_create_db(bool mode)
    {
        m_rol_can_create_db->setValue(mode);
    }

    BooleanField* User::rol_can_login()
    {
        return m_rol_can_login;
    }

    void User::set_rol_can_login(bool login)
    {
        m_rol_can_login->setValue(login);
    }

    BooleanField* User::rol_replication()
    {
        return m_rol_replication;
    }

    void User::set_rol_replication(bool mode)
    {
        m_rol_replication->setValue(mode);
    }


    StringField* User::rol_password()
    {
        return m_rol_password;
    }

    void User::set_rol_password(const std::string password)
    {
        m_rol_password->setValue(password);
    }

    StringField* User::valid_until() const
    {
        return m_valid_until;
    }

    void  User::set_valid_until(const std::string validity)
    {
        m_valid_until->setValue( validity );
    }

    std::string User::role_validity()
    {
        std::string validity{};

        validity = " VALID UNTIL '"+valid_until()->value()+"'";

    //    if (!roleExpire()->value())
    //        validity = " VALID UNTIL '"+validUntil()->value()+"'";
    //    else
    //        validity = " VALID UNTIL 'infinity' ";

        return validity;
    }

    std::string User::filter()
    {
        return " rolname not like 'pg_%'";
    }

    void User::afterMapping(BaseEntity& entity)
    {
    }

//    RoleMember& User::roleMember()
//    {
//        return *m_role_member;
//    }

    QStringList User::tableHeaders() const
    {
        return mHeader;
    }

    std::string User::tableName() const
    {
        return mTableName;
    }

    void User::setTableName(const std::string table_name)
    {
        mTableName = table_name;
    }

    std::string User::searchColumn() const
    {
        return m_role_name->value();
    }

    std::unique_ptr<BaseEntity> User::mapFields(StringMap* e)
    {
    }

    std::vector<std::string> User::tableViewColumns() const
    {
        return tableViewCols<std::string>(role_name()->displayName());
    }

    std::vector<std::string> User::tableViewValues()
    {
        return{role_name()->valueToString()};
    }



    std::string User::make_create_stmt()
    {
        //return BaseRole::make_create_role_stmt();
    }

    std::string User::make_alter_stmt(const std::string name)
    {
        //return BaseRole::make_alter_role_stmt(name);
    }

    std::string User::make_drop_stmt(const std::string name)
    {
        //return BaseRole::make_drop_role_stmt( name );
    }



    std::string User::role_rights()
    {
        //return " NOSUPERUSER INHERIT NOCREATEDB CREATEROLE NOREPLICATION;";
        //return BaseRole::role_rights();
    }

    std::string User::make_alter_role_stmt(const std::string username)
    {
       //std::string s1 = "ALTER ROLE "+username;
       //return s1+role_validity()+role_rights();
        //return BaseRole::make_alter_role_stmt(username);
    }

    std::string User::make_grant_member_stmt(std::vector<std::string>& members)
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
        //BaseRole::make_grant_member_stmt(members);

    }

    std::string User::make_drop_role_stmt(const std::string username)
    {
        //return "DROP ROLE "+username+";";
        //return BaseRole::make_drop_role_stmt(username);
    }

    void User::table_sequences(std::vector<std::string>& tseq)
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


    std::string User::make_create_role_stmt()
    {
        /*
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
    */
        //BaseRole::make_create_role_stmt();
    }

}
