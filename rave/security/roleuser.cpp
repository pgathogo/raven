#include "../framework/entitydatamodel.h"
#include "roleuser.h"


namespace SECURITY
{
    RoleUser::RoleUser()
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

    }

    RoleUser& RoleUser::operator=(const RoleUser& other)
    {
       this->setId(other.id());
       mOId->setValue(other.mOId->value());
       mOId->setMandatory(true);
       mRoleName->setValue(other.mRoleName->value());
       mRoleName->setMandatory(true);
       mValidUntil->setValue(other.mValidUntil->value());
       mRoleExpire->setValue(other.mRoleExpire->value());
       mComment->setValue(other.mComment->value());
       mRoleCanLogin->setValue(other.mRoleCanLogin->value());
       mHeader = other.mHeader;

       getId().setFormOnly(true);
       setTableName("pg_roles");

       return *this;

    }

    RoleUser::~RoleUser()
    {

    }

    int RoleUser::id() const
    {
        return oid()->value();
    }

    IntegerField *RoleUser::uniqueId() const
    {
        return mOId;
    }

    IntegerField *RoleUser::oid() const
    {
        return mOId;
    }

    void RoleUser::setOId(int id)
    {
        mOId->setValue( id );
    }

    StringField *RoleUser::roleName() const
    {
        return mRoleName;
    }

    void RoleUser::setRoleName(const std::string name)
    {
        mRoleName->setValue( name );
    }

    StringField *RoleUser::validUntil() const
    {
        return mValidUntil;
    }

    void RoleUser::setValidUntil(const std::string validity)
    {
        mValidUntil->setValue( validity );
    }

    BooleanField *RoleUser::roleExpire() const
    {
        return mRoleExpire;
    }

    void RoleUser::setRoleExpire(bool val)
    {
        mRoleExpire->setValue( val );
    }

    StringField *RoleUser::comment() const
    {
        return mComment;
    }

    void RoleUser::setComment(const std::string comm)
    {
        mComment->setValue( comm );
    }

    BooleanField *RoleUser::roleCanLogin() const
    {
        return mRoleCanLogin;
    }

    void RoleUser::setRoleCanLogin(bool val)
    {
        mRoleCanLogin->setValue( val );
    }

    std::string RoleUser::role_validity()
    {
        std::string validity{};
        if (!roleExpire()->value())
        validity = " VALID UNTIL '"+validUntil()->value()+"'";
        else
        validity = " VALID UNTIL 'infinity' ";

        return validity;
    }

    std::string RoleUser::make_create_role_stmt()
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

    std::string RoleUser::role_rights()
    {
        return " NOSUPERUSER INHERIT NOCREATEDB CREATEROLE NOREPLICATION;";
    }

    std::string RoleUser::make_alter_role_stmt(const std::string username)
    {
       std::string s1 = "ALTER ROLE "+username;

       return s1+role_validity()+role_rights();
    }

    std::string RoleUser::make_drop_role_stmt(const std::string username)
    {
        return "DROP ROLE "+username+";";
    }

    void RoleUser::table_sequences(std::vector<std::string>& tseq)
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

    std::unique_ptr<BaseEntity> RoleUser::mapFields(StringMap *e)
    {

    }

    std::vector<std::string> RoleUser::tableViewColumns() const
    {
        return tableViewCols<std::string>(roleName()->displayName());
    }

    std::vector<std::string> RoleUser::tableViewValues()
    {
        return{roleName()->valueToString()};
    }

    QStringList RoleUser::tableHeaders() const
    {
        return mHeader;
    }

    std::string RoleUser::tableName() const
    {
        return mTableName;
    }

    void RoleUser::setTableName(const std::string table_name)
    {
        mTableName = table_name;
    }

    std::string RoleUser::searchColumn() const
    {
        return roleName()->valueToString();
    }

    void RoleUser::populateEntity()
    {

    }

    std::shared_ptr<BaseEntity> RoleUser::cloneAsShared()
    {
        return std::make_shared<RoleUser>();
    }

    void RoleUser::afterMapping(BaseEntity& entity)
    {
        //RoleUser& role = dynamic_cast<RoleUser&>(entity);
    }

    std::string RoleUser::displayName()
    {
        return roleName()->value();
    }

    std::string RoleUser::make_create_stmt()
    {
        return make_create_role_stmt();
    }

    std::string RoleUser::make_alter_stmt(const std::string name)
    {
        return make_alter_role_stmt(name);
    }

    std::string RoleUser::make_drop_stmt(const std::string name)
    {
        return make_drop_role_stmt( name );
    }

    std::string RoleUser::order_by() const
    {
        return "rolname";
    }


}
