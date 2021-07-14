#include "../framework/entitydatamodel.h"
#include "role.h"

#include "user.h"
#include "rolemember.h"


Role::Role()
{

    mOId = createField<IntegerField>("oid", "Role ID");
    mOId->setReadOnly(true);

    mRoleName = createField<StringField>("rolname", "Role Name");
    mRoleName->setMandatory(true);
    mValidUntil = createField<StringField>("rolvaliduntil", "Role Valid Until");

    mRoleExpire = createField<BooleanField>("role_expire", "Can Role Expire");
    mRoleExpire->setFormOnly(true);

    mComment = createField<StringField>("role_comment", "Comment");
    mComment->setFormOnly(true);

    mRoleCanLogin = createField<BooleanField>("rolcanlogin", "Role Can Login");

    mHeader << QString::fromStdString(mRoleName->fieldLabel());
    setTableName("pg_roles");

    getId().setFormOnly(true);

    mUser =  std::make_unique<User>();
    mRoleMember = std::make_unique<RoleMember> (this, mUser.get());
}

Role::~Role()
{

}

int Role::id() const
{
    return oid()->value();
}

IntegerField *Role::uniqueId() const
{
    return mOId;
}

IntegerField *Role::oid() const
{
    return mOId;
}

void Role::setOId(int id)
{
    mOId->setValue( id );
}

StringField *Role::roleName() const
{
    return mRoleName;
}

void Role::setRoleName(const std::string name)
{
    mRoleName->setValue( name );
}

StringField *Role::validUntil() const
{
    return mValidUntil;
}

void Role::setValidUntil(const std::string validity)
{
    mValidUntil->setValue( validity );
}

BooleanField *Role::roleExpire() const
{
    return mRoleExpire;
}

void Role::setRoleExpire(bool val)
{
    mRoleExpire->setValue( val );
}

StringField *Role::comment() const
{
    return mComment;
}

void Role::setComment(const std::string comm)
{
    mComment->setValue( comm );
}

BooleanField *Role::roleCanLogin() const
{
    return mRoleCanLogin;
}

void Role::setRoleCanLogin(bool val)
{
    mRoleCanLogin->setValue( val );
}

std::string Role::role_validity()
{
    std::string validity{};
    if (!roleExpire()->value())
        validity = " VALID UNTIL '"+validUntil()->value()+"'";
    else
        validity = " VALID UNTIL 'infinity' ";

    return validity;
}

std::string Role::make_create_role_stmt()
{
    std::string s1 = "CREATE ROLE "+roleName()->value();

    std::string s2{};

    if (!comment()->value().empty())
        s2 = " COMMENT ON ROLE "+roleName()->value()+
                " is '"+comment()->value()+"';";

    std::string s3 = "GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO "+
            roleName()->value()+";";

    std::vector<std::string> seqs;
    table_sequences(seqs);
    std::string s4{};
    for(auto& seq : seqs){
        s4 += "GRANT ALL ON SEQUENCE public."+seq+" TO GROUP "+
                roleName()->value()+" WITH GRANT OPTION;";
    }

    return s1+role_validity()+role_rights()+s2+s3+s4;
}

std::string Role::role_rights()
{
    return " NOSUPERUSER INHERIT NOCREATEDB CREATEROLE NOREPLICATION;";
}

std::string Role::make_alter_role_stmt(const std::string username)
{
   std::string s1 = "ALTER ROLE "+username;

   return s1+role_validity()+role_rights();
}

std::string Role::make_drop_role_stmt(const std::string username)
{
    return "DROP ROLE "+username+";";
}

void Role::table_sequences(std::vector<std::string>& tseq)
{
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


}

std::unique_ptr<BaseEntity> Role::mapFields(StringMap *e)
{

}

std::vector<std::string> Role::tableViewColumns() const
{
    return tableViewCols<std::string>(roleName()->displayName());
}

std::vector<std::string> Role::tableViewValues()
{
    return{roleName()->valueToString()};
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
    return roleName()->valueToString();
}

void Role::populateEntity()
{

}

std::unique_ptr<BaseEntity> Role::cloneAsUnique()
{
    return std::make_unique<Role>();
}

void Role::afterMapping(BaseEntity& entity)
{
    mRoleMember->setParentId(entity.id());
}

std::string Role::displayName()
{
    return roleName()->value();
}

std::string Role::make_create_stmt()
{
    return make_create_role_stmt();
}

std::string Role::make_alter_stmt(const std::string name)
{
    return make_alter_role_stmt(name);
}

std::string Role::make_drop_stmt(const std::string name)
{
    return make_drop_role_stmt( name );
}

std::string Role::order_by() const
{
    return "rolname";
}

User &Role::user()
{
    return *mUser;
}

RoleMember& Role::roleMember()
{
    return *mRoleMember;
}
