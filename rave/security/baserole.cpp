#include <format>
#include <sstream>

#include "../framework/entitydatamodel.h"
#include "baserole.h"

namespace SECURITY
{

    BaseRole::BaseRole()
    {
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

    BaseRole& BaseRole::operator=(const BaseRole& other)
    {
        /*
       setId(other.id());
       m_OId->setValue(other.m_OId->value());
       m_OId->setMandatory(true);
       m_role_name->setValue(other.m_role_name->value());
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
      */

    }


    BaseRole::~BaseRole()
    {
    }


    /*
    void BaseRole::set_id(int id)
    {
       setId(id);
    }

    int BaseRole::id() const
    {
        return oid()->value();
    }

    IntegerField* BaseRole::oid() const
    {
        return m_OId;
    }

    void BaseRole::set_OId(int id)
    {
        m_OId->setValue( id );
    }

    StringField* BaseRole::role_name() const
    {
        return m_role_name;
    }

    void BaseRole::set_role_name(const std::string name)
    {
        m_role_name->setValue( name );
    }

    BooleanField* BaseRole::rol_super()
    {
        return m_rol_super;
    }

    void BaseRole::set_rol_super(bool mode)
    {
        m_rol_super->setValue(mode);
    }

    BooleanField* BaseRole::rol_inherit()
    {
        return m_rol_inherit;
    }

    void BaseRole::set_rol_inherit(bool mode)
    {
        m_rol_inherit->setValue( mode ) ;
    }

    BooleanField* BaseRole::rol_can_create_role()
    {
        return m_rol_can_create_role;
    }

    void BaseRole::set_rol_can_create_role(bool mode)
    {
        m_rol_can_create_role->setValue(mode);
    }

    BooleanField* BaseRole::rol_can_create_db()
    {
        return m_rol_can_create_db;
    }

    void BaseRole::set_rol_can_create_db(bool mode)
    {
        m_rol_can_create_db->setValue(mode);
    }

    BooleanField* BaseRole::rol_can_login()
    {
        return m_rol_can_login;
    }

    void BaseRole::set_rol_can_login(bool login)
    {
        m_rol_can_login->setValue(login);
    }

    BooleanField* BaseRole::rol_replication()
    {
        return m_rol_replication;
    }

    void BaseRole::set_rol_replication(bool mode)
    {
        m_rol_replication->setValue(mode);
    }


    StringField* BaseRole::rol_password()
    {
        return m_rol_password;
    }

    void BaseRole::set_rol_password(const std::string password)
    {
        m_rol_password->setValue(password);
    }

    StringField* BaseRole::valid_until() const
    {
        return m_valid_until;
    }

    void  BaseRole::set_valid_until(const std::string validity)
    {
        m_valid_until->setValue( validity );
    }

    std::string BaseRole::role_validity()
    {
        std::string validity{};

        validity = " VALID UNTIL '"+valid_until()->value()+"'";

    //    if (!roleExpire()->value())
    //        validity = " VALID UNTIL '"+validUntil()->value()+"'";
    //    else
    //        validity = " VALID UNTIL 'infinity' ";

        return validity;
    }
  */


    std::string BaseRole::make_create_role_stmt()
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

        qDebug() <<"s1: "<< stoq(s1);
        qDebug() <<"role_validity(): "<< stoq(role_validity());
        qDebug() <<"role_rights: "<< stoq(role_rights());
        qDebug() <<"s2: "<< stoq(s2);
        qDebug() <<"s3: "<< stoq(s3);
        qDebug() << "s4: "<<stoq(s4);

        std::string stmt =  s1+role_validity()+role_rights()+s2+s3+s4;

        qDebug() << stoq(stmt);

        return stmt;
    }

    std::string BaseRole::role_rights()
    {
        return " NOSUPERUSER INHERIT NOCREATEDB CREATEROLE NOREPLICATION;";
    }

    std::string BaseRole::make_alter_role_stmt(const std::string role_name)
    {
       std::string s1 = "ALTER ROLE "+role_name;

       return s1+role_validity()+role_rights();
    }

    std::string BaseRole::make_grant_member_stmt(std::vector<std::string>& members)
    {
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

    }

    std::string BaseRole::make_drop_role_stmt(const std::string role_name)
    {
       // TODO: Try the 'REASSIGN' method in the future
       // REASSIGN OWNED BY role_name TO postgres;
       // DROP OWNED BY role_name;

       auto s1 = std::format(" REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM {}; ",role_name);
       auto s2 = std::format(" REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM {}; ",role_name);
       auto s3 = std::format(" REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM {}; ",role_name);
       auto s4 = std::format(" DROP ROLE {}; ",role_name);
       auto stmt = s1+s2+s3+s4;
       return stmt;
    }

    void BaseRole::table_sequences(std::vector<std::string>& tseq)
    {
       EntityDataModel edm;
       std::stringstream sql;
       sql << "SELECT sequence_name FROM information_schema.sequences "
           << " WHERE sequence_name ~* 'rave_'; ";

       edm.readRaw(sql.str());

       auto cache = edm.getDBManager()->provider()->cache();
       cache->first();
       do{
           auto elem = cache->currentElement();

           auto it = elem->begin();
           tseq.push_back((*it).second);

           cache->next();

       }while(!cache->isLast());


    }

  /* ---  Methods in BaseEntity --- */

    std::unique_ptr<BaseEntity> BaseRole::mapFields(StringMap *e)
    {
    }

    std::vector<std::string> BaseRole::tableViewColumns() const
    {
        return tableViewCols<std::string>(role_name()->displayName());
    }

    std::vector<std::string> BaseRole::tableViewValues()
    {
        return{role_name()->valueToString()};
    }

    /*
    QStringList BaseRole::tableHeaders() const
    {
        return mHeader;
    }

    std::string BaseRole::tableName() const
    {
        return mTableName;
    }

    void BaseRole::setTableName(const std::string table_name)
    {
        mTableName = table_name;
    }
   */

    std::string BaseRole::searchColumn() const
    {
        return role_name()->valueToString();
    }

    void BaseRole::populateEntity()
    {

    }

    std::shared_ptr<BaseEntity> BaseRole::cloneAsShared()
    {
    }

    void BaseRole::afterMapping(BaseEntity& entity)
    {

    }

    std::string BaseRole::displayName()
    {
        return role_name()->value();
    }


    std::string BaseRole::make_create_stmt()
    {
        qDebug() << "BaseRole::make_create_stmt ";
        return make_create_role_stmt();
    }

    std::string BaseRole::make_alter_stmt(const std::string name)
    {
        return make_alter_role_stmt(name);
    }

    std::string BaseRole::make_drop_stmt(const std::string name)
    {
        return make_drop_role_stmt( name );
    }


    std::string BaseRole::order_by() const
    {
        return "rolname";
    }

}
