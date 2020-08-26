#include <QDebug>
#include "user.h"

User::User()
    :mUserName{nullptr},
     mPassword{nullptr}
{
    mUseSysId = createField<IntegerField>("usesysid", "User System ID");
    mUseSysId->setReadOnly(true);

    mUserName = createField<StringField>("usename","User Name");
    mUserName->setMandatory(true);
    mPassword = createField<StringField>("password","Password");
    mPassword->setMandatory(true);
    mPassword->setFormOnly(true);

    mConfirmPassword = createField<StringField>("confirm_password","Confirm Password");
    mConfirmPassword->setMandatory(true);
    mConfirmPassword->setFormOnly(true);
    mValidUntil = createField<StringField>("valuntil","Valid Until");

    mPasswordExpire = createField<BooleanField>("password_expire", "Password Expire");
    mPasswordExpire->setFormOnly(true);

    mHeader << QString::fromStdString(mUserName->fieldLabel());

    setTableName("pg_user");

    // pg_user table does not have an Id column
    getId().setFormOnly(true);

}

User::~User()
{
}

IntegerField* User::useSysId() const
{
    return mUseSysId;
}

void User::setUseSysId(int val)
{
    mUseSysId->setValue( val );
}

int User::id() const
{
   return useSysId()->value();
}

StringField* User::userName() const
{
   return mUserName;
}

void User::setUserName(const std::string uname)
{
    mUserName->setValue(uname);
}

StringField* User::password() const
{
    return mPassword;
}

void User::setPassword(const std::string pword)
{
    mPassword->setValue( pword );
}

StringField *User::confirmPassword() const
{
   return mConfirmPassword;
}

void User::setConfirmPassword(const std::string pword)
{
    mConfirmPassword->setValue( pword );
}

StringField* User::validUntil() const
{
    return mValidUntil;
}

void User::setValidUntil(const std::string validity)
{
    mValidUntil->setValue( validity );
}

BooleanField *User::passwordExpire() const
{
    return mPasswordExpire;
}

void User::setPasswordExpire(bool val)
{
    mPasswordExpire->setValue( val );
}

std::string User::password_validity()
{
    std::string validity{};
    if (!passwordExpire()->value())
        validity = " VALID UNTIL '"+validUntil()->value()+"'";
    else
        validity = " VALID UNTIL 'infinity' ";

    return validity;
}

std::string User::make_create_user_stmt()
{
    std::string s1 = "CREATE USER "+userName()->value()+
            " WITH LOGIN PASSWORD '"+password()->value()+"'";

    return s1+password_validity()+role_rights();
}

std::string User::role_rights()
{
    return " NOSUPERUSER INHERIT NOCREATEDB CREATEROLE NOREPLICATION;";
}

std::string User::make_drop_user_stmt(const std::string username)
{
    return "DROP USER "+username+";";
}

std::string User::make_alter_user_stmt(const std::string username)
{
   std::string s1 = "ALTER USER "+username+" WITH LOGIN PASSWORD "+
           "'"+password()->value()+"'";

   return s1+password_validity()+role_rights();
}

std::unique_ptr<BaseEntity> User::mapFields(StringMap*)
{
}

std::list<std::string> User::tableViewColumns() const
{
    std::list<std::string> cols;
    cols.push_back(userName()->displayName());
    return cols;
}

std::vector<std::string> User::tableViewValues()
{
    return{userName()->valueToString()};
}

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
    return userName()->valueToString();
}

void User::populateEntity()
{
}

std::unique_ptr<BaseEntity> User::cloneAsUnique()
{
    return std::make_unique<User>();
}

void User::afterMapping(BaseEntity &entity)
{
}

IntegerField* User::uniqueId() const
{
    return useSysId();
}

std::string User::displayName()
{
    return userName()->value();
}

std::string User::make_create_stmt()
{
    return make_create_user_stmt();
}

std::string User::make_alter_stmt(const std::string name)
{
    return make_alter_user_stmt(name);
}

std::string User::make_drop_stmt(const std::string name)
{
    return make_drop_user_stmt( name );
}
