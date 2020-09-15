#include "orderapprover.h"
#include "../security/user.h"

OrderApprover::OrderApprover()
{

    mUserId = createField<ForeignKeyField>("user_id", "User ID",
                                           std::make_unique<User>(), "usename");
    mUserName = createField<StringField>("username", "User Name");
    mUserTitle = createField<StringField>("user_title", "User Title");
    mLevel = createField<IntegerField>("level", "Level");

    mHeader << stoq(mUserName->fieldLabel())
            << stoq(mUserTitle->fieldLabel())
            << stoq(mLevel->fieldLabel());

    setTableName("rave_orderapprover");
}

OrderApprover::~OrderApprover()
{
}

ForeignKeyField *OrderApprover::userId()
{
    return mUserId;
}

void OrderApprover::setUserId(int id)
{
    mUserId->setValue( id );
}

StringField *OrderApprover::userName() const
{
    return mUserName;
}

void OrderApprover::setUserName(const std::string username)
{
    mUserName->setValue( username );
}

StringField *OrderApprover::userTitle() const
{
    return mUserTitle;
}

void OrderApprover::setUserTitle(const std::string title)
{
    mUserTitle->setValue( title );
}

IntegerField *OrderApprover::level() const
{
    return mLevel;
}

void OrderApprover::setLevel(int lvl)
{
    mLevel->setValue( lvl );
}

std::string OrderApprover::tableName() const
{
   return mTableName;
}

void OrderApprover::setTableName(const std::string table_name)
{
    mTableName = table_name;

}

std::unique_ptr<BaseEntity> OrderApprover::mapFields(StringMap*)
{
}

std::list<std::string> OrderApprover::tableViewColumns() const
{
    std::list<std::string> cols;
    cols.push_back(userName()->displayName());
    cols.push_back(userTitle()->displayName());
    cols.push_back(level()->displayName());
    return cols;
}

std::vector<std::string> OrderApprover::tableViewValues()
{
    return {
        userName()->displayName(),
        userTitle()->displayName(),
        level()->displayName()
    };
}

QStringList OrderApprover::tableHeaders() const
{
    return mHeader;
}

std::string OrderApprover::searchColumn() const
{
    return userName()->valueToString();
}

void OrderApprover::populateEntity()
{
}

std::unique_ptr<BaseEntity> OrderApprover::cloneAsUnique()
{
    return std::make_unique<OrderApprover>();
}

void OrderApprover::afterMapping(BaseEntity&)
{
}
